#pragma once

#include <indicators/color.hpp>
#include <indicators/termcolor.hpp>

#include <chrono>
#include <iomanip>
#include <ostream>

#include <cassert>

namespace indicators {
namespace details {

inline void set_stream_color(std::ostream &os, Color color) {
    switch (color) {
        case Color::GREY:
            os << termcolor::grey;
            break;
        case Color::RED:
            os << termcolor::red;
            break;
        case Color::GREEN:
            os << termcolor::green;
            break;
        case Color::YELLOW:
            os << termcolor::yellow;
            break;
        case Color::BLUE:
            os << termcolor::blue;
            break;
        case Color::MAGENTA:
            os << termcolor::magenta;
            break;
        case Color::CYAN:
            os << termcolor::cyan;
            break;
        case Color::WHITE:
            os << termcolor::white;
            break;
        default:
            assert(false);
    }
}

std::ostream &print_duration(std::ostream &os, std::chrono::nanoseconds ns) {
    using namespace std;
    using namespace std::chrono;
    using days = duration<int, ratio<86400>>;
    char fill = os.fill();
    os.fill('0');
    auto d = duration_cast<days>(ns);
    ns -= d;
    auto h = duration_cast<hours>(ns);
    ns -= h;
    auto m = duration_cast<minutes>(ns);
    ns -= m;
    auto s = duration_cast<seconds>(ns);
    if (d.count() > 0)
        os << setw(2) << d.count() << "d:";
    if (h.count() > 0)
        os << setw(2) << h.count() << "h:";
    os << setw(2) << m.count() << "m:" << setw(2) << s.count() << 's';
    os.fill(fill);
    return os;
};

}
}