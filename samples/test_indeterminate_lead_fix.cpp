
#include <indicators/details/stream_helper.hpp>
#include <indicators/indeterminate_progress_bar.hpp>
#include <indicators/progress_bar.hpp>
#include <sstream>
#include <cassert>
#include <iostream>
#include <string>
#include <atomic>

// Helper: check that a callable does NOT hang (returns within a timeout)
// We use a simple flag-based approach since std::thread is available.
template <typename Func>
bool does_not_hang(Func f, int timeout_ms = 5000) {
    std::atomic<bool> finished{false};
    std::thread t([&]() { f(); finished = true; });
    std::this_thread::sleep_for(std::chrono::milliseconds(timeout_ms));
    if (!finished.load()) {
        t.detach();
        return false;
    }
    t.join();
    return true;
}

int main() {
    // ===== IndeterminateProgressScaleWriter tests =====

    // Test 1: Empty lead should not cause infinite loop
    {
        std::ostringstream os;
        indicators::details::IndeterminateProgressScaleWriter writer(os, 10, ".", "");
        writer.write(5);
        std::string out = os.str();
        assert(!out.empty());
        std::cout << "PASS [Indeterminate]: Empty lead at progress=5 -> \""
                  << out << "\" (len=" << out.size() << ")\n";
    }

    // Test 2: Normal lead (regression test)
    {
        std::ostringstream os;
        indicators::details::IndeterminateProgressScaleWriter writer(os, 10, ".", "<==>");
        writer.write(5);
        std::string out = os.str();
        assert(out.find("<==>") != std::string::npos);
        std::cout << "PASS [Indeterminate]: Normal lead at progress=5 -> \""
                  << out << "\" (len=" << out.size() << ")\n";
    }

    // Test 3: Empty lead at end of bar
    {
        std::ostringstream os;
        indicators::details::IndeterminateProgressScaleWriter writer(os, 10, ".", "");
        writer.write(9);
        std::string out = os.str();
        assert(!out.empty());
        std::cout << "PASS [Indeterminate]: Empty lead at progress=9 -> \""
                  << out << "\" (len=" << out.size() << ")\n";
    }

    // Test 4: Empty lead at start of bar
    {
        std::ostringstream os;
        indicators::details::IndeterminateProgressScaleWriter writer(os, 10, ".", "");
        writer.write(0);
        std::string out = os.str();
        assert(!out.empty());
        std::cout << "PASS [Indeterminate]: Empty lead at progress=0 -> \""
                  << out << "\" (len=" << out.size() << ")\n";
    }

    // Test 5: Very wide bar with empty lead (stress test)
    {
        std::ostringstream os;
        indicators::details::IndeterminateProgressScaleWriter writer(os, 100, "=", "");
        writer.write(50);
        std::string out = os.str();
        assert(!out.empty());
        std::cout << "PASS [Indeterminate]: Empty lead, bar_width=100, progress=50 -> length="
                  << out.size() << "\n";
    }

    // ===== ProgressScaleWriter tests (same bug, different class) =====

    // Test 6: Empty fill — should not hang
    {
        std::ostringstream os;
        indicators::details::ProgressScaleWriter writer(os, 10, "", ".", " ");
        writer.write(50.0f);
        std::string out = os.str();
        assert(!out.empty());
        std::cout << "PASS [Determinate]: Empty fill at 50% -> \""
                  << out << "\" (len=" << out.size() << ")\n";
    }

    // Test 7: Empty lead in determinate bar — should not hang
    {
        std::ostringstream os;
        indicators::details::ProgressScaleWriter writer(os, 10, "=", "", " ");
        writer.write(50.0f);
        std::string out = os.str();
        assert(!out.empty());
        std::cout << "PASS [Determinate]: Empty lead in determinate bar at 50% -> \""
                  << out << "\" (len=" << out.size() << ")\n";
    }

    // Test 8: Empty remainder — should not hang
    {
        std::ostringstream os;
        indicators::details::ProgressScaleWriter writer(os, 10, "=", ">", "");
        writer.write(50.0f);
        std::string out = os.str();
        assert(!out.empty());
        std::cout << "PASS [Determinate]: Empty remainder at 50% -> \""
                  << out << "\" (len=" << out.size() << ")\n";
    }

    // Test 9: Normal determinate writer (regression)
    {
        std::ostringstream os;
        indicators::details::ProgressScaleWriter writer(os, 10, "=", ">", " ");
        writer.write(50.0f);
        std::string out = os.str();
        assert(out.find(">") != std::string::npos);
        std::cout << "PASS [Determinate]: Normal (fill='=', lead='>', rem=' ') at 50% -> \""
                  << out << "\" (len=" << out.size() << ")\n";
    }

    // ===== IndeterminateProgressBar constructor fix =====
    // Verify that an IndeterminateProgressBar with empty lead initializes
    // without hanging, prints something, and tick() works.
    {
        std::stringstream ss;
        indicators::IndeterminateProgressBar bar{
            indicators::option::BarWidth{20},
            indicators::option::Start{"["},
            indicators::option::Fill{"."},
            indicators::option::Lead{""},     // ← 空 Lead
            indicators::option::End{"]"},
            indicators::option::Stream{ss}};

        // Print once
        bar.print_progress();
        std::string out = ss.str();
        assert(!out.empty());
        std::cout << "PASS [IndeterminateBar]: Empty lead bar prints -> \""
                  << out.substr(0, 25) << "...\"\n";

        // Tick a few times — should not hang
        for (int i = 0; i < 10; ++i) {
            bar.tick();
        }
        out = ss.str();
        assert(!out.empty());
        std::cout << "PASS [IndeterminateBar]: Empty lead bar tick() x10 works\n";
    }

    // ===== Normal IndeterminateProgressBar regression =====
    {
        std::stringstream ss;
        indicators::IndeterminateProgressBar bar{
            indicators::option::BarWidth{20},
            indicators::option::Start{"["},
            indicators::option::Fill{"."},
            indicators::option::Lead{"<==>"},
            indicators::option::End{"]"},
            indicators::option::Stream{ss}};

        bar.print_progress();
        std::string out = ss.str();
        assert(out.find("<==>") != std::string::npos);
        std::cout << "PASS [IndeterminateBar]: Normal bar with '<==>' works -> \""
                  << out.substr(0, 30) << "...\"\n";
    }

    // ===== Determinate ProgressBar with empty fill (regression) =====
    {
        std::stringstream ss;
        indicators::ProgressBar bar{
            indicators::option::BarWidth{20},
            indicators::option::Start{"["},
            indicators::option::Fill{""},     // ← 空 Fill
            indicators::option::Lead{">"},
            indicators::option::Remainder{" "},
            indicators::option::End{"]"},
            indicators::option::Stream{ss}};

        bar.set_progress(50);
        std::string out = ss.str();
        assert(!out.empty());
        assert(out.find(">") != std::string::npos);
        std::cout << "PASS [DeterminateBar]: Empty fill bar at 50% -> \""
                  << out.substr(0, 30) << "...\"\n";
    }

    std::cout << "\n🎉 All tests passed!\n";
    return 0;
}
