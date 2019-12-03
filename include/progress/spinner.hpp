#pragma once
#include <iostream>
#include <string>
#include <atomic>
#include <mutex>
#include <thread>
#include <vector>

class ProgressSpinner {
  std::string _name{"Running"};
  size_t _bar_width{80};  
  std::mutex _mutex;
  float _progress{0.0};
  size_t _index{0};
  std::vector<std::string> _states{"⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏"};

  void hide_cursor() {
    std::cout << "\e[?25l";
  }

  void show_cursor() {
    std::cout << "\e[?25h";
  }
  
public:

  explicit ProgressSpinner(const std::string& name) : _name(name) {}

  void increment(float value) {
    std::unique_lock<std::mutex> lock{_mutex};
    _progress = value / 100.0;
    float pos = _progress * static_cast<float>(_bar_width);
    hide_cursor();
    std::cout << _states[_index % _states.size()];
    std::cout << " " << static_cast<int>(value) << "%";
    std::cout << " " << _name << "\r";
    std::cout.flush();
    show_cursor();
    _index += 1;
  }
};
