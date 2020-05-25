#include <chrono>
#include <indicators/progress_bar.hpp>
#include <thread>

int main() {
  indicators::ProgressBar bar{indicators::option::BarWidth{50},
                              indicators::option::Start{"["},
                              indicators::option::Fill{"="},
                              indicators::option::Lead{">"},
                              indicators::option::Remainder{" "},
                              indicators::option::End{" ]"},
                              indicators::option::PostfixText{"Getting started"},
                              indicators::option::ForegroundColor{indicators::Color::green},
                              indicators::option::FontStyles{
                                  std::vector<indicators::FontStyle>{indicators::FontStyle::bold}}};

  // Update bar state
  while (true) {
    bar.tick();
    if (bar.is_completed())
      break;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  return 0;
}
