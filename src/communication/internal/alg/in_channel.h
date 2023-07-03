#ifndef TENACITAS_LIB_MESSENGER_INTERNAL_ALG_IN_CHANNEL_H
#define TENACITAS_LIB_MESSENGER_INTERNAL_ALG_IN_CHANNEL_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <atomic>
#include <cstdint>
#include <map>
#include <optional>
#ifdef TENACITAS_LOG
#include <typeindex>
#endif

#ifdef TENACITAS_LOG
#include <tenacitas.lib/src/communication/alg/message.h>
#endif
#include <tenacitas.lib/src/async/alg/dispatcher.h>
#include <tenacitas.lib/src/async/alg/execute.h>
#include <tenacitas.lib/src/log/alg/logger.h>
#include <tenacitas.lib/src/communication/cpt/concepts.h>
#include <tenacitas.lib/src/communication/evt/connection_closed.h>
#include <tenacitas.lib/src/communication/evt/new_connection.h>
#include <tenacitas.lib/src/communication/internal/evt/deserialize.h>
#include <tenacitas.lib/src/communication/internal/evt/new_message.h>
#include <tenacitas.lib/src/communication/internal/evt/send_msg.h>
#include <tenacitas.lib/src/traits/alg/tuple.h>

namespace tenacitas::lib::src::communication::internal::alg {

template <communication::typ::channel_id t_channel_id, cpt::message t_message,
          cpt::io<t_message> t_io,
          cpt::deserializers<t_message> t_deserializers>
struct in_channel {

  in_channel(async::alg::dispatcher::ptr p_dispatcher, t_io &p_io)
      : m_dispatcher(p_dispatcher), m_io(p_io) {

    m_dispatcher->subscribe<communication::evt::new_connection<t_channel_id>>(
        [&](auto p_event) { on_new_connection(std::move(p_event)); });

    m_dispatcher->subscribe<communication::evt::connection_closed<t_channel_id>>(
        [&](auto &&p_event) { on_connection_closed(std::move(p_event)); });

    m_local_dispatcher->subscribe<evt::new_message<t_message>>(
        [&](auto p_event) { on_new_message(std::move(p_event)); });

    subscribe_to_events();
  }

  ~in_channel() {
    m_stop = true;
    for (auto &_value : m_threads) {
      if (_value.second.joinable()) {
        _value.second.join();
      }
    }
  }

private:
  void on_connection_closed(communication::evt::connection_closed<t_channel_id> &&
                            /*p_event*/) {
    //    auto _connection_id{p_event.connection_id};
  }

  template <cpt::deserializer t_deserializer>
  void on_deserialize(evt::deserialize<t_message, t_deserializer> &&p_event) {

    t_deserializer _deserializer;
    auto _maybe{_deserializer(p_event.message)};
    if (_maybe) {
      auto _event{std::move(_maybe.value())};
      m_dispatcher->publish<decltype(_event)>(_event);
    }
  }

  void on_new_message(evt::new_message<t_message> &&p_event) {
    std::apply(
        [&](auto &&...p_deserializer) {
          ((m_local_dispatcher->publish<evt::deserialize<
                t_message, std::remove_reference_t<decltype(p_deserializer)>>>(
               p_event.message)),
           ...);
        },
        m_deserializers);
  }

  void
  on_new_connection(communication::evt::new_connection<t_channel_id> &&p_event) {
    m_threads[p_event.connection_id] =
        std::thread([&]() { receive_loop(p_event.connection_id); });
  }

  void receive_loop(communication::typ::connection_id p_connection_id) {

    while (true) {

      if (m_stop) {
        break;
      }

      auto _receive = [&](std::function<bool()>) -> t_message {
        return {m_io.receive(p_connection_id)};
      };

      std::optional<t_message> _maybe{
          async::alg::execute(m_io.receive_timeout, _receive)};

      if (_maybe) {
        m_local_dispatcher->publish<evt::new_message<t_message>>(
            _maybe.value());
      }
    }
  }

  template <cpt::deserializer t_deserializer> void subscribe_to_event() {

    m_local_dispatcher->subscribe<evt::deserialize<t_message, t_deserializer>>(
        [&](auto p_event) {
          on_deserialize<t_deserializer>(std::move(p_event));
        });
  }

  void subscribe_to_events() {
    std::apply(
        [&](auto &&...p_deserializer) {
          ((subscribe_to_event<
               std::remove_reference_t<decltype(p_deserializer)>>()),
           ...);
        },
        m_deserializers);
  }

private:
  async::alg::dispatcher::ptr m_dispatcher;
  t_io &m_io;
  std::map<communication::typ::connection_id, std::thread> m_threads;
  t_deserializers m_deserializers;
  std::atomic_bool m_stop{false};
  async::alg::dispatcher::ptr m_local_dispatcher{
      async::alg::dispatcher::create()};
};
} // namespace tenacitas::lib::src::communication::internal::alg

#endif
