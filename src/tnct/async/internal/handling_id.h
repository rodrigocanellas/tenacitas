/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_ASYNC_INTERNAL_HANDLING_ID_H
#define TNCT_ASYNC_INTERNAL_HANDLING_ID_H

#include <cstddef>

#include "tnct/async/handling_name.h"

namespace tnct::async::internal
{

using handling_id = std::size_t;

inline handling_id get_handling_id(const handling_name &p_handling_name)
{
  return std::hash<handling_name>{}(p_handling_name);
}

} // namespace tnct::async::internal

#endif
