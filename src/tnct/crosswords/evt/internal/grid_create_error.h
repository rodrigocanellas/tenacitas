#ifndef TNCT_CROSSWORDS_EVT_WORD_TOO_LONG_H
#define TNCT_CROSSWORDS_EVT_WORD_TOO_LONG_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <iostream>
#include <string_view>

#include "tnct/crosswords/dat/error.h"

namespace tnct::crosswords::evt::internal
{

struct grid_create_error
{
  grid_create_error()                                     = default;
  grid_create_error(const grid_create_error &)            = default;
  grid_create_error(grid_create_error &&)                 = default;
  grid_create_error &operator=(const grid_create_error &) = default;
  grid_create_error &operator=(grid_create_error &&)      = default;
  ~grid_create_error()                                    = default;

  grid_create_error(const dat::error &p_error,
                    std::string_view  p_description = "")
      : error{p_error}, description(p_description)
  {
  }

  friend std::ostream &operator<<(std::ostream            &p_out,
                                  const grid_create_error &p_event)
  {
    p_out << "error = " << p_event.error << ", description = '"
          << p_event.description;
    return p_out;
  }

  dat::error  error;
  std::string description;
};
} // namespace tnct::crosswords::evt::internal

#endif
