#pragma once
#include <iostream>
#include <string>
#include <atomic>
#include <mutex>
#include <thread>

class ProgressBar {
  std::string _name{""};
  float _progress{0.0};
  size_t _bar_width{100};  
  std::string _start{"[ "};
  std::string _fill{"■"};
  std::string _lead{"■"};
  std::string _remainder{"-"};
  std::string _end{" ]"};
  std::mutex _mutex;

  void hide_cursor() {
    std::cout << "\e[?25l";
  }

  void show_cursor() {
    std::cout << "\e[?25h";
  }
  
public:

  ProgressBar& start_with(const std::string& start) {
    _start = start;
    return *this;
  }

  ProgressBar& fill_with(const std::string& fill) {
    _fill = fill;
    return *this;
  }

  ProgressBar& lead_with(const std::string& lead) {
    _lead = lead;
    return *this;
  }

  ProgressBar& remainder_with(const std::string& remainder) {
    _remainder = remainder;
    return *this;
  }    

  ProgressBar& end_with(const std::string& end) {
    _end = end;
    return *this;
  }  

  void increment(float value) {
    std::unique_lock<std::mutex> lock{_mutex};    
    _progress = value / 100.0;
    std::cout << _start;
    float pos = _progress * static_cast<float>(_bar_width);
    for (size_t i = 0; i < _bar_width; ++i) {
      if (i < pos) std::cout << _fill;
      else if (i == pos) std::cout << _lead;
      else std::cout << _remainder;
    }
    std::cout << _end << " " << static_cast<int>(value) << "%";
    std::cout << " " << _name << "\r";
    std::cout.flush();
  }
};
