
#ifndef INDICATORS_TERMINAL_SIZE
#define INDICATORS_TERMINAL_SIZE
#include <utility>


#if defined(_WIN32)
#include <windows.h>

namespace indicators {

static inline std::pair<size_t, size_t> terminal_size() {
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  int cols, rows;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
  cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
  rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
  return {static_cast<size_t>(rows), static_cast<size_t>(cols)};
}

static inline size_t terminal_width() { return terminal_size().second; }

} // namespace indicators

#else

#include <fcntl.h>     // open and O_RDWR
#include <sys/ioctl.h> // ioctl() and TIOCGWINSZ
#include <unistd.h>    // close

namespace indicators {

static inline std::pair<size_t, size_t> terminal_size() {
  struct winsize size{};
  int fd = open("/dev/tty", O_RDWR);
  if (fd >= 0) {
    ioctl(fd, TIOCGWINSZ, &size);
    close(fd);
  }
  return {static_cast<size_t>(size.ws_row), static_cast<size_t>(size.ws_col)};
}

static inline size_t terminal_width() { return terminal_size().second; }

} // namespace indicators

#endif

#endif