#ifndef TNCT_CROSSWORDS_ASY_GRID_ATTEMPT_CONFIGURATION_H
#define TNCT_CROSSWORDS_ASY_GRID_ATTEMPT_CONFIGURATION_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <iostream>
#include <optional>

#include "tnct/crosswords/dat/index.h"

namespace tnct::crosswords::evt::internal
{

struct grid_attempt_configuration
{
  grid_attempt_configuration()                                   = default;
  grid_attempt_configuration(const grid_attempt_configuration &) = default;
  grid_attempt_configuration(grid_attempt_configuration &&)      = default;

  grid_attempt_configuration &
  operator=(const grid_attempt_configuration &) = default;
  grid_attempt_configuration &
  operator=(grid_attempt_configuration &&) = default;

  ~grid_attempt_configuration() = default;

  grid_attempt_configuration(dat::index p_num_rows, dat::index p_num_cols,
                             std::size_t p_max_memory_to_be_used,
                             std::size_t p_memory_available,
                             std::size_t p_number_of_permutations,
                             std::optional<std::size_t> p_client = std::nullopt)
      : num_rows(p_num_rows), num_cols(p_num_cols),
        max_memory_to_be_used(p_max_memory_to_be_used),
        memory_available(p_memory_available),
        number_of_permutations(p_number_of_permutations), client(p_client)
  {
  }
  dat::index                 num_rows{dat::invalid_index};
  dat::index                 num_cols{dat::invalid_index};
  std::size_t                max_memory_to_be_used{0};
  std::size_t                memory_available{0};
  std::size_t                number_of_permutations{0};
  std::optional<std::size_t> client = std::nullopt;

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const grid_attempt_configuration &)
  {
    return p_out;
  }
};

} // namespace tnct::crosswords::evt::internal

#endif
