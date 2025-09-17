#ifndef TNCT_CROSSWORDS_EVT_INTERNAL_GRID_ORGANIZE_H
#define TNCT_CROSSWORDS_EVT_INTERNAL_GRID_ORGANIZE_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <iostream>
#include <memory>

#include "tnct/crosswords/dat/grid.h"

namespace tnct::crosswords::evt::internal
{

/// \brief Published when a new grid be assembled
struct grid_to_organize
{

  grid_to_organize()                         = default;
  grid_to_organize(const grid_to_organize &) = default;

  explicit grid_to_organize(std::shared_ptr<dat::grid> p_grid) : grid(p_grid)
  {
  }

  std::shared_ptr<dat::grid> grid;

  friend std::ostream &operator<<(std::ostream &p_out, const grid_to_organize &)
  {
    return p_out;
  }
};

} // namespace tnct::crosswords::evt::internal

#endif
