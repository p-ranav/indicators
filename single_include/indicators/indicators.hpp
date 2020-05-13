#pragma once
// #include <indicators/termcolor.hpp>//!
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
enum class Color { grey, red, green, yellow, blue, magenta, cyan, white, unspecified };
}
#pragma once

namespace indicators {

enum class FontStyle { bold, dark, italic, underline, blink, reverse, concealed, crossed };

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
#pragma once
#include <utility>

namespace indicators {

#if defined(_MSC_VER)
#include <windows.h>

std::pair<size_t, size_t> terminal_size() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns, rows;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    return {static_cast<size_t>(rows), static_cast<size_t>(cols)};
}

size_t terminal_width() {
    return terminal_size().second;
}

#else
#include <sys/ioctl.h> //ioctl() and TIOCGWINSZ
#include <unistd.h> // for STDOUT_FILENO

std::pair<size_t, size_t> terminal_size() {
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    return {static_cast<size_t>(size.ws_row), static_cast<size_t>(size.ws_col)};
}

size_t terminal_width() {
    return terminal_size().second;
}
#endif

}/*
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
#pragma once

#include <cstddef>
// #include <indicators/color.hpp>
// #include <indicators/font_style.hpp>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

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
  spinner_states,
  font_styles,
  hide_bar_when_complete,
  max_progress,
  stream
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
auto get_impl(Default &&/*def*/, T &&first, Args &&... /*tail*/) ->
    typename std::enable_if<(std::decay<T>::type::id == Id),
                            decltype(std::forward<T>(first))>::type {
  return std::forward<T>(first);
}

template <ProgressBarOption Id, typename Default, typename T, typename... Args>
auto get_impl(Default &&def, T &&/*first*/, Args &&... tail) ->
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
using HideBarWhenComplete =
    details::BooleanSetting<details::ProgressBarOption::hide_bar_when_complete>;
using FontStyles =
    details::Setting<std::vector<FontStyle>, details::ProgressBarOption::font_styles>;
using MaxProgress = details::IntegerSetting<details::ProgressBarOption::max_progress>;
using Stream = details::Setting<std::ostream&, details::ProgressBarOption::stream>;
} // namespace option
} // namespace indicators
#pragma once

#if defined(_MSC_VER)
#if !defined(NOMINMAX)
#define NOMINMAX
#endif
#include <io.h>
#include <windows.h>
#else
#include <cstdio>
#endif

namespace indicators {

#if defined(_MSC_VER)

void show_console_cursor(bool const show) {
  HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

  CONSOLE_CURSOR_INFO cursorInfo;

  GetConsoleCursorInfo(out, &cursorInfo);
  cursorInfo.bVisible = show; // set the cursor visibility
  SetConsoleCursorInfo(out, &cursorInfo);
}

#else

void show_console_cursor(bool const show) {
  std::fputs(show ? "\e[?25h" : "\e[?25l", stdout);
}

#endif

} // namespace indicators#pragma once

#if defined(_MSC_VER)
#if !defined(NOMINMAX)
#define NOMINMAX
#endif
#include <io.h>
#include <windows.h>
#else
#include <iostream>
#endif

namespace indicators {

#ifdef _MSC_VER

void move(int x, int y) {
  auto hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
  if (!hStdout)
    return;

  CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
  GetConsoleScreenBufferInfo(hStdout, &csbiInfo);

  COORD cursor;

  cursor.X = csbiInfo.dwCursorPosition.X + x;
  cursor.Y = csbiInfo.dwCursorPosition.Y + y;
  SetConsoleCursorPosition(hStdout, cursor);
}

void move_up(int lines) { move(0, -lines); }
void move_down(int lines) { move(0, -lines); }
void move_right(int cols) { move(cols, 0); }
void move_left(int cols) { move(-cols, 0); }

#else

void move_up(int lines) { std::cout << "\033[" << lines << "A"; }
void move_down(int lines) { std::cout << "\033[" << lines << "B"; }
void move_right(int cols) { std::cout << "\033[" << cols << "C"; }
void move_left(int cols) { std::cout << "\033[" << cols << "D"; }

#endif

} // namespace indicators#pragma once

// #include <indicators/setting.hpp>
// #include <indicators/termcolor.hpp>

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <ostream>
#include <string>
#include <vector>

#include <cassert>
#include <cmath>

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

