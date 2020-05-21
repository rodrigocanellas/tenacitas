#ifndef TENACITAS_COMMUNICATION_COMMUNICATION_H
#define TENACITAS_COMMUNICATION_COMMUNICATION_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <chrono>
#include <cstdint>
#include <functional>
#include <future>
#include <mutex>
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

typedef std::vector<char> buffer;

enum class status : uint16_t {
  ok = 0,
  error_unspecified = 1,
  error_connecting = 2,
  error_sending = 3,
  error_receiving = 4
};

inline std::underlying_type<status>::type s2v(status p_status) {
  return static_cast<std::underlying_type<status>::type>(p_status);
}

template <typename t_logger, typename t_connector> struct caller_device_t {
  typedef t_logger logger;
  typedef t_connector connector;
  typedef typename connector::endpoint endpoint;
  typedef typename connector::connection connection;

  status connect(const endpoint &p_endpoint) {
    try {
      connector _connector;
      std::pair<status, connection> _status =
          _connection(_connector(p_endpoint));
      if (_status.first != status::ok) {
        comm_log_error(logger, "error ", s2v(_status.first),
                       " while connecting");
        return _status.first;
      }
      std::lock_guard<std::mutex> _lock(m_mutex);
      m_connection = std::move(_status.second);

    } catch (...) {
      comm_log_error(logger, "error unspecified while connecting");
      return status::error_unspecified;
    }
  }

  ///
  /// \brief send
  ///
  /// \param p_buffer
  ///
  /// \details blocking
  ///
  /// \return
  ///
  std::pair<status, buffer> send(const buffer &p_buffer) {
    try {
      std::lock_guard<std::mutex> _lock(m_mutex);
      std::pair<status, buffer> _res = m_connection.send(p_buffer);
      if (_res.first != status::ok) {
        comm_log_error(logger, "error ", s2v(_res.first),
                       " while sending and receiving");
        return _res;
      }
      return _res;
    } catch (...) {
      comm_log_error(logger, "error unspecified while connecting");
      return {status::error_unspecified, buffer()};
    }
  }

  ///
  /// \brief send
  ///
  /// \param p_buffer
  ///
  /// \details non-blocking
  ///
  /// \return
  ///
  std::pair<status, buffer> post(const buffer &p_buffer) {
    try {
      std::lock_guard<std::mutex> _lock(m_mutex);
      std::future<std::pair<status, buffer>> _future =
          m_connection.send(p_buffer);
      std::pair<status, buffer> _res = _future.get();
      if (_res.first != status::ok) {
        comm_log_error(logger, "error ", s2v(_res.first),
                       " while sending and receiving");
      }
      return _res;
    } catch (...) {
      comm_log_error(logger, "error unspecified while connecting");
      return {status::error_unspecified, buffer()};
    }
  }

private:
  connection m_connection;
  std::mutex m_mutex;
};

} // namespace communication
} // namespace tenacitas

#endif
