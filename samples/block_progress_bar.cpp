#include <chrono>
#include <indicators/block_progress_bar.hpp>
#include <thread>

int main() {

  // Hide cursor
  std::cout << "\e[?25l";

  indicators::BlockProgressBar bar{indicators::option::BarWidth{80}};

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
