/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_PROGRAM_EXIT_H
#define TNCT_PROGRAM_EXIT_H

#include <iostream>

namespace tnct::program {

/// \brief Event to be published if one wants the 'program
struct exit {
  friend std::ostream &operator<<(std::ostream &p_out, const exit &) {
    p_out << "program::exit";
    return p_out;
  }
};

} // namespace tnct::program

#endif
