#include <chrono>
#include <indicators/cursor_control.hpp>
#include <indicators/progress_bar.hpp>
#include <thread>

int main() {
  // Hide cursor
  indicators::show_console_cursor(false);

  indicators::ProgressBar bar{indicators::option::BarWidth{50},
                              indicators::option::Start{" ["},
                              indicators::option::Fill{"█"},
                              indicators::option::Lead{"█"},
                              indicators::option::Remainder{"-"},
                              indicators::option::End{"]"},
                              indicators::option::PrefixText{"Training Gaze Network 👀"},
                              indicators::option::ForegroundColor{indicators::Color::yellow},
                              indicators::option::ShowElapsedTime{true},
                              indicators::option::ShowRemainingTime{true},
                              indicators::option::FontStyles{
                                  std::vector<indicators::FontStyle>{indicators::FontStyle::bold}}};

  // Update bar state
  while (true) {
    bar.tick();
    if (bar.is_completed())
      break;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  // Show cursor
  indicators::show_console_cursor(true);

  return 0;
}
