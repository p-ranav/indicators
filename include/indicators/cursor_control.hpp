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

#include "terminal_size.hpp"

namespace indicators {

#if defined(_MSC_VER)

static inline void show_console_cursor(bool const show, TerminalHandle hndl = TerminalHandle::StdOut) {
  HANDLE out = GetStdHandle(os_handle(hndl));

  CONSOLE_CURSOR_INFO cursorInfo;

  GetConsoleCursorInfo(out, &cursorInfo);
  cursorInfo.bVisible = show; // set the cursor visibility
  SetConsoleCursorInfo(out, &cursorInfo);
}

static inline void erase_line(TerminalHandle hndl = TerminalHandle::StdOut) {
  auto hStdout = GetStdHandle(os_handle(hndl));
  if (!hStdout)
    return;

  CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
  GetConsoleScreenBufferInfo(hStdout, &csbiInfo);

  COORD cursor;

  cursor.X = 0;
  cursor.Y = csbiInfo.dwCursorPosition.Y;

  DWORD count = 0;

  FillConsoleOutputCharacterA(hStdout, ' ', csbiInfo.dwSize.X, cursor, &count);

  FillConsoleOutputAttribute(hStdout, csbiInfo.wAttributes, csbiInfo.dwSize.X,
                             cursor, &count);

  SetConsoleCursorPosition(hStdout, cursor);
}

#else

static inline FILE* os_stream(TerminalHandle hndl)
{
  switch (hndl) {
    case TerminalHandle::StdOut: return stdout;
    case TerminalHandle::StdErr: return stderr;
  }
  return stdout;
}

static inline void show_console_cursor(bool const show, TerminalHandle hndl = TerminalHandle::StdOut) {
  std::fputs(show ? "\033[?25h" : "\033[?25l", os_stream(hndl));
}

static inline void erase_line(TerminalHandle hndl = TerminalHandle::StdOut) {
  std::fputs("\r\033[K", os_stream(hndl));
}

#endif

} // namespace indicators

#endif
