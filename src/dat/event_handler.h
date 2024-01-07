/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_DAT_EVENT_HANDLER_H
#define TENACITAS_LIB_DAT_EVENT_HANDLER_H

#include <functional>

#include <tenacitas.lib/src/cpt/event.h>

namespace tenacitas::lib::dat {

/// \brief Type of function that subscriber an event
template <tlc::event t_event>
using event_handler = std::function<void(t_event &&)>;

} // namespace tenacitas::lib::dat

namespace tld = tenacitas::lib::dat;

#endif
