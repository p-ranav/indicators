#pragma once
#include <atomic>
#include <indica/color.hpp>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

namespace indica {

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

  void show_spinner() { _show_spinner = true; }

  void hide_spinner() { _show_spinner = false; }  

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

  void set_spinner_states(const std::vector<std::string>& states) {
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
  std::atomic<bool> _show_spinner{true};  
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
    if (_show_spinner)
      std::cout << _states[_index % _states.size()];
    if (_show_percentage) {
      std::cout << " " << std::min(static_cast<size_t>(_progress), size_t(100)) << "%";
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

}
