#include <indicators/block_progress_bar.hpp>
#include <indicators/multi_progress.hpp>

int main() {
  using namespace indicators;
  BlockProgressBar bar1{option::BarWidth{50},
                        option::ForegroundColor{Color::yellow},
                        option::ShowElapsedTime{true},
                        option::ShowRemainingTime{true},
                        option::PrefixText{"Progress Bar #1 "},
                        indicators::option::FontStyles{
                            std::vector<indicators::FontStyle>{indicators::FontStyle::bold}}};

  BlockProgressBar bar2{option::BarWidth{50},
                        option::ForegroundColor{Color::cyan},
                        option::ShowElapsedTime{true},
                        option::ShowRemainingTime{true},
                        option::PrefixText{"Progress Bar #2 "},
                        indicators::option::FontStyles{
                            std::vector<indicators::FontStyle>{indicators::FontStyle::bold}}};

  BlockProgressBar bar3{option::BarWidth{50},
                        option::ForegroundColor{Color::red},
                        option::ShowElapsedTime{true},
                        option::ShowRemainingTime{true},
                        option::PrefixText{"Progress Bar #3 "},
                        indicators::option::FontStyles{
                            std::vector<indicators::FontStyle>{indicators::FontStyle::bold}}};

  indicators::MultiProgress<indicators::BlockProgressBar, 3> bars(bar1, bar2, bar3);

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