inline void set_font_style(std::ostream &os, FontStyle style) {
  switch (style) {
  case FontStyle::bold:
    os << termcolor::bold;
    break;
  case FontStyle::dark:
    os << termcolor::dark;
    break;
  case FontStyle::italic:
    os << termcolor::italic;
    break;
  case FontStyle::underline:
    os << termcolor::underline;
    break;
  case FontStyle::blink:
    os << termcolor::blink;
    break;
  case FontStyle::reverse:
    os << termcolor::reverse;
    break;
  case FontStyle::concealed:
    os << termcolor::concealed;
    break;
  case FontStyle::crossed:
    os << termcolor::crossed;
    break;
  default:
    break;
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
    auto pos = static_cast<size_t>(progress * bar_width / 100.0);
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

class IndeterminateProgressScaleWriter {
public:
  IndeterminateProgressScaleWriter(std::ostream &os, size_t bar_width, const std::string &fill,
                      const std::string &lead)
      : os(os), bar_width(bar_width), fill(fill), lead(lead) {}

  std::ostream &write(size_t progress) {
    for (size_t i = 0; i < bar_width; ++i) {
      if (i < progress)
        os << fill;
      else if (i == progress)
        os << lead;
      else 
        os << fill;
    }
    return os;
  }

private:
  std::ostream &os;
  size_t bar_width = 0;
  std::string fill;
  std::string lead;
};

} // namespace details
} // namespace indicators
#pragma once

// #include <indicators/details/stream_helper.hpp>

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
// #include <indicators/color.hpp>
// #include <indicators/setting.hpp>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <tuple>
#include <type_traits>

