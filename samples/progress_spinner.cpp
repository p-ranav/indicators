#include <indicators/progress_spinner.hpp>

int main() {

  // Hide cursor
  std::cout << "\e[?25l";

  indicators::ProgressSpinner spinner{
    indicators::option::PostfixText{"Checking credentials"},
    indicators::option::ForegroundColor{indicators::Color::YELLOW},
    indicators::option::SpinnerStates{std::vector<std::string>{"⠈", "⠐", "⠠", "⢀", "⡀", "⠄", "⠂", "⠁"}},
  };

  // Update spinner state
  auto job = [&spinner]() {
    while (true) {
      if (spinner.is_completed()) {
        spinner.set_option(indicators::option::ForegroundColor{indicators::Color::GREEN});
        spinner.set_option(indicators::option::PrefixText{"✔"});
        spinner.set_option(indicators::option::SpinnerShow{false});
        spinner.set_option(indicators::option::ShowPercentage{false});
        spinner.set_option(indicators::option::PostfixText{"Authenticated!"});
        spinner.mark_as_completed();
        break;
      } else
        spinner.tick();
      std::this_thread::sleep_for(std::chrono::milliseconds(40));
    }
  };
  std::thread thread(job);
  thread.join();

  // Show cursor
  std::cout << "\e[?25h";

  return 0;
}
