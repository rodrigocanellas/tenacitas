#ifndef TENACITAS_SRC_CROSSWORDS_ASY_GRID_CREATE_SOLVED_H
#define TENACITAS_SRC_CROSSWORDS_ASY_GRID_CREATE_SOLVED_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <iostream>
#include <memory>

#include "tenacitas/src/crosswords/dat/grid.h"

namespace tenacitas::src::crosswords::asy {

/// \brief Published when a grid was assembled
struct grid_create_solved {
  grid_create_solved() = default;
  grid_create_solved(const grid_create_solved &) = default;
  explicit grid_create_solved(std::shared_ptr<dat::grid> p_grid,
                              std::chrono::seconds p_time)
      : grid(p_grid), time(p_time) {}

  std::shared_ptr<dat::grid> grid;
  std::chrono::seconds time;

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const grid_create_solved &) {
    return p_out;
  }
};

} // namespace tenacitas::src::crosswords::asy

#endif
