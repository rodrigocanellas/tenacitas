#ifndef TENACITAS_COMMUNICATION_COMMUNICATION_H
#define TENACITAS_COMMUNICATION_COMMUNICATION_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <chrono>
#include <cstdint>
#include <functional>
#include <future>
#include <iterator>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

#define comm_log_debug(logger, p_params...)                                    \
  logger::debug(__FILE__, __LINE__, p_params)

#define comm_log_info(logger, p_params...)                                     \
  logger::info(__FILE__, __LINE__, p_params)

#define comm_log_warn(logger, p_params...)                                     \
  logger:                                                                      \
  warn(__FILE__, __LINE__, p_params)

#define comm_log_error(logger, p_params...)                                    \
  logger::error(__FILE__, __LINE__, p_params)

#define comm_log_fatal(logger, p_params...)                                    \
  logger::fatal(__FILE__, __LINE__, p_params)

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace communication {

typedef std::string message;

enum class status : uint16_t {
  ok = 0,
  error_unspecified = 1,
  error_connecting = 2,
  error_sending = 3,
  error_posting = 4,
  error_notifying = 5,
  error_creating_security = 6,
  error_receiving = 7
};

inline std::underlying_type<status>::type s2v(status p_status) {
  return static_cast<std::underlying_type<status>::type>(p_status);
}

template <typename t_logger, typename t_connector> struct client_t {
  typedef t_logger logger;
  typedef t_connector connector;
  typedef typename t_connector::connection connection;

  typedef std::pair<status, message> result;

  client_t() = delete;

  client_t(connector &&p_connector) : m_connector(std::move(p_connector)) {}

  template <typename t_endpoint>
  status
  connect(std::function<std::pair<connection, std::string>(const t_endpoint &)>
              p_connector,
          const t_endpoint &p_endpoint) noexcept {
    try {
      std::pair<status, connection> _res(p_connector(p_endpoint));
      if (_res.first != status::ok) {
        comm_log_error(logger, "error ", s2v(_res.first), " while connecting");
        return _res.first;
      }

      std::lock_guard<std::mutex> _lock(m_mutex);
      m_connection = std::move(_res.second);
      m_io_size = m_connection.get_io_size();

    } catch (const std::exception &_ex) {
      comm_log_error(logger, "error '", _ex.what(), "' while connecting");
      return status::error_connecting;
    }
  }

  /// \brief send
  ///
  /// \param p_message
  ///
  /// \details blocking
  ///
  /// \return
  status send(const message &p_message) noexcept {
    try {
      std::string::size_type _size = p_message.size();
      if (_size <= m_io_size) {
        status _status = m_connection.send(p_message.begin(), p_message.end());
        if (_status == status::ok) {
          return _status;
        }
        comm_log_error(logger, "error '", s2v(_status), "' while sending");
        return _status;
      }

      std::string::size_type _sent = 0;
      std::string::const_iterator _ite = p_message.begin();
      while (_sent != _size) {
        std::string::size_type _to_sent = _size - _sent;
        if (_to_sent > m_io_size) {
          _to_sent = m_io_size;
        }

        status _status = m_connection.send(_ite, std::next(_ite, _to_sent));
        if (_status != status::ok) {
          comm_log_error(logger, "error '", s2v(_status), "' while sending");
          return _status;
        }
        _sent += _to_sent;
        std::advance(_ite, _sent);
      }

    } catch (const std::exception &_ex) {
      comm_log_error(logger, "error '", _ex.what(), "' while sending");
      return status::error_sending;
    }
  }

  /// \brief receive
  ///
  /// \details blocking
  ///
  /// \return
  result receive() {
    try {
      return m_connection.receive();
    } catch (const std::exception &_ex) {
      comm_log_error(logger, "error '", _ex.what(), "' while receiving");
      return {status::error_receiving, message()};
    }
  }

  /// \brief post
  ///
  /// \param p_message
  ///
  /// \details non-blocking
  ///
  /// \return
  std::future<status> post(const message &p_message) {
    try {
      return std::async(std::launch::async, m_connection.send(p_message));
    } catch (const std::exception &_ex) {
      comm_log_error(logger,
                     "error '" + std::string(_ex.what()) + "' while posting");
      return std::future<status>();
    }
  }

  /// \brief collect
  ///
  /// \details non-blocking
  ///
  /// \return
  std::future<result> collect() {
    try {
      return std::async(std::launch::async, m_connection.receive());
    } catch (const std::exception &_ex) {
      comm_log_error(logger, "error '", _ex.what(), "' while receiving");
      return std::future<result>();
    }
  }

private:
  typedef typename std::unique_ptr<connection> connection_ptr;

private:
  connector m_connector;
  connection_ptr m_connection;
  uint16_t m_io_size;
  std::mutex m_mutex;
};

} // namespace communication
} // namespace tenacitas

#endif
