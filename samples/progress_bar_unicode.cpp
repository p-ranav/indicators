#include <chrono>
#include <indicators/progress_bar.hpp>
#include <thread>

int main() {

    {
        // Plain old ASCII
        indicators::ProgressBar bar{
            indicators::option::BarWidth{50},
            indicators::option::Start{"["},
            indicators::option::Fill{"="},
            indicators::option::Lead{">"},
            indicators::option::Remainder{" "},
            indicators::option::End{" ]"},
            indicators::option::PostfixText{"Plain-old ASCII"},
            indicators::option::ForegroundColor{indicators::Color::green},
            indicators::option::FontStyles{
                std::vector<indicators::FontStyle>{indicators::FontStyle::bold}}
        };

        // Update bar state
        while (true) {
            bar.tick();
            if (bar.is_completed())
            break;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    {
        // Unicode
        indicators::ProgressBar bar{
            indicators::option::BarWidth{50},
            indicators::option::Start{"["},
            indicators::option::Fill{"驚くばかり"},
            indicators::option::Lead{">"},
            indicators::option::Remainder{" "},
            indicators::option::End{" ]"},
            indicators::option::PostfixText{"Japanese"},
            indicators::option::ForegroundColor{indicators::Color::yellow},
            indicators::option::FontStyles{
                std::vector<indicators::FontStyle>{indicators::FontStyle::bold}}
        };

        // Update bar state
        while (true) {
            bar.tick();
            if (bar.is_completed())
            break;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    {
        // Russian
        indicators::ProgressBar bar{
            indicators::option::BarWidth{50},
            indicators::option::Start{"["},
            indicators::option::Fill{"Потрясающие"},
            indicators::option::Remainder{" "},
            indicators::option::End{" ]"},
            indicators::option::PostfixText{"Russian"},
            indicators::option::ForegroundColor{indicators::Color::red},
            indicators::option::FontStyles{
                std::vector<indicators::FontStyle>{indicators::FontStyle::bold}}
        };

        // Update bar state
        while (true) {
            bar.tick();
            if (bar.is_completed())
            break;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    {
        // Greek
        indicators::ProgressBar bar{
            indicators::option::BarWidth{50},
            indicators::option::Start{"["},
            indicators::option::Fill{"Φοβερός"},
            indicators::option::Remainder{" "},
            indicators::option::End{" ]"},
            indicators::option::PostfixText{"Greek"},
            indicators::option::ForegroundColor{indicators::Color::cyan},
            indicators::option::FontStyles{
                std::vector<indicators::FontStyle>{indicators::FontStyle::bold}}
        };

        // Update bar state
        while (true) {
            bar.tick();
            if (bar.is_completed())
            break;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    {
        // Chinese
        indicators::ProgressBar bar{
            indicators::option::BarWidth{50},
            indicators::option::Start{"["},
            indicators::option::Fill{"太棒了"},
            indicators::option::Remainder{" "},
            indicators::option::End{" ]"},
            indicators::option::PostfixText{"Chinese"},
            indicators::option::ForegroundColor{indicators::Color::green},
            indicators::option::FontStyles{
                std::vector<indicators::FontStyle>{indicators::FontStyle::bold}}
        };

        // Update bar state
        while (true) {
            bar.tick();
            if (bar.is_completed())
            break;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }        
    }

    {
        // Emojis
        indicators::ProgressBar bar{
            indicators::option::BarWidth{50},
            indicators::option::Start{"["},
            indicators::option::Fill{"🔥"},
            indicators::option::Lead{"🔥"},
            indicators::option::Remainder{" "},
            indicators::option::End{" ]"},
            indicators::option::PostfixText{"Emojis"},
            indicators::option::ForegroundColor{indicators::Color::white},
            indicators::option::FontStyles{
                std::vector<indicators::FontStyle>{indicators::FontStyle::bold}}
        };

        // Update bar state
        while (true) {
            bar.tick();
            if (bar.is_completed())
            break;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

  return 0;
}
