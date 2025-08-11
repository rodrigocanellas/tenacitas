/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_ASYNC_INTERNAL_HANDLING_FUNCTION_ID_H
#define TNCT_ASYNC_INTERNAL_HANDLING_FUNCTION_ID_H

#include <typeinfo>

#include "tnct/async/traits/is_event.h"
#include "tnct/async/traits/is_handler.h"

namespace tnct::async::internal
{

using handler_id = size_t;

template <async::traits::is_event            t_event,
          async::traits::is_handler<t_event> t_handler>
inline handler_id get_handler_id()
{
  return static_cast<handler_id>(typeid(t_handler).hash_code());
};

} // namespace tnct::async::internal

#endif
