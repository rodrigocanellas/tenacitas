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
#include <communication/internal/timeout.h>
#include <communication/status.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace communication {

/// \brief client_t is a client for remote communication
///
/// \tparam t_logger must provide
/// \code
///
/// template <typename ...p_params>
/// static void debug(const char *, int, params...)
///
/// template <typename ...p_params>
/// static void info(const char *, int, params...)
///
/// template <typename ...p_params>
/// static void warn(const char *, int, params...)
///
/// template <typename ...p_params>
/// static void error(const char *, int, params...)
///
/// template <typename ...p_params>
/// static void fatal(const char *, int, params...)
///
/// \endcode
///
/// \param t_connection must provide
/// \code
///
/// status open(<some-type-of-endpoint>)
///
/// template<typename t_char_iterator>
/// status send(t_char_iterator begin, t_char_iterator end)
///
/// template<typename t_char_iterator>
/// std::pair<status, t_char_iterator> receive(t_char_iterator begin,
/// t_char_iterator end)
///
/// \endcode
///
/// \tparam t_buffer_size is the size of the buffer to send and receive messages
template <typename t_logger, typename t_connection,
          size_t t_buffer_size = 8 * 1024>
struct client_t {

  /// \brief alias for the logger
  typedef t_logger logger;

  /// \brief alias for the connection
  typedef t_connection connection;

  /// \brief type for the i/o buffer
  typedef std::array<char, t_buffer_size> buffer;

  /// \brief type for the buffer iterator
  typedef typename buffer::const_iterator buffer_const_iterator;

  /// \brief constructor
  ///
  /// \param p_connection is the connection to the remote endpoint
  client_t(connection &&p_connection = connection())
      : m_connection(std::move(p_connection)) {}

