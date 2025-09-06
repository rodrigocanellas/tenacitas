#ifndef TNCT_CROSSWORDS_ASY_GRID_CREATE_UNSOLVED_H
#define TNCT_CROSSWORDS_ASY_GRID_CREATE_UNSOLVED_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <iostream>
#include <optional>

#include "tnct/crosswords/dat/index.h"

namespace tnct::crosswords::evt::internal
{

/// \brief Published when a grid was not assembled
struct grid_create_unsolved
{
  grid_create_unsolved()                             = default;
  grid_create_unsolved(const grid_create_unsolved &) = default;
  grid_create_unsolved(grid_create_unsolved &&)      = default;

  grid_create_unsolved &operator=(const grid_create_unsolved &) = default;
  grid_create_unsolved &operator=(grid_create_unsolved &&)      = default;

  ~grid_create_unsolved() = default;

  explicit grid_create_unsolved(
      dat::index p_num_rows, dat::index p_num_cols,
      std::optional<std::size_t> p_client = std::nullopt)
      : num_rows(p_num_rows), num_cols(p_num_cols), client(p_client)
  {
  }

  dat::index                 num_rows{dat::invalid_index};
  dat::index                 num_cols{dat::invalid_index};
  std::optional<std::size_t> client{std::nullopt};

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const grid_create_unsolved &)
  {
    return p_out;
  }
};

} // namespace tnct::crosswords::evt::internal

#endif
