#ifndef TENACITAS_LIB_IPC_POSIX_ACTIVE_CONNECTOR_H
#define TENACITAS_LIB_IPC_POSIX_ACTIVE_CONNECTOR_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <arpa/inet.h"
#include <array>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <ctime>
#include <functional>
#include <iterator>
#include <limits>
#include <netdb.h"
#include <netinet/in.h"
#include <optional>
#include <ostream>
#include <stdio.h"
#include <stdlib.h"
#include <string_view>
#include <sys/socket.h"
#include <sys/time.h"
#include <thread>
#include <unistd.h"
#include <utility>
#include <vector>

#include "tnct/network/alg/dispatcher.h"
#include <tenacitas/lib/log/alg/logger.h"
#include <tenacitas/lib/number/typ/id.h"
#include "tnct/network/alg/posix/connection.h"
#include "tnct/network/evt/events.h"
#include "tnct/network/typ/types.h"

using namespace std::chrono_literals;

namespace tenacitas::lib::socket::alg {

template <typ::link, typ::flow> struct active_connector_t;

template <> struct active_connector_t<typ::link::IPV4, typ::flow::STREAM> {
  using connection = alg::connection_t<typ::flow::STREAM>;

  active_connector_t(
      async::alg::dispatcher::ptr p_dispatcher,
      std::function<std::pair<typ::status, typ::buffer>(connection &)>
          &&p_reader,
      std::function<typ::status(connection &, const std::byte *,
                                const std::byte *)> &&p_writer)
      : m_dispatcher(p_dispatcher), m_reader(std::move(p_reader)),
        m_writer(std::move(p_writer)) {}

  std::shared_ptr<connection> connect_sync(const typ::endpoint &p_endpoint) {
    return connect(p_endpoint);
  }

  void connect_async(const typ::endpoint &p_endpoint) {
    std::thread([&]() -> void {
      std::shared_ptr<connection> _connection{connect(p_endpoint)};
      if (!_connection) {
        m_dispatcher->publish<evt::error>(p_endpoint,
                                          typ::status::ERROR_NOT_CONNECTED);
      }

      auto _read = [_connection,
                    this]() -> std::pair<typ::status, typ::buffer> {
        return m_reader(*_connection);
      };

      auto _write = [_connection, this](const std::byte *p_begin,
                                        const std::byte *p_end) -> typ::status {
        return m_writer(*_connection, p_begin, p_end);
      };

      m_dispatcher->publish<evt::new_connection>(p_endpoint, _read, _write);
    }).detach();
  }

private:
  std::shared_ptr<connection> connect(const typ::endpoint &p_endpoint) {
    int _socket{-1};
    struct sockaddr_in _serv_addr;
    struct hostent *_server;

    // Create a socket point
    _socket = socket(AF_INET, SOCK_STREAM, 0);

    if (_socket < 0) {
      TNCT_LOG_ERR("ERROR opening socket");
      return {};
    }

    _server = gethostbyname(p_endpoint.ip.c_str());

    if (_server == nullptr) {
      TNCT_LOG_ERR("ERROR, no such host\n");
      return {};
    }

    ::memset((char *)&_serv_addr, 0, sizeof(_serv_addr));
    _serv_addr.sin_family = AF_INET;
    ::memcpy((char *)_server->h_addr, (char *)&_serv_addr.sin_addr.s_addr,
             _server->h_length);
    _serv_addr.sin_port = htons(p_endpoint.port);

    if (::connect(_socket, (struct sockaddr *)&_serv_addr, sizeof(_serv_addr)) <
        0) {
      TNCT_LOG_ERR("ERROR connecting");
      return {};
    }

    return std::make_shared<connection>(m_dispatcher, p_endpoint, _socket);
  }

private:
  async::alg::dispatcher ::ptr m_dispatcher;
  std::function<std::pair<typ::status, typ::buffer>(connection &)> m_reader;
  std::function<typ::status(connection &, const std::byte *, const std::byte *)>
      m_writer;
};

} // namespace tenacitas::lib::socket::alg

#endif
