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

  std::thread first_job(
    [&bar]() {
      for (size_t i = 0; i <= 50; ++i) {
	bar.tick();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }
    });

  std::thread second_job(
    [&bar]() {
      for (size_t i = 0; i <= 50; ++i) {
  	bar.tick();
  	std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }
    });

  first_job.join();
  second_job.join();

  return 0;
}
