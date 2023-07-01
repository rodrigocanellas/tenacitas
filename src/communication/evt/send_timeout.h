#ifndef TENACITAS_LIB_ASYNC_EVT_SEND_TIMEOUT_H
#define TENACITAS_LIB_ASYNC_EVT_SEND_TIMEOUT_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <iostream>

#include <tenacitas.lib/src/communication/cpt/concepts.h>
#include <tenacitas.lib/src/communication/typ/channel_id.h>
#include <tenacitas.lib/src/communication/typ/connection_id.h>

namespace tenacitas::lib::communication::evt {

template <cpt::message t_message> struct send_timeout {
  send_timeout() = default;
  send_timeout(const send_timeout &) = default;
  send_timeout(send_timeout &&) = default;
  send_timeout &operator=(const send_timeout &) = default;
  send_timeout &operator=(send_timeout &&) = default;
  ~send_timeout() = default;

  send_timeout(typ::channel_id p_channel_id, typ::connection_id p_connection_id,
               const t_message &p_message)
      : channel_id(p_channel_id), connection_id(p_connection_id),
        message(p_message) {}

  typ::channel_id channel_id;
  typ::connection_id connection_id;
  t_message message;
};

} // namespace tenacitas::lib::communication::evt

#endif
