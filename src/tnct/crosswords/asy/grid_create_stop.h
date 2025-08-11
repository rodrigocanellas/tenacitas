#ifndef TENACITAS_SRC_CROSSWORDS_ASY_GRID_CREATE_STOP_H
#define TENACITAS_SRC_CROSSWORDS_ASY_GRID_CREATE_STOP_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <iostream>

namespace tenacitas::src::crosswords::asy {

/// \brief Published when all attemps of assembling grids should stop
struct grid_create_stop {

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const grid_create_stop &) {
    return p_out;
  }
};

} // namespace tenacitas::src::crosswords::asy

#endif
