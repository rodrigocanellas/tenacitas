/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_ASYNC_EXP_DISPATCHER_HANDLER_ID_H
#define TENACITAS_LIB_ASYNC_EXP_DISPATCHER_HANDLER_ID_H

// #include "tenacitas.lib/generic/id.h"
//#include <cstdint>
#include <thread>

namespace tenacitas::lib::async::exp {

using handler_id = std::thread::id;
// generic::id<uint16_t>;

} // namespace tenacitas::lib::async::exp

#endif
