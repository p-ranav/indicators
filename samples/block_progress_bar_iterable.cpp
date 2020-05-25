#include <chrono>
#include <indicators/block_progress_bar.hpp>
#include <indicators/cursor_control.hpp>
#include <thread>

int main() {

  // Hide cursor
  indicators::show_console_cursor(false);

  // Random list of numbers
  std::vector<size_t> numbers;
  for (size_t i = 0; i < 1259438; ++i) {
    numbers.push_back(i);
  }

  using namespace indicators;
  BlockProgressBar bar{option::BarWidth{80}, option::ForegroundColor{Color::white},
                       option::FontStyles{std::vector<FontStyle>{FontStyle::bold}},
                       option::MaxProgress{numbers.size()}};

  std::cout << "Iterating over a list of numbers (size = " << numbers.size() << ")\n";

  std::vector<size_t> result;
  for (size_t i = 0; i < numbers.size(); ++i) {

    // Perform some computation
    result.push_back(numbers[i] * numbers[i]);

    // Show iteration as postfix text
    bar.set_option(option::PostfixText{std::to_string(i) + "/" + std::to_string(numbers.size())});

    // update progress bar
    bar.tick();
  }

  bar.mark_as_completed();

  // Show cursor
  indicators::show_console_cursor(true);

  return 0;
}
