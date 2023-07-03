#ifndef TENACITAS_LIB_IPC_POSIX_CONNECTION_H
#define TENACITAS_LIB_IPC_POSIX_CONNECTION_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <algorithm>
#include <array>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <ctime>
#include <functional>
#include <iterator>
#include <limits>
#include <netdb.h>
#include <optional>
#include <ostream>
#include <stdio.h>
#include <stdlib.h>
#include <string_view>
#include <sys/time.h>
#include <unistd.h>
#include <utility>
#include <vector>

#include <tenacitas/lib/async/alg/dispatcher.h>
#include <tenacitas/lib/async/typ/message.h>
#include <tenacitas/lib/log/alg/logger.h>
#include <tenacitas/lib/number/typ/id.h>
#include <tenacitas/lib/socket/alg/posix/timeout.h>
#include <tenacitas/lib/socket/typ/types.h>

using namespace std::chrono_literals;

namespace tenacitas::lib::socket::alg {

template <typ::flow> struct connection_t;

template <> struct connection_t<typ::flow::STREAM> {

  template <typename t_time_send, typename t_time_receive>
  connection_t(const typ::endpoint &p_endpoint, int p_socket,
               t_time_send p_timeout_send, t_time_receive p_timeout_receive)
      : m_endpoint(p_endpoint), m_socket(p_socket),
        m_timeout_send(std::chrono::duration_cast<std::chrono::milliseconds>(
            p_timeout_send)),
        m_timeout_receive(std::chrono::duration_cast<std::chrono::milliseconds>(
            p_timeout_receive)) {}

  connection_t() = default;

  connection_t(const connection_t &) = default;

  connection_t(connection_t &&p_conn)
      : m_endpoint(std::move(p_conn.m_endpoint)), m_socket(p_conn.m_socket),
        m_timeout_send(p_conn.m_timeout_send) {

    p_conn.set_socket_invalid();
  }

  connection_t &operator=(const connection_t &) = default;

  connection_t &operator=(connection_t &&p_conn) {
    if (this != &p_conn) {
      m_socket = p_conn.m_socket;
      m_timeout_send = p_conn.m_timeout_send;
#ifdef TENACITAS_LOG
      TNCT_LOG_DEB("p_conn.m_socket = ", p_conn.m_socket,
                   ", m_socket = ", m_socket);
#endif
      p_conn.set_socket_invalid();
#ifdef TENACITAS_LOG
      TNCT_LOG_DEB("p_conn.m_socket = ", p_conn.m_socket,
                   ", m_socket = ", m_socket);
#endif
    }
    return *this;
  }

  ~connection_t() {
    if (m_socket != -1) {
      ::close(m_socket);
    }
  }

  friend std::iostream &operator<<(std::iostream &p_out,
                                   const connection_t &p_conn) {
    p_out << p_conn.m_endpoint;
    return p_out;
  }

  inline bool operator==(const connection_t &p_conn) const {
    return m_endpoint == p_conn.m_endpoint;
  }

  inline bool operator!=(const connection_t &p_conn) const {
    return m_endpoint != p_conn.m_endpoint;
  }

  inline constexpr const typ::endpoint &get_endpoint() const {
    return m_endpoint;
  }

  typ::status send(const async::typ::message &p_message) {
    if (!is_socket_valid()) {
      TNCT_LOG_ERR("invalid socket");
      return typ::status::ERROR_NOT_CONNECTED;
    }

    const auto _len{static_cast<ssize_t>(p_message.size())};
    if (_len <= 0) {
      TNCT_LOG_ERR("invalid buffer");
      return typ::status::ERROR_INVALID_BUFFER;
    }

    typ::status _timeout_writing = timeout_write(m_timeout_send);
    if (_timeout_writing != typ::status::OK) {
      TNCT_LOG_ERR("timeout writing");
      return _timeout_writing;
    };

    if (::write(m_socket, &p_message[0], _len) != _len) {
      TNCT_LOG_ERR("error writing");
      return typ::status::ERROR_WRITING;
    }

    return typ::status::OK;
  }

  /** \brief reads \p size bytes
  typ::status::ERROR_NOT_CONNECTED
  typ::status::ERROR_TIMEOUT_READING
  typ::status::CONNECTION_CLOSED
  typ::status::ERROR_READING
  */
  std::pair<typ::status, ssize_t> read(std::byte *p_buffer, size_t p_len) {
    if (!is_socket_valid()) {
      TNCT_LOG_ERR("invalid socket");
      return {typ::status::ERROR_NOT_CONNECTED, 0};
    }

    typ::status _timeout_status = timeout_read(m_timeout_receive);
    if (_timeout_status != typ::status::OK) {
#ifdef TENACITAS_LOG
      TNCT_LOG_DEB("no timeout reading");
#endif
      return {_timeout_status, 0};
    }
#ifdef TENACITAS_LOG
    TNCT_LOG_DEB("no timeout reading");
#endif

    ssize_t _read = ::read(m_socket, p_buffer, p_len);
    if (_read > 0) {
      return {typ::status::OK, _read};
    } else if (_read == 0) {
      return {typ::status::CONNECTION_CLOSED, 0};
    } else {
      return {typ::status::ERROR_READING, 0};
    }
  }

private:
  template <typename t_time> typ::status timeout_read(t_time p_timeout) {

    if (p_timeout.count() == 0) {
      return typ::status::OK;
    }

    typ::status _timeout_status = timeout(m_socket, p_timeout);
    if (_timeout_status == typ::status::ERROR_TIMEOUT) {
#ifdef TENACITAS_LOG
      TNCT_LOG_WAR("timeout reading");
#endif
      return typ::status::ERROR_TIMEOUT_READING;
    }
    if (_timeout_status == typ::status::ERROR_IO) {
#ifdef TENACITAS_LOG
      TNCT_LOG_ERR("error reading");
#endif
      return typ::status::ERROR_READING;
    }
    return typ::status::OK;
  }

  template <typename t_time> typ::status timeout_write(t_time p_timeout) {

    if (p_timeout.count() == 0) {
      return typ::status::OK;
    }

    typ::status _timeout_status = timeout(m_socket, p_timeout);
    if (_timeout_status == typ::status::ERROR_TIMEOUT) {
#ifdef TENACITAS_LOG
      TNCT_LOG_WAR("timeout writing");
#endif
      return typ::status::ERROR_TIMEOUT_WRITING;
    }
    if (_timeout_status == typ::status::ERROR_IO) {
#ifdef TENACITAS_LOG
      TNCT_LOG_ERR("error writing");
#endif
      return typ::status::ERROR_WRITING;
    }
    return typ::status::OK;
  }

  inline bool is_socket_valid() const { return m_socket > 0; }

  inline void set_socket_invalid() { m_socket = -1; }

private:
  typ::endpoint m_endpoint;
  int m_socket{-1};
  std::chrono::milliseconds m_timeout_send;
  std::chrono::milliseconds m_timeout_receive;
};

} // namespace tenacitas::lib::socket::alg

#endif
