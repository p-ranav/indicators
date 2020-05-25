
#pragma once
#include <utility>

namespace indicators {

#if defined(_MSC_VER)
#include <windows.h>

static inline std::pair<size_t, size_t> terminal_size() {
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  int columns, rows;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
  columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
  rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
  return {static_cast<size_t>(rows), static_cast<size_t>(cols)};
}

size_t terminal_width() { return terminal_size().second; }

#else
#include <sys/ioctl.h> //ioctl() and TIOCGWINSZ
#include <unistd.h>    // for STDOUT_FILENO

static inline std::pair<size_t, size_t> terminal_size() {
  struct winsize size;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
  return {static_cast<size_t>(size.ws_row), static_cast<size_t>(size.ws_col)};
}

static inline size_t terminal_width() { return terminal_size().second; }
#endif

} // namespace indicators