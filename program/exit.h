/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_PROGRAM_EXIT_H
#define TENACITAS_LIB_PROGRAM_EXIT_H

#include <iostream>

namespace tenacitas::lib::program {

/// \brief Event to be published if one wants the 'program
struct exit {
  friend std::ostream &operator<<(std::ostream &p_out, const exit &) {
    p_out << "program::exit";
    return p_out;
  }
};

} // namespace tenacitas::lib::program

#endif
