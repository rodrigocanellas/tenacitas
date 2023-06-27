#ifndef TENACITAS_LIB_ASYNC_INTERNAL_EVT_NEW_MESSAGE_H
#define TENACITAS_LIB_ASYNC_INTERNAL_EVT_NEW_MESSAGE_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <tenacitas.lib/communication/cpt/concepts.h>

namespace tenacitas::lib::communication::internal::evt {

template <cpt::message t_message> struct new_message {
  new_message() = default;
  new_message(const t_message &p_message) : message(p_message) {}
  t_message message;
};

} // namespace tenacitas::lib::communication::internal::evt

#endif
