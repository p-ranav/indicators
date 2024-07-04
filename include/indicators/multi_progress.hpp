
#ifndef INDICATORS_MULTI_PROGRESS
#define INDICATORS_MULTI_PROGRESS
#include <atomic>
#include <functional>
#include <iostream>
#include <mutex>
#include <vector>
#include <numeric>

#include <indicators/color.hpp>
#include <indicators/cursor_movement.hpp>
#include <indicators/details/stream_helper.hpp>

namespace indicators {

template <typename Indicator, size_t count> class MultiProgress {
public:
  template <typename... Indicators,
            typename = typename std::enable_if<(sizeof...(Indicators) == count)>::type>
  explicit MultiProgress(Indicators &... bars) {
    bars_ = {bars...};
    for (auto &bar : bars_) {
      bar.get().multi_progress_mode_ = true;
    }
  }

  template <size_t index>
  typename std::enable_if<(index >= 0 && index < count), void>::type set_progress(size_t value) {
    if (!bars_[index].get().is_completed())
      bars_[index].get().set_progress(value);
    print_progress();
  }

  template <size_t index>
  typename std::enable_if<(index >= 0 && index < count), void>::type set_progress(float value) {
    if (!bars_[index].get().is_completed())
      bars_[index].get().set_progress(value);
    print_progress();
  }

  template <size_t index>
  typename std::enable_if<(index >= 0 && index < count), void>::type tick() {
    if (!bars_[index].get().is_completed())
      bars_[index].get().tick();
    print_progress();
  }

  template <size_t index>
  typename std::enable_if<(index >= 0 && index < count), bool>::type is_completed() const {
    return bars_[index].get().is_completed();
  }

private:
  std::atomic<bool> started_{false};
  std::mutex mutex_;
  std::vector<std::reference_wrapper<Indicator>> bars_;

  bool _all_completed() {
    bool result{true};
    for (size_t i = 0; i < count; ++i)
      result &= bars_[i].get().is_completed();
    return result;
  }

public:
  void print_progress() {
    std::lock_guard<std::mutex> lock{mutex_};
    
    if (started_) {
      // move all the way up to start of first progress bar
      const auto wrapped_lines = std::accumulate(begin(bars_), end(bars_), 0, [](size_t acc, auto &bar) {
        return acc + bar.get().extra_wrapped_lines();
      });
      move_up(count + wrapped_lines);
    }

    for (auto &bar : bars_) {
      auto wrapped_line = bar.get().extra_wrapped_lines();
      if (wrapped_line > 0) {
        // for each bar before calling `print_progress`, cursor needs to be on last line that bar printed
        move_down(wrapped_line);
      }
      bar.get().print_progress(true);
      std::cout << "\n";
    }
    std::cout << termcolor::reset;
    if (!started_)
      started_ = true;
  }
};

} // namespace indicators

#endif