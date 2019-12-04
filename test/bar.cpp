#include <iostream>
#include <progress/bar.hpp>

int main() {
  ProgressBar bar;

  // Configure progress bar
  bar.bar_width(50);
  bar.start_with("[");
  bar.fill_progress_with("■");
  bar.lead_progress_with("■");
  bar.fill_remainder_with("-");
  bar.end_with(" ]");

  // As configured, the bar will look like this:
  // 
  // [■■■■■■■■■■■■■■■■■■■■ --------] 70% 
  //
  //

  auto job = [&bar]() {
	       while(true) {
		 if (bar.completed())
		   break;
		 bar.tick();
		 std::this_thread::sleep_for(std::chrono::milliseconds(100));
	       }   
	     };

  std::thread first_job(job);
  std::thread second_job(job);
  std::thread third_job(job);
  std::thread last_job(job);

  first_job.join();
  second_job.join();
  third_job.join();
  last_job.join();

  return 0;
}
