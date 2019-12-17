<p align="center">
  <img height="70" src="img/logo.png"/>  
</p>

<p align="center">
  <a href="https://github.com/p-ranav/indicators/blob/master/LICENSE">
    <img src="https://img.shields.io/badge/License-MIT-yellow.svg" alt="license"/>
  </a>
  <img src="https://img.shields.io/badge/version-1.1-blue.svg?cacheSeconds=2592000" alt="version"/>
</p>

<p align="center">
  <img src="img/demo.gif"/> 
</p>

# Highlights

* Thread-safe progress bars and spinners
* Header-only library. Grab a copy of `include/indicators`
* Source for the above GIF can be found [here](demo/demo.cpp)
* MIT License

# Progress bar

To introduce a progress bar in your application, include `indicators/progress_bar.hpp` and create a `ProgressBar` object. 

```cpp
#include <indicators/progress_bar.hpp>

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
  bar.hide_elapsed_time();
  bar.hide_remaining_time();
  
  // Update bar state

  return 0;
}
```

Here's the general structure of a progress bar:

```
{prefix} {start} {fill} {lead} {remaining} {end} {percentage} [{elapsed}<{remaining}?] {postfix}
         ^^^^^^^^^^^^^ Bar Width ^^^^^^^^^^^^^^^   
```

The amount of progress in ProgressBar is maintained as a float in range `[0, 100]`. When progress reaches 100, the progression is complete. 

From application-level code, there are two ways in which you can update this progress:

## Update progress using `bar.tick()`

You can update the progress bar using `bar.tick()` which increments progress by exactly `1%`.

```cpp
#include <indicators/progress_bar.hpp>
#include <thread>
#include <chrono>

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
  bar.hide_elapsed_time();
  bar.hide_remaining_time();
  
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

## Updating progress using `bar.set_progress(value)`

If you'd rather control progress of the bar in discrete steps, consider using `bar.set_progress(value)`. Example:

```cpp
#include <indicators/progress_bar.hpp>
#include <thread>
#include <chrono>

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
  bar.hide_elapsed_time();
  bar.hide_remaining_time();
  
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
```

## Multi-threaded Example

```cpp
#include <indicators/progress_bar.hpp>
#include <vector>

int main() {

  indicators::ProgressBar bar;
  bar.set_bar_width(50);
  bar.start_bar_with("[");
  bar.fill_bar_progress_with("■");
  bar.lead_bar_progress_with("■");
  bar.fill_bar_remainder_with("-");
  bar.end_bar_with("]");
  bar.set_foreground_color(indicators::Color::YELLOW);
  bar.hide_elapsed_time();
  bar.hide_remaining_time();

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
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
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

# Block Progress Bar

Are you in need of a smooth block progress bar using [unicode block elements](https://en.wikipedia.org/wiki/Block_Elements)? Use `BlockProgressBar` instead of `ProgressBar`. Thanks to [this blog post](https://mike42.me/blog/2018-06-make-better-cli-progress-bars-with-unicode-block-characters) for making `BlockProgressBar` an easy addition to the library. 

<p align="center">
  <img src="img/block_progress_bar.gif"/>  
</p>

```cpp
#include <indicators/block_progress_bar.hpp>
#include <thread>
#include <chrono>

int main() {

  // Hide cursor
  std::cout << "\e[?25l";
  
  indicators::BlockProgressBar bar;
  
  // Configure the bar
  bar.set_bar_width(80);
  bar.start_bar_with("[");
  bar.end_bar_with("]");
  bar.set_foreground_color(indicators::Color::WHITE); 
  bar.hide_elapsed_time();
  bar.hide_remaining_time();
  
  // Update bar state
  auto progress = 0.0f;
  while (true) {
    bar.set_progress(progress);
    progress += 0.25f;
    if (bar.is_completed())
      break;
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }

  // Show cursor
  std::cout << "\e[?25h";  

  return 0;
}
```

# Progress Spinner

To introduce a progress spinner in your application, include `indicators/progress_spinner.hpp` and create a `ProgressSpinner` object. 

```cpp
#include <indicators/progress_spinner.hpp>

int main() {
  indicators::ProgressSpinner spinner;
  
  // Configure the spinner
  spinner.set_prefix_text("  ");
  spinner.set_postfix_text("Checking credentials");
  spinner.set_foreground_color(indicators::Color::YELLOW);
  spinner.set_spinner_states({"⠈", "⠐", "⠠", "⢀", "⡀", "⠄", "⠂", "⠁"});
  spinner.hide_elapsed_time();
  spinner.hide_remaining_time();
  
  // Update spinner state

  return 0;
}
```

Here's the general structure of a progress spinner:

```
{prefix} {spinner} {percentage} [{elapsed}<{remaining}] {postfix}
```

ProgressSpinner has a vector of strings: `spinner_states`. At each update, the spinner will pick the next string from this sequence to print to the console. The spinner state can be updated similarly to ProgressBars: Using either `tick()` or `set_progress(value)`. 

```cpp
#include <indicators/progress_spinner.hpp>

int main() {
  indicators::ProgressSpinner spinner;
  
  // Configure the spinner
  spinner.set_postfix_text("Checking credentials");
  spinner.set_foreground_color(indicators::Color::YELLOW);
  spinner.set_spinner_states({"⠈", "⠐", "⠠", "⢀", "⡀", "⠄", "⠂", "⠁"});
  spinner.hide_elapsed_time();
  spinner.hide_remaining_time();
  
  // Update spinner state
  auto job = [&spinner]() {
    while (true) {
      if (spinner.is_completed()) {
        spinner.set_foreground_color(indicators::Color::GREEN);
        spinner.set_prefix_text("✔");
        spinner.hide_spinner();
        spinner.hide_percentage();
        spinner.set_postfix_text("Authenticated!");
        spinner.mark_as_completed();	
        break;
      } else
        spinner.tick();
      std::this_thread::sleep_for(std::chrono::milliseconds(40));
    }
  };
  std::thread thread(job);
  thread.join();  

  return 0;
}
```

## Contributing
Contributions are welcome, have a look at the [CONTRIBUTING.md](CONTRIBUTING.md) document for more information.

## License
The project is available under the [MIT](https://opensource.org/licenses/MIT) license.
