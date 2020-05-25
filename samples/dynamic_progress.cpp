#include <indicators/dynamic_progress.hpp>
#include <indicators/progress_bar.hpp>
using namespace indicators;

int main() {

  ProgressBar bar1{option::BarWidth{50},
                   option::ForegroundColor{Color::red},
                   option::ShowElapsedTime{true},
                   option::ShowRemainingTime{true},
                   option::PrefixText{"5c90d4a2d1a8: Downloading "},
                   indicators::option::FontStyles{
                       std::vector<indicators::FontStyle>{indicators::FontStyle::bold}}};

  ProgressBar bar2{option::BarWidth{50},
                   option::ForegroundColor{Color::yellow},
                   option::ShowElapsedTime{true},
                   option::ShowRemainingTime{true},
                   option::PrefixText{"22337bfd13a9: Downloading "},
                   indicators::option::FontStyles{
                       std::vector<indicators::FontStyle>{indicators::FontStyle::bold}}};

  ProgressBar bar3{option::BarWidth{50},
                   option::ForegroundColor{Color::green},
                   option::ShowElapsedTime{true},
                   option::ShowRemainingTime{true},
                   option::PrefixText{"10f26c680a34: Downloading "},
                   indicators::option::FontStyles{
                       std::vector<indicators::FontStyle>{indicators::FontStyle::bold}}};

  ProgressBar bar4{option::BarWidth{50},
                   option::ForegroundColor{Color::white},
                   option::ShowElapsedTime{true},
                   option::ShowRemainingTime{true},
                   option::PrefixText{"6364e0d7a283: Downloading "},
                   indicators::option::FontStyles{
                       std::vector<indicators::FontStyle>{indicators::FontStyle::bold}}};

  ProgressBar bar5{option::BarWidth{50},
                   option::ForegroundColor{Color::blue},
                   option::ShowElapsedTime{true},
                   option::ShowRemainingTime{true},
                   option::PrefixText{"ff1356ba118b: Downloading "},
                   indicators::option::FontStyles{
                       std::vector<indicators::FontStyle>{indicators::FontStyle::bold}}};

  ProgressBar bar6{option::BarWidth{50},
                   option::ForegroundColor{Color::cyan},
                   option::ShowElapsedTime{true},
                   option::ShowRemainingTime{true},
                   option::PrefixText{"5a17453338b4: Downloading "},
                   indicators::option::FontStyles{
                       std::vector<indicators::FontStyle>{indicators::FontStyle::bold}}};

  std::cout << termcolor::bold << termcolor::white << "Pulling image foo:bar/baz\n";

  DynamicProgress<ProgressBar> bars(bar1, bar2, bar3);
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
