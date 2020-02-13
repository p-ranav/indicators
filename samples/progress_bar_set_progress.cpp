#include <chrono>
#include <indicators/progress_bar.hpp>
#include <thread>

int main() {

  // Hide cursor
  std::cout << "\e[?25l";

  indicators::ProgressBar bar{
      indicators::option::BarWidth{50},
      indicators::option::Start{"["},
      indicators::option::Fill{"■"},
      indicators::option::Lead{"■"},
      indicators::option::Remainder{"-"},
      indicators::option::End{" ]"},
      indicators::option::PostfixText{"Loading dependency 1/4"},
      indicators::option::ForegroundColor{indicators::Color::cyan},
  };

  // Update bar state
  bar.set_progress(10); // 10% done

  // do some work
  std::this_thread::sleep_for(std::chrono::milliseconds(800));

  bar.set_option(indicators::option::PostfixText{"Loading dependency 2/4"});

  bar.set_progress(30); // 30% done

  // do some more work
  std::this_thread::sleep_for(std::chrono::milliseconds(700));

  bar.set_option(indicators::option::PostfixText{"Loading dependency 3/4"});

  bar.set_progress(65); // 65% done

  // do final bit of work
  std::this_thread::sleep_for(std::chrono::milliseconds(900));

  bar.set_option(indicators::option::PostfixText{"Loaded dependencies!"});

  bar.set_progress(100); // all done

  bar.mark_as_completed();

  // Show cursor
  std::cout << "\e[?25h";

  return 0;
}
