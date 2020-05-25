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
  typedef typename connector::ptr connector_ptr;
  typedef typename t_connector::connection connection;

  /// \brief client_t constructor
  /// \param p_connector
  client_t(connector_ptr p_connector) : m_connector(p_connector) {}

  /// \brief connect
  ///
  /// \tparam t_endpoint
  ///
  /// \param p_endpoint
  ///
  /// \return
  template <typename t_endpoint>
  status connect(const t_endpoint &p_endpoint) noexcept {
    try {
      std::pair<status, connection> _res(m_connector->connect(p_endpoint));
      if (_res.first != status::ok) {
        comm_log_error(logger, "error ", _res.first, " while connecting");
        return _res.first;
      }

      std::lock_guard<std::mutex> _lock(m_mutex);
      m_connection = std::move(_res.second);

      return status::ok;

    } catch (const std::exception &_ex) {
      comm_log_error(logger, "error '", _ex.what(), "' while connecting");
      return status::error_connecting;
    }
  }

  /// \brief send
  ///
  /// \tparam t_message
  ///
  /// \param p_message
  ///
  /// \details blocking
  ///
  /// \return
  template <typename t_message> inline status send(t_message p_message) {
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
      auto _max_send_size = m_connection.get_send_size();
      typedef decltype(_max_send_size) size;
      size _msg_size = static_cast<size>(std::distance(p_begin, p_end));
      comm_log_debug(logger, "io size = ", _max_send_size,
                     ", msg size = ", _msg_size);
      if (_msg_size <= _max_send_size) {

        status _status = m_connection.send(p_begin, p_end);
        if (_status == status::ok) {
          return _status;
        }
        comm_log_error(logger, "error '", _status, "' while sending");
        return _status;
      }

      size _sent = 0;
      t_iterator _ite = p_begin;
      while (_sent != _msg_size) {
        size _to_send = _msg_size - _sent;
        if (_to_send > _max_send_size) {
          _to_send = _max_send_size;
        }
        comm_log_debug(logger, "sending ", _to_send, " bytes");

        status _status = m_connection.send(_ite, std::next(_ite, _to_send));
        if (_status != status::ok) {
          comm_log_error(logger, "error '", _status, "' while sending");
          return _status;
        }
        _sent += _to_send;
        std::advance(_ite, _sent);
      }

    } catch (const std::exception &_ex) {
      comm_log_error(logger, "error '", _ex.what(), "' while sending");
      return status::error_sending;
    }
    return status::ok;
  }

  /// \brief receive
  ///
  /// \details blocking
  ///
  /// \return
  template <typename t_message> std::pair<status, t_message> receive() {
    try {
      t_message _all;
      auto _max_receive_size = m_connection.get_receive_size();
      t_message _some(_max_receive_size, typename t_message::value_type());
      std::pair<status, typename t_message::const_iterator> _res =
          m_connection.receive(_some.begin(), _some.end());
      while (true) {
        if (_res.first == status::end_of_message) {
          typename t_message::const_iterator _begin = _some.begin();
          typename t_message::const_iterator _end = _res.second();
          t_message _aux(_begin, _end);
          _all += _aux;
          return {status::ok, _all};
        }

        if (_res.first == status::ok) {
          _all += t_message(_some.begin(), _res.second);
          _res = m_connection.receive(_some.begin(), _some.end());
        }

        if (_res.first != status::ok) {
          comm_log_error(logger, "error ", _res.first, " while receiving");
          return {_res.first, t_message()};
        }
      }

    } catch (const std::exception &_ex) {
      comm_log_error(logger, "error '", _ex.what(), "' while receiving");
      return {status::error_receiving, t_message()};
    }
  }

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
      return async(launch::async,
                   [this, p_begin, p_end]() { send(p_begin, p_end); });
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
  template <typename t_message, typename t_timeout>
  std::future<status> post(t_message p_message, t_timeout p_timeout) {
    using namespace std;
    typename t_message::const_iterator _begin = p_message.begin();
    typename t_message::const_iterator _end = p_message.end();
    return async([this, _begin, _end, p_timeout]() {
      return launch_post(_begin, _end, p_timeout);
    });
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
        return send(p_begin, p_end);
      });
      while (true) {
        future_status _future_status = _future.wait_for(p_timeout);
        if (_future_status == future_status::ready) {
          comm_log_debug(logger, "no timeout");
          return _future.get();
        }
        if (_future_status == future_status::timeout) {
          comm_log_debug(logger, "timeout!!!");
          return status::error_timeout;
        }
      }

    } catch (const exception &_ex) {
      comm_log_error(logger, "error '", _ex.what(), "' while posting");
      return status::error_posting;
    }
  }

private:
  connector_ptr m_connector;
  connection m_connection;
  std::mutex m_mutex;
};

} // namespace communication
} // namespace tenacitas

#endif