  /// \brief connect
  ///
  /// \tparam t_endpoint
  ///
  /// \param p_endpoint
  ///
  /// \return
  template <typename t_endpoint>
  status connect(t_endpoint p_endpoint) noexcept {
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

  /// \brief send blocking, no timeout control
  ///
  /// \tparam t_iterator
  ///
  /// \param p_begin
  ///
  /// \param p_end
  ///
  /// \return
  template <typename t_iterator>
  inline status send(t_iterator p_begin, t_iterator p_end) noexcept {
    return internal_send(p_begin, p_end, false);
  }

  /// \brief send blocking, with timeout control
  ///
  /// \tparam t_iterator
  ///
  /// \param p_begin
  ///
  /// \param p_end
  ///
  /// \return
  template <typename t_iterator, typename t_timeout>
  inline status send(t_iterator p_begin, t_iterator p_end,
                     t_timeout p_timeout) noexcept {
    return internal_send(p_begin, p_end, p_timeout);
  }

  /// \brief post non blocking, no timeout control
  ///
  /// \tparam t_iterator
  ///
  /// \param p_begin
  ///
  /// \param p_end
  ///
  /// \return
  template <typename t_iterator>
  inline std::future<status> post(t_iterator p_begin, t_iterator p_end) {
    return std::async(std::launch::async, [this, p_begin, p_end]() {
      return send(p_begin, p_end);
    });
  }

  /// \brief post non blocking, with timeout control
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
  inline std::future<status> post(t_iterator p_begin, t_iterator p_end,
                                  t_timeout p_timeout) {
    return std::async(std::launch::async,
                      [this, p_begin, p_end, p_timeout]() -> status {
                        return send(p_begin, p_end, p_timeout);
                      });
  }

  /// \brief notify sends some data through the connection,
  /// asynchronously, i.e., the caller function will not have any results of the
  /// sending
  ///
  /// \tparam t_iterator
  ///
  /// \param p_begin
  ///
  /// \param p_end
  ///
  /// \details
  template <typename t_iterator>
  void notify(t_iterator p_begin, t_iterator p_end) {
    comm_log_debug(logger, "inside 'send_some_sync'");
    //    auto _rsome = [this, p_begin, p_end]() -> void {
    //      comm_log_debug(logger, "inside '_rsome'");
    //      send(p_begin, p_end);
    //    };

    //    auto _launch = [this, _rsome]() -> void { launch(_rsome); };
    //    std::async(std::launch::async, _launch);

    std::async(std::launch::async,
               [this, p_begin, p_end]() { send(p_begin, p_end); });
  }

  /// \brief send_all
  ///
  /// \tparam t_message
  ///
  /// \param p_message
  ///
  /// \details blocking
  ///
  /// \return
  template <typename t_message> inline status send_all(t_message p_message) {
    return send(p_message.begin(), p_message.end());
  }

  /// \brief send_all with timeout
  ///
  /// \tparam t_message
  ///
  /// \param p_message
  ///
  /// \details blocking
  ///
  /// \return
  template <typename t_message, typename t_timeout>
  inline status send_all(t_message p_message, t_timeout p_timeout) {
    return send(p_message.begin(), p_message.end(), p_timeout);
  }

  /// \brief post_all
  ///
  /// \tparam t_message
  ///
  /// \param p_end
  ///
  /// \details non blocking
  ///
  /// \return
  template <typename t_message>
  inline std::future<status> post_all(t_message p_message) {
    return std::async(std::launch::async, [this, p_message]() -> status {
      return send_all(p_message);
    });
  }

  /// \brief post_all
  ///
  /// \tparam t_message
  ///
  /// \param p_end
  ///
  /// \details non blocking
  ///
  /// \return
  template <typename t_message, typename t_timeout>
  inline std::future<status> post_all(t_message p_message,
                                      t_timeout p_timeout) {
    return std::async(std::launch::async,
                      [this, p_message, p_timeout]() -> status {
                        return send_all(p_message, p_timeout);
                      });
  }

  /// \brief receive_some_block
  ///
  /// \param p_handler called to handle the buffer read
  ///
  /// \details blocking
  ///
  /// \return
  status receive_some_block(
      std::function<status(buffer_const_iterator, buffer_const_iterator)>
          p_handler) {
    return internal_receive(p_handler, false);
  }

  /// \brief receive_some_non_block
  ///
  /// \param p_handler
  ///
  /// \details non blocking
  ///
  /// \return
  std::future<status> receive_some_no_block(
      std::function<status(buffer_const_iterator, buffer_const_iterator)>
          p_handler) {
    return std::async(std::launch::async, [this, p_handler]() {
      return receive_some_block(p_handler);
    });
  }

  /// \brief receive_some_non_block
  ///
  /// \tparam t_timeout
  ///
  /// \param p_handler
  ///
  /// \param p_timeout
  ///
  /// \details non blocking
  ///
  /// \return
  template <typename t_timeout>
  std::future<status> receive_some_no_block(
      std::function<status(buffer_const_iterator, buffer_const_iterator)>
          p_handler,
      t_timeout p_timeout) {

    auto launch_receive = [this, p_handler, p_timeout]() -> status {
      auto rsome = [this, p_handler]() -> status {
        return receive_some_block(p_handler);
      };

      return launch(rsome, p_timeout, status::error_timeout);
    };
    return std::async(std::launch::async, launch_receive);
  }

  /// \brief receive_all_block receives all the message, which final size is
  /// unknown
  ///
  /// \tparam t_message is the type of the message
  /// <tt> std::back_inserter </tt> must be defined for \p p_message
  ///
  /// \details blocking
  ///
  /// \return
  template <typename t_message> status receive_all_block(t_message &p_all) {
    return receive_some_block([&p_all](buffer_const_iterator p_begin,
                                       buffer_const_iterator p_end) -> status {
      std::copy(p_begin, p_end, std::back_inserter(p_all));
      return status::ok;
    });
  }

  /// \brief receive_all_non_block receives all the message, which final size is
  /// unknown
  ///
  /// \tparam t_message is the type of the message
  /// <tt> std::back_inserter </tt> must be defined for \p p_message
  ///
  /// \details non blocking
  ///
  /// \return
  template <typename t_message>
  std::future<status> receive_all_no_block(t_message &p_all) {
    return std::async(std::launch::async, [this, &p_all]() -> status {
      return receive_all_block(p_all);
    });
  }

  template <typename t_message, typename t_timeout>
  std::future<status> receive_all_no_block(t_message &p_all,
                                           t_timeout p_timeout) {

    auto launch_receive = [this, &p_all, p_timeout]() -> status {
      auto rall = [this, &p_all]() -> status {
        return receive_all_block(p_all);
      };
      return launch(rall, p_timeout, status::error_timeout);
    };
    return std::async(std::launch::async, launch_receive);
  }

private:
  template <typename t_iterator, typename t_timeout>
  status internal_send(t_iterator p_begin, t_iterator p_end,
                       t_timeout p_timeout) noexcept {
    try {

      typedef decltype(t_buffer_size) size;
      size _msg_size = static_cast<size>(std::distance(p_begin, p_end));
      comm_log_debug(logger, "io size = ", t_buffer_size,
                     ", msg size = ", _msg_size, " sending '",
                     std::string(p_begin, p_end), "'");
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
      size _to_send = 0;
      timeout _timeout(p_timeout);
      while (_sent != _msg_size) {

        if (_timeout()) {
          comm_log_error(logger, "timeout");
          return status::error_timeout;
        }

        std::advance(_ite, _to_send);

        _to_send = _msg_size - _sent;
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
      }

    } catch (const std::exception &_ex) {
      comm_log_error(logger, "error '", _ex.what(), "' while sending");
      return status::error_sending;
    }
    return status::ok;
  }

  template <typename t_timeout>
  status internal_receive(
      std::function<status(buffer_const_iterator, buffer_const_iterator)>
          p_handler,
      t_timeout p_timeout) {
    try {

      buffer _buffer;

      typename buffer::iterator _begin = _buffer.begin();

      timeout _timeout(p_timeout);
      std::pair<status, decltype(t_buffer_size)> _res =
          m_connection.receive(_begin, t_buffer_size);
      while (true) {
        if (_timeout()) {
          comm_log_error(logger, "timeout");
          return status::error_timeout;
        }

        if (_res.first == status::end_of_message) {
          comm_log_info(logger, "end of message");
          p_handler(&(*_begin), std::next(_begin, _res.second));
          return status::ok;
        }

        if (_res.first != status::ok) {
          comm_log_error(logger, "error ", _res.first, " while receiving");
          return _res.first;
        }

        comm_log_debug(logger, "more ", _res.second, " bytes read");
        p_handler(&(*_begin), std::next(_begin, _res.second));
        _res = m_connection.receive(_begin, t_buffer_size);
      }

    } catch (const std::exception &_ex) {
      comm_log_error(logger, "error '", _ex.what(), "' while receiving");
      return status::error_receiving;
    }
  }

  /// \brief launch generic method to launch a function with timeout control
  ///
  /// \tparam t_function
  ///
  /// \tparam t_timeout
  ///
  /// \param p_function
  ///
  /// \param p_timeout
  ///
  /// \param p_if_error
  template <typename t_function, typename t_timeout>
  status launch(t_function p_function, t_timeout p_timeout, status p_if_error) {
    using namespace std;
    try {
      future<status> _future = async(launch::async, p_function);
      while (true) {
        future_status _future_status = _future.wait_for(p_timeout);
        if (_future_status == future_status::ready) {
          comm_log_debug(logger, "no timeout");
          return _future.get();
        }
        if (_future_status == future_status::timeout) {
          comm_log_debug(logger, "timeout!!!");
          return p_if_error;
        }
      }

    } catch (const exception &_ex) {
      comm_log_error(logger, "error '", _ex.what());
      return status::error_posting;
    }
  }

  /// \brief launch generic method to launch a function with timeout control
  ///
  /// \tparam t_function
  ///
  /// \param p_function
  template <typename t_function> void launch(t_function p_function) {
    comm_log_debug(logger, "inside 'launch'");
    try {
      std::async(std::launch::async, p_function);

    } catch (const std::exception &_ex) {
      comm_log_error(logger, "error '", _ex.what());
    }
  }

private:
  connection m_connection;
};

} // namespace communication
} // namespace tenacitas

#endif
