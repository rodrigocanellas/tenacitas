#ifndef TENACITAS_LIB_ASYNC_EVT_CONNECTION_CLOSED_H
#define TENACITAS_LIB_ASYNC_EVT_CONNECTION_CLOSED_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <iostream>

#include <tenacitas.lib/src/communication/typ/channel_id.h>
#include <tenacitas.lib/src/communication/typ/connection_id.h>

namespace tenacitas::lib::communication::evt {

template <typ::channel_id t_channel_id> struct connection_closed {
  connection_closed() = default;
  connection_closed(const connection_closed &) = default;
  connection_closed(connection_closed &&) = default;
  connection_closed &operator=(const connection_closed &) = default;
  connection_closed &operator=(connection_closed &&) = default;
  ~connection_closed() = default;

  connection_closed(typ::connection_id p_connection_id)
      : channel_id(t_channel_id), connection_id(p_connection_id) {}

  typ::channel_id channel_id;
  typ::connection_id connection_id;
};

} // namespace tenacitas::lib::communication::evt

#endif
