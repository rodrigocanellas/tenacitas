#ifndef TNCT_CROSSWORDS_ASY_GRID_CREATE_UNSOLVED_H
#define TNCT_CROSSWORDS_ASY_GRID_CREATE_UNSOLVED_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <iostream>

#include "tnct/crosswords/dat/index.h"

namespace tnct::crosswords::evt {

/// \brief Published when a grid was assembled
struct grid_create_unsolved {

  grid_create_unsolved() = default;
  grid_create_unsolved(const grid_create_unsolved &) = default;

  explicit grid_create_unsolved(dat::index p_num_rows, dat::index p_num_cols)
      : num_rows(p_num_rows), num_cols(p_num_cols) {}

  dat::index num_rows;
  dat::index num_cols;

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const grid_create_unsolved &) {
    return p_out;
  }
};

} // namespace tnct::crosswords::evt

#endif
