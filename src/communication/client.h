#ifndef TENACITAS_COMMUNICATION_CLIENT_H
#define TENACITAS_COMMUNICATION_CLIENT_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <array>
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
          size_t t_buffer_size = 8 * 1024>
struct client_t {

  typedef t_logger logger;
  typedef t_connection connection;
  typedef std::array<char, t_buffer_size> buffer;
  typedef typename buffer::const_iterator buffer_const_iterator;

  client_t(connection &&p_connection = connection())
      : m_connection(std::move(p_connection)) {}

  size_t get_io_send_size() const { return t_buffer_size; }

  size_t get_io_receive_size() const { return t_buffer_size; }

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

      typedef decltype(t_buffer_size) size;
      size _msg_size = static_cast<size>(std::distance(p_begin, p_end));
      comm_log_debug(logger, "io size = ", t_buffer_size,
                     ", msg size = ", _msg_size);
      if (_msg_size <= t_buffer_size) {

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
        if (_to_send > t_buffer_size) {
          _to_send = t_buffer_size;
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
  /// <tt> std::back_inserter </tt> must be defined for \p p_message
  ///
  /// \details blocking
  ///
  /// \return
  template <typename t_message> status receive(t_message &p_all) {
    try {

      buffer _some;

      typedef typename buffer::iterator iterator;
      iterator _begin = _some.begin();
      iterator _end = _some.end();

      std::pair<status, iterator> _res = m_connection.receive(_begin, _end);
      while (true) {
        if (_res.first == status::ok) {
          comm_log_debug(logger, "more ", std::distance(_begin, _res.second),
                         " bytes read");
          std::copy(_begin, _res.second, std::back_inserter(p_all));

          _res = m_connection.receive(_begin, _end);
        }

        if (_res.first == status::end_of_message) {
          comm_log_info(logger, "end of message");
          std::copy(_begin, _res.second, std::back_inserter(p_all));
          return status::ok;
        }

        if (_res.first != status::ok) {
          comm_log_error(logger, "error ", _res.first, " while receiving");
          return _res.first;
        }
      }

    } catch (const std::exception &_ex) {
      comm_log_error(logger, "error '", _ex.what(), "' while receiving");
      return status::error_receiving;
    }
  }

  status
  receive(std::function<status(buffer_const_iterator, buffer_const_iterator)>
              p_handler) {
    try {

      buffer _some;

      typedef typename buffer::iterator iterator;
      iterator _begin = _some.begin();
      iterator _end = _some.end();

      std::pair<status, iterator> _res = m_connection.receive(_begin, _end);
      while (true) {
        if (_res.first == status::ok) {
          comm_log_debug(logger, "more ", std::distance(_begin, _res.second),
                         " bytes read");
          p_handler(&(*_begin), &(*_res.second));

          _res = m_connection.receive(_begin, _end);
        }

        if (_res.first == status::end_of_message) {
          comm_log_info(logger, "end of message");
          p_handler(&(*_begin), &(*_res.second));
          return status::ok;
        }

        if (_res.first != status::ok) {
          comm_log_error(logger, "error ", _res.first, " while receiving");
          return _res.first;
        }
      }

    } catch (const std::exception &_ex) {
      comm_log_error(logger, "error '", _ex.what(), "' while receiving");
      return status::error_receiving;
    }
  }

  std::future<status>
  collect(std::function<status(buffer_const_iterator, buffer_const_iterator)>
              p_handler) {
    using namespace std;
    try {
      return async(launch::async,
                   [this, p_handler]() { return receive(p_handler); });
    } catch (const exception &_ex) {
      comm_log_error(logger, "error '", _ex.what(), "' while collecting");
      return future<status>();
    }
  }

  template <typename t_timeout>
  std::future<status>
  collect(std::function<status(buffer_const_iterator, buffer_const_iterator)>
              p_handler,
          t_timeout p_timeout) {
    using namespace std;
    return async([this, p_handler, p_timeout]() {
      return launch_collect(p_handler, p_timeout);
    });
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

  template <typename t_timeout>
  status launch_collect(
      std::function<status(buffer_const_iterator, buffer_const_iterator)>
          p_handler,
      t_timeout p_timeout) {
    using namespace std;
    try {
      future<status> _future = async(
          launch::async, [this, p_handler]() { return receive(p_handler); });
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
  connection m_connection;
  std::mutex m_mutex;
};

} // namespace communication
} // namespace tenacitas

#endif
