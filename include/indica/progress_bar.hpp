#pragma once
#include <atomic>
#include <indica/color.hpp>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

namespace indica {

class ProgressBar {
public:
  void set_foreground_color(Color color) {
    std::unique_lock<std::mutex> lock{_mutex};
    _foreground_color = color;
  }

  void set_bar_width(size_t bar_width) {
    std::unique_lock<std::mutex> lock{_mutex};
    _bar_width = bar_width;
  }

  void start_bar_with(const std::string &start) {
    std::unique_lock<std::mutex> lock{_mutex};
    _start = start;
  }

  void fill_bar_progress_with(const std::string &fill) {
    std::unique_lock<std::mutex> lock{_mutex};
    _fill = fill;
  }

  void lead_bar_progress_with(const std::string &lead) {
    std::unique_lock<std::mutex> lock{_mutex};
    _lead = lead;
  }

  void fill_bar_remainder_with(const std::string &remainder) {
    std::unique_lock<std::mutex> lock{_mutex};
    _remainder = remainder;
  }

  void end_bar_with(const std::string &end) {
    std::unique_lock<std::mutex> lock{_mutex};
    _end = end;
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

  void set_progress(float value) {
    {
      std::unique_lock<std::mutex> lock{_mutex};
      _progress = value;
    }
    _print_progress();
  }

  void tick() {
    {
      std::unique_lock<std::mutex> lock{_mutex};
      _progress += 1;
    }
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
  std::mutex _mutex;
  Color _foreground_color;

  void _print_progress() {
    std::unique_lock<std::mutex> lock{_mutex};
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
    std::cout << _start;
    float pos = _progress * static_cast<float>(_bar_width) / 100.0;
    for (size_t i = 0; i < _bar_width; ++i) {
      if (i < pos)
        std::cout << _fill;
      else if (i == pos)
        std::cout << _lead;
      else
        std::cout << _remainder;
    }
    std::cout << _end;
    if (_show_percentage) {
      std::cout << " " << std::min(static_cast<size_t>(_progress), size_t(100)) << "%";
    }
    if (_max_postfix_text_length == 0)
      _max_postfix_text_length = 10;
    std::cout << " " << _postfix_text << std::string(_max_postfix_text_length, ' ') << "\r";
    std::cout.flush();
    if (_progress > 100.0) {
      _completed = true;
    }
    if (_completed)
      std::cout << termcolor::reset << std::endl;
  }
};

}
