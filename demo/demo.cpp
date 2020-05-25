#include <indicators/cursor_control.hpp>
#include <indicators/progress_bar.hpp>
#include <indicators/progress_spinner.hpp>
#include <vector>

int main() {
  using namespace indicators;

  // Hide cursor
  show_console_cursor(false);

  {
    //
    // PROGRESS BAR 1
    //
    indicators::ProgressBar p{
        option::BarWidth{50},
        option::Start{"["},
        option::Fill{"■"},
        option::Lead{"■"},
        option::Remainder{" "},
        option::End{" ]"},
        option::ForegroundColor{indicators::Color::yellow},
        option::FontStyles{std::vector<indicators::FontStyle>{indicators::FontStyle::bold}}};

    std::atomic<size_t> index{0};
    std::vector<std::string> status_text = {"Rocket.exe is not responding",
                                            "Buying more snacks",
                                            "Finding a replacement engineer",
                                            "Assimilating the modding community",
                                            "Crossing fingers",
                                            "Porting KSP to a Nokia 3310",
                                            "Flexing struts",
                                            "Releasing space whales",
                                            "Watching paint dry"};

    auto job = [&p, &index, &status_text]() {
      while (true) {
        if (p.is_completed())
          break;
        p.set_option(option::PostfixText{status_text[index % status_text.size()]});
        p.set_progress(index * 10);
        index += 1;
        std::this_thread::sleep_for(std::chrono::milliseconds(600));
      }
    };
    std::thread thread(job);
    thread.join();
  }

  {
    //
    // PROGRESS BAR 2
    //
    indicators::ProgressBar p;
    p.set_option(option::BarWidth{0});
    p.set_option(option::PrefixText{"Reading package list... "});
    p.set_option(option::Start{""});
    p.set_option(option::Fill{""});
    p.set_option(option::Lead{""});
    p.set_option(option::Remainder{""});
    p.set_option(option::End{""});
    p.set_option(option::ForegroundColor{indicators::Color::white});
    p.set_option(option::ShowPercentage{false});
    p.set_option(
        option::FontStyles{std::vector<indicators::FontStyle>{indicators::FontStyle::bold}});
    auto job = [&p]() {
      while (true) {
        p.set_option(
            option::PrefixText{"Reading package list... " + std::to_string(p.current()) + "% "});
        if (p.current() + 2 >= 100)
          p.set_option(option::PrefixText{"Reading package list... Done"});
        p.tick();
        if (p.is_completed()) {
          break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
      }
    };
    std::thread thread(job);
    thread.join();
  }

  {
    //
    // PROGRESS BAR 3
    //
    indicators::ProgressBar p;
    p.set_option(option::BarWidth{50});
    p.set_option(option::Start{"["});
    p.set_option(option::Fill{"="});
    p.set_option(option::Lead{">"});
    p.set_option(option::Remainder{" "});
    p.set_option(option::End{"]"});
    p.set_option(option::PostfixText{"Getting started"});
    p.set_option(option::ForegroundColor{indicators::Color::green});
    p.set_option(
        option::FontStyles{std::vector<indicators::FontStyle>{indicators::FontStyle::bold}});
    auto job = [&p]() {
      while (true) {
        auto ticks = p.current();
        if (ticks > 20 && ticks < 50)
          p.set_option(option::PostfixText{"Delaying the inevitable"});
        else if (ticks > 50 && ticks < 80)
          p.set_option(option::PostfixText{"Crying quietly"});
        else if (ticks > 80 && ticks < 98)
          p.set_option(option::PostfixText{"Almost there"});
        else if (ticks >= 98)
          p.set_option(option::PostfixText{"Done"});
        p.tick();
        if (p.is_completed())
          break;
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
      }
    };
    std::thread thread(job);
    thread.join();
  }

  {
    //
    // PROGRESS BAR 4
    //
    std::vector<std::string> lead_spinner{"⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏"};
    indicators::ProgressBar p4;
    p4.set_option(option::BarWidth{40});
    p4.set_option(option::Start{""});
    p4.set_option(option::Fill{"⠸"});
    p4.set_option(option::Lead{""});
    p4.set_option(option::Remainder{" "});
    p4.set_option(option::End{""});
    p4.set_option(option::ForegroundColor{indicators::Color::cyan});
    p4.set_option(option::PostfixText{"Restoring system state"});
    p4.set_option(option::ShowPercentage{false});
    p4.set_option(
        option::FontStyles{std::vector<indicators::FontStyle>{indicators::FontStyle::bold}});
    std::atomic<size_t> index4{0};
    auto job4 = [&p4, &index4, &lead_spinner]() {
      while (true) {
        p4.set_option(option::PrefixText{"{ " + std::to_string(p4.current()) + "% } "});
        p4.set_option(option::Lead{lead_spinner[index4 % lead_spinner.size()]});
        index4 += 1;
        if (p4.current() + 2 >= 100) {
          std::cout << std::endl;
          p4.set_option(option::ForegroundColor{indicators::Color::red});
          p4.set_option(option::PrefixText{"{ ERROR }"});
          p4.set_option(option::BarWidth{0});
          p4.set_option(option::Start{});
          p4.set_option(option::Fill{});
          p4.set_option(option::Lead{});
          p4.set_option(option::Remainder{});
          p4.set_option(option::ShowPercentage{true});
          p4.set_option(option::PostfixText{"Failed to restore system"});
          p4.mark_as_completed();
          break;
        }
        p4.tick();
        std::this_thread::sleep_for(std::chrono::milliseconds(40));
      }
    };
    std::thread thread4(job4);
    thread4.join();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    {
      //
      // GOING BACKWARDS
      //
      indicators::ProgressBar p{
          option::BarWidth{50},
          option::ProgressType{ProgressType::decremental},
          option::Start{"["},
          option::Fill{"■"},
          option::Lead{"■"},
          option::Remainder{"-"},
          option::End{"]"},
          option::ForegroundColor{indicators::Color::white},
          option::PostfixText{"Reverting system restore"},
          option::FontStyles{std::vector<indicators::FontStyle>{indicators::FontStyle::bold}}};
      auto job = [&p]() {
        while (true) {
          p.tick();
          if (p.is_completed()) {
            p.set_option(option::PostfixText{"Revert complete!"});
            break;
          }
          std::this_thread::sleep_for(std::chrono::milliseconds(60));
        }
      };
      std::thread thread(job);
      thread.join();
    }
  }

  {
    //
    // PROGRESS BAR 5
    //
    indicators::ProgressSpinner p{
        option::PrefixText{""}, option::PostfixText{"Checking credentials"},
        option::ForegroundColor{indicators::Color::yellow},
        option::SpinnerStates{std::vector<std::string>{"⠈", "⠐", "⠠", "⢀", "⡀", "⠄", "⠂", "⠁"}},
        option::FontStyles{std::vector<indicators::FontStyle>{indicators::FontStyle::bold}}};

    auto job = [&p]() {
      while (true) {
        if (p.is_completed()) {
          p.set_option(option::ForegroundColor{indicators::Color::green});
          p.set_option(option::PrefixText{"✔"});
          p.set_option(option::ShowSpinner{false});
          p.set_option(option::ShowPercentage{false});
          p.set_option(option::PostfixText{"Authenticated!"});
          p.mark_as_completed();
          break;
        } else
          p.tick();
        std::this_thread::sleep_for(std::chrono::milliseconds(40));
      }
    };
    std::thread thread(job);
    thread.join();
  }

  std::cout << "  Compiling mission\n";
  {
    //
    // PROGRESS BAR 6
    //
    indicators::ProgressSpinner p{
        option::PrefixText{" - "},
        option::PostfixText{"Searching for the Moon"},
        option::ForegroundColor{indicators::Color::white},
        option::ShowPercentage{false},
        option::SpinnerStates{std::vector<std::string>{"▖", "▘", "▝", "▗"}},
        option::FontStyles{std::vector<indicators::FontStyle>{indicators::FontStyle::bold}}};
    auto job = [&p]() {
      while (true) {
        auto current = p.current();
        if (current == 24) {
          p.set_option(option::PrefixText{" - ✔"});
          p.set_option(option::ShowSpinner{false});
        } else if (current == 25) {
          std::cout << std::endl;
          p.set_option(option::ShowSpinner{true});
          p.set_option(option::PrefixText{" - "});
          p.set_option(option::PostfixText{"Contacting Kerbal headquarters"});
        } else if (current == 49) {
          p.set_option(option::PrefixText{" - ✔"});
          p.set_option(option::ShowSpinner{false});
        } else if (current == 50) {
          std::cout << std::endl;
          p.set_option(option::ShowSpinner{true});
          p.set_option(option::PrefixText{" - "});
          p.set_option(option::PostfixText{"Designing spaceship"});
        } else if (current == 74) {
          p.set_option(option::PrefixText{" - ✔"});
          p.set_option(option::ShowSpinner{false});
        } else if (current == 75) {
          std::cout << std::endl;
          p.set_option(option::ShowSpinner{true});
          p.set_option(option::PrefixText{" - "});
          p.set_option(option::PostfixText{"Launching rocket"});
        } else if (current == 95) {
          p.set_option(option::PrefixText{" - ✔"});
          p.set_option(option::ShowSpinner{false});
        } else if (current == 99) {
          std::cout << std::endl;
          //
          // NESTED PROGRESS BAR
          //
          indicators::ProgressBar p2{
              option::BarWidth{30},
              option::PrefixText{" - "},
              option::Start{"🌎"},
              option::Fill{"·"},
              option::Lead{"🚀"},
              option::Remainder{" "},
              option::End{"🌑"},
              option::PostfixText{"Achieved low-Earth orbit"},
              option::ForegroundColor{indicators::Color::white},
              option::FontStyles{std::vector<indicators::FontStyle>{indicators::FontStyle::bold}}};
          std::vector<std::string> ship_trail{"⠁", "⠂", "⠄", "⡀", "⢀", "⠠", "⠐", "⠈"};
          std::atomic<int> ship_trail_index{0};
          auto job2 = [&p2, &ship_trail_index, &ship_trail]() {
            while (true) {
              auto ticks = p2.current();
              if (ticks > 20 && ticks < 50)
                p2.set_option(option::PostfixText{"Switching to trans-lunar trajectory"});
              else if (ticks > 50 && ticks < 80)
                p2.set_option(option::PostfixText{"Transferred to Lunar lander"});
              else if (ticks > 80 && ticks < 98)
                p2.set_option(option::PostfixText{"Almost there"});
              else if (ticks >= 98)
                p2.set_option(option::PostfixText{"Landed on the Moon"});
              p2.tick();
              ship_trail_index += 1;
              if (p2.is_completed())
                break;
              std::this_thread::sleep_for(std::chrono::milliseconds(80));
            }
          };
          std::thread thread2(job2);
          thread2.join();
          p.set_option(indicators::option::PostfixText{"Mission successful!"});
          p.mark_as_completed();
          break;
        }
        p.tick();
        if (p.is_completed())
          break;
        std::this_thread::sleep_for(std::chrono::milliseconds(60));
      }
    };
    std::thread thread(job);
    thread.join();
  }

  // Show cursor
  show_console_cursor(true);

  return 0;
}