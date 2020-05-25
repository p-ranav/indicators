#include <indicators/progress_bar.hpp>
#include <vector>

int main() {

  indicators::ProgressBar bar{indicators::option::BarWidth{50},
                              indicators::option::Start{"["},
                              indicators::option::Fill{"■"},
                              indicators::option::Lead{"■"},
                              indicators::option::Remainder{"-"},
                              indicators::option::End{" ]"},
                              indicators::option::ForegroundColor{indicators::Color::yellow},
                              indicators::option::FontStyles{
                                  std::vector<indicators::FontStyle>{indicators::FontStyle::bold}}};

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
      bar.set_option(indicators::option::PostfixText{status_text[index % status_text.size()]});
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
