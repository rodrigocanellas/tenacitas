#ifndef TENACITAS_SRC_CROSSWORDS_ASY_GRID_ATTEMPT_CONFIGURATION_H
#define TENACITAS_SRC_CROSSWORDS_ASY_GRID_ATTEMPT_CONFIGURATION_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <iostream>

#include "tenacitas/src/crosswords/dat/index.h"

namespace tenacitas::src::crosswords::asy {

struct grid_attempt_configuration {
  grid_attempt_configuration() = default;
  grid_attempt_configuration(const grid_attempt_configuration &) = default;
  grid_attempt_configuration(dat::index p_num_rows, dat::index p_num_cols,
                             std::size_t p_max_memory_to_be_used,
                             std::size_t p_memory_available,
                             std::size_t p_number_of_permutations)
      : num_rows(p_num_rows), num_cols(p_num_cols),
        max_memory_to_be_used(p_max_memory_to_be_used),
        memory_available(p_memory_available),
        number_of_permutations(p_number_of_permutations) {}
  dat::index num_rows;
  dat::index num_cols;
  std::size_t max_memory_to_be_used;
  std::size_t memory_available;
  std::size_t number_of_permutations;

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const grid_attempt_configuration &) {
    return p_out;
  }
};

} // namespace tenacitas::src::crosswords::asy

#endif
