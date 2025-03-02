/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_ASYNC_HANDLER_H
#define TNCT_ASYNC_HANDLER_H

#include <functional>

#include "tnct/traits/async/event.h"

namespace tnct::async {

/// \brief Type of function that handles an event
template <traits::async::event t_event>
using handler = std::function<void(t_event &&)>;

// template<traits::async::event t_event>
// using handler = void (*)(t_event &&);

} // namespace tnct::async

#endif
