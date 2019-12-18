#include <chrono>
#include <indicators/progress_bar.hpp>
#include <thread>

int main() {

  // Hide cursor
  std::cout << "\e[?25l";

  indicators::ProgressBar bar;

  // Configure the bar
  bar.set_bar_width(50);
  bar.start_bar_with("[");
  bar.fill_bar_progress_with("■");
  bar.lead_bar_progress_with("■");
  bar.fill_bar_remainder_with("-");
  bar.end_bar_with(" ]");
  bar.set_postfix_text("Loading dependency 1/4");
  bar.set_foreground_color(indicators::Color::CYAN);

  // Update bar state
  bar.set_progress(10); // 10% done

  // do some work
  std::this_thread::sleep_for(std::chrono::milliseconds(800));

  bar.set_postfix_text("Loading dependency 2/4");

  bar.set_progress(30); // 30% done

  // do some more work
  std::this_thread::sleep_for(std::chrono::milliseconds(700));

  bar.set_postfix_text("Loading dependency 3/4");

  bar.set_progress(65); // 65% done

  // do final bit of work
  std::this_thread::sleep_for(std::chrono::milliseconds(900));

  bar.set_postfix_text("Loaded dependencies!");

  bar.set_progress(100); // all done

  bar.mark_as_completed();

  // Show cursor
  std::cout << "\e[?25h";

  return 0;
}
