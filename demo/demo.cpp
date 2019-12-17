#include <indicators/progress_bar.hpp>
#include <indicators/progress_spinner.hpp>
#include <vector>

int main() {

  // Hide cursor
  std::cout << "\e[?25l";

  {
    //
    // PROGRESS BAR 1
    //
    indicators::ProgressBar p;
    p.set_bar_width(50);
    p.start_bar_with("[");
    p.fill_bar_progress_with("■");
    p.lead_bar_progress_with("■");
    p.fill_bar_remainder_with(" ");
    p.end_bar_with(" ]");
    p.set_foreground_color(indicators::Color::YELLOW);

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
        p.set_postfix_text(status_text[index % status_text.size()]);
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
    p.set_bar_width(40);
    p.set_prefix_text("Reading package list... ");
    p.start_bar_with("");
    p.fill_bar_progress_with("");
    p.lead_bar_progress_with("");
    p.fill_bar_remainder_with("");
    p.end_bar_with("");
    p.set_foreground_color(indicators::Color::WHITE);
    p.hide_percentage();
    auto job = [&p]() {
      while (true) {
        p.set_prefix_text("Reading package list... " + std::to_string(p.current()) + "% ");
        if (p.current() + 2 >= 100)
          p.set_prefix_text("Reading package list... Done");
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
    p.set_bar_width(50);
    p.start_bar_with("[");
    p.fill_bar_progress_with("=");
    p.lead_bar_progress_with(">");
    p.fill_bar_remainder_with(" ");
    p.end_bar_with("]");
    p.set_postfix_text("Getting started");
    p.set_foreground_color(indicators::Color::GREEN);
    auto job = [&p]() {
      while (true) {
        auto ticks = p.current();
        if (ticks > 20 && ticks < 50)
          p.set_postfix_text("Delaying the inevitable");
        else if (ticks > 50 && ticks < 80)
          p.set_postfix_text("Crying quietly");
        else if (ticks > 80 && ticks < 98)
          p.set_postfix_text("Almost there");
        else if (ticks >= 98)
          p.set_postfix_text("Done");
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
    p4.set_bar_width(40);
    p4.start_bar_with("");
    p4.fill_bar_progress_with("⠸");
    p4.lead_bar_progress_with("");
    p4.fill_bar_remainder_with(" ");
    p4.end_bar_with("");
    p4.set_foreground_color(indicators::Color::CYAN);
    p4.set_postfix_text("Restoring system state");
    p4.hide_percentage();
    std::atomic<size_t> index4{0};
    auto job4 = [&p4, &index4, &lead_spinner]() {
      while (true) {
        p4.set_prefix_text("{ " + std::to_string(p4.current()) + "% } ");
        p4.lead_bar_progress_with(lead_spinner[index4 % lead_spinner.size()]);
        index4 += 1;
        if (p4.current() + 2 >= 100) {
          std::cout << std::endl;
          p4.set_foreground_color(indicators::Color::RED);
          p4.set_prefix_text("{ ERROR }");
          p4.start_bar_with("");
          p4.fill_bar_progress_with("");
          p4.lead_bar_progress_with("");
          p4.fill_bar_remainder_with("");
          p4.end_bar_with("");
          p4.show_percentage();
          p4.set_postfix_text("Failed to restore system");
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
      indicators::ProgressBar p;
      p.set_bar_width(50);
      p.start_bar_with("[");
      p.fill_bar_progress_with("■");
      p.lead_bar_progress_with("■");
      p.fill_bar_remainder_with("-");
      p.end_bar_with("]");
      p.set_progress(100);
      p.set_foreground_color(indicators::Color::WHITE);
      p.set_postfix_text("Reverting system restore");
      std::atomic<size_t> progress{100};
      auto job = [&p, &progress]() {
        while (true) {
          progress -= 1;
          p.set_progress(progress);
          if (progress == 0) {
            p.set_postfix_text("Revert complete!");
            p.mark_as_completed();
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
    indicators::ProgressSpinner p;
    p.set_prefix_text("");
    p.set_postfix_text("Checking credentials");
    p.set_foreground_color(indicators::Color::YELLOW);
    p.set_spinner_states({"⠈", "⠐", "⠠", "⢀", "⡀", "⠄", "⠂", "⠁"});
    auto job = [&p]() {
      while (true) {
        if (p.is_completed()) {
          p.set_foreground_color(indicators::Color::GREEN);
          p.set_prefix_text("✔");
          p.hide_spinner();
          p.hide_percentage();
          p.set_postfix_text("Authenticated!");
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
    indicators::ProgressSpinner p;
    p.set_prefix_text(" - ");
    p.set_postfix_text("Searching for the Moon");
    p.set_foreground_color(indicators::Color::WHITE);
    p.set_spinner_states({"▖", "▘", "▝", "▗"});
    p.hide_percentage();
    auto job = [&p]() {
      while (true) {
        auto current = p.current();
        if (current == 24) {
          p.set_prefix_text(" - ✔");
          p.hide_spinner();
        } else if (current == 25) {
          std::cout << std::endl;
          p.show_spinner();
          p.set_prefix_text(" - ");
          p.set_postfix_text("Contacting Kerbal headquarters");
        } else if (current == 49) {
          p.set_prefix_text(" - ✔");
          p.hide_spinner();
        } else if (current == 50) {
          std::cout << std::endl;
          p.show_spinner();
          p.set_prefix_text(" - ");
          p.set_postfix_text("Designing spaceship");
        } else if (current == 74) {
          p.set_prefix_text(" - ✔");
          p.hide_spinner();
        } else if (current == 75) {
          std::cout << std::endl;
          p.show_spinner();
          p.set_prefix_text(" - ");
          p.set_postfix_text("Launching rocket");
        } else if (current == 95) {
          p.set_prefix_text(" - ✔");
          p.hide_spinner();
        } else if (current == 99) {
          std::cout << std::endl;
          //
          // NESTED PROGRESS BAR
          //
          indicators::ProgressBar p2;
          p2.set_bar_width(30);
          p2.set_prefix_text(" - ");
          p2.start_bar_with("🌎");
          p2.fill_bar_progress_with("·");
          p2.lead_bar_progress_with("🚀");
          p2.fill_bar_remainder_with(" ");
          p2.end_bar_with("🌑");
          p2.set_postfix_text("Achieved low-Earth orbit");
          p2.set_foreground_color(indicators::Color::WHITE);
          std::vector<std::string> ship_trail{"⠁", "⠂", "⠄", "⡀", "⢀", "⠠", "⠐", "⠈"};
          std::atomic<int> ship_trail_index{0};
          auto job2 = [&p2, &ship_trail_index, &ship_trail]() {
            while (true) {
              auto ticks = p2.current();
              if (ticks > 20 && ticks < 50)
                p2.set_postfix_text("Switching to trans-lunar trajectory");
              else if (ticks > 50 && ticks < 80)
                p2.set_postfix_text("Transferred to Lunar lander");
              else if (ticks > 80 && ticks < 98)
                p2.set_postfix_text("Almost there");
              else if (ticks >= 98)
                p2.set_postfix_text("Landed on the Moon");
              p2.tick();
              ship_trail_index += 1;
              if (p2.is_completed())
                break;
              std::this_thread::sleep_for(std::chrono::milliseconds(80));
            }
          };
          std::thread thread2(job2);
          thread2.join();
          p.set_postfix_text("Mission successful!");
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
  std::cout << "\e[?25h";

  return 0;
}
