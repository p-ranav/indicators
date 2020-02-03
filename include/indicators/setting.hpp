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

#include <cstddef>
#include <indicators/color.hpp>
#include <string>
#include <type_traits>
#include <utility>


namespace indicators{

enum class ProgressBarOption{
  BAR_WIDTH=0,
  PREFIX_TEXT,
  POSTFIX_TEXT,
  START,
  END,
  FILL,
  LEAD,
  REMAINDER,
  MAX_POSTFIX_TEXT_LEN,
  COMPLETED,
  SHOW_PERCENTAGE,
  SHOW_ELAPSED_TIME,
  SHOW_REMAINING_TIME,
  SAVED_START_TIME,
  FOREGROUND_COLOR,
};

template <typename T, ProgressBarOption Id>
struct Setting{
  template <typename... Args, typename = typename std::enable_if<std::is_constructible<T, Args...>::value>::type>
  explicit Setting(Args&&... args) : value(std::forward<Args>(args)...){}
  Setting(const Setting&) = default;
  Setting(Setting&&) = default;

  static constexpr auto id = Id;
  using type = T;

  T value;
};

template <typename T>
struct is_setting : std::false_type{};

template <ProgressBarOption Id, typename T>
struct is_setting<Setting<T, Id>> : std::true_type{};


template <typename T, typename... Args>
struct are_settings_impl{
  static constexpr bool value = is_setting<T>::value && are_settings_impl<Args...>::value;
};

template <typename T>
struct are_settings_impl<T> : is_setting<T>{};


template <typename... Args>
struct are_settings{
  static constexpr bool value = are_settings_impl<Args...>::value;
};

template <>
struct are_settings<>{
  static constexpr bool value = true;
};

namespace detail{

template <ProgressBarOption Id>
struct always_true{
  static constexpr auto value = true;
};

template <ProgressBarOption Id>
struct get_ret_type;

template<ProgressBarOption Id>
typename get_ret_type<Id>::type get(){
  static_assert(!always_true<Id>::value, "No default value for option specified!");
} // customization point, should never be called

template <ProgressBarOption Id, typename T, typename... Args>
auto get(T&& first, Args&&... tail) -> typename std::enable_if<
                                                (std::decay<T>::type::id == Id),
                                                decltype(std::forward<T>(first))>
                                                ::type{
    return std::forward<T>(first);
}

template <ProgressBarOption Id, typename T, typename... Args>
auto get(T&& first, Args&&... tail) -> typename std::enable_if<
    (std::decay<T>::type::id != Id),
    decltype(get<Id>(std::forward<Args>(tail)...))>::type{
  return get<Id>(std::forward<Args>(tail)...);
}

}

template <ProgressBarOption Id, typename... Args, typename = typename std::enable_if<are_settings<Args...>::value, void>::type>
auto get(Args&&... args) -> decltype(detail::get<Id>(std::forward<Args>(args)...)){
  return detail::get<Id>(std::forward<Args>(args)...);
}

template <ProgressBarOption Id>
using StringSetting = Setting<std::string, Id>;

template <ProgressBarOption Id>
using IntegerSetting = Setting<std::size_t, Id>;

template <ProgressBarOption Id>
using BooleanSetting = Setting<bool, Id>;

namespace option{
  using BarWidth = IntegerSetting<ProgressBarOption::BAR_WIDTH>;
  using PrefixText = StringSetting<ProgressBarOption::PREFIX_TEXT>;
  using PostfixText = StringSetting<ProgressBarOption::POSTFIX_TEXT>;
  using Start = StringSetting<ProgressBarOption::START>;
  using End = StringSetting<ProgressBarOption::END>;
  using Fill = StringSetting<ProgressBarOption::FILL>;
  using Lead = StringSetting<ProgressBarOption::LEAD>;
  using Remainder = StringSetting<ProgressBarOption::REMAINDER>;
  using MaxPostfixTextLen = IntegerSetting<ProgressBarOption::MAX_POSTFIX_TEXT_LEN>;
  using Completed = BooleanSetting<ProgressBarOption::COMPLETED>;
  using ShowPercentage = BooleanSetting<ProgressBarOption::SHOW_PERCENTAGE>;
  using ShowElapsedTime = BooleanSetting<ProgressBarOption::SHOW_ELAPSED_TIME>;
  using ShowRemainingTime = BooleanSetting<ProgressBarOption::SHOW_REMAINING_TIME>;
  using SavedStartTime = BooleanSetting<ProgressBarOption::SAVED_START_TIME>;
  using ForegroundColor = Setting<Color, ProgressBarOption::FOREGROUND_COLOR>;
}

namespace detail{
  template<>
  struct get_ret_type<ProgressBarOption::BAR_WIDTH>{
    using type = ::indicators::option::BarWidth;
  };

  template<> get_ret_type<ProgressBarOption::BAR_WIDTH>::type
  get<ProgressBarOption::BAR_WIDTH>(){
    return indicators::option::BarWidth{100};
  }

