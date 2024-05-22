/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_ASYNC_INTERNAL_SUBSCRIPTION_HANDLER_H
#define TENACITAS_LIB_ASYNC_INTERNAL_SUBSCRIPTION_HANDLER_H

#include <functional>

#include <tenacitas.lib/traits/event.h>

namespace tenacitas::lib::async::internal {

/// \brief Type of function that handles an event
template <traits::event t_event>
using handler = std::function<void(t_event &&)>;

} // namespace tenacitas::lib::async::internal

#endif
