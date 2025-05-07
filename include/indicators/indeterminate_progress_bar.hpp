
#ifndef INDICATORS_INDETERMINATE_PROGRESS_BAR
#define INDICATORS_INDETERMINATE_PROGRESS_BAR

#include <indicators/color.hpp>
#include <indicators/details/stream_helper.hpp>
#include <indicators/setting.hpp>
#include <indicators/terminal_size.hpp>

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <tuple>
#include <type_traits>
#include <utility>

namespace indicators {

class IndeterminateProgressBar {
  // clang-format off
  using Settings = std::tuple<option::BarWidth, option::Start, option::End,
                              option::Fill, option::Lead,
                              option::PrefixText, option::PostfixText, option::MaxPostfixTextLen,
                              option::Completed,
                              option::ForegroundColor, option::FontStyles,
                              option::Stream>;
  // clang-format on
  enum class Direction { forward, backward };

  Direction direction_{Direction::forward};

public:
  template <typename... Args,
            typename std::enable_if<
                details::are_settings_from_tuple<
                    Settings, typename std::decay<Args>::type...>::value,
                void *>::type = nullptr>
  explicit IndeterminateProgressBar(Args &&...args)
      : settings_(
            details::get<details::ProgressBarOption::bar_width>(
                option::BarWidth{100}, std::forward<Args>(args)...),
            details::get<details::ProgressBarOption::start>(
                option::Start{"["}, std::forward<Args>(args)...),
            details::get<details::ProgressBarOption::end>(
                option::End{"]"}, std::forward<Args>(args)...),
            details::get<details::ProgressBarOption::fill>(
                option::Fill{"."}, std::forward<Args>(args)...),
            details::get<details::ProgressBarOption::lead>(
                option::Lead{"<==>"}, std::forward<Args>(args)...),
            details::get<details::ProgressBarOption::prefix_text>(
                option::PrefixText{}, std::forward<Args>(args)...),
            details::get<details::ProgressBarOption::postfix_text>(
                option::PostfixText{}, std::forward<Args>(args)...),
            details::get<details::ProgressBarOption::max_postfix_text_len>(
                option::MaxPostfixTextLen{0}, std::forward<Args>(args)...),
            details::get<details::ProgressBarOption::completed>(
                option::Completed{false}, std::forward<Args>(args)...),
            details::get<details::ProgressBarOption::foreground_color>(
                option::ForegroundColor{Color::unspecified},
                std::forward<Args>(args)...),
            details::get<details::ProgressBarOption::font_styles>(
                option::FontStyles{std::vector<FontStyle>{}},
                std::forward<Args>(args)...),
            details::get<details::ProgressBarOption::stream>(
                option::Stream{std::cout}, std::forward<Args>(args)...)) {
    // starts with [<==>...........]
    // progress_ = 0

    // ends with   [...........<==>]
    //             ^^^^^^^^^^^^^^^^^ bar_width
    //             ^^^^^^^^^^^^ (bar_width - len(lead))
    // progress_ = bar_width - len(lead)
    max_progress_ = get_value<details::ProgressBarOption::bar_width>() -
                    get_value<details::ProgressBarOption::lead>().size() +
                    get_value<details::ProgressBarOption::start>().size() +
                    get_value<details::ProgressBarOption::end>().size();
  }

  template <typename T, details::ProgressBarOption id>
  void set_option(details::Setting<T, id> &&setting) {
    static_assert(
        !std::is_same<T, typename std::decay<decltype(details::get_value<id>(
                             std::declval<Settings>()))>::type>::value,
        "Setting has wrong type!");
    std::lock_guard<std::mutex> lock(mutex_);
    get_value<id>() = std::move(setting).value;
  }

  template <typename T, details::ProgressBarOption id>
  void set_option(const details::Setting<T, id> &setting) {
    static_assert(
        !std::is_same<T, typename std::decay<decltype(details::get_value<id>(
                             std::declval<Settings>()))>::type>::value,
        "Setting has wrong type!");
    std::lock_guard<std::mutex> lock(mutex_);
    get_value<id>() = setting.value;
  }

  void
  set_option(const details::Setting<
             std::string, details::ProgressBarOption::postfix_text> &setting) {
    std::lock_guard<std::mutex> lock(mutex_);
    get_value<details::ProgressBarOption::postfix_text>() = setting.value;
    if (setting.value.length() >
        get_value<details::ProgressBarOption::max_postfix_text_len>()) {
      get_value<details::ProgressBarOption::max_postfix_text_len>() =
          setting.value.length();
    }
  }

