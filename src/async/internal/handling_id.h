/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_SRC_ASYNC_INTERNAL_HANDLING_ID_H
#define TENACITAS_SRC_ASYNC_INTERNAL_HANDLING_ID_H

#include <cstddef>

#include "tenacitas/src/async/handling_name.h"

namespace tenacitas::src::async::internal
{

using handling_id = std::size_t;

inline handling_id get_handling_id(const handling_name &p_handling_name)
{
  return std::hash<handling_name>{}(p_handling_name);
}

} // namespace tenacitas::src::async::internal

#endif
