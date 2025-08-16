#ifndef TNCT_CROSSWORDS_ASY_GRID_CREATE_STATUS_H
#define TNCT_CROSSWORDS_ASY_GRID_CREATE_STATUS_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <string>
#include <string_view>

namespace tnct::crosswords::evt {

/// \brief Published when a new attempt to assemble a grid has started
struct grid_create_status {

  grid_create_status() = default;
  grid_create_status(const grid_create_status &) = default;

  grid_create_status(std::string_view p_status) : status(p_status) {}

  std::string status;

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const grid_create_status &) {
    return p_out;
  }
};

} // namespace tnct::crosswords::evt

#endif