  template<>
  struct get_ret_type<ProgressBarOption::PREFIX_TEXT>{
    using type = ::indicators::option::PrefixText;
  };

  template<> get_ret_type<ProgressBarOption::PREFIX_TEXT>::type
  get<ProgressBarOption::PREFIX_TEXT>(){
    return indicators::option::PrefixText{};
  }

  template<>
  struct get_ret_type<ProgressBarOption::POSTFIX_TEXT>{
    using type = ::indicators::option::PostfixText;
  };

  template<> get_ret_type<ProgressBarOption::POSTFIX_TEXT>::type
  get<ProgressBarOption::POSTFIX_TEXT>(){
    return indicators::option::PostfixText{};
  }

  template<>
  struct get_ret_type<ProgressBarOption::START>{
    using type = ::indicators::option::Start;
  };

  template<> get_ret_type<ProgressBarOption::START>::type
  get<ProgressBarOption::START>(){
    return indicators::option::Start{"["};
  }

  template<>
  struct get_ret_type<ProgressBarOption::FILL>{
    using type = ::indicators::option::Fill;
  };

  template<> get_ret_type<ProgressBarOption::FILL>::type
  get<ProgressBarOption::FILL>(){
    return indicators::option::Fill{"="};
  }

  template<>
  struct get_ret_type<ProgressBarOption::LEAD>{
    using type = ::indicators::option::Lead;
  };

  template<> get_ret_type<ProgressBarOption::LEAD>::type
  get<ProgressBarOption::LEAD>(){
    return indicators::option::Lead{">"};
  }

  template<>
  struct get_ret_type<ProgressBarOption::REMAINDER>{
    using type = ::indicators::option::Remainder;
  };

  template<> get_ret_type<ProgressBarOption::REMAINDER>::type
  get<ProgressBarOption::REMAINDER>(){
    return indicators::option::Remainder{" "};
  }

  template<>
  struct get_ret_type<ProgressBarOption::END>{
    using type = ::indicators::option::End;
  };

  template<> get_ret_type<ProgressBarOption::END>::type
  get<ProgressBarOption::END>(){
    return indicators::option::End{"]"};
  }

  template<>
  struct get_ret_type<ProgressBarOption::MAX_POSTFIX_TEXT_LEN>{
    using type = ::indicators::option::MaxPostfixTextLen;
  };

  template<> get_ret_type<ProgressBarOption::MAX_POSTFIX_TEXT_LEN>::type
  get<ProgressBarOption::MAX_POSTFIX_TEXT_LEN>(){
    return indicators::option::MaxPostfixTextLen{0};
  }

  template<>
  struct get_ret_type<ProgressBarOption::COMPLETED>{
    using type = ::indicators::option::Completed;
  };

  template<> get_ret_type<ProgressBarOption::COMPLETED>::type
  get<ProgressBarOption::COMPLETED>(){
    return indicators::option::Completed{false};
  }

  template<>
  struct get_ret_type<ProgressBarOption::SHOW_PERCENTAGE>{
    using type = ::indicators::option::ShowPercentage;
  };

  template<> get_ret_type<ProgressBarOption::SHOW_PERCENTAGE>::type
  get<ProgressBarOption::SHOW_PERCENTAGE>(){
    return indicators::option::ShowPercentage{false};
  }

  template<>
  struct get_ret_type<ProgressBarOption::SHOW_ELAPSED_TIME>{
    using type = ::indicators::option::ShowElapsedTime;
  };

  template<> get_ret_type<ProgressBarOption::SHOW_ELAPSED_TIME>::type
  get<ProgressBarOption::SHOW_ELAPSED_TIME>(){
    return indicators::option::ShowElapsedTime{false};
  }

  template<>
  struct get_ret_type<ProgressBarOption::SHOW_REMAINING_TIME>{
    using type = ::indicators::option::ShowRemainingTime;
  };

  template<> get_ret_type<ProgressBarOption::SHOW_REMAINING_TIME>::type
  get<ProgressBarOption::SHOW_REMAINING_TIME>(){
    return indicators::option::ShowRemainingTime{false};
  }

  template<>
  struct get_ret_type<ProgressBarOption::SAVED_START_TIME>{
    using type = ::indicators::option::SavedStartTime;
  };

  template<> get_ret_type<ProgressBarOption::SAVED_START_TIME>::type
  get<ProgressBarOption::SAVED_START_TIME>(){
    return indicators::option::SavedStartTime{false};
  }

  template<>
  struct get_ret_type<ProgressBarOption::FOREGROUND_COLOR>{
    using type = ::indicators::option::ForegroundColor;
  };

  template<> get_ret_type<ProgressBarOption::FOREGROUND_COLOR>::type
  get<ProgressBarOption::FOREGROUND_COLOR>(){
    return indicators::option::ForegroundColor{::indicators::Color::WHITE};
  }

}
}