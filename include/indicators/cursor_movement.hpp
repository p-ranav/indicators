#pragma once

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

void move_up(int lines) { std::cout << "x1b[" << lines << "A"; }
void move_down(int lines) { std::cout << "x1b[" << lines << "B"; }
void move_right(int cols) { std::cout << "x1b[" << cols << "C"; }
void move_left(int cols) { std::cout << "x1b[" << cols << "D"; }

#endif

} // namespace indicators