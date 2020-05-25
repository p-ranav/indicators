#include <indicators/progress_bar.hpp>
#include <indicators/multi_progress.hpp>
#include <indicators/terminal_size.hpp>
using namespace indicators;

int main() {

    std::cout << "Terminal width: " << terminal_size().second << "\n";

    // prepare progress bar
    auto prepare_p = [](ProgressBar *p, const std::string &prefix){
        p->set_option(option::PrefixText{prefix});
        p->set_option(option::Start{""});
        p->set_option(option::Fill{""});
        p->set_option(option::Lead{""});
        p->set_option(option::Remainder{""});
        p->set_option(option::End{""});
        p->set_option(option::BarWidth{0});
    };

    ProgressBar p1, p2;

    prepare_p(&p1, "Progress #1");
    prepare_p(&p2, "Progress #2");

    MultiProgress<ProgressBar, 2> mp(p1, p2);

    std::string some_text[] = {"foo", "bar", "independence", "beta", "alfa"};
    std::string dynamic_text;

    // first pb with static postfix text
    p1.set_option(option::PostfixText{"Static text"});

    // second pb with dynamic postfix text
    for (auto &t: some_text) {
        dynamic_text += t + " ";
        p2.set_option(option::PostfixText{dynamic_text});
        mp.set_progress<0>(size_t(0));
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // update postfix to little text for pb #2
    p2.set_option(option::PostfixText{"abcd"});
    mp.set_progress<0>(size_t(0));

}
