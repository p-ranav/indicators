#pragma once
#include <iostream>
#include <string>
#include <atomic>
#include <mutex>
#include <thread>

class ProgressBar {
  std::string _name{""};
  float _progress{0.0};
  size_t _bar_width{80};  
  std::string _start{"["};
  std::string _step{"■"};
  std::string _head{"■"};
  std::string _end{"]"};
  std::mutex _mutex;

  void hide_cursor() {
    std::cout << "\e[?25l";
  }

  void show_cursor() {
    std::cout << "\e[?25h";
  }
  
public:

  explicit ProgressBar(const std::string& name) : _name(name) {}

  void increment(float value) {
    std::unique_lock<std::mutex> lock{_mutex};    
    _progress = value / 100.0;
    hide_cursor();
    std::cout << _start;
    float pos = _progress * static_cast<float>(_bar_width);
    for (size_t i = 0; i < _bar_width; ++i) {
      if (i < pos) std::cout << _step;
      else if (i == pos) std::cout << _head;
      else std::cout << "-";
    }
    std::cout << _end << " " << static_cast<int>(value) << "%";
    std::cout << " " << _name << "\r";
    std::cout.flush();
    show_cursor();
  }
};
