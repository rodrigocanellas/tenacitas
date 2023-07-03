#ifndef TENACITAS_LIB_IPC_POSIX_PASSIVE_CONNECTOR_H
#define TENACITAS_LIB_IPC_POSIX_PASSIVE_CONNECTOR_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <arpa/inet.h>
#include <array>
#include <atomic>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <ctime>
#include <fcntl.h>
#include <functional>
#include <future>
#include <iterator>
#include <limits>
#include <memory>
#include <netdb.h>
#include <netinet/in.h>
#include <optional>
#include <ostream>
#include <stdio.h>
#include <stdlib.h>
#include <string_view>
#include <sys/socket.h>
#include <sys/time.h>
#include <thread>
#include <unistd.h>
#include <utility>
#include <vector>

#include <tenacitas/lib/async/alg/dispatcher.h>
#include <tenacitas/lib/log/alg/logger.h>
#include <tenacitas/lib/number/typ/id.h>
#include <tenacitas/lib/socket/alg/posix/connection.h>
#include <tenacitas/lib/socket/alg/posix/timeout.h>
#include <tenacitas/lib/socket/evt/events.h>
#include <tenacitas/lib/socket/typ/types.h>

using namespace std::chrono_literals;

namespace tenacitas::lib::src::socket::alg {

template <typ::link, typ::flow> struct passive_connector_t;

template <> struct passive_connector_t<typ::link::IPV4, typ::flow::STREAM> {
  using connection = alg::connection_t<typ::flow::STREAM>;
  using connection_ptr = typename std::shared_ptr<connection>;
  using connection_handler = std::function<void(connection_ptr)>;

  passive_connector_t(
      async::alg::dispatcher::ptr p_dispatcher, typ::endpoint &&p_endpoint,
      std::function<std::pair<typ::status, typ::buffer>(connection &)>
          &&p_reader,
      std::function<typ::status(connection &, const std::byte *,
                                const std::byte *)> &&p_writer)
      : m_dispatcher(p_dispatcher),
        m_endpoint(std::forward<typ::endpoint>(p_endpoint)), m_reader(p_reader),
        m_writer(p_writer) {
    if (m_dispatcher == nullptr) {
      TNCT_LOG_ERR("dispatcher is null!!!");
      throw std::runtime_error("dispatcher is null!!!");
    }

    m_dispatcher->subscribe<evt::stop_server>([this](auto) -> void { stop(); });
    m_dispatcher->subscribe<evt::restart_server>([this](auto) -> void {
      stop();
      start();
    });
  }

  ~passive_connector_t() {
    TNCT_LOG_DEB("destructor");
    if (m_server_socket != -1) {
      stop();
      ::close(m_server_socket);
    }
  }

  template <typename t_time> inline void set_accept_timeout(t_time p_timeout) {
    if (p_timeout.count()) {
      m_accept_timeout =
          std::chrono::duration_cast<decltype(m_accept_timeout)>(p_timeout);
    }
  }

  inline void stop() {
    TNCT_LOG_DEB("stopping");
    if (m_stop) {
      TNCT_LOG_DEB("not stoping because it is already stopped");
      return;
    }
    m_stop = true;

    TNCT_LOG_DEB("joining start thread");
    if (m_start_thread.joinable()) {
      m_start_thread.join();
    }
    TNCT_LOG_DEB("start thread joined");
  }

  /// \brief starts to wait for connections on \p p_port
  ///
  /// \details publishes tenacitas::lib::socket::evt::new_connection_t
  void start() {
    stop();
    m_start_thread = std::thread([this]() -> void { wait(); });
  }

  /// \brief starts to wait for connections on \p p_port, and call \p
  /// p_connection_handler to handle the incoming connection with a timout of \p
  /// p_timeout
  template <typename t_time = std::chrono::seconds>
  void start(connection_handler p_connection_handler,
             t_time p_handling_timeout = 30s) {
    stop();

    m_start_thread =
        std::thread([this, p_connection_handler, p_handling_timeout]() -> void {
          wait(p_connection_handler, p_handling_timeout);
        });
  }

private:
  template <typename t_time>
  void wait(connection_handler p_connection_handler,
            t_time p_handling_timeout) {
    m_stop = false;

    struct sockaddr_in _addr;

    if (!listen(_addr)) {
      return;
    }

    while (true) {
      if (m_stop) {
        break;
      }

      int _client_socket = accept(_addr);

      if (m_stop) {
        break;
      }

      if (_client_socket != -1) {
        typ::endpoint _endpoint{m_endpoint};
        std::future<void> _result =
            std::async(std::launch::async, [this, p_connection_handler,
                                            _client_socket, &_endpoint]() {
              p_connection_handler(std::make_shared<connection>(
                  m_dispatcher, std::move(_endpoint), _client_socket));
            });
        if (_result.wait_for(p_handling_timeout)) {
          TNCT_LOG_WAR("timeout while handling connection");
        }
      }
    }
  }

  void wait() {
    m_stop = false;

    struct sockaddr_in _addr;

    if (!listen(_addr)) {
      return;
    }

    while (true) {
      if (m_stop) {
        break;
      }

      int _client_socket = accept(_addr);

      //      if (m_stop) {
      //        break;
      //      }

      if (_client_socket != -1) {
        typ::endpoint _endpoint{get_ip_str((const struct sockaddr *)&_addr),
                                _addr.sin_port};
#ifdef TENACITAS_LOG
        TNCT_LOG_DEB("connection from ", _endpoint);
#endif
        auto _connection{std::make_shared<connection>(m_dispatcher, _endpoint,
                                                      _client_socket)};

        auto _read = [_connection,
                      this]() -> std::pair<typ::status, typ::buffer> {
          return m_reader(*_connection);
        };

        auto _write = [_connection,
                       this](const std::byte *p_begin,
                             const std::byte *p_end) -> typ::status {
          return m_writer(*_connection, p_begin, p_end);
        };

        m_dispatcher->publish<evt::new_connection>(_endpoint, _read, _write);
      }
    }
  }

  bool listen(struct sockaddr_in &p_addr) {
    memset((void *)&p_addr, 0, sizeof(sockaddr_in));
    p_addr.sin_family = AF_INET;
    p_addr.sin_port = htons(m_endpoint.port);
    p_addr.sin_addr.s_addr = INADDR_ANY;

    m_server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_server_socket == -1) {
      TNCT_LOG_ERR("Socket creation error");
      return false;
    }

    if (bind(m_server_socket, (struct sockaddr *)&p_addr, sizeof(p_addr)) ==
        -1) {
      close(m_server_socket);
      m_server_socket = -1;
      return false;
    }

    if (::listen(m_server_socket, 5) == -1) {
      TNCT_LOG_ERR("Listen error");
      close(m_server_socket);
      m_server_socket = -1;
      return false;
    }
    return true;
  }

