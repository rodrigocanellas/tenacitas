#ifndef TNCT_CROSSWORDS_ASY_GRID_CREATE_SOLVED_H
#define TNCT_CROSSWORDS_ASY_GRID_CREATE_SOLVED_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <iostream>
#include <memory>

#include "tnct/crosswords/dat/grid.h"

namespace tnct::crosswords::evt::internal
{

/// \brief Published when a grid was assembled
struct grid_create_solved
{
  grid_create_solved()                           = default;
  grid_create_solved(const grid_create_solved &) = default;
  grid_create_solved(grid_create_solved &&)      = default;

  grid_create_solved &operator=(const grid_create_solved &) = default;
  grid_create_solved &operator=(grid_create_solved &&)      = default;

  ~grid_create_solved() = default;

  explicit grid_create_solved(
      std::shared_ptr<dat::grid> p_grid, std::chrono::seconds p_time,
      std::uint64_t              p_max_permutations,
      std::optional<std::size_t> p_client = std::nullopt)
      : grid(p_grid), time(p_time), max_permutations(p_max_permutations),
        client(p_client)
  {
  }

  std::shared_ptr<dat::grid> grid;
  std::chrono::seconds       time{std::chrono::seconds::zero()};
  std::uint64_t              max_permutations;
  std::optional<std::size_t> client{std::nullopt};

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const grid_create_solved &)
  {
    return p_out;
  }
};

} // namespace tnct::crosswords::evt::internal

#endif
