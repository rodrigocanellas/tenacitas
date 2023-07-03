#ifndef TENACITAS_LIB_ASYNC_EVT_NEW_CONNECTION_H
#define TENACITAS_LIB_ASYNC_EVT_NEW_CONNECTION_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <iostream>

#include <tenacitas.lib/src/communication/typ/channel_id.h>
#include <tenacitas.lib/src/communication/typ/connection_id.h>

namespace tenacitas::lib::communication::evt {

template <typ::channel_id t_channel_id> struct new_connection {
  new_connection() = default;
  new_connection(const new_connection &) = default;
  new_connection(new_connection &&) = default;
  new_connection &operator=(const new_connection &) = default;
  new_connection &operator=(new_connection &&) = default;
  ~new_connection() = default;

  new_connection(typ::connection_id p_connection_id)
      : channel_id(t_channel_id), connection_id(p_connection_id) {}

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const new_connection &p_new_connection) {
    p_out << "new_connection {channel = " << p_new_connection.channel_id
          << ", connection = " << p_new_connection.connection_id << '}';
    return p_out;
  }

  typ::channel_id channel_id;
  typ::connection_id connection_id;
};

} // namespace tenacitas::lib::communication::evt

#endif
