#include <indicators/progress_spinner.hpp>

int main() {

  // Hide cursor
  std::cout << "\e[?25l";

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

  // Show cursor
  std::cout << "\e[?25h";

  return 0;
}
