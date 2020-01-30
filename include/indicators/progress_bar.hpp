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

#include <indicators/details/stream_helper.hpp>

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

namespace indicators {

class ProgressBar {
public:
  void set_foreground_color(Color color) {
    std::lock_guard<std::mutex> lock{_mutex};
    _foreground_color = color;
  }

  void set_bar_width(size_t bar_width) {
    std::lock_guard<std::mutex> lock{_mutex};
    _bar_width = bar_width;
  }

  void start_bar_with(const std::string &start) {
    std::lock_guard<std::mutex> lock{_mutex};
    _start = start;
  }

  void fill_bar_progress_with(const std::string &fill) {
    std::lock_guard<std::mutex> lock{_mutex};
    _fill = fill;
  }

  void lead_bar_progress_with(const std::string &lead) {
    std::lock_guard<std::mutex> lock{_mutex};
    _lead = lead;
  }

  void fill_bar_remainder_with(const std::string &remainder) {
    std::lock_guard<std::mutex> lock{_mutex};
    _remainder = remainder;
  }

  void end_bar_with(const std::string &end) {
    std::lock_guard<std::mutex> lock{_mutex};
    _end = end;
  }

  void set_prefix_text(const std::string &text) {
    std::lock_guard<std::mutex> lock{_mutex};
    _prefix_text = text;
  }

  void set_postfix_text(const std::string &text) {
    std::lock_guard<std::mutex> lock{_mutex};
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

  void set_progress(float value) {
    {
      std::lock_guard<std::mutex> lock{_mutex};
      _progress = value;
    }
    _save_start_time();
    _print_progress();
  }

  void tick() {
    {
      std::lock_guard<std::mutex> lock{_mutex};
      _progress += 1;
    }
    _save_start_time();
    _print_progress();
  }

  size_t current() {
    std::lock_guard<std::mutex> lock{_mutex};
    return std::min(static_cast<size_t>(_progress), size_t(100));
  }

  bool is_completed() const { return _completed; }

  void mark_as_completed() {
    _completed = true;
    _print_progress();
  }

private:
  float _progress{0.0};
  size_t _bar_width{100};
  std::string _prefix_text{""};
  std::string _start{"["};
  std::string _fill{"="};
  std::string _lead{">"};
  std::string _remainder{" "};
  std::string _end{"]"};
  std::string _postfix_text{""};
  std::atomic<size_t> _max_postfix_text_length{0};
  std::atomic<bool> _completed{false};
  std::atomic<bool> _show_percentage{true};
  std::atomic<bool> _show_elapsed_time{false};
  std::atomic<bool> _show_remaining_time{false};
  std::atomic<bool> _saved_start_time{false};
  std::chrono::nanoseconds _elapsed;
  std::chrono::time_point<std::chrono::high_resolution_clock> _start_time_point;
  std::mutex _mutex;
  Color _foreground_color{indicators::Color::WHITE};

  template <typename Indicator, size_t count> friend class MultiProgress;
  std::atomic<bool> _multi_progress_mode{false};

  void _save_start_time() {
    if ((_show_elapsed_time || _show_remaining_time) && !_saved_start_time) {
      _start_time_point = std::chrono::high_resolution_clock::now();
      _saved_start_time = true;
    }
  }

  void _print_progress(bool from_multi_progress = false) {
    if (_multi_progress_mode && !from_multi_progress) {
      if (_progress > 100.0) {
        _completed = true;
      }
      return;
    }
    std::lock_guard<std::mutex> lock{_mutex};
    auto now = std::chrono::high_resolution_clock::now();
    if (!_completed)
      _elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(now - _start_time_point);

    std::cout << termcolor::bold;
    details::set_stream_color(std::cout, _foreground_color);
    std::cout << _prefix_text;

    std::cout << _start;

    details::ProgressScaleWriter writer{std::cout, _bar_width, _fill, _lead, _remainder};
    writer.write(_progress);

    std::cout << _end;

    if (_show_percentage) {
      std::cout << " " << std::min(static_cast<size_t>(_progress), size_t(100)) << "%";
    }

    if (_show_elapsed_time) {
      std::cout << " [";
      details::write_duration(std::cout, _elapsed);
    }

    if (_show_remaining_time) {
      if (_show_elapsed_time)
        std::cout << "<";
      else
        std::cout << " [";
      auto eta = std::chrono::nanoseconds(
          _progress > 0 ? static_cast<long long>(_elapsed.count() * 100 / _progress) : 0);
      auto remaining = eta > _elapsed ? (eta - _elapsed) : (_elapsed - eta);
      details::write_duration(std::cout, remaining);
      std::cout << "]";
    } else {
      if (_show_elapsed_time)
        std::cout << "]";
    }

    if (_max_postfix_text_length == 0)
      _max_postfix_text_length = 10;
    std::cout << " " << _postfix_text << std::string(_max_postfix_text_length, ' ') << "\r";
    std::cout.flush();
    if (_progress > 100.0) {
      _completed = true;
    }
    if (_completed && !from_multi_progress) // Don't std::endl if calling from MultiProgress
      std::cout << termcolor::reset << std::endl;
  }
};

} // namespace indicators
