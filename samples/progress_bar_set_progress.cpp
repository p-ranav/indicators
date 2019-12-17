#include <chrono>
#include <indicators/progress_bar.hpp>
#include <thread>

int main() {
  indicators::ProgressBar bar;

  // Configure the bar
  bar.set_bar_width(50);
  bar.start_bar_with("[");
  bar.fill_bar_progress_with("=");
  bar.lead_bar_progress_with(">");
  bar.fill_bar_remainder_with(" ");
  bar.end_bar_with("]");
  bar.set_postfix_text("Getting started");
  bar.set_foreground_color(indicators::Color::GREEN);

  // Update bar state

  bar.set_progress(10); // 10% done

  // do some work
  std::this_thread::sleep_for(std::chrono::milliseconds(100));

  bar.set_progress(30); // 30% done

  // do some more work
  std::this_thread::sleep_for(std::chrono::milliseconds(600));

  bar.set_progress(65); // 65% done

  // do final bit of work
  std::this_thread::sleep_for(std::chrono::milliseconds(300));

  bar.set_progress(100); // all done

  return 0;
}
