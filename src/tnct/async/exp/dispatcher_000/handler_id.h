/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_ASYNC_EXP_DISPATCHER_000_HANDLER_ID_H
#define TNCT_ASYNC_EXP_DISPATCHER_000_HANDLER_ID_H

#include <thread>

namespace tnct::async::exp
{

using handler_id = std::thread::id;

} // namespace tnct::async::exp

#endif
