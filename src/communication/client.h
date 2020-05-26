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

template <typename t_logger, typename t_connection,
          typename t_io_size = uint16_t>
struct client_t {

  typedef t_logger logger;
  typedef t_connection connection;
  typedef t_io_size io_size;

  client_t(io_size p_send_size = 8 * 1024, io_size p_receive_size = 8 * 1024,
           connection &&p_connection = connection())
      : m_send_size(p_send_size), m_receive_size(p_receive_size),
        m_connection(std::move(p_connection)) {}

  void set_io_send_size(io_size p_send_size) { m_send_size = p_send_size; }

  void set_io_receive_size(io_size p_receive_size) {
    m_receive_size = p_receive_size;
  }

  io_size set_io_send_size() const { return m_send_size; }

  io_size set_io_receive_size() const { return m_receive_size; }

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
      status _status(m_connection.open(p_endpoint));
      if (_status != status::ok) {
        comm_log_error(logger, "error ", _status, " while connecting");
        return _status;
      }

      return status::ok;

    } catch (const std::exception &_ex) {
      comm_log_error(logger, "error '", _ex.what(), "' while connecting");
      return status::error_connecting;
    }
  }

  template <typename t_endpoint>
  status connect(t_endpoint &&p_endpoint) noexcept {
    try {
      status _status(m_connection.open(p_endpoint));
      if (_status != status::ok) {
        comm_log_error(logger, "error ", _status, " while connecting");
        return _status;
      }

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

      typedef decltype(m_send_size) size;
      size _msg_size = static_cast<size>(std::distance(p_begin, p_end));
      comm_log_debug(logger, "io size = ", m_send_size,
                     ", msg size = ", _msg_size);
      if (_msg_size <= m_send_size) {

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
        if (_to_send > m_send_size) {
          _to_send = m_send_size;
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

  /// \brief receive receives all the message, which final size is unknown
  ///
  /// \tparam t_message is the type of the message
  ///
  /// \details blocking
  ///
  /// \return
  template <typename t_message> std::pair<status, t_message> receive() {
    try {
      t_message _all;
      typedef typename t_message::iterator iterator;
      t_message _some(m_send_size, typename t_message::value_type());

      iterator _end = _some.end();

      status _status = m_connection.receive(_some.begin(), _end);
      while (true) {
        if (_status == status::ok) {
          comm_log_debug(logger, "more ", std::distance(_some.begin(), _end),
                         " bytes read");
          _all += t_message(_some.begin(), _end);
          _status = m_connection.receive(_some.begin(), _end);
        }

        if (_status == status::end_of_message) {
          comm_log_info(logger, "end of message");
          t_message _aux(_some.begin(), _end);
          _all += _aux;
          return {status::ok, _all};
        }

        if (_status != status::ok) {
          comm_log_error(logger, "error ", _status, " while receiving");
          return {_status, t_message()};
        }
      }

    } catch (const std::exception &_ex) {
      comm_log_error(logger, "error '", _ex.what(), "' while receiving");
      return {status::error_receiving, t_message()};
    }
  }

  /// \brief post
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
  io_size m_send_size = 8 * 1024;
  io_size m_receive_size = 8 * 1024;
  connection m_connection;
  std::mutex m_mutex;
};

} // namespace communication
} // namespace tenacitas

#endif
