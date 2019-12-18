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
#define NOMINMAX
#include <indicators/progress_bar.hpp>
#include <vector>
#include <functional>

namespace indicators {

template <size_t count>
class MultiProgress {
public:
  void add_progress_bar(ProgressBar& bar) {
    _bars.push_back(bar);
    bar._multi_progress_mode = true;
  }

  template <size_t index>
  typename std::enable_if<(index < count), void>::type
  set_progress(float value) {
    _bars[index].get().set_progress(value);
    _print_progress();    
  }

  template <size_t index>
  typename std::enable_if<(index < count), void>::type    
  tick() {
    _bars[index].get().tick();
    _print_progress();
  }

  template <size_t index>
  typename std::enable_if<(index < count), bool>::type    
  is_completed() const { return _bars[index].get().is_completed(); }

private:
  std::vector<std::reference_wrapper<ProgressBar>> _bars;

  void _print_progress() {
    for (size_t i = 0; i < count; ++i)
      std::cout << "\x1b[A";
    for (auto& bar: _bars) {
      bar.get()._print_progress();
      std::cout << "\n";
    }
  }
  
};

}

