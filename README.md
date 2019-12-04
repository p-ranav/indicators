<p align="center">
  <img height="80" src="https://i.imgur.com/BfflF5h.png"/>	
</p>

## Progress Bar

```cpp
#include <progress/bar.hpp>

int main() {
  ProgressBar bar;
  bar.bar_width(50);
  bar.start_with("[");
  bar.fill_progress_with("■");
  bar.lead_progress_with("■");
  bar.fill_remainder_with("-");
  bar.end_with("]");
  bar.color(ProgressBar::Color::YELLOW);

  // As configured, the bar will look like this:
  // 
  // [■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■-------------] 70%
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

  std::thread first_thread(job);
  std::thread second_thread(job);
  std::thread third_thread(job);
  std::thread last_thread(job);

  first_thread.join();
  second_thread.join();
  third_thread.join();
  last_thread.join();

  return 0;
}
```
