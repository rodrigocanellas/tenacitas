/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_ASYNC_INTERNAL_HANDLER_ID_H
#define TENACITAS_LIB_ASYNC_INTERNAL_HANDLER_ID_H

#include <typeindex>

#include <tenacitas.lib/traits/handler.h>

namespace tenacitas::lib::async::internal {

template <traits::handler t_handler> inline size_t handler_id() {
  return std::type_index(typeid(t_handler)).hash_code();
};

} // namespace tenacitas::lib::async::internal

#endif
