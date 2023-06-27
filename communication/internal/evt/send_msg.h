#ifndef TENACITAS_LIB_ASYNC_INTERNAL_EVT_SEND_MSG_H
#define TENACITAS_LIB_ASYNC_INTERNAL_EVT_SEND_MSG_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <iostream>

#include <tenacitas.lib/communication/cpt/concepts.h>
#include <tenacitas.lib/communication/typ/channel_id.h>
#include <tenacitas.lib/communication/typ/connection_id.h>

namespace tenacitas::lib::communication::internal::evt {

template <cpt::message t_message> struct send_msg {

  send_msg(const t_message &p_message) : message(p_message) {}

  send_msg() = default;
  send_msg(const send_msg &) = default;
  send_msg(send_msg &&) = default;
  send_msg &operator=(send_msg &&) = default;
  send_msg &operator=(const send_msg &) = default;

  t_message message;
};

} // namespace tenacitas::lib::communication::internal::evt

#endif
