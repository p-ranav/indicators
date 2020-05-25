#include <indicators/multi_progress.hpp>
#include <indicators/progress_bar.hpp>

int main() {

  indicators::ProgressBar bar1{indicators::option::BarWidth{50},
                               indicators::option::Start{"["},
                               indicators::option::Fill{"■"},
                               indicators::option::Lead{"■"},
                               indicators::option::Remainder{" "},
                               indicators::option::End{" ]"},
                               indicators::option::ForegroundColor{indicators::Color::yellow},
                               indicators::option::ShowElapsedTime{true},
                               indicators::option::ShowRemainingTime{true},
                               indicators::option::PrefixText{"Progress Bar #1 "},
                               indicators::option::FontStyles{std::vector<indicators::FontStyle>{
                                   indicators::FontStyle::bold}}};

  indicators::ProgressBar bar2{indicators::option::BarWidth{50},
                               indicators::option::Start{"["},
                               indicators::option::Fill{"="},
                               indicators::option::Lead{">"},
                               indicators::option::Remainder{" "},
                               indicators::option::End{" ]"},
                               indicators::option::ForegroundColor{indicators::Color::cyan},
                               indicators::option::ShowElapsedTime{true},
                               indicators::option::ShowRemainingTime{true},
                               indicators::option::PrefixText{"Progress Bar #2 "},
                               indicators::option::FontStyles{std::vector<indicators::FontStyle>{
                                   indicators::FontStyle::bold}}};

  indicators::ProgressBar bar3{indicators::option::BarWidth{50},
                               indicators::option::Start{"["},
                               indicators::option::Fill{"#"},
                               indicators::option::Lead{"#"},
                               indicators::option::Remainder{" "},
                               indicators::option::End{" ]"},
                               indicators::option::ForegroundColor{indicators::Color::red},
                               indicators::option::ShowElapsedTime{true},
                               indicators::option::ShowRemainingTime{true},
                               indicators::option::PrefixText{"Progress Bar #3 "},
                               indicators::option::FontStyles{std::vector<indicators::FontStyle>{
                                   indicators::FontStyle::bold}}};

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
