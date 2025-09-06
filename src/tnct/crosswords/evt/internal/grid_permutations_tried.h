#ifndef TNCT_CROSSWORDS_ASY_GRID_CREATE_NEW_ATTEMPT_H
#define TNCT_CROSSWORDS_ASY_GRID_CREATE_NEW_ATTEMPT_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <cstddef>
#include <iostream>
#include <optional>

namespace tnct::crosswords::evt::internal
{

/// \brief Published when a new attempt to assemble a grid has started
struct grid_permutations_tried
{

  grid_permutations_tried()                                = default;
  grid_permutations_tried(const grid_permutations_tried &) = default;
  grid_permutations_tried(grid_permutations_tried &&)      = default;

  grid_permutations_tried &operator=(const grid_permutations_tried &) = default;
  grid_permutations_tried &operator=(grid_permutations_tried &&)      = default;

  ~grid_permutations_tried() = default;

  grid_permutations_tried(size_t                     p_permutations,
                          std::optional<std::size_t> p_client = std::nullopt)
      : permutations(p_permutations), client(p_client)
  {
  }

  size_t                     permutations{0};
  std::optional<std::size_t> client = std::nullopt;

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const grid_permutations_tried &)
  {
    return p_out;
  }
};

} // namespace tnct::crosswords::evt::internal

#endif
