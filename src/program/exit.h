/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_SRC_PROGRAM_EXIT_H
#define TENACITAS_SRC_PROGRAM_EXIT_H

#include <iostream>

namespace tenacitas::src::program {

/// \brief Event to be published if one wants the 'program
struct exit {
  friend std::ostream &operator<<(std::ostream &p_out, const exit &) {
    p_out << "program::exit";
    return p_out;
  }
};

} // namespace tenacitas::src::program

#endif
