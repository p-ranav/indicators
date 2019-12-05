<p align="center">
  <img height="100" src="img/logo.png"/>  
</p>

<p align="center">
  <img src="img/demo.gif"/> 
</p>

## Progress bar

To introduce a progress bar in your application, include `indica/progress_bar.hpp` and create a `ProgressBar` object. 

```cpp
#include <indica/progress_bar.hpp>

int main() {
  indica::ProgressBar bar;
  return 0;
}
```

Here's the general structure of a progress bar:

```
<prefix_text> <bar_start> <fill> <lead> <remaining> <bar_end>   <progress_percentage> <postfix_text>
              ^^^^^^^^^^^^^^^^^^ Bar Width ^^^^^^^^^^^^^^^^^^   ^^^^^ Show/Hide ^^^^^
```

Each of these elements (and more) can be configured using the ProgressBar API:

```cpp
#include <indica/progress_bar.hpp>

int main() {
  indica::ProgressBar bar;
  
  // Configure the bar
  bar.set_bar_width(50);
  bar.start_bar_with("[");
  bar.fill_bar_progress_with("=");
  bar.lead_bar_progress_with(">");
  bar.fill_bar_remainder_with(" ");
  bar.end_bar_with("]");
  bar.set_postfix_text("Getting started");
  bar.set_foreground_color(indica::Color::GREEN); 

  return 0;
}
```

Now that the bar is configured, let's update the state of the bar. The amount of progress in ProgressBar is maintained as a float in range `[0, 100]`. When progress reaches 100, the progression is complete. 

From application-level code, there are two ways in which you can update this progress:

### Update progress using `tick()`

You can update the progress bar using `bar.tick()` which increments progress by exactly `1%`.

```cpp
#include <indica/progress_bar.hpp>
#include <thread>
#include <chrono>

int main() {
  indica::ProgressBar bar;
  
  // Configure the bar
  bar.set_bar_width(50);
  bar.start_bar_with("[");
  bar.fill_bar_progress_with("=");
  bar.lead_bar_progress_with(">");
  bar.fill_bar_remainder_with(" ");
  bar.end_bar_with("]");
  bar.set_postfix_text("Getting started");
  bar.set_foreground_color(indica::Color::GREEN); 
  
  // Update bar state
  while (true) {
    bar.tick();
    if (bar.is_completed())
      break;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  return 0;
}
```

The above code will print a progress bar that goes from 0 to 100% at the rate of 1% every 100 ms.

### Updating progress using `bar.set_progress(value)`

If you'd rather control progress of the bar in discrete steps, consider using `bar.set_progress(value)`. Example:

```cpp
#include <indica/progress_bar.hpp>
#include <thread>
#include <chrono>

int main() {
  indica::ProgressBar bar;
  
  // Configure the bar
  bar.set_bar_width(50);
  bar.start_bar_with("[");
  bar.fill_bar_progress_with("=");
  bar.lead_bar_progress_with(">");
  bar.fill_bar_remainder_with(" ");
  bar.end_bar_with("]");
  bar.set_postfix_text("Getting started");
  bar.set_foreground_color(indica::Color::GREEN); 
  
  // Update bar state
  size_t i = 0;
  while (i < 101) {
    bar.set_progress(i);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    i += 10;
  }

  return 0;
}
```

The above code will print a progress bar that goes from 0 to 100% at the rate of 10% every 100 ms.

For more examples, checkout the examples in the `samples/` directory.

### Multi-threaded Example

```cpp
#include <indica/progress_bar.hpp>
#include <vector>

int main() {

  indica::ProgressBar bar;
  bar.set_bar_width(50);
  bar.start_bar_with("[");
  bar.fill_bar_progress_with("■");
  bar.lead_bar_progress_with("■");
  bar.fill_bar_remainder_with("-");
  bar.end_bar_with("]");
  bar.set_foreground_color(indica::Color::YELLOW);

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
  // You can use bar.set_postfix_text(...) to append text to the right
  //
  // [■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■-------------] 70% Finding a replacement engineer
  //                                                        ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  //
  //  

  auto job = [&bar, &index, &status_text]() {
    while (true) {
      if (bar.is_completed()) {
        break;
      }
      bar.set_postfix_text(status_text[index % status_text.size()]);
      bar.tick();
      index += 1;
      std::this_thread::sleep_for(std::chrono::milliseconds(2000));
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
