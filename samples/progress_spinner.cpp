#include <indicators/cursor_control.hpp>
#include <indicators/progress_spinner.hpp>

int main() {

  // Hide cursor
  indicators::show_console_cursor(false);

  indicators::ProgressSpinner spinner{
      indicators::option::PostfixText{"Checking credentials"},
      indicators::option::ForegroundColor{indicators::Color::yellow},
      indicators::option::SpinnerStates{
          std::vector<std::string>{"⠈", "⠐", "⠠", "⢀", "⡀", "⠄", "⠂", "⠁"}},
      indicators::option::FontStyles{
          std::vector<indicators::FontStyle>{indicators::FontStyle::bold}}};

  // Update spinner state
  auto job = [&spinner]() {
    while (true) {
      if (spinner.is_completed()) {
        spinner.set_option(indicators::option::ForegroundColor{indicators::Color::green});
        spinner.set_option(indicators::option::PrefixText{"✔"});
        spinner.set_option(indicators::option::ShowSpinner{false});
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
  indicators::show_console_cursor(true);

  return 0;
}
