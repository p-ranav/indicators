#ifndef INDICATORS_TERMINAL_SIZE
#define INDICATORS_TERMINAL_SIZE
#include <utility>

namespace indicators {
  enum class TerminalHandle {
    StdOut, StdErr
  };
};

#if defined(_WIN32)
#include <windows.h>

namespace indicators {

static inline DWORD os_handle(TerminalHandle hndl)
{
  switch (hndl) {
    case TerminalHandle::StdOut: return STD_OUTPUT_HANDLE;
    case TerminalHandle::StdErr: return STD_ERROR_HANDLE;
  }
  return STD_OUTPUT_HANDLE;
}

static inline std::pair<size_t, size_t> terminal_size(TerminalHandle hndl) {
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  int cols, rows;
  GetConsoleScreenBufferInfo(GetStdHandle(os_handle(hndl)), &csbi);
  cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
  rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
  return {static_cast<size_t>(rows), static_cast<size_t>(cols)};
}

static inline size_t terminal_width(TerminalHandle hndl) { return terminal_size(hndl).second; }

} // namespace indicators

#else

#include <sys/ioctl.h> //ioctl() and TIOCGWINSZ
#include <unistd.h>    // for STDOUT_FILENO

namespace indicators {

static inline int os_handle(TerminalHandle hndl)
{
  switch (hndl) {
    case TerminalHandle::StdOut: return STDOUT_FILENO;
    case TerminalHandle::StdErr: return STDERR_FILENO;
  }
  return STDOUT_FILENO;
}


static inline std::pair<size_t, size_t> terminal_size(TerminalHandle hndl) {
  struct winsize size{};
  ioctl(os_handle(hndl), TIOCGWINSZ, &size);
  return {static_cast<size_t>(size.ws_row), static_cast<size_t>(size.ws_col)};
}

static inline size_t terminal_width(TerminalHandle hndl) { return terminal_size(hndl).second; }

} // namespace indicators

#endif

#endif
