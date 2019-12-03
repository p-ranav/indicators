#pragma once
#include <iostream>
#include <string>
#include <atomic>
#include <mutex>
#include <thread>

template <typename T>
class progress_bar {
  std::string _name{"Running"};
  size_t _bar_width{80};  
  std::string _start{"|"};
  std::string _end{"|"};
  std::mutex _mutex;
  T _progress{T()};
  
public:

  ~progress_bar() {
    std::cout << std::endl;
  }
  
  void increment(T value) {
    std::unique_lock<std::mutex> lock{_mutex};
    _progress = value / 100.0;
    std::cout << _name << " [";
    T pos = _progress * static_cast<T>(_bar_width);
    for (size_t i = 0; i < _bar_width; ++i) {
      if (i < pos) std::cout << '#';
      else if (i == pos) std::cout << ">";
      else std::cout << " ";
    }
    std::cout << "] " << static_cast<int>(value) << " %\r" << std::flush;
  }
};
