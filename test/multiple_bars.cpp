#include <progress/bar.hpp>
#include <vector>

int main() {

  //
  // PROGRESS BAR 1
  //
  ProgressBar p1;
  p1.bar_width(50);
  p1.start_with("[");
  p1.fill_progress_with("■");
  p1.lead_progress_with("■");
  p1.fill_remainder_with(" ");
  p1.end_with(" ]");  
  p1.color(ProgressBar::Color::YELLOW);

  std::atomic<size_t> index1{0};
  std::vector<std::string> status_text1 =
    {
     "Rocket.exe is not responding",
     "Finding a replacement engineer",
     "Buying more snacks",
     "Assimilating the modding community",
     "Crossing fingers",
     "Porting KSP to a Nokia 3310",
     "Flexing struts",
     "Releasing space whales",
     "Watching paint dry"
  };
  
  auto job1 = [&p1, &index1, &status_text1]() {
	       while (true) {
		 if (p1.completed())
		   break;
		 p1.append_text(status_text1[index1 % status_text1.size()]);
		 p1.set_progress(index1 * 15);
		 index1 += 1;
		 std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	       }
	     };
  std::thread thread1(job1);
  thread1.join();

  //
  // PROGRESS BAR 2
  //
  ProgressBar p2;
  p2.bar_width(50);
  p2.start_with("Getting started [");
  p2.fill_progress_with("#");
  p2.lead_progress_with("#");
  p2.fill_remainder_with(" ");
  p2.end_with("]");  
  p2.color(ProgressBar::Color::GREEN);
  auto job2 = [&p2]() {
	       while (true) {
		 if (p2.completed())
		   break;
		 p2.tick();
		 auto ticks = p2.current();
		 if (ticks > 20 && ticks < 50)
		   p2.start_with("Delaying the inevitable [");
		 else if (ticks > 50 && ticks < 80)
		   p2.start_with("Crying quietly [");
		 else if (ticks > 80)
		   p2.start_with("Almost there [");
		 else if (ticks > 98)
		   p2.start_with("Done [");
		 std::this_thread::sleep_for(std::chrono::milliseconds(100));
	       }
	     };
  std::thread thread2(job2);  
  thread2.join();

  //
  // PROGRESS BAR 3
  //
  ProgressBar p3;
  p3.bar_width(40);
  p3.start_with("Downloading the Sun ");
  p3.fill_progress_with(".");
  p3.lead_progress_with(".");
  p3.fill_remainder_with(" ");
  p3.end_with("");  
  p3.color(ProgressBar::Color::WHITE);
  p3.show_percentage(false);
  auto job3 = [&p3]() {
	       while (true) {
		 p3.start_with("Downloading the Sun " + std::to_string(p3.current()) + "% ");
		 p3.tick();
		 if (p3.completed()) {
		   p3.start_with("Downloading the Sun 100%");
		   break;
		 }		 
		 std::this_thread::sleep_for(std::chrono::milliseconds(50));
	       }
	     };
  std::thread thread3(job3);  
  thread3.join();
  
  return 0;
}
