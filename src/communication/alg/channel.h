#ifndef TENACITAS_LIB_MESSENGER_ALG_CHANNEL_H
#define TENACITAS_LIB_MESSENGER_ALG_CHANNEL_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <tenacitas.lib/src/async/alg/dispatcher.h>
#include <tenacitas.lib/src/communication/cpt/concepts.h>
#include <tenacitas.lib/src/communication/evt/new_connection.h>
#include <tenacitas.lib/src/communication/internal/alg/in_channel.h>
#include <tenacitas.lib/src/communication/internal/alg/out_channel.h>
#include <tenacitas.lib/src/communication/typ/channel_id.h>

namespace tenacitas::lib::src::communication::alg {

/**
 * @brief channel
 * Every \p evt::new_connection for the \p t_channel_id is supposed to want all
 * the messages that can be serialized by one of the \p cpt::serializer in \p
 * t_serializers, and may it is supposed to send any of the messages that can be
 * deserialized by one of the \p cpt::deserializer in \p t_deserializers
 *
 */
template <typ::channel_id t_channel_id, cpt::message t_message,
          cpt::io<t_message> t_io, cpt::serializers<t_message> t_serializers,
          cpt::deserializers<t_message> t_deserializers>
struct channel {

  using message = t_message;

  using io = t_io;

  using serializers = t_serializers;

  using deserializers = t_deserializers;

  channel(async::alg::dispatcher::ptr p_dispatcher)
      : m_out_channel(p_dispatcher, m_io), m_in_channel(p_dispatcher, m_io) {}

  static constexpr typ::channel_id channel_id{t_channel_id};

private:
  using out_channel =
      internal::alg::out_channel<t_channel_id, t_message, t_io, t_serializers>;

  using in_channel =
      internal::alg::in_channel<t_channel_id, t_message, t_io, t_deserializers>;

private:
  out_channel m_out_channel;
  in_channel m_in_channel;
  io m_io;
};

} // namespace tenacitas::lib::src::communication::alg

#endif
