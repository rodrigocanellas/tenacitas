#ifndef TENACITAS_LIB_CROSSWORDS_EVT_EVENTS_H
#define TENACITAS_LIB_CROSSWORDS_EVT_EVENTS_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <iostream>
#include <memory>

#include <tenacitas.lib/src/async/alg/dispatcher.h>
#include <tenacitas.lib/src/crosswords/typ/grid.h>

namespace tenacitas::lib::crosswords::evt {

/// \brief Published when a new grid be assembled
struct new_grid_to_organize {
  new_grid_to_organize() = default;
  explicit new_grid_to_organize(std::shared_ptr<typ::grid> p_grid)
      : grid(p_grid) {}

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const new_grid_to_organize &) {
    p_out << "new grid to organize";
    return p_out;
  }

  std::shared_ptr<typ::grid> grid;
};

/// \brief Publiished when a grid was assembled
struct assembly_finished {
  assembly_finished() = default;
  explicit assembly_finished(std::shared_ptr<typ::grid> p_grid)
      : grid(p_grid) {}
  friend std::ostream &operator<<(std::ostream &p_out,
                                  const assembly_finished &p_evt) {
    if (p_evt.grid) {
      p_out << "organization_finished - grid organized: " << *(p_evt.grid);
    } else {
      p_out << "organization_finished - grid not organized ";
    }
    return p_out;
  }

  std::shared_ptr<typ::grid> grid;
};

/// \brief Published when all attemps of assembling grids should stop
struct stop_organizing {
  stop_organizing() = default;
  friend std::ostream &operator<<(std::ostream &p_out,
                                  const stop_organizing &) {
    p_out << "stop_organizing";
    return p_out;
  }
};

/// \brief Published when a new attempt to assemble a grid has started
struct new_attempt {
  new_attempt() = default;
  new_attempt(uint64_t p_number) : number(p_number) {}

  new_attempt(const new_attempt &) = default;
  new_attempt(new_attempt &) = default;
  new_attempt &operator=(const new_attempt &) = default;
  new_attempt &operator=(new_attempt &) = default;
  friend std::ostream &operator<<(std::ostream &p_out,
                                  const new_attempt &p_new_attempt) {
    p_out << p_new_attempt.number;
    return p_out;
  }
  uint64_t number{0};
};

} // namespace tenacitas::lib::crosswords::evt

#endif
