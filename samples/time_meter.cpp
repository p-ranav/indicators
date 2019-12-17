#include <chrono>
#include <indicators/progress_bar.hpp>
#include <thread>

int main() {
  indicators::ProgressBar bar;

  // Configure the bar
  bar.set_bar_width(50);
  bar.start_bar_with(" [");
  bar.fill_bar_progress_with("█");
  bar.lead_bar_progress_with("█");
  bar.fill_bar_remainder_with("-");
  bar.end_bar_with("]");
  bar.set_prefix_text("Training Gaze Network 👀");
  bar.set_foreground_color(indicators::Color::YELLOW);

  // Show time elapsed and remaining
  bar.show_elapsed_time();
  bar.show_remaining_time();

  // Update bar state
  while (true) {
    bar.tick();
    if (bar.is_completed())
      break;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  // Show cursor
  std::cout << "\e[?25h";

  return 0;
}
