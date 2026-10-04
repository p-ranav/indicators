#ifdef INDICATORS_TEST_SINGLE_HEADER
#include <indicators/indicators.hpp>
#else
#include <indicators/progress_spinner.hpp>
#endif

#include <iostream>
#include <sstream>
#include <string>

namespace {
int failures = 0;

void check(bool condition, const char *message) {
  if (!condition) {
    std::cerr << message << '\n';
    ++failures;
  }
}

void test_ticks(size_t maximum) {
  using namespace indicators;
  std::ostringstream output;
  ProgressSpinner spinner{option::MaxProgress{maximum}, option::ShowSpinner{false},
                          option::Stream{output}};
  for (size_t i = 0; i < maximum; ++i) {
    check(!spinner.is_completed(), "spinner completed before its maximum");
    spinner.tick();
    check(spinner.current() == i + 1, "tick changed the current progress incorrectly");
  }
  check(spinner.is_completed(), "spinner did not complete on its final tick");
  const std::string text = output.str();
  check(text.find(" 100%") != std::string::npos, "final tick did not print 100%");
  check(!text.empty() && text.back() == '\n', "final tick did not terminate the line");
}

void test_set_progress(size_t maximum) {
  using namespace indicators;
  std::ostringstream output;
  ProgressSpinner spinner{option::MaxProgress{maximum}, option::ShowSpinner{false},
                          option::Stream{output}};
  spinner.set_progress(maximum - 1);
  check(!spinner.is_completed(), "set_progress completed before the maximum");
  check(output.str().find('\n') == std::string::npos,
        "incomplete spinner unexpectedly terminated the line");
  output.str("");
  spinner.set_progress(maximum);
  check(spinner.is_completed(), "set_progress did not complete at the maximum");
  check(spinner.current() == maximum, "set_progress changed the current value incorrectly");
  check(output.str() == " 100%           \r\n", "completed spinner output is incorrect");
}

void test_other_completion_paths() {
  using namespace indicators;
  std::ostringstream output;
  ProgressSpinner overshoot{option::MaxProgress{3}, option::ShowSpinner{false},
                           option::Stream{output}};
  overshoot.set_progress(4);
  check(overshoot.is_completed(), "overshooting the maximum no longer completes");
  check(overshoot.current() == 3, "overshoot current value is no longer clamped");

  ProgressSpinner manual{option::ShowSpinner{false}, option::Stream{output}};
  manual.mark_as_completed();
  check(manual.is_completed(), "manual completion no longer works");

  ProgressSpinner defaults{option::ShowSpinner{false}, option::Stream{output}};
  defaults.set_progress(100);
  check(defaults.is_completed(), "default maximum does not complete at 100");
}
} // namespace

int main() {
  for (size_t maximum : {size_t(1), size_t(3), size_t(100)}) {
    test_ticks(maximum);
    test_set_progress(maximum);
  }
  test_other_completion_paths();
  std::cout << failures << " failed checks\n";
  return failures == 0 ? 0 : 1;
}
