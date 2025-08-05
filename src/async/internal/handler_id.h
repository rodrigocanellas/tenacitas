/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_SRC_ASYNC_INTERNAL_HANDLING_FUNCTION_ID_H
#define TENACITAS_SRC_ASYNC_INTERNAL_HANDLING_FUNCTION_ID_H

#include <typeinfo>

#include "tenacitas/src/async/traits/is_event.h"
#include "tenacitas/src/async/traits/is_handler.h"

namespace tenacitas::src::async::internal
{

using handler_id = size_t;

template <src::async::traits::is_event                      t_event,
          src::async::traits::is_handler<t_event> t_handler>
inline handler_id get_handler_id()
{
  return static_cast<handler_id>(
      typeid(t_handler).hash_code());
};

} // namespace tenacitas::src::async::internal

#endif
