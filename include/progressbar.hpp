#pragma once
#include <iostream>
#include <string>
#include <atomic>
#include <mutex>
#include <thread>

class progress_bar {
  std::string _name{"Running"};
  size_t _bar_width{100};
  std::string _start{"|"};
  std::string _end{"|"};
  std::mutex _mutex;
  std::atomic<size_t> _progress{0};
  
public:

  ~progress_bar() {
    std::cout << std::endl;
  }
  
  void increment(size_t value) {
    std::unique_lock<std::mutex> lock{_mutex};
    _progress = value;
    std::cout << _name << " [";
    for (size_t i = 0; i < _bar_width; i++) {
      if (i < value) std::cout << "#";
      else if (i == value) std::cout << ">";
      else std::cout << " ";
    }
    std::cout << "] " << int(_progress) << " %\r";
    std::cout.flush();
  }

  size_t current() {
    return _progress;
  }
};
