#include <indicators/progress_bar.hpp>
#include <vector>

int main() {

  indicators::ProgressBar bar;
  bar.set_bar_width(50);
  bar.start_bar_with("[");
  bar.fill_bar_progress_with("■");
  bar.lead_bar_progress_with("■");
  bar.fill_bar_remainder_with("-");
  bar.end_bar_with("]");
  bar.set_foreground_color(indicators::Color::YELLOW);

  // As configured, the bar will look like this:
  //
  // [■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■-------------] 70%
  //
  //

  std::atomic<size_t> index{0};
  std::vector<std::string> status_text = {"Rocket.exe is not responding",
                                          "Finding a replacement engineer",
                                          "Buying more snacks",
                                          "Assimilating the modding community",
                                          "Crossing fingers",
                                          "Porting KSP to a Nokia 3310"};

  // Let's say you want to append some status text to the right of the progress bar
  // You can use bar.set_postfix_text(...) to append text to the right
  //
  // [■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■-------------] 70% Finding a replacement engineer
  //                                                        ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  //
  //

  auto job = [&bar, &index, &status_text]() {
    while (true) {
      if (bar.is_completed()) {
        break;
      }
      bar.set_postfix_text(status_text[index % status_text.size()]);
      bar.tick();
      index += 1;
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
  };

  std::thread first_job(job);
  std::thread second_job(job);
  std::thread third_job(job);
  std::thread last_job(job);

  first_job.join();
  second_job.join();
  third_job.join();
  last_job.join();

  return 0;
}
