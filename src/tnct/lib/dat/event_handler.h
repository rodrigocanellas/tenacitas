/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_LIB_DAT_EVENT_HANDLER_H
#define TNCT_LIB_DAT_EVENT_HANDLER_H

#include <functional>

#include <tnct/lib/cpt/event.h>

namespace tnct::lib::dat {

/// \brief Type of function that subscriber an event
template <tlc::event t_event>
using event_handler = std::function<void(t_event &&)>;

} // namespace tnct::lib::dat

namespace tld = tnct::lib::dat;

#endif
