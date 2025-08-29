#ifndef TNCT_CROSSWORDS_ASY_GRID_CREATE_STOP_H
#define TNCT_CROSSWORDS_ASY_GRID_CREATE_STOP_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <iostream>
#include <optional>

namespace tnct::crosswords::evt
{

/// \brief Published when all attemps of assembling grids should stop
struct grid_create_stop
{

  grid_create_stop()                         = default;
  grid_create_stop(const grid_create_stop &) = default;
  grid_create_stop(grid_create_stop &&)      = default;

  grid_create_stop &operator=(const grid_create_stop &) = default;
  grid_create_stop &operator=(grid_create_stop &&)      = default;

  ~grid_create_stop() = default;

  grid_create_stop(std::optional<std::size_t> p_client) : client(p_client)
  {
  }

  friend std::ostream &operator<<(std::ostream &p_out, const grid_create_stop &)
  {
    return p_out;
  }

  std::optional<std::size_t> client = std::nullopt;
};

} // namespace tnct::crosswords::evt

#endif
