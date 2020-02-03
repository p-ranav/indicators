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

#include <indicators/details/stream_helper.hpp>

#define NOMINMAX
#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
#include <indicators/color.hpp>
#include <indicators/setting.hpp>
#include <tuple>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

namespace indicators {

class ProgressBar {
public:
  template <typename... Args>
  explicit ProgressBar(Args&&... args) :
    settings_(
        indicators::get<ProgressBarOption::BAR_WIDTH>(std::forward<Args>(args)...),
        indicators::get<ProgressBarOption::PREFIX_TEXT>(std::forward<Args>(args)...),
        indicators::get<ProgressBarOption::POSTFIX_TEXT>(std::forward<Args>(args)...),
        indicators::get<ProgressBarOption::START>(std::forward<Args>(args)...),
        indicators::get<ProgressBarOption::END>(std::forward<Args>(args)...),
        indicators::get<ProgressBarOption::FILL>(std::forward<Args>(args)...),
        indicators::get<ProgressBarOption::LEAD>(std::forward<Args>(args)...),
        indicators::get<ProgressBarOption::REMAINDER>(std::forward<Args>(args)...),
        indicators::get<ProgressBarOption::MAX_POSTFIX_TEXT_LEN>(std::forward<Args>(args)...),
        indicators::get<ProgressBarOption::COMPLETED>(std::forward<Args>(args)...),
        indicators::get<ProgressBarOption::SHOW_PERCENTAGE>(std::forward<Args>(args)...),
        indicators::get<ProgressBarOption::SHOW_ELAPSED_TIME>(std::forward<Args>(args)...),
        indicators::get<ProgressBarOption::SHOW_REMAINING_TIME>(std::forward<Args>(args)...),
        indicators::get<ProgressBarOption::SAVED_START_TIME>(std::forward<Args>(args)...),
        indicators::get<ProgressBarOption::FOREGROUND_COLOR>(std::forward<Args>(args)...)
        )
  {}

  template <typename T, ProgressBarOption id>
  void set_option(Setting<T, id>&& setting){
    static_assert(std::is_same<T, typename std::decay<decltype(detail::get<id>())>::type>::value, "Setting has wrong type!");
    std::lock_guard<std::mutex> lock(_mutex);
    get_value<id>() = std::move(setting).value;
  }

  template <typename T, ProgressBarOption id>
  void set_option(const Setting<T, id>& setting){
    static_assert(std::is_same<T, typename std::decay<decltype(detail::get<id>())>::type>::value, "Setting has wrong type!");
    std::lock_guard<std::mutex> lock(_mutex);
    get_value<id>() = setting.value;
  }

  void set_option(const Setting<std::string, ProgressBarOption::POSTFIX_TEXT>& setting){
    std::lock_guard<std::mutex> lock(_mutex);
    get_value<ProgressBarOption::POSTFIX_TEXT>() = setting.value;
    if(setting.value.length() > get_value<ProgressBarOption::MAX_POSTFIX_TEXT_LEN>()){
      get_value<ProgressBarOption::MAX_POSTFIX_TEXT_LEN>() = setting.value.length();
    }
  }

  void set_option(Setting<std::string, ProgressBarOption::POSTFIX_TEXT>&& setting){
    std::lock_guard<std::mutex> lock(_mutex);
    get_value<ProgressBarOption::POSTFIX_TEXT>() = std::move(setting).value;
    auto& new_value = get_value<ProgressBarOption::POSTFIX_TEXT>();
    if(new_value.length() > get_value<ProgressBarOption::MAX_POSTFIX_TEXT_LEN>()){
      get_value<ProgressBarOption::MAX_POSTFIX_TEXT_LEN>() = new_value.length();
    }
  }

  void set_progress(float newProgress){
    {
      std::lock_guard<std::mutex> lck(_mutex);
      _progress = newProgress;
    }

    _save_start_time();
    _print_progress();
  }

  void tick() {
    {
      std::lock_guard<std::mutex> lock{_mutex};
      _progress += 1;
    }
    _save_start_time();
    _print_progress();
  }

  size_t current() {
    std::lock_guard<std::mutex> lock{_mutex};
    return std::min(static_cast<size_t>(_progress), size_t(100));
  }

  bool is_completed() const { return get_value<ProgressBarOption::COMPLETED>(); }

