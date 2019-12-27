<p align="center">
  <img height="70" src="img/logo.png"/>  
</p>

<p align="center">
  <a href="https://travis-ci.com/p-ranav/indicators">
    <img src="https://travis-ci.com/p-ranav/indicators.svg?branch=master" alt="ci status"/>
  </a>
  <a href="https://www.codacy.com/manual/p-ranav/indicators?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=p-ranav/indicators&amp;utm_campaign=Badge_Grade">
    <img src="https://api.codacy.com/project/badge/Grade/93401e73f250407cb32445afec4e3e99" alt="codacy"/>
  </a>
  <a href="https://github.com/p-ranav/indicators/blob/master/LICENSE">
    <img src="https://img.shields.io/badge/License-MIT-yellow.svg" alt="license"/>
  </a>
  <img src="https://img.shields.io/badge/version-1.5-blue.svg?cacheSeconds=2592000" alt="version"/>
</p>

<p align="center">
  <img src="img/demo.gif"/> 
</p>

# Highlights

* Thread-safe progress bars and spinners
* Header-only library. Grab a copy of `include/indicators`
* Source for the above GIF can be found [here](demo/demo.cpp)
* MIT License

# Table of Contents

* [Progress Bar](#progress-bar)
* [Block Progress Bar](#block-progress-bar)
* [Multi Progress](#multiprogress)
* [Progress Spinner](#progress-spinner)
* [Contributing](#contributing)
* [License](#license)

# Progress bar

To introduce a progress bar in your application, include `indicators/progress_bar.hpp` and create a `ProgressBar` object. Here's the general structure of a progress bar:

```
{prefix} {start} {fill} {lead} {remaining} {end} {percentage} [{elapsed}<{remaining}] {postfix}
         ^^^^^^^^^^^^^ Bar Width ^^^^^^^^^^^^^^^   
```

The amount of progress in ProgressBar is maintained as a float in range `[0, 100]`. When progress reaches 100, the progression is complete. 

From application-level code, there are two ways in which you can update this progress:

## Update progress using `bar.tick()`

You can update the progress bar using `bar.tick()` which increments progress by exactly `1%`.

<p align="center">
  <img src="img/progress_bar_tick.gif"/>  
</p>

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
  bar.set_postfix_text("Extracting Archive");
  bar.set_foreground_color(indicators::Color::GREEN); 
  
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


<p align="center">
  <img src="img/progress_bar_set_progress.gif"/>  
</p>

```cpp
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
```

## Showing Time Elapsed/Remaining

All progress bars and spinners in `indicators` support showing time elapsed and time remaining. Inspired by python's [tqdm](https://github.com/tqdm/tqdm) module, the format of this meter is `[{elapsed}<{remaining}]`:

<p align="center">
  <img src="img/time_meter.gif"/>  
</p>

```cpp
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

# MultiProgress

`indicators` supports management of multiple progress bars with the `MultiProgress` class template. 

`template <typename Indicator, size_t count> class MultiProgress` is a class template that holds references to multiple progress bars and provides a safe interface to update the state of each bar. `MultiProgress` works with both `ProgressBar` and `BlockProgressBar` classes.

Below is an example `MultiProgress` object that manages three `ProgressBar` objects.

<p align="center">
  <img src="img/multi_progress.gif"/>  
</p>

```cpp
#include <indicators/multi_progress.hpp>
#include <indicators/progress_bar.hpp>

int main() {

  // Configure first progress bar
  indicators::ProgressBar bar1;
  bar1.set_bar_width(50);
  bar1.start_bar_with("[");
  bar1.fill_bar_progress_with("■");
  bar1.lead_bar_progress_with("■");
  bar1.fill_bar_remainder_with(" ");
  bar1.end_bar_with(" ]");
  bar1.set_foreground_color(indicators::Color::YELLOW);
  bar1.show_elapsed_time();
  bar1.show_remaining_time();
  bar1.set_prefix_text("Progress Bar #1 ");

  // Configure second progress bar
  indicators::ProgressBar bar2;
  bar2.set_bar_width(50);
  bar2.start_bar_with("[");
  bar2.fill_bar_progress_with("=");
  bar2.lead_bar_progress_with(">");
  bar2.fill_bar_remainder_with(" ");
  bar2.end_bar_with(" ]");
  bar2.set_foreground_color(indicators::Color::CYAN);
  bar2.show_elapsed_time();
  bar2.show_remaining_time();
  bar2.set_prefix_text("Progress Bar #2 ");

  // Configure third progress bar
  indicators::ProgressBar bar3;
  bar3.set_bar_width(50);
  bar3.start_bar_with("[");
  bar3.fill_bar_progress_with("#");
  bar3.lead_bar_progress_with("#");
  bar3.fill_bar_remainder_with(" ");
  bar3.end_bar_with(" ]");
  bar3.set_foreground_color(indicators::Color::RED);
  bar3.show_elapsed_time();
  bar3.show_remaining_time();
  bar3.set_prefix_text("Progress Bar #3 ");

  // Construct MultiProgress object
  indicators::MultiProgress<indicators::ProgressBar, 3> bars(bar1, bar2, bar3);

  std::cout << "Multiple Progress Bars:\n";

  auto job1 = [&bars]() {
    while (true) {
      bars.tick<0>();
      if (bars.is_completed<0>())
        break;
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  };

  auto job2 = [&bars]() {
    while (true) {
      bars.tick<1>();
      if (bars.is_completed<1>())
        break;
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
  };

  auto job3 = [&bars]() {
    while (true) {
      bars.tick<2>();
      if (bars.is_completed<2>())
        break;
      std::this_thread::sleep_for(std::chrono::milliseconds(60));
    }
  };

  std::thread first_job(job1);
  std::thread second_job(job2);
  std::thread third_job(job3);

  first_job.join();
  second_job.join();
  third_job.join();

  return 0;
}
```

# Progress Spinner

To introduce a progress spinner in your application, include `indicators/progress_spinner.hpp` and create a `ProgressSpinner` object. Here's the general structure of a progress spinner:

```
{prefix} {spinner} {percentage} [{elapsed}<{remaining}] {postfix}
```

ProgressSpinner has a vector of strings: `spinner_states`. At each update, the spinner will pick the next string from this sequence to print to the console. The spinner state can be updated similarly to ProgressBars: Using either `tick()` or `set_progress(value)`. 

<p align="center">
  <img src="img/progress_spinner.gif"/>  
</p>

```cpp
#include <indicators/progress_spinner.hpp>

int main() {
  indicators::ProgressSpinner spinner;
  
  // Configure the spinner
  spinner.set_postfix_text("Checking credentials");
  spinner.set_foreground_color(indicators::Color::YELLOW);
  spinner.set_spinner_states({"⠈", "⠐", "⠠", "⢀", "⡀", "⠄", "⠂", "⠁"});
  
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