  int accept(struct sockaddr_in &p_addr) {
    typ::status _timeout_result = timeout(m_server_socket, m_accept_timeout);

    if (_timeout_result == typ::status::ERROR_TIMEOUT) {
      TNCT_LOG_DEB("timeout accepting connection");
      return -1;
    }
    if (_timeout_result == typ::status::ERROR_IO) {
      TNCT_LOG_ERR("error accepting connection");
      return -1;
    }
    //    struct sockaddr_in _client_addr;
    socklen_t _len = sizeof(p_addr);
    int _socket = ::accept(m_server_socket, (struct sockaddr *)&p_addr, &_len);

    return _socket;
  }

  std::string get_ip_str(const struct sockaddr *p_addr) {
    const size_t maxlen{500};
    char _str[maxlen];

    switch (p_addr->sa_family) {
    case AF_INET:
      inet_ntop(AF_INET, &(((struct sockaddr_in *)p_addr)->sin_addr), _str,
                maxlen);
      break;
      break;

    case AF_INET6:
      inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)p_addr)->sin6_addr), _str,
                maxlen);
      break;

    default:
      return {};
    }

    return {_str};
  }

private:
  async::alg::dispatcher::ptr m_dispatcher;
  typ::endpoint m_endpoint;
  std::function<std::pair<typ::status, typ::buffer>(connection &)> m_reader;
  std::function<typ::status(connection &, const std::byte *, const std::byte *)>
      m_writer;

  std::chrono::milliseconds m_accept_timeout = 10s;

  std::atomic_bool m_stop{true};
  int m_server_socket{-1};

  std::thread m_start_thread;
  std::mutex m_mutex_start;
  std::condition_variable m_cond_start;
};

} // namespace tenacitas::lib::src::socket::alg

#endif
