<p align="center">
  <img height="80" src="https://i.imgur.com/BfflF5h.png"/>	
</p>

## Progress Bar

```cpp
#include <progress/bar.hpp>
#include <vector>

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
  
  std::atomic<size_t> index{0};
  std::vector<std::string> status_text =
    {
     "Rocket.exe is not responding",
     "Finding a replacement engineer",
     "Buying more snacks",
     "Assimilating the modding community",
     "Crossing fingers",
     "Porting KSP to a Nokia 3310"
  };
  
  // Let's say you want to append some status text to the right of the progress bar
  // You can use bar.append_text(...) to append text to the right
  //
  // [■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■-------------] 70% Finding a replacement engineer
  //                                                        ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  //  

  auto job = [&bar, &index, &status_text]() {
    while (true) {
      if (bar.completed()) {
        break;
      }
      bar.append_text(status_text[index % status_text.size()]);
      bar.tick();
      index += 1;
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
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
```
