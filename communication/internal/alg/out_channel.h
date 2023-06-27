#ifndef TENACITAS_LIB_MESSENGER_INTERNAL_ALG_OUT_CHANNEL_H
#define TENACITAS_LIB_MESSENGER_INTERNAL_ALG_OUT_CHANNEL_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <cstdint>
#include <map>
#ifdef TENACITAS_LOG
#include <typeindex>
#endif

#include <tenacitas.lib/async/alg/dispatcher.h>
#include <tenacitas.lib/async/alg/execute.h>
#include <tenacitas.lib/log/alg/logger.h>
#include <tenacitas.lib/communication/alg/message.h>
#include <tenacitas.lib/communication/cpt/concepts.h>
#include <tenacitas.lib/communication/evt/connection_closed.h>
#include <tenacitas.lib/communication/evt/new_connection.h>
#include <tenacitas.lib/communication/evt/send_timeout.h>
#include <tenacitas.lib/communication/internal/evt/send_msg.h>
#include <tenacitas.lib/traits/alg/tuple.h>

namespace tenacitas::lib::communication::internal::alg {

template <communication::typ::channel_id t_channel_id, cpt::message t_message,
          cpt::io<t_message> t_io, cpt::serializers<t_message> t_serializers>
struct out_channel {

  out_channel(async::alg::dispatcher::ptr p_dispatcher, t_io &p_io)
      : m_dispatcher(p_dispatcher), m_io(p_io) {

    subscribe_to_events();

    m_dispatcher->subscribe<communication::evt::new_connection<t_channel_id>>(
        [this](auto p_event) { on_new_connection(std::move(p_event)); });

    m_dispatcher->subscribe<communication::evt::connection_closed<t_channel_id>>(
        [&](auto &&p_event) { on_connection_closed(std::move(p_event)); });
  }

private:
  void on_connection_closed(
      communication::evt::connection_closed<t_channel_id> && /*p_event*/) {
    //    auto _connection_id{p_event.connection_id};
  }

  void
  on_new_connection(communication::evt::new_connection<t_channel_id> &&p_event) {

#ifdef TENACITAS_LOG
    TNCT_LOG_DEB("handling on_connected for connection ",
                 p_event.connection_id);
#endif

    auto _connection_id{p_event.connection_id};
    m_dispatcher->subscribe<evt::send_msg<t_message>>(
        [&](auto p_event) { on_send_msg(_connection_id, p_event.message); });
  }

  void on_send_msg(communication::typ::connection_id p_connection_id,
                   const t_message &p_message) {
#ifdef TENACITAS_LOG
    TNCT_LOG_DEB("sending message ", communication::alg::msg2str(p_message),
                 " through connection ", p_connection_id);
#endif
    auto _send = [&](std::function<bool()>) {
      m_io.send(p_connection_id, p_message);
    };
    if (!async::alg::execute(m_io.send_timeout, _send)) {
#ifdef TENACITAS_LOG
      TNCT_LOG_DEB("timeout sending message ",
                   communication::alg::msg2str(p_message), " through connection ",
                   p_connection_id);
#endif
      m_dispatcher->publish<communication::evt::send_timeout<t_message>>(
          t_channel_id, p_connection_id, p_message);
    }
  }

  template <size_t t_index> void subscribe_to_event() {

    using event = typename std::tuple_element_t<t_index, t_serializers>::event;

#ifdef TENACITAS_LOG
    TNCT_LOG_DEB("subscribing to event ", typeid(event).name());
#endif

    m_dispatcher->subscribe<event>([&](auto p_event) {
      t_message _message{std::get<t_index>(m_serializers)(p_event)};

#ifdef TENACITAS_LOG
      TNCT_LOG_DEB("handling event ", typeid(event).name(),
                   " about to publish send_msg for message ",
                   communication::alg::msg2str(_message));
#endif

      m_dispatcher->publish<evt::send_msg<t_message>>(_message);
    });
  }

  void subscribe_to_events() {
    std::apply(
        [&](auto &&...p_serializer) {
          ((subscribe_to_event<traits::alg::index_v<
                std::remove_reference_t<decltype(p_serializer)>,
                t_serializers>>()),
           ...);
        },
        m_serializers);
  }

private:
  async::alg::dispatcher::ptr m_dispatcher;
  t_io &m_io;
  t_serializers m_serializers;
};
} // namespace tenacitas::lib::communication::internal::alg

#endif
