#ifndef TENACITAS_LIB_ASYNC_TYP_SUBSCRIBER_H
#define TENACITAS_LIB_ASYNC_TYP_SUBSCRIBER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <functional>

#include <tenacitas.lib/async/cpt/concepts.h>

namespace tenacitas::lib::async::typ {

/// \brief Type of function that subscriber an event
template <cpt::event t_event>
using subscriber_t = std::function<void(t_event &&)>;

} // namespace tenacitas::lib::async::typ

#endif
