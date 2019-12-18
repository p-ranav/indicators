/*
Activity Indicators for Modern C++
https://github.com/p-ranav/indicators

Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2019 Pranav Srinivas Kumar <pranav.srinivas.kumar@gmail.com>.

Permission is hereby  granted, free of charge, to any  person obtaining a copy
of this software and associated  documentation files (the "Software"), to deal
in the Software  without restriction, including without  limitation the rights
to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#pragma once
#define NOMINMAX
#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
#include <indicators/color.hpp>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

namespace indicators {

class ProgressSpinner {
public:
  void set_foreground_color(Color color) {
    std::unique_lock<std::mutex> lock{_mutex};
    _foreground_color = color;
  }

  void set_prefix_text(const std::string &text) {
    std::unique_lock<std::mutex> lock{_mutex};
    _prefix_text = text;
  }

  void set_postfix_text(const std::string &text) {
    std::unique_lock<std::mutex> lock{_mutex};
    _postfix_text = text;
    if (_postfix_text.length() > _max_postfix_text_length)
      _max_postfix_text_length = _postfix_text.length();
  }

  void show_percentage() { _show_percentage = true; }

  void hide_percentage() { _show_percentage = false; }

  void show_elapsed_time() { _show_elapsed_time = true; }

  void hide_elapsed_time() { _show_elapsed_time = false; }

  void show_remaining_time() { _show_remaining_time = true; }

  void hide_remaining_time() { _show_remaining_time = false; }

  void show_spinner() { _show_spinner = true; }

  void hide_spinner() { _show_spinner = false; }

  void set_progress(float value) {
    {
      std::unique_lock<std::mutex> lock{_mutex};
      _progress = value;
    }
    _save_start_time();
    _print_progress();
  }

  void tick() {
    {
      std::unique_lock<std::mutex> lock{_mutex};
      _progress += 1;
    }
    _save_start_time();
    _print_progress();
  }

  size_t current() {
    std::unique_lock<std::mutex> lock{_mutex};
    return std::min(static_cast<size_t>(_progress), size_t(100));
  }

  bool is_completed() const { return _completed; }

  void mark_as_completed() {
    _completed = true;
    _print_progress();
  }

  void set_spinner_states(const std::vector<std::string> &states) {
    std::unique_lock<std::mutex> lock{_mutex};
    _states = states;
  }

private:
  float _progress{0.0};
  std::string _prefix_text{""};
  size_t _index{0};
  std::vector<std::string> _states{"⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏"};
  std::string _postfix_text{""};
  std::atomic<size_t> _max_postfix_text_length{0};
  std::atomic<bool> _completed{false};
  std::atomic<bool> _show_percentage{true};
  std::atomic<bool> _show_elapsed_time{false};
  std::atomic<bool> _show_remaining_time{false};
  std::atomic<bool> _saved_start_time{false};
  std::chrono::time_point<std::chrono::high_resolution_clock> _start_time_point;
  std::atomic<bool> _show_spinner{true};
  std::mutex _mutex;
  Color _foreground_color;

  std::ostream &_print_duration(std::ostream &os, std::chrono::nanoseconds ns) {
    using namespace std;
    using namespace std::chrono;
    typedef duration<int, ratio<86400>> days;
    char fill = os.fill();
    os.fill('0');
    auto d = duration_cast<days>(ns);
    ns -= d;
    auto h = duration_cast<hours>(ns);
    ns -= h;
    auto m = duration_cast<minutes>(ns);
    ns -= m;
    auto s = duration_cast<seconds>(ns);
    if (d.count() > 0)
      os << setw(2) << d.count() << "d:";
    if (h.count() > 0)
      os << setw(2) << h.count() << "h:";
    os << setw(2) << m.count() << "m:" << setw(2) << s.count() << 's';
    os.fill(fill);
    return os;
  };

  void _save_start_time() {
    if ((_show_elapsed_time || _show_remaining_time) && !_saved_start_time) {
      _start_time_point = std::chrono::high_resolution_clock::now();
      _saved_start_time = true;
    }
  }

  void _print_progress() {
    std::unique_lock<std::mutex> lock{_mutex};
    auto now = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(now - _start_time_point);

    std::cout << termcolor::bold;
    switch (_foreground_color) {
    case Color::GREY:
      std::cout << termcolor::grey;
      break;
    case Color::RED:
      std::cout << termcolor::red;
      break;
    case Color::GREEN:
      std::cout << termcolor::green;
      break;
    case Color::YELLOW:
      std::cout << termcolor::yellow;
      break;
    case Color::BLUE:
      std::cout << termcolor::blue;
      break;
    case Color::MAGENTA:
      std::cout << termcolor::magenta;
      break;
    case Color::CYAN:
      std::cout << termcolor::cyan;
      break;
    case Color::WHITE:
      std::cout << termcolor::white;
      break;
    }
    std::cout << _prefix_text;
    if (_show_spinner)
      std::cout << _states[_index % _states.size()];
    if (_show_percentage) {
      std::cout << " " << std::min(static_cast<size_t>(_progress), size_t(100)) << "%";
    }

    if (_show_elapsed_time) {
      std::cout << " [";
      _print_duration(std::cout, elapsed);
    }

    if (_show_remaining_time) {
      if (_show_elapsed_time)
        std::cout << "<";
      else
        std::cout << " [";
      auto eta = std::chrono::nanoseconds(
          _progress > 0 ? static_cast<long long>(elapsed.count() * 100 / _progress) : 0);
      auto remaining = eta > elapsed ? (eta - elapsed) : (elapsed - eta);
      _print_duration(std::cout, remaining);
      std::cout << "]";
    } else {
      if (_show_elapsed_time)
        std::cout << "]";
    }

    if (_max_postfix_text_length == 0)
      _max_postfix_text_length = 10;
    std::cout << " " << _postfix_text << std::string(_max_postfix_text_length, ' ') << "\r";
    std::cout.flush();
    _index += 1;
    if (_progress > 100.0) {
      _completed = true;
    }
    if (_completed)
      std::cout << termcolor::reset << std::endl;
  }
};

} // namespace indicators
