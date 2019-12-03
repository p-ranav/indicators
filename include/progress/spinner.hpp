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
  std::string _start{"|"};
  std::string _end{"|"};
  std::mutex _mutex;
  float _progress{0.0};
  size_t _index{0};
  std::vector<std::string> _states{"⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏"}; //{"-", "\\", "|", "/"};
  
public:

  explicit ProgressSpinner(const std::string& name) : _name(name) {}

  void increment(float value) {
    std::unique_lock<std::mutex> lock{_mutex};
    std::cout << _name << " [";
    _progress = value / 100.0;
    float pos = _progress * static_cast<float>(_bar_width);
    std::cout << _states[_index % _states.size()] << "] " << static_cast<int>(value) << "%\r";
    std::cout.flush();
    _index += 1;
  }
};
