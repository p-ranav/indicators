/*
Activity Indicators for Modern C++
https://github.com/p-ranav/indica

Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2019 Pranav Srinivas Kumar <pranav.srinivas.kumar@gmail.com>.

Setting class written by Dawid Pilarski
Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2019 Dawid Pilarski <dawid.pilarski@panicsoftware.com>.

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
#include <algorithm>
#include <atomic>
#include <cassert>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <ostream>
#include <string>
#include <thread>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace indicators {
enum class Color { grey, red, green, yellow, blue, magenta, cyan, white };
}

//!
//! termcolor
//! ~~~~~~~~~
//!
//! termcolor is a header-only c++ library for printing colored messages
//! to the terminal. Written just for fun with a help of the Force.
//!
//! :copyright: (c) 2013 by Ihor Kalnytskyi
//! :license: BSD, see LICENSE for details
//!

#ifndef TERMCOLOR_HPP_
#define TERMCOLOR_HPP_

// the following snippet of code detects the current OS and
// defines the appropriate macro that is used to wrap some
// platform specific things
#if defined(_WIN32) || defined(_WIN64)
#define TERMCOLOR_OS_WINDOWS
#elif defined(__APPLE__)
#define TERMCOLOR_OS_MACOS
#elif defined(__unix__) || defined(__unix)
#define TERMCOLOR_OS_LINUX
#else
#error unsupported platform
#endif

// This headers provides the `isatty()`/`fileno()` functions,
// which are used for testing whether a standart stream refers
// to the terminal. As for Windows, we also need WinApi funcs
// for changing colors attributes of the terminal.
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
#include <unistd.h>
#elif defined(TERMCOLOR_OS_WINDOWS)
#if !defined(NOMINMAX)
#define NOMINMAX
#endif
#include <io.h>
#include <windows.h>
#endif

#include <cstdio>
#include <iostream>

namespace termcolor {
// Forward declaration of the `_internal` namespace.
// All comments are below.
namespace _internal {
// An index to be used to access a private storage of I/O streams. See
// colorize / nocolorize I/O manipulators for details.
static int colorize_index = std::ios_base::xalloc();

inline FILE *get_standard_stream(const std::ostream &stream);
inline bool is_colorized(std::ostream &stream);
inline bool is_atty(const std::ostream &stream);

#if defined(TERMCOLOR_OS_WINDOWS)
inline void win_change_attributes(std::ostream &stream, int foreground, int background = -1);
#endif
} // namespace _internal

inline std::ostream &colorize(std::ostream &stream) {
  stream.iword(_internal::colorize_index) = 1L;
  return stream;
}

inline std::ostream &nocolorize(std::ostream &stream) {
  stream.iword(_internal::colorize_index) = 0L;
  return stream;
}

inline std::ostream &reset(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[00m";
#elif defined(TERMCOLOR_OS_WINDOWS)
    _internal::win_change_attributes(stream, -1, -1);
#endif
  }
  return stream;
}

inline std::ostream &bold(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[1m";
#elif defined(TERMCOLOR_OS_WINDOWS)
#endif
  }
  return stream;
}

inline std::ostream &dark(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[2m";
#elif defined(TERMCOLOR_OS_WINDOWS)
#endif
  }
  return stream;
}

inline std::ostream &italic(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[3m";
#elif defined(TERMCOLOR_OS_WINDOWS)
#endif
  }
  return stream;
}

inline std::ostream &underline(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[4m";
#elif defined(TERMCOLOR_OS_WINDOWS)
#endif
  }
  return stream;
}

inline std::ostream &blink(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[5m";
#elif defined(TERMCOLOR_OS_WINDOWS)
#endif
  }
  return stream;
}

inline std::ostream &reverse(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[7m";
#elif defined(TERMCOLOR_OS_WINDOWS)
#endif
  }
  return stream;
}

inline std::ostream &concealed(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[8m";
#elif defined(TERMCOLOR_OS_WINDOWS)
#endif
  }
  return stream;
}

inline std::ostream &crossed(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[9m";
#elif defined(TERMCOLOR_OS_WINDOWS)
#endif
  }
  return stream;
}

inline std::ostream &grey(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[30m";
#elif defined(TERMCOLOR_OS_WINDOWS)
    _internal::win_change_attributes(stream,
                                     0 // grey (black)
    );
#endif
  }
  return stream;
}

inline std::ostream &red(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[31m";
#elif defined(TERMCOLOR_OS_WINDOWS)
    _internal::win_change_attributes(stream, FOREGROUND_RED);
#endif
  }
  return stream;
}

inline std::ostream &green(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[32m";
#elif defined(TERMCOLOR_OS_WINDOWS)
    _internal::win_change_attributes(stream, FOREGROUND_GREEN);
#endif
  }
  return stream;
}

inline std::ostream &yellow(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[33m";
#elif defined(TERMCOLOR_OS_WINDOWS)
    _internal::win_change_attributes(stream, FOREGROUND_GREEN | FOREGROUND_RED);
#endif
  }
  return stream;
}

inline std::ostream &blue(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[34m";
#elif defined(TERMCOLOR_OS_WINDOWS)
    _internal::win_change_attributes(stream, FOREGROUND_BLUE);
#endif
  }
  return stream;
}

inline std::ostream &magenta(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[35m";
#elif defined(TERMCOLOR_OS_WINDOWS)
    _internal::win_change_attributes(stream, FOREGROUND_BLUE | FOREGROUND_RED);
#endif
  }
  return stream;
}

inline std::ostream &cyan(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[36m";
#elif defined(TERMCOLOR_OS_WINDOWS)
    _internal::win_change_attributes(stream, FOREGROUND_BLUE | FOREGROUND_GREEN);
#endif
  }
  return stream;
}

inline std::ostream &white(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[37m";
#elif defined(TERMCOLOR_OS_WINDOWS)
    _internal::win_change_attributes(stream, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
#endif
  }
  return stream;
}

inline std::ostream &on_grey(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[40m";
#elif defined(TERMCOLOR_OS_WINDOWS)
    _internal::win_change_attributes(stream, -1,
                                     0 // grey (black)
    );
#endif
  }
  return stream;
}

inline std::ostream &on_red(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[41m";
#elif defined(TERMCOLOR_OS_WINDOWS)
    _internal::win_change_attributes(stream, -1, BACKGROUND_RED);
#endif
  }
  return stream;
}

inline std::ostream &on_green(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[42m";
#elif defined(TERMCOLOR_OS_WINDOWS)
    _internal::win_change_attributes(stream, -1, BACKGROUND_GREEN);
#endif
  }
  return stream;
}

inline std::ostream &on_yellow(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[43m";
#elif defined(TERMCOLOR_OS_WINDOWS)
    _internal::win_change_attributes(stream, -1, BACKGROUND_GREEN | BACKGROUND_RED);
#endif
  }
  return stream;
}

inline std::ostream &on_blue(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[44m";
#elif defined(TERMCOLOR_OS_WINDOWS)
    _internal::win_change_attributes(stream, -1, BACKGROUND_BLUE);
#endif
  }
  return stream;
}

inline std::ostream &on_magenta(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[45m";
#elif defined(TERMCOLOR_OS_WINDOWS)
    _internal::win_change_attributes(stream, -1, BACKGROUND_BLUE | BACKGROUND_RED);
#endif
  }
  return stream;
}

inline std::ostream &on_cyan(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[46m";
#elif defined(TERMCOLOR_OS_WINDOWS)
    _internal::win_change_attributes(stream, -1, BACKGROUND_GREEN | BACKGROUND_BLUE);
#endif
  }
  return stream;
}

inline std::ostream &on_white(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[47m";
#elif defined(TERMCOLOR_OS_WINDOWS)
    _internal::win_change_attributes(stream, -1,
                                     BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED);
#endif
  }

  return stream;
}

//! Since C++ hasn't a way to hide something in the header from
//! the outer access, I have to introduce this namespace which
//! is used for internal purpose and should't be access from
//! the user code.
namespace _internal {
//! Since C++ hasn't a true way to extract stream handler
//! from the a given `std::ostream` object, I have to write
//! this kind of hack.
inline FILE *get_standard_stream(const std::ostream &stream) {
  if (&stream == &std::cout)
    return stdout;
  else if ((&stream == &std::cerr) || (&stream == &std::clog))
    return stderr;

  return 0;
}

// Say whether a given stream should be colorized or not. It's always
// true for ATTY streams and may be true for streams marked with
// colorize flag.
inline bool is_colorized(std::ostream &stream) {
  return is_atty(stream) || static_cast<bool>(stream.iword(colorize_index));
}

//! Test whether a given `std::ostream` object refers to
//! a terminal.
inline bool is_atty(const std::ostream &stream) {
  FILE *std_stream = get_standard_stream(stream);

  // Unfortunately, fileno() ends with segmentation fault
  // if invalid file descriptor is passed. So we need to
  // handle this case gracefully and assume it's not a tty
  // if standard stream is not detected, and 0 is returned.
  if (!std_stream)
    return false;

#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
  return ::isatty(fileno(std_stream));
#elif defined(TERMCOLOR_OS_WINDOWS)
  return ::_isatty(_fileno(std_stream));
#endif
}

#if defined(TERMCOLOR_OS_WINDOWS)
//! Change Windows Terminal colors attribute. If some
//! parameter is `-1` then attribute won't changed.
inline void win_change_attributes(std::ostream &stream, int foreground, int background) {
  // yeah, i know.. it's ugly, it's windows.
  static WORD defaultAttributes = 0;

  // Windows doesn't have ANSI escape sequences and so we use special
  // API to change Terminal output color. That means we can't
  // manipulate colors by means of "std::stringstream" and hence
  // should do nothing in this case.
  if (!_internal::is_atty(stream))
    return;

  // get terminal handle
  HANDLE hTerminal = INVALID_HANDLE_VALUE;
  if (&stream == &std::cout)
    hTerminal = GetStdHandle(STD_OUTPUT_HANDLE);
  else if (&stream == &std::cerr)
    hTerminal = GetStdHandle(STD_ERROR_HANDLE);

  // save default terminal attributes if it unsaved
  if (!defaultAttributes) {
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (!GetConsoleScreenBufferInfo(hTerminal, &info))
      return;
    defaultAttributes = info.wAttributes;
  }

  // restore all default settings
  if (foreground == -1 && background == -1) {
    SetConsoleTextAttribute(hTerminal, defaultAttributes);
    return;
  }

  // get current settings
  CONSOLE_SCREEN_BUFFER_INFO info;
  if (!GetConsoleScreenBufferInfo(hTerminal, &info))
    return;

  if (foreground != -1) {
    info.wAttributes &= ~(info.wAttributes & 0x0F);
    info.wAttributes |= static_cast<WORD>(foreground);
  }

  if (background != -1) {
    info.wAttributes &= ~(info.wAttributes & 0xF0);
    info.wAttributes |= static_cast<WORD>(background);
  }

  SetConsoleTextAttribute(hTerminal, info.wAttributes);
}
#endif // TERMCOLOR_OS_WINDOWS

} // namespace _internal

} // namespace termcolor

#undef TERMCOLOR_OS_WINDOWS
#undef TERMCOLOR_OS_MACOS
#undef TERMCOLOR_OS_LINUX

#endif // TERMCOLOR_HPP_

namespace indicators {
namespace details {

inline void set_stream_color(std::ostream &os, Color color) {
  switch (color) {
  case Color::grey:
    os << termcolor::grey;
    break;
  case Color::red:
    os << termcolor::red;
    break;
  case Color::green:
    os << termcolor::green;
    break;
  case Color::yellow:
    os << termcolor::yellow;
    break;
  case Color::blue:
    os << termcolor::blue;
    break;
  case Color::magenta:
    os << termcolor::magenta;
    break;
  case Color::cyan:
    os << termcolor::cyan;
    break;
  case Color::white:
    os << termcolor::white;
    break;
  default:
    assert(false);
  }
}

inline std::ostream &write_duration(std::ostream &os, std::chrono::nanoseconds ns) {
  using namespace std;
  using namespace std::chrono;
  using days = duration<int, ratio<86400>>;
  char fill = os.fill();
  os.fill('0');
  auto d = duration_cast<days>(ns);
  ns -= d;
  auto h = duration_cast<hours>(ns);
  ns -= h;
  auto m = duration_cast<minutes>(ns);
  ns -= m;
  auto s = duration_cast<seconds>(ns);
  if (d.count() > 0)
    os << setw(2) << d.count() << "d:";
  if (h.count() > 0)
    os << setw(2) << h.count() << "h:";
  os << setw(2) << m.count() << "m:" << setw(2) << s.count() << 's';
  os.fill(fill);
  return os;
}

class BlockProgressScaleWriter {
public:
  BlockProgressScaleWriter(std::ostream &os, size_t bar_width) : os(os), bar_width(bar_width) {}

  std::ostream &write(float progress) {
    std::string fill_text{"█"};
    std::vector<std::string> lead_characters{" ", "▏", "▎", "▍", "▌", "▋", "▊", "▉"};
    auto value = std::min(1.0f, std::max(0.0f, progress / 100.0f));
    auto whole_width = std::floor(value * bar_width);
    auto remainder_width = fmod((value * bar_width), 1.0f);
    auto part_width = std::floor(remainder_width * lead_characters.size());
    std::string lead_text = lead_characters[size_t(part_width)];
    if ((bar_width - whole_width - 1) < 0)
      lead_text = "";
    for (size_t i = 0; i < whole_width; ++i)
      os << fill_text;
    os << lead_text;
    for (size_t i = 0; i < (bar_width - whole_width - 1); ++i)
      os << " ";
    return os;
  }

private:
  std::ostream &os;
  size_t bar_width = 0;
};

class ProgressScaleWriter {
public:
  ProgressScaleWriter(std::ostream &os, size_t bar_width, const std::string &fill,
                      const std::string &lead, const std::string &remainder)
      : os(os), bar_width(bar_width), fill(fill), lead(lead), remainder(remainder) {}

  std::ostream &write(float progress) {
    auto pos = static_cast<size_t>(progress * static_cast<float>(bar_width) / 100.0);
    for (size_t i = 0; i < bar_width; ++i) {
      if (i < pos)
        os << fill;
      else if (i == pos)
        os << lead;
      else
        os << remainder;
    }
    return os;
  }

private:
  std::ostream &os;
  size_t bar_width = 0;
  std::string fill;
  std::string lead;
  std::string remainder;
};

} // namespace details
} // namespace indicators

/*
Activity Indicators for Modern C++
https://github.com/p-ranav/indicators

Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2019 Dawid Pilarski <dawid.pilarski@panicsoftware.com>.

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

namespace indicators {

namespace details {

template <bool condition> struct if_else;

template <> struct if_else<true> { using type = std::true_type; };

template <> struct if_else<false> { using type = std::false_type; };

template <bool condition, typename True, typename False> struct if_else_type;

template <typename True, typename False> struct if_else_type<true, True, False> {
  using type = True;
};

template <typename True, typename False> struct if_else_type<false, True, False> {
  using type = False;
};

template <typename... Ops> struct conjuction;

template <> struct conjuction<> : std::true_type {};

template <typename Op, typename... TailOps>
struct conjuction<Op, TailOps...>
    : if_else_type<!Op::value, std::false_type, conjuction<TailOps...>>::type {};

template <typename... Ops> struct disjunction;

template <> struct disjunction<> : std::false_type {};

template <typename Op, typename... TailOps>
struct disjunction<Op, TailOps...>
    : if_else_type<Op::value, std::true_type, disjunction<TailOps...>>::type {};

enum class ProgressBarOption {
  bar_width = 0,
  prefix_text,
  postfix_text,
  start,
  end,
  fill,
  lead,
  remainder,
  max_postfix_text_len,
  completed,
  show_percentage,
  show_elapsed_time,
  show_remaining_time,
  saved_start_time,
  foreground_color,
  spinner_show,
  spinner_states
};

template <typename T, ProgressBarOption Id> struct Setting {
  template <typename... Args,
            typename = typename std::enable_if<std::is_constructible<T, Args...>::value>::type>
  explicit Setting(Args &&... args) : value(std::forward<Args>(args)...) {}
  Setting(const Setting &) = default;
  Setting(Setting &&) = default;

  static constexpr auto id = Id;
  using type = T;

  T value{};
};

template <typename T> struct is_setting : std::false_type {};

template <ProgressBarOption Id, typename T> struct is_setting<Setting<T, Id>> : std::true_type {};

template <typename... Args>
struct are_settings : if_else<conjuction<is_setting<Args>...>::value>::type {};

template <> struct are_settings<> : std::true_type {};

template <typename Setting, typename Tuple> struct is_setting_from_tuple;

template <typename Setting> struct is_setting_from_tuple<Setting, std::tuple<>> : std::true_type {};

template <typename Setting, typename... TupleTypes>
struct is_setting_from_tuple<Setting, std::tuple<TupleTypes...>>
    : if_else<disjunction<std::is_same<Setting, TupleTypes>...>::value>::type {};

template <typename Tuple, typename... Settings>
struct are_settings_from_tuple
    : if_else<conjuction<is_setting_from_tuple<Settings, Tuple>...>::value>::type {};

template <ProgressBarOption Id> struct always_true { static constexpr auto value = true; };

template <ProgressBarOption Id, typename Default> Default &&get_impl(Default &&def) {
  return std::forward<Default>(def);
}

template <ProgressBarOption Id, typename Default, typename T, typename... Args>
auto get_impl(Default &&def, T &&first, Args &&... tail) ->
    typename std::enable_if<(std::decay<T>::type::id == Id),
                            decltype(std::forward<T>(first))>::type {
  return std::forward<T>(first);
}

template <ProgressBarOption Id, typename Default, typename T, typename... Args>
auto get_impl(Default &&def, T &&first, Args &&... tail) ->
    typename std::enable_if<(std::decay<T>::type::id != Id),
                            decltype(get_impl<Id>(std::forward<Default>(def),
                                                  std::forward<Args>(tail)...))>::type {
  return get_impl<Id>(std::forward<Default>(def), std::forward<Args>(tail)...);
}

template <ProgressBarOption Id, typename Default, typename... Args,
          typename = typename std::enable_if<are_settings<Args...>::value, void>::type>
auto get(Default &&def, Args &&... args)
    -> decltype(details::get_impl<Id>(std::forward<Default>(def), std::forward<Args>(args)...)) {
  return details::get_impl<Id>(std::forward<Default>(def), std::forward<Args>(args)...);
}

template <ProgressBarOption Id> using StringSetting = Setting<std::string, Id>;

template <ProgressBarOption Id> using IntegerSetting = Setting<std::size_t, Id>;

template <ProgressBarOption Id> using BooleanSetting = Setting<bool, Id>;

template <ProgressBarOption Id, typename Tuple, std::size_t counter = 0> struct option_idx;

template <ProgressBarOption Id, typename T, typename... Settings, std::size_t counter>
struct option_idx<Id, std::tuple<T, Settings...>, counter>
    : if_else_type<(Id == T::id), std::integral_constant<std::size_t, counter>,
                   option_idx<Id, std::tuple<Settings...>, counter + 1>>::type {};

template <ProgressBarOption Id, std::size_t counter> struct option_idx<Id, std::tuple<>, counter> {
  static_assert(always_true<(ProgressBarOption)Id>::value, "No such option was found");
};

template <ProgressBarOption Id, typename Settings>
auto get_value(Settings &&settings)
    -> decltype((std::get<option_idx<Id, typename std::decay<Settings>::type>::value>(
        std::declval<Settings &&>()))) {
  return std::get<option_idx<Id, typename std::decay<Settings>::type>::value>(
      std::forward<Settings>(settings));
}

} // namespace details

namespace option {
using BarWidth = details::IntegerSetting<details::ProgressBarOption::bar_width>;
using PrefixText = details::StringSetting<details::ProgressBarOption::prefix_text>;
using PostfixText = details::StringSetting<details::ProgressBarOption::postfix_text>;
using Start = details::StringSetting<details::ProgressBarOption::start>;
using End = details::StringSetting<details::ProgressBarOption::end>;
using Fill = details::StringSetting<details::ProgressBarOption::fill>;
using Lead = details::StringSetting<details::ProgressBarOption::lead>;
using Remainder = details::StringSetting<details::ProgressBarOption::remainder>;
using MaxPostfixTextLen = details::IntegerSetting<details::ProgressBarOption::max_postfix_text_len>;
using Completed = details::BooleanSetting<details::ProgressBarOption::completed>;
using ShowPercentage = details::BooleanSetting<details::ProgressBarOption::show_percentage>;
using ShowElapsedTime = details::BooleanSetting<details::ProgressBarOption::show_elapsed_time>;
using ShowRemainingTime = details::BooleanSetting<details::ProgressBarOption::show_remaining_time>;
using SavedStartTime = details::BooleanSetting<details::ProgressBarOption::saved_start_time>;
using ForegroundColor = details::Setting<Color, details::ProgressBarOption::foreground_color>;
using ShowSpinner = details::BooleanSetting<details::ProgressBarOption::spinner_show>;
using SpinnerStates =
    details::Setting<std::vector<std::string>, details::ProgressBarOption::spinner_states>;
} // namespace option
} // namespace indicators

namespace indicators {

class ProgressBar {
  using Settings =
      std::tuple<option::BarWidth, option::PrefixText, option::PostfixText, option::Start,
                 option::End, option::Fill, option::Lead, option::Remainder,
                 option::MaxPostfixTextLen, option::Completed, option::ShowPercentage,
                 option::ShowElapsedTime, option::ShowRemainingTime, option::SavedStartTime,
                 option::ForegroundColor>;

public:
  template <typename... Args,
            typename std::enable_if<details::are_settings_from_tuple<
                                        Settings, typename std::decay<Args>::type...>::value,
                                    void *>::type = nullptr>
  explicit ProgressBar(Args &&... args)
      : settings_(details::get<details::ProgressBarOption::bar_width>(option::BarWidth{100},
                                                                      std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::prefix_text>(
                      option::PrefixText{}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::postfix_text>(
                      option::PostfixText{}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::start>(option::Start{"["},
                                                                  std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::end>(option::End{"]"},
                                                                std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::fill>(option::Fill{"="},
                                                                 std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::lead>(option::Lead{">"},
                                                                 std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::remainder>(option::Remainder{" "},
                                                                      std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::max_postfix_text_len>(
                      option::MaxPostfixTextLen{0}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::completed>(option::Completed{false},
                                                                      std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::show_percentage>(
                      option::ShowPercentage{false}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::show_elapsed_time>(
                      option::ShowElapsedTime{false}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::show_remaining_time>(
                      option::ShowRemainingTime{false}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::saved_start_time>(
                      option::SavedStartTime{false}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::foreground_color>(
                      option::ForegroundColor{Color::white}, std::forward<Args>(args)...)) {}

  template <typename T, details::ProgressBarOption id>
  void set_option(details::Setting<T, id> &&setting) {
    static_assert(!std::is_same<T, typename std::decay<decltype(details::get_value<id>(
                                       std::declval<Settings>()))>::type>::value,
                  "Setting has wrong type!");
    std::lock_guard<std::mutex> lock(mutex_);
    get_value<id>() = std::move(setting).value;
  }

  template <typename T, details::ProgressBarOption id>
  void set_option(const details::Setting<T, id> &setting) {
    static_assert(!std::is_same<T, typename std::decay<decltype(details::get_value<id>(
                                       std::declval<Settings>()))>::type>::value,
                  "Setting has wrong type!");
    std::lock_guard<std::mutex> lock(mutex_);
    get_value<id>() = setting.value;
  }

  void set_option(
      const details::Setting<std::string, details::ProgressBarOption::postfix_text> &setting) {
    std::lock_guard<std::mutex> lock(mutex_);
    get_value<details::ProgressBarOption::postfix_text>() = setting.value;
    if (setting.value.length() > get_value<details::ProgressBarOption::max_postfix_text_len>()) {
      get_value<details::ProgressBarOption::max_postfix_text_len>() = setting.value.length();
    }
  }

  void
  set_option(details::Setting<std::string, details::ProgressBarOption::postfix_text> &&setting) {
    std::lock_guard<std::mutex> lock(mutex_);
    get_value<details::ProgressBarOption::postfix_text>() = std::move(setting).value;
    auto &new_value = get_value<details::ProgressBarOption::postfix_text>();
    if (new_value.length() > get_value<details::ProgressBarOption::max_postfix_text_len>()) {
      get_value<details::ProgressBarOption::max_postfix_text_len>() = new_value.length();
    }
  }

  void set_progress(float new_progress) {
    {
      std::lock_guard<std::mutex> lck(mutex_);
      progress_ = new_progress;
    }

    save_start_time();
    print_progress();
  }

  void tick() {
    {
      std::lock_guard<std::mutex> lock{mutex_};
      progress_ += 1;
    }
    save_start_time();
    print_progress();
  }

  size_t current() {
    std::lock_guard<std::mutex> lock{mutex_};
    return std::min(static_cast<size_t>(progress_), size_t(100));
  }

  bool is_completed() const { return get_value<details::ProgressBarOption::completed>(); }

  void mark_as_completed() {
    get_value<details::ProgressBarOption::completed>() = true;
    print_progress();
  }

private:
  template <details::ProgressBarOption id>
  auto get_value() -> decltype((details::get_value<id>(std::declval<Settings &>()).value)) {
    return details::get_value<id>(settings_).value;
  }

  template <details::ProgressBarOption id>
  auto get_value() const
      -> decltype((details::get_value<id>(std::declval<const Settings &>()).value)) {
    return details::get_value<id>(settings_).value;
  }

  float progress_{0};
  Settings settings_;
  std::chrono::nanoseconds elapsed_;
  std::chrono::time_point<std::chrono::high_resolution_clock> start_time_point_;
  std::mutex mutex_;

  template <typename Indicator, size_t count> friend class MultiProgress;
  std::atomic<bool> multi_progress_mode_{false};

  void save_start_time() {
    auto &show_elapsed_time = get_value<details::ProgressBarOption::show_elapsed_time>();
    auto &saved_start_time = get_value<details::ProgressBarOption::saved_start_time>();
    auto &show_remaining_time = get_value<details::ProgressBarOption::show_remaining_time>();
    if ((show_elapsed_time || show_remaining_time) && !saved_start_time) {
      start_time_point_ = std::chrono::high_resolution_clock::now();
      saved_start_time = true;
    }
  }

  void print_progress(bool from_multi_progress = false) {
    if (multi_progress_mode_ && !from_multi_progress) {
      if (progress_ > 100.0) {
        get_value<details::ProgressBarOption::completed>() = true;
      }
      return;
    }
    std::lock_guard<std::mutex> lock{mutex_};
    auto now = std::chrono::high_resolution_clock::now();
    if (!get_value<details::ProgressBarOption::completed>())
      elapsed_ = std::chrono::duration_cast<std::chrono::nanoseconds>(now - start_time_point_);

    std::cout << termcolor::bold;
    details::set_stream_color(std::cout, get_value<details::ProgressBarOption::foreground_color>());
    std::cout << get_value<details::ProgressBarOption::prefix_text>();

    std::cout << get_value<details::ProgressBarOption::start>();

    details::ProgressScaleWriter writer{std::cout,
                                        get_value<details::ProgressBarOption::bar_width>(),
                                        get_value<details::ProgressBarOption::fill>(),
                                        get_value<details::ProgressBarOption::lead>(),
                                        get_value<details::ProgressBarOption::remainder>()};
    writer.write(progress_);

    std::cout << get_value<details::ProgressBarOption::end>();

    if (get_value<details::ProgressBarOption::show_percentage>()) {
      std::cout << " " << std::min(static_cast<size_t>(progress_), size_t(100)) << "%";
    }

    if (get_value<details::ProgressBarOption::show_elapsed_time>()) {
      std::cout << " [";
      details::write_duration(std::cout, elapsed_);
    }

    if (get_value<details::ProgressBarOption::show_remaining_time>()) {
      if (get_value<details::ProgressBarOption::show_elapsed_time>())
        std::cout << "<";
      else
        std::cout << " [";
      auto eta = std::chrono::nanoseconds(
          progress_ > 0 ? static_cast<long long>(elapsed_.count() * 100 / progress_) : 0);
      auto remaining = eta > elapsed_ ? (eta - elapsed_) : (elapsed_ - eta);
      details::write_duration(std::cout, remaining);
      std::cout << "]";
    } else {
      if (get_value<details::ProgressBarOption::show_elapsed_time>())
        std::cout << "]";
    }

    if (get_value<details::ProgressBarOption::max_postfix_text_len>() == 0)
      get_value<details::ProgressBarOption::max_postfix_text_len>() = 10;
    std::cout << " " << get_value<details::ProgressBarOption::postfix_text>()
              << std::string(get_value<details::ProgressBarOption::max_postfix_text_len>(), ' ')
              << "\r";
    std::cout.flush();
    if (progress_ > 100.0) {
      get_value<details::ProgressBarOption::completed>() = true;
    }
    if (get_value<details::ProgressBarOption::completed>() &&
        !from_multi_progress) // Don't std::endl if calling from MultiProgress
      std::cout << termcolor::reset << std::endl;
  }
};

} // namespace indicators

namespace indicators {

class BlockProgressBar {
  using Settings = std::tuple<option::ForegroundColor, option::BarWidth, option::Start, option::End,
                              option::PrefixText, option::PostfixText, option::ShowPercentage,
                              option::ShowElapsedTime, option::ShowRemainingTime, option::Completed,
                              option::SavedStartTime, option::MaxPostfixTextLen>;

public:
  template <typename... Args,
            typename std::enable_if<details::are_settings_from_tuple<
                                        Settings, typename std::decay<Args>::type...>::value,
                                    void *>::type = nullptr>
  explicit BlockProgressBar(Args &&... args)
      : settings_(details::get<details::ProgressBarOption::foreground_color>(
                      option::ForegroundColor{Color::white}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::bar_width>(option::BarWidth{100},
                                                                      std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::start>(option::Start{"["},
                                                                  std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::end>(option::End{"]"},
                                                                std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::prefix_text>(
                      option::PrefixText{""}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::postfix_text>(
                      option::PostfixText{""}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::show_percentage>(
                      option::ShowPercentage{true}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::show_elapsed_time>(
                      option::ShowElapsedTime{false}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::show_remaining_time>(
                      option::ShowRemainingTime{false}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::completed>(option::Completed{false},
                                                                      std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::saved_start_time>(
                      option::SavedStartTime{false}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::max_postfix_text_len>(
                      option::MaxPostfixTextLen{0}, std::forward<Args>(args)...)) {}

  template <typename T, details::ProgressBarOption id>
  void set_option(details::Setting<T, id> &&setting) {
    static_assert(!std::is_same<T, typename std::decay<decltype(details::get_value<id>(
                                       std::declval<Settings>()))>::type>::value,
                  "Setting has wrong type!");
    std::lock_guard<std::mutex> lock(mutex_);
    get_value<id>() = std::move(setting).value;
  }

  template <typename T, details::ProgressBarOption id>
  void set_option(const details::Setting<T, id> &setting) {
    static_assert(!std::is_same<T, typename std::decay<decltype(details::get_value<id>(
                                       std::declval<Settings>()))>::type>::value,
                  "Setting has wrong type!");
    std::lock_guard<std::mutex> lock(mutex_);
    get_value<id>() = setting.value;
  }

  void set_option(
      const details::Setting<std::string, details::ProgressBarOption::postfix_text> &setting) {
    std::lock_guard<std::mutex> lock(mutex_);
    get_value<details::ProgressBarOption::postfix_text>() = setting.value;
    if (setting.value.length() > get_value<details::ProgressBarOption::max_postfix_text_len>()) {
      get_value<details::ProgressBarOption::max_postfix_text_len>() = setting.value.length();
    }
  }

  void
  set_option(details::Setting<std::string, details::ProgressBarOption::postfix_text> &&setting) {
    std::lock_guard<std::mutex> lock(mutex_);
    get_value<details::ProgressBarOption::postfix_text>() = std::move(setting).value;
    auto &new_value = get_value<details::ProgressBarOption::postfix_text>();
    if (new_value.length() > get_value<details::ProgressBarOption::max_postfix_text_len>()) {
      get_value<details::ProgressBarOption::max_postfix_text_len>() = new_value.length();
    }
  }

  void set_progress(float value) {
    {
      std::lock_guard<std::mutex> lock{mutex_};
      progress_ = value;
    }
    save_start_time();
    print_progress();
  }

  void tick() {
    {
      std::lock_guard<std::mutex> lock{mutex_};
      progress_ += 1;
    }
    save_start_time();
    print_progress();
  }

  size_t current() {
    std::lock_guard<std::mutex> lock{mutex_};
    return std::min(static_cast<size_t>(progress_), size_t(100));
  }

  bool is_completed() const { return get_value<details::ProgressBarOption::completed>(); }

  void mark_as_completed() {
    get_value<details::ProgressBarOption::completed>() = true;
    print_progress();
  }

private:
  template <details::ProgressBarOption id>
  auto get_value() -> decltype((details::get_value<id>(std::declval<Settings &>()).value)) {
    return details::get_value<id>(settings_).value;
  }

  template <details::ProgressBarOption id>
  auto get_value() const
      -> decltype((details::get_value<id>(std::declval<const Settings &>()).value)) {
    return details::get_value<id>(settings_).value;
  }

  Settings settings_;
  float progress_{0.0};
  std::chrono::time_point<std::chrono::high_resolution_clock> start_time_point_;
  std::mutex mutex_;

  template <typename Indicator, size_t count> friend class MultiProgress;
  std::atomic<bool> multi_progress_mode_{false};

  void save_start_time() {
    auto &show_elapsed_time = get_value<details::ProgressBarOption::show_elapsed_time>();
    auto &saved_start_time = get_value<details::ProgressBarOption::saved_start_time>();
    auto &show_remaining_time = get_value<details::ProgressBarOption::show_remaining_time>();
    if ((show_elapsed_time || show_remaining_time) && !saved_start_time) {
      start_time_point_ = std::chrono::high_resolution_clock::now();
      saved_start_time = true;
    }
  }

  void print_progress(bool from_multi_progress = false) {
    if (multi_progress_mode_ && !from_multi_progress) {
      if (progress_ > 100.0) {
        get_value<details::ProgressBarOption::completed>() = true;
      }
      return;
    }
    std::lock_guard<std::mutex> lock{mutex_};
    auto now = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(now - start_time_point_);

    std::cout << termcolor::bold;
    details::set_stream_color(std::cout, get_value<details::ProgressBarOption::foreground_color>());
    std::cout << get_value<details::ProgressBarOption::prefix_text>();
    std::cout << get_value<details::ProgressBarOption::start>();

    details::BlockProgressScaleWriter writer{std::cout,
                                             get_value<details::ProgressBarOption::bar_width>()};
    writer.write(progress_);

    std::cout << get_value<details::ProgressBarOption::end>();
    if (get_value<details::ProgressBarOption::show_percentage>()) {
      std::cout << " " << std::min(static_cast<size_t>(progress_), size_t(100)) << "%";
    }

    if (get_value<details::ProgressBarOption::show_elapsed_time>()) {
      std::cout << " [";
      details::write_duration(std::cout, elapsed);
    }

    if (get_value<details::ProgressBarOption::show_remaining_time>()) {
      if (get_value<details::ProgressBarOption::show_elapsed_time>())
        std::cout << "<";
      else
        std::cout << " [";
      auto eta = std::chrono::nanoseconds(
          progress_ > 0 ? static_cast<long long>(elapsed.count() * 100 / progress_) : 0);
      auto remaining = eta > elapsed ? (eta - elapsed) : (elapsed - eta);
      details::write_duration(std::cout, remaining);
      std::cout << "]";
    } else {
      if (get_value<details::ProgressBarOption::show_elapsed_time>())
        std::cout << "]";
    }

    if (get_value<details::ProgressBarOption::max_postfix_text_len>() == 0)
      get_value<details::ProgressBarOption::max_postfix_text_len>() = 10;
    std::cout << " " << get_value<details::ProgressBarOption::postfix_text>()
              << std::string(get_value<details::ProgressBarOption::max_postfix_text_len>(), ' ')
              << "\r";
    std::cout.flush();
    if (progress_ > 100.0) {
      get_value<details::ProgressBarOption::completed>() = true;
    }
    if (get_value<details::ProgressBarOption::completed>() &&
        !from_multi_progress) // Don't std::endl if calling from MultiProgress
      std::cout << termcolor::reset << std::endl;
  }
};

} // namespace indicators

namespace indicators {

class ProgressSpinner {
  using Settings =
      std::tuple<option::ForegroundColor, option::PrefixText, option::PostfixText,
                 option::ShowPercentage, option::ShowElapsedTime, option::ShowRemainingTime,
                 option::ShowSpinner, option::SavedStartTime, option::Completed,
                 option::MaxPostfixTextLen, option::SpinnerStates>;

public:
  template <typename... Args,
            typename std::enable_if<details::are_settings_from_tuple<
                                        Settings, typename std::decay<Args>::type...>::value,
                                    void *>::type = nullptr>
  explicit ProgressSpinner(Args &&... args)
      : settings_(details::get<details::ProgressBarOption::foreground_color>(
                      option::ForegroundColor{Color::white}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::prefix_text>(
                      option::PrefixText{}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::postfix_text>(
                      option::PostfixText{}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::show_percentage>(
                      option::ShowPercentage{true}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::show_elapsed_time>(
                      option::ShowElapsedTime{false}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::show_remaining_time>(
                      option::ShowRemainingTime{false}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::spinner_show>(
                      option::ShowSpinner{true}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::saved_start_time>(
                      option::SavedStartTime{false}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::completed>(option::Completed{false},
                                                                      std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::max_postfix_text_len>(
                      option::MaxPostfixTextLen{0}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::spinner_states>(
                      option::SpinnerStates{std::vector<std::string>{"⠋", "⠙", "⠹", "⠸", "⠼", "⠴",
                                                                     "⠦", "⠧", "⠇", "⠏"}},
                      std::forward<Args>(args)...)) {}

  template <typename T, details::ProgressBarOption id>
  void set_option(details::Setting<T, id> &&setting) {
    static_assert(!std::is_same<T, typename std::decay<decltype(details::get_value<id>(
                                       std::declval<Settings>()))>::type>::value,
                  "Setting has wrong type!");
    std::lock_guard<std::mutex> lock(mutex_);
    get_value<id>() = std::move(setting).value;
  }

  template <typename T, details::ProgressBarOption id>
  void set_option(const details::Setting<T, id> &setting) {
    static_assert(!std::is_same<T, typename std::decay<decltype(details::get_value<id>(
                                       std::declval<Settings>()))>::type>::value,
                  "Setting has wrong type!");
    std::lock_guard<std::mutex> lock(mutex_);
    get_value<id>() = setting.value;
  }

  void set_option(
      const details::Setting<std::string, details::ProgressBarOption::postfix_text> &setting) {
    std::lock_guard<std::mutex> lock(mutex_);
    get_value<details::ProgressBarOption::postfix_text>() = setting.value;
    if (setting.value.length() > get_value<details::ProgressBarOption::max_postfix_text_len>()) {
      get_value<details::ProgressBarOption::max_postfix_text_len>() = setting.value.length();
    }
  }

  void
  set_option(details::Setting<std::string, details::ProgressBarOption::postfix_text> &&setting) {
    std::lock_guard<std::mutex> lock(mutex_);
    get_value<details::ProgressBarOption::postfix_text>() = std::move(setting).value;
    auto &new_value = get_value<details::ProgressBarOption::postfix_text>();
    if (new_value.length() > get_value<details::ProgressBarOption::max_postfix_text_len>()) {
      get_value<details::ProgressBarOption::max_postfix_text_len>() = new_value.length();
    }
  }

  void set_progress(float value) {
    {
      std::lock_guard<std::mutex> lock{mutex_};
      progress_ = value;
    }
    save_start_time();
    print_progress();
  }

  void tick() {
    {
      std::lock_guard<std::mutex> lock{mutex_};
      progress_ += 1;
    }
    save_start_time();
    print_progress();
  }

  size_t current() {
    std::lock_guard<std::mutex> lock{mutex_};
    return std::min(static_cast<size_t>(progress_), size_t(100));
  }

  bool is_completed() const { return get_value<details::ProgressBarOption::completed>(); }

  void mark_as_completed() {
    get_value<details::ProgressBarOption::completed>() = true;
    print_progress();
  }

private:
  Settings settings_;
  float progress_{0.0};
  size_t index_{0};
  std::chrono::time_point<std::chrono::high_resolution_clock> start_time_point_;
  std::mutex mutex_;

  template <details::ProgressBarOption id>
  auto get_value() -> decltype((details::get_value<id>(std::declval<Settings &>()).value)) {
    return details::get_value<id>(settings_).value;
  }

  template <details::ProgressBarOption id>
  auto get_value() const
      -> decltype((details::get_value<id>(std::declval<const Settings &>()).value)) {
    return details::get_value<id>(settings_).value;
  }

  void save_start_time() {
    auto &show_elapsed_time = get_value<details::ProgressBarOption::show_elapsed_time>();
    auto &show_remaining_time = get_value<details::ProgressBarOption::show_remaining_time>();
    auto &saved_start_time = get_value<details::ProgressBarOption::saved_start_time>();
    if ((show_elapsed_time || show_remaining_time) && !saved_start_time) {
      start_time_point_ = std::chrono::high_resolution_clock::now();
      saved_start_time = true;
    }
  }

  void print_progress() {
    std::lock_guard<std::mutex> lock{mutex_};
    auto now = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(now - start_time_point_);

    std::cout << termcolor::bold;
    details::set_stream_color(std::cout, get_value<details::ProgressBarOption::foreground_color>());
    std::cout << get_value<details::ProgressBarOption::prefix_text>();
    if (get_value<details::ProgressBarOption::spinner_show>())
      std::cout << get_value<details::ProgressBarOption::spinner_states>()
              [index_ % get_value<details::ProgressBarOption::spinner_states>().size()];
    if (get_value<details::ProgressBarOption::show_percentage>()) {
      std::cout << " " << std::min(static_cast<size_t>(progress_), size_t(100)) << "%";
    }

    if (get_value<details::ProgressBarOption::show_elapsed_time>()) {
      std::cout << " [";
      details::write_duration(std::cout, elapsed);
    }

    if (get_value<details::ProgressBarOption::show_remaining_time>()) {
      if (get_value<details::ProgressBarOption::show_elapsed_time>())
        std::cout << "<";
      else
        std::cout << " [";
      auto eta = std::chrono::nanoseconds(
          progress_ > 0 ? static_cast<long long>(elapsed.count() * 100 / progress_) : 0);
      auto remaining = eta > elapsed ? (eta - elapsed) : (elapsed - eta);
      details::write_duration(std::cout, remaining);
      std::cout << "]";
    } else {
      if (get_value<details::ProgressBarOption::show_elapsed_time>())
        std::cout << "]";
    }

    if (get_value<details::ProgressBarOption::max_postfix_text_len>() == 0)
      get_value<details::ProgressBarOption::max_postfix_text_len>() = 10;
    std::cout << " " << get_value<details::ProgressBarOption::postfix_text>()
              << std::string(get_value<details::ProgressBarOption::max_postfix_text_len>(), ' ')
              << "\r";
    std::cout.flush();
    index_ += 1;
    if (progress_ > 100.0) {
      get_value<details::ProgressBarOption::completed>() = true;
    }
    if (get_value<details::ProgressBarOption::completed>())
      std::cout << termcolor::reset << std::endl;
  }
};

} // namespace indicators

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

  void print_progress() {
    std::lock_guard<std::mutex> lock{mutex_};
    if (started_)
      for (size_t i = 0; i < count; ++i)
        std::cout << "\x1b[A";
    for (auto &bar : bars_) {
      bar.get().print_progress(true);
      std::cout << "\n";
    }
    std::cout << termcolor::reset;
    if (!started_)
      started_ = true;
  }
};

} // namespace indicators
