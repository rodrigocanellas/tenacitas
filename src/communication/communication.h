#ifndef TENACITAS_COMMUNICATION_COMMUNICATION_H
#define TENACITAS_COMMUNICATION_COMMUNICATION_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <chrono>
#include <cstdint>
#include <functional>
#include <future>
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

typedef std::vector<char> buffer;

enum class status : uint16_t {
  ok = 0,
  error_unspecified = 1,
  error_connecting = 2,
  error_sending = 3,
  error_posting = 4,
  error_notifying = 5,
  error_creating_security = 6
};

inline std::underlying_type<status>::type s2v(status p_status) {
  return static_cast<std::underlying_type<status>::type>(p_status);
}

template <typename t_logger, typename t_endpoint, typename t_connector,
          typename t_security>
struct client_t {
  typedef t_logger logger;
  typedef t_endpoint endpoint;
  typedef t_connector connector;
  typedef typename connector::connection connection;
  typedef t_security security;

  typedef std::pair<status, buffer> result;

  status connect(std::function<std::pair<status, connection>(const endpoint &)>
                     p_connector,
                 const endpoint &p_endpoint) noexcept {
    try {
      std::pair<status, connection> _res(p_connector(p_endpoint));
      if (_res.first != status::ok) {
        comm_log_error(logger, "error ", s2v(_res.first), " while connecting");
        return _res.first;
      }

      std::lock_guard<std::mutex> _lock(m_mutex);
      m_security = std::make_unique<security>(_res.second);
      if (!m_security) {
        comm_log_error(logger, "error crating security");
        return status::error_creating_security;
      }

    } catch (const std::exception &_ex) {
      comm_log_error(logger, "error '" + std::string(_ex.what()) +
                                 "' while connecting");
      return status::error_connecting;
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
  result send(const buffer &p_buffer) noexcept {
    try {
      return m_security->send(p_buffer);
    } catch (const std::exception &_ex) {
      comm_log_error(logger,
                     "error '" + std::string(_ex.what()) + "' while sending");
      return {status::error_sending, buffer()};
    }
  }

  ///
  /// \brief post
  ///
  /// \param p_buffer
  ///
  /// \details non-blocking
  ///
  /// \return
  ///
  std::future<result> post(const buffer &p_buffer) {
    try {
      return std::async(std::launch::async, m_security->send(p_buffer));
    } catch (const std::exception &_ex) {
      comm_log_error(logger,
                     "error '" + std::string(_ex.what()) + "' while posting");
      return std::future<result>();
    }
  }

  ///
  /// \brief notify
  ///
  /// \param p_buffer
  ///
  /// \param p_handler handles the reply
  ///
  /// \details asynchronous
  ///
  status notify(const buffer &p_buffer,
                std::function<void(const buffer &)> p_handler) noexcept {
    try {
      std::future<result> _future = post(p_buffer);
      result _result = _future.get();
      if (_result.first == status::ok) {
        std::async(std::launch::deferred, p_handler, _result.second);
        return _result.first;
      }
      comm_log_error(logger, "error ", s2v(_result.first), " while notifying");

    } catch (const std::exception &_ex) {
      comm_log_error(logger,
                     "error '" + std::string(_ex.what()) + "' while notifying");
      return status::error_notifying;
    }
  }

private:
  typedef std::unique_ptr<security> security_ptr;

private:
  security_ptr m_security;
  std::mutex m_mutex;
};

} // namespace communication
} // namespace tenacitas

#endif
