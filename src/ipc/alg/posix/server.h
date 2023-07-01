#ifndef TENACITAS_LIB_IPC_BUS_POSIX_SERVER_H
#define TENACITAS_LIB_IPC_BUS_POSIX_SERVER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <cstdint>
#include <map>

#include <tenacitas/lib/async/alg/dispatcher.h>
#include <tenacitas/lib/socket/bus/posix/passive_connector.h>
#include <tenacitas/lib/socket/evt/events.h>
#include <tenacitas/lib/socket/typ/types.h>
#include <tenacitas/lib/log/alg/logger.h>

using namespace std::chrono_literals;

namespace tenacitas::lib::socket::alg {

template <typ::link, typ::flow> struct server_t;

template <> struct server_t<typ::link::IPV4, typ::flow::STREAM> {
  using connection = bus::connection_t<typ::flow::STREAM>;

  using new_connection = evt::new_connection_t<connection>;

  server_t(async::alg::dispatcher::ptr p_dispatcher)
      : m_dispatcher(p_dispatcher), m_connector(p_dispatcher) {
    //      m_dispatcher->subscribe<new_connection>(
    //        [this](auto p_event) -> void { new_connection_handler(p_event); },
    //        255);
    //    m_dispatcher->subscribe<evt::error_not_connected>(
    //        [this](auto p_event) -> void { not_connected_handler(p_event); },
    //        255);
    //    m_dispatcher->subscribe<evt::error_timeout_reading>(
    //        [this](auto p_event) -> void { timeout_reading_handler(p_event);
    //        }, 255);
    //    m_dispatcher->subscribe<evt::error_timeout_writing>(
    //        [this](auto p_event) -> void { timeout_writing_handler(p_event);
    //        }, 255);
    //    m_dispatcher->subscribe<evt::error_max_reading>(
    //        [this](auto p_event) -> void { error_max_reading_handler(p_event);
    //        }, 255);
  }

  //  void wait(uint16_t p_port, std::chrono::seconds p_read_timeout = 0s,
  //            std::chrono::seconds p_write_timeout = 0s,
  //            uint8_t p_max_read_tries = std::numeric_limits<uint8_t>::max())
  //            {
  //        m_connector.wait(p_port, p_read_timeout, p_write_timeout,
  //        p_max_read_tries);
  //  }

private:
  void new_connection_handler(std::shared_ptr<new_connection> p_event);
  void not_connected_handler(std::shared_ptr<evt::error_not_connected> p_event);
  void
  timeout_reading_handler(std::shared_ptr<evt::error_timeout_reading> p_event);
  void
  timeout_writing_handler(std::shared_ptr<evt::error_timeout_writing> p_event);
  void error_reading_handler(std::shared_ptr<evt::error_reading> p_event);
  void
  error_max_reading_handler(std::shared_ptr<evt::error_max_reading> p_event);

private:
  using connector =
      bus::passive_connector_t<typ::link::IPV4, typ::flow::STREAM>;

private:
  async::alg::dispatcher::ptr m_dispatcher;
  connector m_connector;
  async::handler_t<new_connection> m_new_connection_handler;
  async::handler_t<evt::error_not_connected> m_not_connected_handler;
  async::handler_t<evt::error_timeout_reading> m_timeout_reading_handler;
  async::handler_t<evt::error_timeout_writing> m_timeout_writing_handler;
  async::handler_t<evt::error_max_reading> m_error_max_reading_handler;
};

} // namespace tenacitas::lib::socket::alg

#endif