  void set_option(
      details::Setting<std::string, details::ProgressBarOption::postfix_text>
          &&setting) {
    std::lock_guard<std::mutex> lock(mutex_);
    get_value<details::ProgressBarOption::postfix_text>() =
        std::move(setting).value;
    auto &new_value = get_value<details::ProgressBarOption::postfix_text>();
    if (new_value.length() >
        get_value<details::ProgressBarOption::max_postfix_text_len>()) {
      get_value<details::ProgressBarOption::max_postfix_text_len>() =
          new_value.length();
    }
  }

  void tick() {
    {
      std::lock_guard<std::mutex> lock{mutex_};
      if (get_value<details::ProgressBarOption::completed>())
        return;

      tick_ += (direction_ == Direction::forward) ? 1 : -1;
      if (direction_ == Direction::forward && tick_ == max_progress_) {
        // time to go back
        direction_ = Direction::backward;
      } else if (direction_ == Direction::backward && tick_ == 0) {
        direction_ = Direction::forward;
      }
    }
    print_progress();
  }

  bool is_completed() {
    return get_value<details::ProgressBarOption::completed>();
  }

  void mark_as_completed() {
    get_value<details::ProgressBarOption::completed>() = true;
    print_progress();
  }

private:
  template <details::ProgressBarOption id>
  auto get_value()
      -> decltype((details::get_value<id>(std::declval<Settings &>()).value)) {
    return details::get_value<id>(settings_).value;
  }

  template <details::ProgressBarOption id>
  auto get_value() const -> decltype((
      details::get_value<id>(std::declval<const Settings &>()).value)) {
    return details::get_value<id>(settings_).value;
  }

  Settings settings_;
  size_t tick_{0};
  size_t max_progress_;
  std::chrono::nanoseconds elapsed_;
  std::mutex mutex_;

  template <typename Indicator, size_t count> friend class MultiProgress;
  template <typename Indicator> friend class DynamicProgress;
  std::atomic<bool> multi_progress_mode_{false};

  std::pair<std::string, int> get_prefix_text() {
    std::stringstream os;
    os << get_value<details::ProgressBarOption::prefix_text>();
    const auto result = os.str();
    const auto result_size = unicode::display_width(result);
    return {result, result_size};
  }

  std::pair<std::string, int> get_postfix_text() {
    std::stringstream os;
    os << " " << get_value<details::ProgressBarOption::postfix_text>();

    const auto result = os.str();
    const auto result_size = unicode::display_width(result);
    return {result, result_size};
  }

public:
  void print_progress(bool from_multi_progress = false) {
    std::lock_guard<std::mutex> lock{mutex_};

    auto &os = get_value<details::ProgressBarOption::stream>();

    if (multi_progress_mode_ && !from_multi_progress) {
      return;
    }
    if (get_value<details::ProgressBarOption::foreground_color>() !=
        Color::unspecified)
      details::set_stream_color(
          os, get_value<details::ProgressBarOption::foreground_color>());

    for (auto &style : get_value<details::ProgressBarOption::font_styles>())
      details::set_font_style(os, style);

    const auto prefix_pair = get_prefix_text();
    const auto prefix_text = prefix_pair.first;
    const auto prefix_length = prefix_pair.second;
    os << "\r" << prefix_text;

    os << get_value<details::ProgressBarOption::start>();

    details::IndeterminateProgressScaleWriter writer{
        os, get_value<details::ProgressBarOption::bar_width>(),
        get_value<details::ProgressBarOption::fill>(),
        get_value<details::ProgressBarOption::lead>()};
    writer.write(tick_);

    os << get_value<details::ProgressBarOption::end>();

    const auto postfix_pair = get_postfix_text();
    const auto postfix_text = postfix_pair.first;
    const auto postfix_length = postfix_pair.second;
    os << postfix_text;

    // Get length of prefix text and postfix text
    const auto start_length =
        get_value<details::ProgressBarOption::start>().size();
    const auto bar_width = get_value<details::ProgressBarOption::bar_width>();
    const auto end_length = get_value<details::ProgressBarOption::end>().size();
    const auto terminal_width = terminal_size().second;
    // prefix + bar_width + postfix should be <= terminal_width
    const int remaining =
        terminal_width - (prefix_length + start_length + bar_width +
                          end_length + postfix_length);
    if (prefix_length == -1 || postfix_length == -1) {
      os << "\r";
    } else if (remaining > 0) {
      os << std::string(remaining, ' ') << "\r";
    } else if (remaining < 0) {
      // Do nothing. Maybe in the future truncate postfix with ...
    }
    os.flush();

    if (get_value<details::ProgressBarOption::completed>() &&
        !from_multi_progress) // Don't std::endl if calling from MultiProgress
      os << termcolor::reset << std::endl;
  }
};

} // namespace indicators

#endif
