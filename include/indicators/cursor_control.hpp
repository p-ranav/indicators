
#ifndef INDICATORS_CURSOR_CONTROL
#define INDICATORS_CURSOR_CONTROL

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

static inline void show_console_cursor(bool const show) {
  HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

  CONSOLE_CURSOR_INFO cursorInfo;

  GetConsoleCursorInfo(out, &cursorInfo);
  cursorInfo.bVisible = show; // set the cursor visibility
  SetConsoleCursorInfo(out, &cursorInfo);
}

#else

static inline void show_console_cursor(bool const show) {
  std::fputs(show ? "\033[?25h" : "\033[?25l", stdout);
}

#endif

} // namespace indicators

#endif