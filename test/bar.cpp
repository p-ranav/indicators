#include <iostream>
#include <progress/bar.hpp>

int main() {
  ProgressBar bar;

  // Configure progress bar
  bar.bar_width(100);
  bar.start_with("[");
  bar.fill_progress_with("=");
  bar.lead_progress_with(">");
  bar.fill_remainder_with(" ");
  bar.end_with("]");

  // As configured, the bar will look like this:
  // 
  // [=================>         ] 70%
  //
  //

  for (size_t i = 0; i < 101; i += 5) {
    bar.set_progress(i);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  return 0;
}
