#ifndef TENACITAS_COMMUNICATION_CLIENT_H
#define TENACITAS_COMMUNICATION_CLIENT_H

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
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

#include <communication/internal/log.h>
#include <communication/status.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace communication {

template <typename t_logger, typename t_connector> struct client_t {
  typedef t_logger logger;
  typedef t_connector connector;
  typedef typename t_connector::connection connection;

  client_t(connector &p_connector) : m_connector(p_connector) {}

  template <typename t_endpoint>
  status connect(const t_endpoint &p_endpoint) noexcept {
    try {
      std::pair<status, connection> _res = m_connector.connect(p_endpoint);
      comm_log_debug(logger, "connector returned ", _res.first);
      if (_res.first != status::ok) {
        comm_log_error(logger, "error ", _res.first, " while connecting");
        return _res.first;
      }

      std::lock_guard<std::mutex> _lock(m_mutex);
      m_connection = std::move(_res.second);
      m_io_size = m_connection.get_io_size();
      return status::ok;

    } catch (const std::exception &_ex) {
      comm_log_error(logger, "error '", _ex.what(), "' while connecting");
      return status::error_connecting;
    }
  }

  template <typename t_message> status send(const t_message &p_message) {
    return send(p_message.begin(), p_message.end());
  }

  template <typename t_message> status send(t_message &&p_message) {
    return send(p_message.begin(), p_message.end());
  }

  /// \brief send
  ///
  /// \tparam t_iterator
  ///
  /// \param p_begin
  ///
  /// \param p_end
  ///
  /// \details blocking
  ///
  /// \return
  template <typename t_iterator>
  status send(t_iterator p_begin, t_iterator p_end) noexcept {
    try {
      auto _size = std::distance(p_begin, p_end);
      if (_size <= m_io_size) {
        status _status = m_connection.send(p_begin, p_end);
        if (_status == status::ok) {
          return _status;
        }
        comm_log_error(logger, "error '", _status, "' while sending");
        return _status;
      }

      decltype(_size) _sent = 0;
      t_iterator _ite = p_begin;
      while (_sent != _size) {
        decltype(_size) _to_sent = _size - _sent;
        if (_to_sent > m_io_size) {
          _to_sent = m_io_size;
        }

        status _status = m_connection.send(_ite, std::next(_ite, _to_sent));
        if (_status != status::ok) {
          comm_log_error(logger, "error '", _status, "' while sending");
          return _status;
        }
        _sent += _to_sent;
        std::advance(_ite, _sent);
      }

    } catch (const std::exception &_ex) {
      comm_log_error(logger, "error '", _ex.what(), "' while sending");
      return status::error_sending;
    }
    return status::ok;
  }

  //  /// \brief receive
  //  ///
  //  /// \details blocking
  //  ///
  //  /// \return
  //  result receive() {
  //    try {
  //      return m_connection.receive();
  //    } catch (const std::exception &_ex) {
  //      comm_log_error(logger, "error '", _ex.what(), "' while receiving");
  //      return {status::error_receiving, message()};
  //    }
  //  }

  /// \brief send
  ///
  /// \tparam t_iterator
  ///
  /// \param p_begin
  ///
  /// \param p_end
  ///
  /// \details non blocking
  ///
  /// \return
  template <typename t_iterator>
  std::future<status> post(t_iterator p_begin, t_iterator p_end) {
    using namespace std;
    try {
      return async(launch::async, [this, p_begin, p_end]() {
        m_connection.send(p_begin, p_end);
      });
    } catch (const exception &_ex) {
      comm_log_error(logger, "error '", _ex.what(), "' while posting");
      return future<status>();
    }
  }

  /// \brief send
  ///
  /// \tparam t_iterator
  ///
  /// \tparam t_iterator
  ///
  /// \param p_begin
  ///
  /// \param p_end
  ///
  /// \param p_timeout
  ///
  /// \details non blocking
  ///
  /// \return
  template <typename t_iterator, typename t_timeout>
  std::future<status> post(t_iterator p_begin, t_iterator p_end,
                           t_timeout p_timeout) {
    using namespace std;
    return async([this, p_begin, p_end, p_timeout]() {
      return launch_post(p_begin, p_end, p_timeout);
    });
  }

  //  /// \brief collect
  //  ///
  //  /// \details non-blocking
  //  ///
  //  /// \return
  //  std::future<result> collect() {
  //    try {
  //      return std::async(std::launch::async, m_connection.receive());
  //    } catch (const std::exception &_ex) {
  //      comm_log_error(logger, "error '", _ex.what(), "' while receiving");
  //      return std::future<result>();
  //    }
  //  }

private:
  template <typename t_iterator, typename t_timeout>
  status launch_post(t_iterator p_begin, t_iterator p_end,
                     t_timeout p_timeout) {
    using namespace std;
    try {
      future<status> _future = async(launch::async, [this, p_begin, p_end]() {
        m_connection.send(p_begin, p_end);
      });
      while (true) {
        future_status _future_status = _future.wait_for(p_timeout);
        if (_future_status == future_status::ready) {
          return _future.get();
        }
        if (_future_status == future_status::timeout) {
          return status::error_timeout;
        }
      }

    } catch (const exception &_ex) {
      comm_log_error(logger, "error '", _ex.what(), "' while posting");
      return status::error_posting;
    }
  }

private:
  connector &m_connector;
  connection m_connection;
  uint16_t m_io_size;
  std::mutex m_mutex;
};

} // namespace communication
} // namespace tenacitas

#endif