namespace indicators {

class ProgressBar {
  using Settings =
      std::tuple<option::BarWidth, option::PrefixText, option::PostfixText, option::Start,
                 option::End, option::Fill, option::Lead, option::Remainder,
                 option::MaxPostfixTextLen, option::Completed, option::ShowPercentage,
                 option::ShowElapsedTime, option::ShowRemainingTime, option::SavedStartTime,
                 option::ForegroundColor, option::FontStyles, option::MaxProgress, option::Stream>;

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
                      option::ForegroundColor{Color::unspecified}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::font_styles>(
                      option::FontStyles{std::vector<FontStyle>{}}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::max_progress>(
                      option::MaxProgress{100}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::stream>(
                      option::Stream{std::cout}, std::forward<Args>(args)...)) {}

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

  void set_progress(size_t new_progress) {
    {
      std::lock_guard<std::mutex> lock(mutex_);
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
    return std::min(progress_, size_t(get_value<details::ProgressBarOption::max_progress>()));
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

  size_t progress_{0};
  Settings settings_;
  std::chrono::nanoseconds elapsed_;
  std::chrono::time_point<std::chrono::high_resolution_clock> start_time_point_;
  std::mutex mutex_;

  template <typename Indicator, size_t count> friend class MultiProgress;
  template <typename Indicator> friend class DynamicProgress;
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

public:
  void print_progress(bool from_multi_progress = false) {
    std::lock_guard<std::mutex> lock{mutex_};

    auto& os = get_value<details::ProgressBarOption::stream>();

    const auto max_progress = get_value<details::ProgressBarOption::max_progress>();
    if (multi_progress_mode_ && !from_multi_progress) {
      if (progress_ >= max_progress) {
        get_value<details::ProgressBarOption::completed>() = true;
      }
      return;
    }
    auto now = std::chrono::high_resolution_clock::now();
    if (!get_value<details::ProgressBarOption::completed>())
      elapsed_ = std::chrono::duration_cast<std::chrono::nanoseconds>(now - start_time_point_);

    if (get_value<details::ProgressBarOption::foreground_color>() != Color::unspecified)
      details::set_stream_color(os, get_value<details::ProgressBarOption::foreground_color>());

    for (auto &style : get_value<details::ProgressBarOption::font_styles>())
      details::set_font_style(os, style);

    os << get_value<details::ProgressBarOption::prefix_text>();

    os << get_value<details::ProgressBarOption::start>();

    details::ProgressScaleWriter writer{os,
                                        get_value<details::ProgressBarOption::bar_width>(),
                                        get_value<details::ProgressBarOption::fill>(),
                                        get_value<details::ProgressBarOption::lead>(),
                                        get_value<details::ProgressBarOption::remainder>()};
    writer.write(double(progress_) / double(max_progress) * 100.0f);

    os << get_value<details::ProgressBarOption::end>();

    if (get_value<details::ProgressBarOption::show_percentage>()) {
      os << " " << std::min(static_cast<size_t>(static_cast<float>(progress_) / max_progress * 100), size_t(100)) << "%";
    }

    auto &saved_start_time = get_value<details::ProgressBarOption::saved_start_time>();

    if (get_value<details::ProgressBarOption::show_elapsed_time>()) {
      os << " [";
      if (saved_start_time)
        details::write_duration(os, elapsed_);
      else
        os << "00:00s";
    }

    if (get_value<details::ProgressBarOption::show_remaining_time>()) {
      if (get_value<details::ProgressBarOption::show_elapsed_time>())
        os << "<";
      else
        os << " [";

      if (saved_start_time) {
        auto eta = std::chrono::nanoseconds(
            progress_ > 0 ? static_cast<long long>(elapsed_.count() * max_progress / progress_) : 0);
        auto remaining = eta > elapsed_ ? (eta - elapsed_) : (elapsed_ - eta);
        details::write_duration(os, remaining);
      } else {
        os << "00:00s";
      }

      os << "]";
    } else {
      if (get_value<details::ProgressBarOption::show_elapsed_time>())
        os << "]";
    }

    if (get_value<details::ProgressBarOption::max_postfix_text_len>() == 0)
      get_value<details::ProgressBarOption::max_postfix_text_len>() = 10;
    os << " " << get_value<details::ProgressBarOption::postfix_text>()
              << std::string(get_value<details::ProgressBarOption::max_postfix_text_len>(), ' ')
              << "\r";
    os.flush();
    if (progress_ >= max_progress) {
      get_value<details::ProgressBarOption::completed>() = true;
    }
    if (get_value<details::ProgressBarOption::completed>() &&
        !from_multi_progress) // Don't std::endl if calling from MultiProgress
      os << termcolor::reset << std::endl;
  }
};

} // namespace indicators
#pragma once

// #include <indicators/color.hpp>
// #include <indicators/details/stream_helper.hpp>

#include <algorithm>
#include <atomic>
#include <chrono>
// #include <indicators/setting.hpp>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <tuple>

namespace indicators {

class BlockProgressBar {
  using Settings = std::tuple<option::ForegroundColor, option::BarWidth, option::Start, option::End,
                              option::PrefixText, option::PostfixText, option::ShowPercentage,
                              option::ShowElapsedTime, option::ShowRemainingTime, option::Completed,
                              option::SavedStartTime, option::MaxPostfixTextLen, option::FontStyles,
                              option::MaxProgress, option::Stream>;

public:
  template <typename... Args,
            typename std::enable_if<details::are_settings_from_tuple<
                                        Settings, typename std::decay<Args>::type...>::value,
                                    void *>::type = nullptr>
  explicit BlockProgressBar(Args &&... args)
      : settings_(details::get<details::ProgressBarOption::foreground_color>(
                      option::ForegroundColor{Color::unspecified}, std::forward<Args>(args)...),
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
                      option::MaxPostfixTextLen{0}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::font_styles>(
                      option::FontStyles{std::vector<FontStyle>{}}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::max_progress>(option::MaxProgress{100},
                                                                         std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::stream>(
                      option::Stream{std::cout}, std::forward<Args>(args)...)
                  ) {}

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
    return std::min(static_cast<size_t>(progress_), size_t(get_value<details::ProgressBarOption::max_progress>()));
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
  template <typename Indicator> friend class DynamicProgress;
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

public:
  void print_progress(bool from_multi_progress = false) {
    std::lock_guard<std::mutex> lock{mutex_};

    auto& os = get_value<details::ProgressBarOption::stream>();

    const auto max_progress = get_value<details::ProgressBarOption::max_progress>();
    if (multi_progress_mode_ && !from_multi_progress) {
      if (progress_ > max_progress) {
        get_value<details::ProgressBarOption::completed>() = true;
      }
      return;
    }

    auto now = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(now - start_time_point_);

    if (get_value<details::ProgressBarOption::foreground_color>() != Color::unspecified)
      details::set_stream_color(os, get_value<details::ProgressBarOption::foreground_color>());

    for (auto &style : get_value<details::ProgressBarOption::font_styles>())
      details::set_font_style(os, style);
    
    os << get_value<details::ProgressBarOption::prefix_text>();
    os << get_value<details::ProgressBarOption::start>();

    details::BlockProgressScaleWriter writer{os,
                                             get_value<details::ProgressBarOption::bar_width>()};
    writer.write(progress_ / max_progress * 100);

    os << get_value<details::ProgressBarOption::end>();
    if (get_value<details::ProgressBarOption::show_percentage>()) {
      os << " " << std::min(static_cast<size_t>(progress_ / max_progress * 100.0), size_t(100)) << "%";
    }

    auto &saved_start_time = get_value<details::ProgressBarOption::saved_start_time>();

    if (get_value<details::ProgressBarOption::show_elapsed_time>()) {
      os << " [";
      if (saved_start_time)
        details::write_duration(os, elapsed);
      else
        os << "00:00s";
    }

    if (get_value<details::ProgressBarOption::show_remaining_time>()) {
      if (get_value<details::ProgressBarOption::show_elapsed_time>())
        os << "<";
      else
        os << " [";

      if (saved_start_time) {
        auto eta = std::chrono::nanoseconds(
            progress_ > 0 ? static_cast<long long>(elapsed.count() * max_progress / progress_) : 0);
        auto remaining = eta > elapsed ? (eta - elapsed) : (elapsed - eta);
        details::write_duration(os, remaining);
      } else {
        os << "00:00s";
      }

      os << "]";
    } else {
      if (get_value<details::ProgressBarOption::show_elapsed_time>())
        os << "]";
    }

    if (get_value<details::ProgressBarOption::max_postfix_text_len>() == 0)
      get_value<details::ProgressBarOption::max_postfix_text_len>() = 10;
    os << " " << get_value<details::ProgressBarOption::postfix_text>()
              << std::string(get_value<details::ProgressBarOption::max_postfix_text_len>(), ' ')
              << "\r";
    os.flush();
    if (progress_ > max_progress) {
      get_value<details::ProgressBarOption::completed>() = true;
    }
    if (get_value<details::ProgressBarOption::completed>() &&
        !from_multi_progress) // Don't std::endl if calling from MultiProgress
      os << termcolor::reset << std::endl;
  }
};

} // namespace indicators
#pragma once

// #include <indicators/details/stream_helper.hpp>

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
// #include <indicators/color.hpp>
// #include <indicators/setting.hpp>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <tuple>
#include <type_traits>

namespace indicators {

class IndeterminateProgressBar {
  using Settings =
      std::tuple<option::BarWidth, option::PrefixText, option::PostfixText, option::Start,
                 option::End, option::Fill, option::Lead,
                 option::MaxPostfixTextLen, option::Completed,
                 option::ForegroundColor, option::FontStyles, option::Stream>;

  enum class Direction {
    forward,
    backward
  };

  Direction direction_{Direction::forward};

public:
  template <typename... Args,
            typename std::enable_if<details::are_settings_from_tuple<
                                        Settings, typename std::decay<Args>::type...>::value,
                                    void *>::type = nullptr>
  explicit IndeterminateProgressBar(Args &&... args)
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
                  details::get<details::ProgressBarOption::fill>(option::Fill{"."},
                                                                 std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::lead>(option::Lead{"<==>"},
                                                                 std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::max_postfix_text_len>(
                      option::MaxPostfixTextLen{0}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::completed>(option::Completed{false},
                                                                      std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::foreground_color>(
                      option::ForegroundColor{Color::unspecified}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::font_styles>(
                      option::FontStyles{std::vector<FontStyle>{}}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::stream>(
                      option::Stream{std::cout}, std::forward<Args>(args)...)) {
    // starts with [<==>...........]
    // progress_ = 0

    // ends with   [...........<==>]
    //             ^^^^^^^^^^^^^^^^^ bar_width
    //             ^^^^^^^^^^^^ (bar_width - len(lead))
    // progress_ = bar_width - len(lead)
    progress_ = 0;
    max_progress_ = get_value<details::ProgressBarOption::bar_width>()
      - get_value<details::ProgressBarOption::lead>().size() 
      + get_value<details::ProgressBarOption::start>().size()
      + get_value<details::ProgressBarOption::end>().size();
  }

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

  void tick() {
    {
      std::lock_guard<std::mutex> lock{mutex_};
      if (get_value<details::ProgressBarOption::completed>())
        return;

      progress_ += (direction_ == Direction::forward) ? 1 : -1;
      if (direction_ == Direction::forward && progress_ == max_progress_) {
        // time to go back
        direction_ = Direction::backward;
      } else if (direction_ == Direction::backward && progress_ == 0) {
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
  auto get_value() -> decltype((details::get_value<id>(std::declval<Settings &>()).value)) {
    return details::get_value<id>(settings_).value;
  }

  template <details::ProgressBarOption id>
  auto get_value() const
      -> decltype((details::get_value<id>(std::declval<const Settings &>()).value)) {
    return details::get_value<id>(settings_).value;
  }

  size_t progress_{0};
  size_t max_progress_;
  Settings settings_;
  std::chrono::nanoseconds elapsed_;
  std::mutex mutex_;

  template <typename Indicator, size_t count> friend class MultiProgress;
  template <typename Indicator> friend class DynamicProgress;
  std::atomic<bool> multi_progress_mode_{false};

public:
  void print_progress(bool from_multi_progress = false) {
    std::lock_guard<std::mutex> lock{mutex_};

    auto& os = get_value<details::ProgressBarOption::stream>();

    if (multi_progress_mode_ && !from_multi_progress) {
      return;
    }
    if (get_value<details::ProgressBarOption::foreground_color>() != Color::unspecified)
      details::set_stream_color(os, get_value<details::ProgressBarOption::foreground_color>());

    for (auto &style : get_value<details::ProgressBarOption::font_styles>())
      details::set_font_style(os, style);

    os << get_value<details::ProgressBarOption::prefix_text>();

    os << get_value<details::ProgressBarOption::start>();

    details::IndeterminateProgressScaleWriter writer{os,
                                        get_value<details::ProgressBarOption::bar_width>(),
                                        get_value<details::ProgressBarOption::fill>(),
                                        get_value<details::ProgressBarOption::lead>()};
    writer.write(progress_);

    os << get_value<details::ProgressBarOption::end>();

    if (get_value<details::ProgressBarOption::max_postfix_text_len>() == 0)
      get_value<details::ProgressBarOption::max_postfix_text_len>() = 10;
    os << " " << get_value<details::ProgressBarOption::postfix_text>()
              << std::string(get_value<details::ProgressBarOption::max_postfix_text_len>(), ' ')
              << "\r";
    os.flush();
    if (get_value<details::ProgressBarOption::completed>() &&
        !from_multi_progress) // Don't std::endl if calling from MultiProgress
      os << termcolor::reset << std::endl;
  }
};

} // namespace indicators
#pragma once
#include <atomic>
#include <functional>
#include <iostream>
#include <mutex>
#include <vector>

// #include <indicators/cursor_movement.hpp>
// #include <indicators/color.hpp>

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
    if (started_)
      move_up(count);
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
#pragma once
#include <atomic>
#include <functional>
// #include <indicators/color.hpp>
// #include <indicators/setting.hpp>
#include <iostream>
#include <mutex>
#include <vector>

namespace indicators {

template <typename Indicator> class DynamicProgress {
  using Settings = std::tuple<option::HideBarWhenComplete>;

public:
  template <typename... Indicators> explicit DynamicProgress(Indicators &... bars) {
    bars_ = {bars...};
    for (auto &bar : bars_) {
      bar.get().multi_progress_mode_ = true;
      ++total_count_;
      ++incomplete_count_;
    }
  }

  Indicator &operator[](size_t index) {
    print_progress();
    std::lock_guard<std::mutex> lock{mutex_};
    return bars_[index].get();
  }

  size_t push_back(Indicator &bar) {
    std::lock_guard<std::mutex> lock{mutex_};
    bar.multi_progress_mode_ = true;
    bars_.push_back(bar);
    return bars_.size() - 1;
  }

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

private:
  Settings settings_;
  std::atomic<bool> started_{false};
  std::mutex mutex_;
  std::vector<std::reference_wrapper<Indicator>> bars_;
  std::atomic<size_t> total_count_{0};
  std::atomic<size_t> incomplete_count_{0};

  template <details::ProgressBarOption id>
  auto get_value() -> decltype((details::get_value<id>(std::declval<Settings &>()).value)) {
    return details::get_value<id>(settings_).value;
  }

  template <details::ProgressBarOption id>
  auto get_value() const
      -> decltype((details::get_value<id>(std::declval<const Settings &>()).value)) {
    return details::get_value<id>(settings_).value;
  }

public:
  void print_progress() {
    std::lock_guard<std::mutex> lock{mutex_};
    auto &hide_bar_when_complete = get_value<details::ProgressBarOption::hide_bar_when_complete>();
    if (hide_bar_when_complete) {
      // Hide completed bars
      if (started_) {
        for (size_t i = 0; i < incomplete_count_; ++i)
          std::cout << "\033[A\r\033[K" << std::flush;
      }
      incomplete_count_ = 0;
      for (auto &bar : bars_) {
        if (!bar.get().is_completed()) {
          bar.get().print_progress(true);
          std::cout << "\n";
          ++incomplete_count_;
        }
      }
      if (!started_)
        started_ = true;
    } else {
      // Don't hide any bars
      if (started_) {
        for (size_t i = 0; i < total_count_; ++i)
          std::cout << "\x1b[A";
      }
      for (auto &bar : bars_) {
        bar.get().print_progress(true);
        std::cout << "\n";
      }
      if (!started_)
        started_ = true;
    }
    total_count_ = bars_.size();
    std::cout << termcolor::reset;
  }
};

} // namespace indicators
#pragma once

// #include <indicators/details/stream_helper.hpp>

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
// #include <indicators/color.hpp>
// #include <indicators/setting.hpp>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <tuple>
#include <vector>

namespace indicators {

class ProgressSpinner {
  using Settings =
      std::tuple<option::ForegroundColor, option::PrefixText, option::PostfixText,
                 option::ShowPercentage, option::ShowElapsedTime, option::ShowRemainingTime,
                 option::ShowSpinner, option::SavedStartTime, option::Completed,
                 option::MaxPostfixTextLen, option::SpinnerStates, option::FontStyles,
                 option::MaxProgress, option::Stream>;

public:
  template <typename... Args,
            typename std::enable_if<details::are_settings_from_tuple<
                                        Settings, typename std::decay<Args>::type...>::value,
                                    void *>::type = nullptr>
  explicit ProgressSpinner(Args &&... args)
      : settings_(details::get<details::ProgressBarOption::foreground_color>(
                      option::ForegroundColor{Color::unspecified}, std::forward<Args>(args)...),
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
                      std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::font_styles>(
                      option::FontStyles{std::vector<FontStyle>{}}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::max_progress>(
                      option::MaxProgress{100}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::stream>(
                      option::Stream{std::cout}, std::forward<Args>(args)...)) {}

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

  void set_progress(size_t value) {
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
    return std::min(progress_, size_t(get_value<details::ProgressBarOption::max_progress>()));
  }

  bool is_completed() const { return get_value<details::ProgressBarOption::completed>(); }

  void mark_as_completed() {
    get_value<details::ProgressBarOption::completed>() = true;
    print_progress();
  }

private:
  Settings settings_;
  size_t progress_{0};
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

public:
  void print_progress() {
    std::lock_guard<std::mutex> lock{mutex_};

    auto& os = get_value<details::ProgressBarOption::stream>();

    const auto max_progress = get_value<details::ProgressBarOption::max_progress>();
    auto now = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(now - start_time_point_);

    if (get_value<details::ProgressBarOption::foreground_color>() != Color::unspecified)
      details::set_stream_color(os, get_value<details::ProgressBarOption::foreground_color>());

    for (auto &style : get_value<details::ProgressBarOption::font_styles>())
      details::set_font_style(os, style);

    os << get_value<details::ProgressBarOption::prefix_text>();
    if (get_value<details::ProgressBarOption::spinner_show>())
      os << get_value<details::ProgressBarOption::spinner_states>()
              [index_ % get_value<details::ProgressBarOption::spinner_states>().size()];
    if (get_value<details::ProgressBarOption::show_percentage>()) {
      os << " " << std::min(progress_, size_t(max_progress)) << "%";
    }

    if (get_value<details::ProgressBarOption::show_elapsed_time>()) {
      os << " [";
      details::write_duration(os, elapsed);
    }

    if (get_value<details::ProgressBarOption::show_remaining_time>()) {
      if (get_value<details::ProgressBarOption::show_elapsed_time>())
        os << "<";
      else
        os << " [";
      auto eta = std::chrono::nanoseconds(
          progress_ > 0 ? static_cast<long long>(elapsed.count() * max_progress / progress_) : 0);
      auto remaining = eta > elapsed ? (eta - elapsed) : (elapsed - eta);
      details::write_duration(os, remaining);
      os << "]";
    } else {
      if (get_value<details::ProgressBarOption::show_elapsed_time>())
        os << "]";
    }

    if (get_value<details::ProgressBarOption::max_postfix_text_len>() == 0)
      get_value<details::ProgressBarOption::max_postfix_text_len>() = 10;
    os << " " << get_value<details::ProgressBarOption::postfix_text>()
              << std::string(get_value<details::ProgressBarOption::max_postfix_text_len>(), ' ')
              << "\r";
    os.flush();
    index_ += 1;
    if (progress_ > max_progress) {
      get_value<details::ProgressBarOption::completed>() = true;
    }
    if (get_value<details::ProgressBarOption::completed>())
      os << termcolor::reset << std::endl;
  }
};

} // namespace indicators
