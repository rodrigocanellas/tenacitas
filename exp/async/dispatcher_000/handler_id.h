/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef EXP_TNCT_ASYNC_EXP_DISPATCHER_HANDLER_ID_H
#define EXP_TNCT_ASYNC_EXP_DISPATCHER_HANDLER_ID_H

#include <thread>

namespace tenacitas::exp::async {

using handler_id = std::thread::id;

} // namespace tenacitas::exp::async

#endif
