#pragma once
#include <iostream>
#include <string>
#include <atomic>
#include <mutex>
#include <thread>

class ProgressBar {
  std::string _name{"Running"};
  size_t _bar_width{80};  
  std::string _start{"|"};
  std::string _end{"|"};
  std::mutex _mutex;
  float _progress{0.0};
  
public:

  explicit ProgressBar(const std::string& name) : _name(name) {}

  void increment(float value) {
    std::unique_lock<std::mutex> lock{_mutex};    
    _progress = value / 100.0;
    std::cout << _name << " [";
    float pos = _progress * static_cast<float>(_bar_width);
    for (size_t i = 0; i < _bar_width; ++i) {
      if (i < pos) std::cout << '#';
      else if (i == pos) std::cout << ">";
      else std::cout << " ";
    }
    std::cout << "] " << static_cast<int>(value) << "%\r";
    std::cout.flush();
  }
};
