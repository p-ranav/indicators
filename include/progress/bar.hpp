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
  std::atomic<bool> _completed;

  void _print_progress() {
    std::unique_lock<std::mutex> lock{_mutex};    
    std::cout << _start;
    float pos = _progress * static_cast<float>(_bar_width) / 100.0;
    for (size_t i = 0; i < _bar_width; ++i) {
      if (i < pos) std::cout << _fill;
      else if (i == pos) std::cout << _lead;
      else std::cout << _remainder;
    }
    std::cout << _end << " " << static_cast<int>(_progress) << "%";
    std::cout << " " << _name << "\r";
    std::cout.flush();
    if (_completed)
      std::cout << std::endl;    
  }

public:

  void bar_width(size_t bar_width) {
    std::unique_lock<std::mutex> lock{_mutex};
    _bar_width = bar_width;
  }

  void start_with(const std::string& start) {
    std::unique_lock<std::mutex> lock{_mutex};
    _start = start;
  }

  void fill_progress_with(const std::string& fill) {
    std::unique_lock<std::mutex> lock{_mutex};    
    _fill = fill;
  }

  void lead_progress_with(const std::string& lead) {
    std::unique_lock<std::mutex> lock{_mutex};    
    _lead = lead;
  }

  void fill_remainder_with(const std::string& remainder) {
    std::unique_lock<std::mutex> lock{_mutex};    
    _remainder = remainder;
  }    

  void end_with(const std::string& end) {
    std::unique_lock<std::mutex> lock{_mutex};    
    _end = end;
  }  

  void set_progress(float value) {
    {
      std::unique_lock<std::mutex> lock{_mutex};
      if (_completed) return;
      _progress = value;
      if (static_cast<int>(_progress) == 100) {
	_completed = true;
      }
    }
    _print_progress();
  }

  void tick() {
    if (_completed) return;
    set_progress(_progress + 1);
  }
  
};
