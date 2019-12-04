#include <progress/bar.hpp>
#include <vector>

/*
  [
    "|/-\\",
    "⠂-–—–-",
    "◐◓◑◒",
    "◴◷◶◵",
    "◰◳◲◱",
    "▖▘▝▗",
    "■□▪▫",
    "▌▀▐▄",
    "▉▊▋▌▍▎▏▎▍▌▋▊▉",
    "▁▃▄▅▆▇█▇▆▅▄▃",
    "←↖↑↗→↘↓↙",
    "┤┘┴└├┌┬┐",
    "◢◣◤◥",
    ".oO°Oo.",
    ".oO@*",
    ["🌍", "🌎", "🌏"],
    "◡◡ ⊙⊙ ◠◠",
    "☱☲☴",
    "⠋⠙⠹⠸⠼⠴⠦⠧⠇⠏",
    "⠋⠙⠚⠞⠖⠦⠴⠲⠳⠓",
    "⠄⠆⠇⠋⠙⠸⠰⠠⠰⠸⠙⠋⠇⠆",
    "⠋⠙⠚⠒⠂⠂⠒⠲⠴⠦⠖⠒⠐⠐⠒⠓⠋",
    "⠁⠉⠙⠚⠒⠂⠂⠒⠲⠴⠤⠄⠄⠤⠴⠲⠒⠂⠂⠒⠚⠙⠉⠁",
    "⠈⠉⠋⠓⠒⠐⠐⠒⠖⠦⠤⠠⠠⠤⠦⠖⠒⠐⠐⠒⠓⠋⠉⠈",
    "⠁⠁⠉⠙⠚⠒⠂⠂⠒⠲⠴⠤⠄⠄⠤⠠⠠⠤⠦⠖⠒⠐⠐⠒⠓⠋⠉⠈⠈",
    "⢄⢂⢁⡁⡈⡐⡠",
    "⢹⢺⢼⣸⣇⡧⡗⡏",
    "⣾⣽⣻⢿⡿⣟⣯⣷",
    "⠁⠂⠄⡀⢀⠠⠐⠈",
    ["🌑", "🌒", "🌓", "🌔", "🌕", "🌝", "🌖", "🌗", "🌘", "🌚"],
    ["🕛", "🕐", "🕑", "🕒", "🕓", "🕔", "🕕", "🕖", "🕗", "🕘", "🕙", "🕚"]
  ]
*/

int main() {

  // Hide cursor
  std::cout << "\e[?25l";

  {

  }

  //
  // PROGRESS BAR 1
  //
  ProgressBar p1;
  p1.bar_width(50);
  p1.start_with("[");
  p1.fill_progress_with("■");
  p1.lead_progress_with("■");
  p1.fill_remainder_with(" ");
  p1.end_with("]");
  p1.color(ProgressBar::Color::YELLOW);

  std::atomic<size_t> index1{0};
  std::vector<std::string> status_text1 = {"Rocket.exe is not responding",
                                           "Buying more snacks",
                                           "Finding a replacement engineer",
                                           "Assimilating the modding community",
                                           "Crossing fingers",
                                           "Porting KSP to a Nokia 3310",
                                           "Flexing struts",
                                           "Releasing space whales",
                                           "Watching paint dry"};

  auto job1 = [&p1, &index1, &status_text1]() {
    while (true) {
      if (p1.completed())
        break;
      p1.set_status_text(status_text1[index1 % status_text1.size()]);
      p1.set_progress(index1 * 10);
      index1 += 1;
      std::this_thread::sleep_for(std::chrono::milliseconds(600));
    }
  };
  std::thread thread1(job1);
  thread1.join();

  //
  // PROGRESS BAR 3
  //
  ProgressBar p3;
  p3.bar_width(40);
  p3.start_with("Reading package list... ");
  p3.fill_progress_with("");
  p3.lead_progress_with("");
  p3.fill_remainder_with("");
  p3.end_with("");
  p3.color(ProgressBar::Color::WHITE);
  p3.show_percentage(false);
  auto job3 = [&p3]() {
    while (true) {
      p3.start_with("Reading package list... " + std::to_string(p3.current()) +
                    "% ");
      if (p3.current() + 2 >= 100)
        p3.start_with("Reading package list... Done");
      p3.tick();
      if (p3.completed()) {
        break;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(25));
    }
  };
  std::thread thread3(job3);
  thread3.join();

  //
  // PROGRESS BAR 2
  //
  ProgressBar p2;
  p2.bar_width(50);
  p2.start_with("[");
  p2.fill_progress_with("=");
  p2.lead_progress_with(">");
  p2.fill_remainder_with(" ");
  p2.end_with("]");
  p2.set_status_text("Getting started");
  p2.color(ProgressBar::Color::GREEN);
  auto job2 = [&p2]() {
    while (true) {
      auto ticks = p2.current();
      if (ticks > 20 && ticks < 50)
        p2.set_status_text("Delaying the inevitable");
      else if (ticks > 50 && ticks < 80)
        p2.set_status_text("Crying quietly");
      else if (ticks > 80 && ticks < 98)
        p2.set_status_text("Almost there");
      else if (ticks >= 98)
        p2.set_status_text("Done");
      p2.tick();
      if (p2.completed())
        break;
      std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
  };
  std::thread thread2(job2);
  thread2.join();

  //
  // PROGRESS BAR 5
  //
  ProgressBar p5;
  p5.bar_width(100);
  p5.start_with("🌏");
  p5.fill_progress_with("-");
  p5.lead_progress_with("🛸");
  p5.fill_remainder_with(" ");
  p5.end_with("🌑");
  p5.show_percentage(true);
  p5.color(ProgressBar::Color::WHITE);
  auto job5 = [&p5]() {
    while (true) {
      p5.tick();
      if (p5.completed()) {
        break;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(75));
    }
  };
  std::thread thread5(job5);
  thread5.join();

  //
  // PROGRESS BAR 4
  //
  std::vector<std::string> lead_spinner{"⠋", "⠙", "⠹", "⠸", "⠼",
                                        "⠴", "⠦", "⠧", "⠇", "⠏"};
  ProgressBar p4;
  p4.bar_width(50);
  p4.start_with("[");
  p4.fill_progress_with("⠸");
  p4.lead_progress_with("");
  p4.fill_remainder_with(" ");
  p4.end_with(" ]");
  p4.color(ProgressBar::Color::CYAN);
  p4.set_status_text("Restoring system state");
  std::atomic<size_t> index4{0};
  auto job4 = [&p4, &index4, &lead_spinner]() {
    while (true) {
      p4.lead_progress_with(lead_spinner[index4 % lead_spinner.size()]);
      index4 += 1;
      if (p4.current() + 2 >= 100)
        p4.set_status_text("State restored to Restore_Point_1241531");
      p4.tick();
      if (p4.completed()) {
        break;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
  };
  std::thread thread4(job4);
  thread4.join();

  std::this_thread::sleep_for(std::chrono::milliseconds(5000));

  // Show cursor
  std::cout << "\e[?25h";

  return 0;
}