  void mark_as_completed() {
    get_value<ProgressBarOption::COMPLETED>() = true;
    _print_progress();
  }

private:
  using Settings = std::tuple<
      option::BarWidth,
      option::PrefixText,
      option::PostfixText,
      option::Start,
      option::End,
      option::Fill,
      option::Lead,
      option::Remainder,
      option::MaxPostfixTextLen,
      option::Completed,
      option::ShowPercentage,
      option::ShowElapsedTime,
      option::ShowRemainingTime,
      option::SavedStartTime,
      option::ForegroundColor
      >;

  template <ProgressBarOption id>
  auto get_value() -> decltype((std::get<static_cast<int>(id)>(std::declval<Settings&>()).value)) {
    return std::get<static_cast<int>(id)>(settings_).value;
  }

  template <ProgressBarOption id>
  auto get_value() const -> decltype((std::get<static_cast<int>(id)>(std::declval<const Settings&>()).value)) {
    return std::get<static_cast<int>(id)>(settings_).value;
  }

  float _progress{0};
  Settings settings_;
  std::chrono::nanoseconds _elapsed;
  std::chrono::time_point<std::chrono::high_resolution_clock> _start_time_point;
  std::mutex _mutex;

  template <typename Indicator, size_t count> friend class MultiProgress;
  std::atomic<bool> _multi_progress_mode{false};

  void _save_start_time() {
    auto& show_elapsed_time = get_value<ProgressBarOption::SHOW_ELAPSED_TIME>();
    auto& saved_start_time = get_value<ProgressBarOption::SAVED_START_TIME>();
    auto& show_remaining_time = get_value<ProgressBarOption::SHOW_REMAINING_TIME>();
    if ((show_elapsed_time || show_remaining_time) && !saved_start_time) {
      _start_time_point = std::chrono::high_resolution_clock::now();
      saved_start_time = true;
    }
  }

  void _print_progress(bool from_multi_progress = false) {
    if (_multi_progress_mode && !from_multi_progress) {
      if (_progress > 100.0) {
        get_value<ProgressBarOption::COMPLETED>() = true;
      }
      return;
    }
    std::lock_guard<std::mutex> lock{_mutex};
    auto now = std::chrono::high_resolution_clock::now();
    if (!get_value<ProgressBarOption::COMPLETED>())
      _elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(now - _start_time_point);

    std::cout << termcolor::bold;
    details::set_stream_color(std::cout, get_value<ProgressBarOption::FOREGROUND_COLOR>());
    std::cout << get_value<ProgressBarOption::PREFIX_TEXT>();

    std::cout << get_value<ProgressBarOption::START>();

    details::ProgressScaleWriter writer{std::cout, get_value<ProgressBarOption::BAR_WIDTH>(),
                                        get_value<ProgressBarOption::FILL>(),
                                        get_value<ProgressBarOption::LEAD>(),
                                        get_value<ProgressBarOption::REMAINDER>()};
    writer.write(_progress);

    std::cout << get_value<ProgressBarOption::END>();

    if (get_value<ProgressBarOption::SHOW_PERCENTAGE>()) {
      std::cout << " " << std::min(static_cast<size_t>(_progress), size_t(100)) << "%";
    }

    if (get_value<ProgressBarOption::SHOW_ELAPSED_TIME>()) {
      std::cout << " [";
      details::write_duration(std::cout, _elapsed);
    }

    if (get_value<ProgressBarOption::SHOW_REMAINING_TIME>()) {
      if (get_value<ProgressBarOption::SHOW_ELAPSED_TIME>())
        std::cout << "<";
      else
        std::cout << " [";
      auto eta = std::chrono::nanoseconds(
          _progress > 0 ? static_cast<long long>(_elapsed.count() * 100 / _progress) : 0);
      auto remaining = eta > _elapsed ? (eta - _elapsed) : (_elapsed - eta);
      details::write_duration(std::cout, remaining);
      std::cout << "]";
    } else {
      if (get_value<ProgressBarOption::SHOW_ELAPSED_TIME>())
        std::cout << "]";
    }

    if (get_value<ProgressBarOption::MAX_POSTFIX_TEXT_LEN>() == 0)
      get_value<ProgressBarOption::MAX_POSTFIX_TEXT_LEN>() = 10;
    std::cout << " " << get_value<ProgressBarOption::POSTFIX_TEXT>() << std::string(get_value<ProgressBarOption::MAX_POSTFIX_TEXT_LEN>(), ' ') << "\r";
    std::cout.flush();
    if (_progress > 100.0) {
      get_value<ProgressBarOption::COMPLETED>() = true;
    }
    if (get_value<ProgressBarOption::COMPLETED>() && !from_multi_progress) // Don't std::endl if calling from MultiProgress
      std::cout << termcolor::reset << std::endl;
  }
};

} // namespace indicators
