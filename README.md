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
  <img src="https://img.shields.io/badge/version-1.7-blue.svg?cacheSeconds=2592000" alt="version"/>
</p>

<p align="center">
  <img src="img/demo.gif"/> 
</p>

# Highlights

* Thread-safe progress bars and spinners
* Header-only library. Grab a copy of `include/indicators`
* Source for the above GIF can be found [here](demo/demo.cpp)
* MIT License

```bash
git clone https://github.com/p-ranav/indicators
cd indicators
mkdir build && cd build
cmake -DINDICATORS_SAMPLES=ON -DINDICATORS_DEMO=ON ..
make
```

# Table of Contents

* [Progress Bar](#progress-bar)
* [Block Progress Bar](#block-progress-bar)
* [Multi Progress](#multiprogress)
* [Dynamic Progress](#dynamicprogress)
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
  using namespace indicators;
  ProgressBar bar{
    option::BarWidth{50},
    option::Start{"["},
    option::Fill{"="},
    option::Lead{">"},
    option::Remainder{" "},
    option::End{"]"},
    option::PostfixText{"Extracting Archive"},
    option::ForegroundColor{Color::green}
  };
  
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
  using namespace indicators;
  ProgressBar bar{
    option::BarWidth{50},
    option::Start{"["},
    option::Fill{"■"},
    option::Lead{"■"},
    option::Remainder{"-"},
    option::End{" ]"},
    option::PostfixText{"Loading dependency 1/4"},
    option::ForegroundColor{Color::cyan}  
  };

  // Update bar state
  bar.set_progress(10); // 10% done

  // do some work
  std::this_thread::sleep_for(std::chrono::milliseconds(800));

  bar.set_option(option::PostfixText{"Loading dependency 2/4"});  

  bar.set_progress(30); // 30% done

  // do some more work
  std::this_thread::sleep_for(std::chrono::milliseconds(700));

  bar.set_option(option::PostfixText{"Loading dependency 3/4"});  

  bar.set_progress(65); // 65% done

  // do final bit of work
  std::this_thread::sleep_for(std::chrono::milliseconds(900));

  bar.set_option(option::PostfixText{"Loaded dependencies!"});

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
  using namespace indicators;
  indicators::ProgressBar bar{
    option::BarWidth{50},
    option::Start{" ["},
    option::Fill{"█"},
    option::Lead{"█"},
    option::Remainder{"-"},
    option::End{"]"},
    option::PrefixText{"Training Gaze Network 👀"},
    option::ForegroundColor{Color::yellow},
    option::ShowElapsedTime{true},
    option::ShowRemainingTime{true}
  };

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
  using namespace indicators;

  BlockProgressBar bar{
    option::BarWidth{80},
    option::Start{"["},
    option::End{"]"},
    option::ForegroundColor{Color::white}  
  };
  
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
  using namespace indicators;
  // Configure first progress bar
  ProgressBar bar1{
    option::BarWidth{50},
    option::Start{"["},
    option::Fill{"■"},
    option::Lead{"■"},
    option::Remainder{" "},
    option::End{" ]"},
    option::ForegroundColor{Color::yellow},
    option::ShowElapsedTime{true},
    option::ShowRemainingTime{true},
    option::PrefixText{"Progress Bar #1 "}
  };

  // Configure second progress bar

  ProgressBar bar2{
    option::BarWidth{50},
    option::Start{"["},
    option::Fill{"="},
    option::Lead{">"},
    option::Remainder{" "},
    option::End{" ]"},
    option::ForegroundColor{Color::cyan},
    option::ShowElapsedTime{true},
    option::ShowRemainingTime{true},
    option::PrefixText{"Progress Bar #2 "}
  };
  
  // Configure third progress bar
  indicators::ProgressBar bar3{
    option::BarWidth{50},
    option::Start{"["},
    option::Fill{"#"},
    option::Lead{"#"},
    option::Remainder{" "},
    option::End{" ]"},
    option::ForegroundColor{Color::red},
    option::ShowElapsedTime{true},
    option::ShowRemainingTime{true},
    option::PrefixText{"Progress Bar #3 "}
  };

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

# DynamicProgress

`DynamicProgress` is a container class, similar to `MultiProgress`, for managing multiple progress bars. As the name suggests, with `DynamicProgress`, you can dynamically add new progress bars. Simply call `bars.push_back`. 

Below is an example `DynamicProgress` object that manages six `ProgressBar` objects. Three of these bars are added dynamically.

<p align="center">
  <img src="img/dynamic_progress_bar.gif"/>  
</p>

```cpp
#include <indicators/dynamic_progress.hpp>
#include <indicators/progress_bar.hpp>
using namespace indicators;

int main() {

  ProgressBar bar1{option::BarWidth{50}, option::ForegroundColor{Color::red},
                   option::ShowElapsedTime{true}, option::ShowRemainingTime{true},
                   option::PrefixText{"5c90d4a2d1a8: Downloading "}};

  ProgressBar bar2{option::BarWidth{50}, option::ForegroundColor{Color::yellow},
                   option::ShowElapsedTime{true}, option::ShowRemainingTime{true},
                   option::PrefixText{"22337bfd13a9: Downloading "}};

  ProgressBar bar3{option::BarWidth{50}, option::ForegroundColor{Color::green},
                   option::ShowElapsedTime{true}, option::ShowRemainingTime{true},
                   option::PrefixText{"10f26c680a34: Downloading "}};

  ProgressBar bar4{option::BarWidth{50}, option::ForegroundColor{Color::white},
                   option::ShowElapsedTime{true}, option::ShowRemainingTime{true},
                   option::PrefixText{"6364e0d7a283: Downloading "}};

  ProgressBar bar5{option::BarWidth{50}, option::ForegroundColor{Color::blue},
                   option::ShowElapsedTime{true}, option::ShowRemainingTime{true},
                   option::PrefixText{"ff1356ba118b: Downloading "}};

  ProgressBar bar6{option::BarWidth{50}, option::ForegroundColor{Color::cyan},
                   option::ShowElapsedTime{true}, option::ShowRemainingTime{true},
                   option::PrefixText{"5a17453338b4: Downloading "}};

  std::cout << termcolor::bold << termcolor::white << "Pulling image foo:bar/baz\n";

  // Construct with 3 progress bars. We'll add 3 more at a later point
  DynamicProgress<ProgressBar> bars(bar1, bar2, bar3);
  
  // Do not hide bars when completed
  bars.set_option(option::HideBarWhenComplete{false});

  std::thread fourth_job, fifth_job, sixth_job;

  auto job4 = [&bars](size_t i) {
    while (true) {
      bars[i].tick();
      if (bars[i].is_completed()) {
        bars[i].set_option(option::PrefixText{"6364e0d7a283: Pull complete "});
        bars[i].mark_as_completed();
        break;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
  };

  auto job5 = [&bars](size_t i) {
    while (true) {
      bars[i].tick();
      if (bars[i].is_completed()) {
        bars[i].set_option(option::PrefixText{"ff1356ba118b: Pull complete "});
        bars[i].mark_as_completed();
        break;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  };

  auto job6 = [&bars](size_t i) {
    while (true) {
      bars[i].tick();
      if (bars[i].is_completed()) {
        bars[i].set_option(option::PrefixText{"5a17453338b4: Pull complete "});
        bars[i].mark_as_completed();
        break;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(40));
    }
  };

  auto job1 = [&bars, &bar6, &sixth_job, &job6]() {
    while (true) {
      bars[0].tick();
      if (bars[0].is_completed()) {
        bars[0].set_option(option::PrefixText{"5c90d4a2d1a8: Pull complete "});
        // bar1 is completed, adding bar6
        auto i = bars.push_back(bar6);
        sixth_job = std::thread(job6, i);
        sixth_job.join();
        break;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(140));
    }
  };

  auto job2 = [&bars, &bar5, &fifth_job, &job5]() {
    while (true) {
      bars[1].tick();
      if (bars[1].is_completed()) {
        bars[1].set_option(option::PrefixText{"22337bfd13a9: Pull complete "});
        // bar2 is completed, adding bar5
        auto i = bars.push_back(bar5);
        fifth_job = std::thread(job5, i);
        fifth_job.join();
        break;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(25));
    }
  };

  auto job3 = [&bars, &bar4, &fourth_job, &job4]() {
    while (true) {
      bars[2].tick();
      if (bars[2].is_completed()) {
        bars[2].set_option(option::PrefixText{"10f26c680a34: Pull complete "});
        // bar3 is completed, adding bar4
        auto i = bars.push_back(bar4);
        fourth_job = std::thread(job4, i);
        fourth_job.join();
        break;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
  };

  std::thread first_job(job1);
  std::thread second_job(job2);
  std::thread third_job(job3);

  third_job.join();
  second_job.join();
  first_job.join();

  std::cout << termcolor::bold << termcolor::green << "✔ Downloaded image foo/bar:baz" << std::endl;
  std::cout << termcolor::reset;

  return 0;
}
```

In the above code, notice the option `bars.set_option(option::HideBarWhenComplete{true});`. Yes, you can hide progress bars as and when they complete by setting this option to `true`. If you do so, the above example will look like this:

<p align="center">
  <img src="img/dynamic_progress_bar_hide_completed.gif"/>  
</p>

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
  using namespace indicators;
  indicators::ProgressSpinner spinner{
    option::PostfixText{"Checking credentials"},
    option::ForegroundColor{Color::yellow},
    option::SpinnerStates{std::vector<std::string>{"⠈", "⠐", "⠠", "⢀", "⡀", "⠄", "⠂", "⠁"}}
  };
 
  // Update spinner state
  auto job = [&spinner]() {
    while (true) {
      if (spinner.is_completed()) {
        spinner.set_option(option::ForegroundColor{Color::green});
        spinner.set_option(option::PrefixText{"✔"});
        spinner.set_option(option::ShowSpinner{false});
        spinner.set_option(option::ShowPercentage{false});
        spinner.set_option(option::PostfixText{"Authenticated!"});
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
