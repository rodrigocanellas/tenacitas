#ifndef TNCT_CROSSWORDS_ASY_GRID_CREATE_NEW_ATTEMPT_H
#define TNCT_CROSSWORDS_ASY_GRID_CREATE_NEW_ATTEMPT_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <cstddef>
#include <iostream>

namespace tnct::crosswords::evt {

/// \brief Published when a new attempt to assemble a grid has started
struct grid_permutations_tried {

  grid_permutations_tried() = default;
  grid_permutations_tried(const grid_permutations_tried &) = default;

  grid_permutations_tried(size_t p_permutations)
      : permutations(p_permutations) {}

  size_t permutations{0};

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const grid_permutations_tried &) {
    return p_out;
  }
};

} // namespace tnct::crosswords::evt

#endif
