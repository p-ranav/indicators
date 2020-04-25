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

void enable_cursor_movement() {
  auto hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleMode(hStdout,
                 ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

void show_console_cursor(bool const show) {
  HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

  CONSOLE_CURSOR_INFO cursorInfo;

  GetConsoleCursorInfo(out, &cursorInfo);
  cursorInfo.bVisible = show; // set the cursor visibility
  SetConsoleCursorInfo(out, &cursorInfo);
}

#else

void enable_cursor_movement() {}

void show_console_cursor(bool const show) {
  std::fputs(show ? "\e[?25h" : "\e[?25l", stdout);
}

#endif

} // namespace indicators