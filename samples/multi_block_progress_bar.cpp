#include <indicators/block_progress_bar.hpp>
#include <indicators/multi_progress.hpp>

int main() {

  indicators::BlockProgressBar bar1;
  bar1.set_bar_width(50);
  bar1.set_foreground_color(indicators::Color::YELLOW);
  bar1.show_elapsed_time();
  bar1.show_remaining_time();
  bar1.set_prefix_text("Progress Bar #1 ");

  indicators::BlockProgressBar bar2;
  bar2.set_bar_width(50);
  bar2.set_foreground_color(indicators::Color::CYAN);
  bar2.show_elapsed_time();
  bar2.show_remaining_time();
  bar2.set_prefix_text("Progress Bar #2 ");

  indicators::BlockProgressBar bar3;
  bar3.set_bar_width(50);
  bar3.set_foreground_color(indicators::Color::RED);
  bar3.show_elapsed_time();
  bar3.show_remaining_time();
  bar3.set_prefix_text("Progress Bar #3 ");

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
