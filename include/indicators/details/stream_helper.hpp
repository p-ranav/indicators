#pragma once

#include <indicators/color.hpp>

#include <ostream>
#include <cassert>

namespace indicators {
namespace details {

inline void set_stream_color(std::ostream& stream, Color color) {
    switch (color) {
    case Color::GREY:
      stream << termcolor::grey;
      break;
    case Color::RED:
      stream << termcolor::red;
      break;
    case Color::GREEN:
      stream << termcolor::green;
      break;
    case Color::YELLOW:
      stream << termcolor::yellow;
      break;
    case Color::BLUE:
      stream << termcolor::blue;
      break;
    case Color::MAGENTA:
      stream << termcolor::magenta;
      break;
    case Color::CYAN:
      stream << termcolor::cyan;
      break;
    case Color::WHITE:
      stream << termcolor::white;
      break;
    default:
      assert(false);
    }
}

}
}