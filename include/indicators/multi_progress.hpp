/*
Activity Indicators for Modern C++
https://github.com/p-ranav/indicators

Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2019 Pranav Srinivas Kumar <pranav.srinivas.kumar@gmail.com>.

Permission is hereby  granted, free of charge, to any  person obtaining a copy
of this software and associated  documentation files (the "Software"), to deal
in the Software  without restriction, including without  limitation the rights
to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#pragma once
#include <atomic>
#include <functional>
#include <indicators/color.hpp>
#include <iostream>
#include <mutex>
#include <vector>

namespace indicators {

template <typename Indicator, size_t count> class MultiProgress {
public:
  template <typename... Indicators,
            typename = typename std::enable_if<(sizeof...(Indicators) == count)>::type>
  explicit MultiProgress(Indicators &... bars) {
    _bars = {bars...};
    for (auto &bar : _bars) {
      bar.get()._multi_progress_mode = true;
    }
  }

  template <size_t index>
  typename std::enable_if<(index >= 0 && index < count), void>::type set_progress(float value) {
    if (!_bars[index].get().is_completed())
      _bars[index].get().set_progress(value);
    _print_progress();
  }

  template <size_t index>
  typename std::enable_if<(index >= 0 && index < count), void>::type tick() {
    if (!_bars[index].get().is_completed())
      _bars[index].get().tick();
    _print_progress();
  }

  template <size_t index>
  typename std::enable_if<(index >= 0 && index < count), bool>::type is_completed() const {
    return _bars[index].get().is_completed();
  }

private:
  std::atomic<bool> _started{false};
  std::mutex _mutex;
  std::vector<std::reference_wrapper<Indicator>> _bars;

  bool _all_completed() {
    bool result{true};
    for (size_t i = 0; i < count; ++i)
      result &= _bars[i].get().is_completed();
    return result;
  }

  void _print_progress() {
    std::lock_guard<std::mutex> lock{_mutex};
    if (_started)
      for (size_t i = 0; i < count; ++i)
        std::cout << "\x1b[A";
    for (auto &bar : _bars) {
      bar.get()._print_progress(true);
      std::cout << "\n";
    }
    std::cout << termcolor::reset;
    if (!_started)
      _started = true;
  }
};

} // namespace indicators
