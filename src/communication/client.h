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
/// \tparam t_receive_buffer_size is the size of the buffer to send and receive
/// messages
template <typename t_logger, typename t_connection,
          size_t t_receive_buffer_size = 8 * 1024>
struct client_t {

  /// \brief alias for the logger
  typedef t_logger logger;

  /// \brief alias for the connection
  typedef t_connection connection;

  /// \brief type for the i/o buffer
  typedef std::array<char, t_receive_buffer_size> buffer;

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
  /// \param p_begin
  ///
  /// \param p_end
  ///
  /// \return

  template <typename t_iterator>
  status send_block(t_iterator p_begin, t_iterator p_end) noexcept {
    try {
      return m_connection.send(p_begin, p_end);
    } catch (const std::exception &_ex) {
      comm_log_error(logger, "error '", _ex.what(), "' while sending");
      return status::error_sending;
    }
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
  inline status send_block(t_iterator p_begin, t_iterator p_end,
                           t_timeout p_timeout) noexcept {
    using namespace std;
    try {
      future<status> _future =
          async(launch::async, [this, p_begin, p_end]() -> status {
            return send_block(p_begin, p_end);
          });
      future_status _future_status = _future.wait_for(p_timeout);
      if (_future_status == future_status::timeout) {
        return status::error_timeout;
      }
      if (_future_status == future_status::deferred) {
        return status::error_sending;
      }
      return _future.get();
    } catch (const std::exception &_ex) {
      comm_log_error(logger, "error '", _ex.what(), "' while sending");
      return status::error_sending;
    }
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
  template <typename t_message>
  inline status send_block_all(t_message p_message) noexcept {
    return send_block(p_message.begin(), p_message.end());
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
  inline status send_block_all(t_message p_message,
                               t_timeout p_timeout) noexcept {
    return send_block(p_message.begin(), p_message.end(), p_timeout);
  }

  /// \brief post non blocking, no timeout control
  ///
  /// \tparam const char *
  ///
  /// \param p_begin
  ///
  /// \param p_end
  ///
  /// \return
  template <typename t_iterator>
  inline std::future<status> send_non_block(t_iterator p_begin,
                                            t_iterator p_end) noexcept {
    try {
      return std::async(std::launch::async, [this, p_begin, p_end]() {
        return send_block(p_begin, p_end);
      });

    } catch (const std::exception &_ex) {
      comm_log_error(logger, "error '", _ex.what(), "'");
      return std::future<status>();
    }
  }

  /// \brief send non blocking, with timeout control
  ///
  /// \tparam const char *
  ///
  /// \tparam const char *
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
  std::future<status> send_non_block(t_iterator p_begin, t_iterator p_end,
                                     t_timeout p_timeout) noexcept {
    using namespace std;
    try {
      return async(
          launch::async, [this, p_begin, p_end, p_timeout]() -> status {
            future<status> _future =
                async(launch::async, [this, p_begin, p_end]() -> status {
                  return send_block(p_begin, p_end);
                });
            future_status _future_status = _future.wait_for(p_timeout);
            if (_future_status == future_status::timeout) {
              return status::error_timeout;
            }
            if (_future_status == future_status::deferred) {
              return status::error_sending;
            }
            return _future.get();
          });

    } catch (const std::exception &_ex) {
      comm_log_error(logger, "error '", _ex.what(), "'");
      return std::future<status>();
    }
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
  std::future<status> send_non_block_all(t_message p_message) noexcept {
    try {
      return std::async(std::launch::async, [this, p_message]() -> status {
        return send_block_all(p_message);
      });

    } catch (const std::exception &_ex) {
      comm_log_error(logger, "error '", _ex.what(), "'");
      return std::future<status>();
    }
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
  inline std::future<status> send_non_block_all(t_message p_message,
                                                t_timeout p_timeout) noexcept {
    return send_non_block(p_message.begin(), p_message.end(), p_timeout);
  }

  /// \brief notify sends some data through the connection,
  /// asynchronously, i.e., the caller function will not have any results of
  /// the sending
  ///
  /// \tparam const char *
  ///
  /// \param p_begin
  ///
  /// \param p_end
  ///
  /// \details
  template <typename t_iterator>
  void send_async(t_iterator p_begin, t_iterator p_end) noexcept {
    try {
      comm_log_debug(logger, "inside 'send_async'");

      std::async(std::launch::async, [this, p_begin, p_end]() -> void {
        send_block(p_begin, p_end);
      });
    } catch (const std::exception &_ex) {
      comm_log_error(logger, "error '", _ex.what(), "'");
    }
  }

  template <typename t_iterator, typename t_timeout>
  void send_async(t_iterator p_begin, t_iterator p_end,
                  t_timeout p_timeout) noexcept {
    try {
      comm_log_debug(logger, "inside 'send_async'");
      std::async(std::launch::async,
                 [this, p_begin, p_end, p_timeout]() -> void {
                   send_block(p_begin, p_end, p_timeout);
                 });

    } catch (const std::exception &_ex) {
      comm_log_error(logger, "error '", _ex.what(), "'");
    }
  }

  template <typename t_message>
  inline void send_async_all(t_message p_message) noexcept {
    return send_async(p_message.begin(), p_message.end());
  }

  template <typename t_message, typename t_timeout>
  void send_async_all(t_message p_message, t_timeout p_timeout) noexcept {
    return send_async(p_message.begin(), p_message.end(), p_timeout);
  }

  /// \brief receive blocks to repeatedly receive an amount of chars, and
  /// call a function to handle them, until all the message is read
  ///
  /// \param p_handler called to handle the amount of chars read
  ///
  /// \return
  status receive_block(
      std::function<status(const char *, const char *)> p_handler) noexcept {
    return receive(p_handler);
  }

  /// \brief receive blocks to repeatedly receive an amount of chars, and
  /// call a function to handle them, until all the message is read, with a
  /// timeout for all the message to be received
  ///
  /// \tparam type of time definition (std::chrono::seconds, for example)
  ///
  /// \param p_handler called to handle the amount of chars read
  ///
  /// \param p_timeout timeout for the message to be received
  ///
  /// \return
  template <typename t_timeout>
  status
  receive_block(std::function<status(const char *, const char *)> p_handler,
                t_timeout p_timeout) noexcept {
    using namespace std;
    future<status> _future =
        async(launch::async,
              [this, p_handler]() -> status { return receive(p_handler); });
    future_status _future_status = _future.wait_for(p_timeout);
    if ((_future_status == future_status::timeout) ||
        (_future_status == future_status::deferred)) {
      return status::error_timeout;
    }
    return _future.get();
  }

  /// \brief receive_all receives all the message, which final size is
  /// unknown
  ///
  /// \tparam t_message is the type of the message
  /// <tt> std::back_inserter </tt> must be defined for \p p_message
  ///
  /// \details blocking
  ///
  /// \return
  template <typename t_message>
  status receive_block_all(t_message &p_all) noexcept {
    return receive_block(
        [&p_all](const char *p_begin, const char *p_end) -> status {
          std::copy(p_begin, p_end, std::back_inserter(p_all));
          return status::ok;
        });
  }

  template <typename t_message, typename t_timeout>
  status receive_block_all(t_message &p_all, t_timeout p_timeout) noexcept {
    return receive_block(
        [&p_all](const char *p_begin, const char *p_end) -> status {
          std::copy(p_begin, p_end, std::back_inserter(p_all));
          return status::ok;
        },
        p_timeout);
  }

  /// \brief collect does not block to repeatedly receive an amount of chars,
  /// and call a function to handle them, until all the message is read
  ///
  /// \param p_handler called to handle the amount of chars read
  ///
  /// \return
  std::future<status> receive_non_block(
      std::function<status(const char *, const char *)> p_handler) noexcept {
    return std::async(std::launch::async,
                      [this, p_handler]() { return receive_block(p_handler); });
  }

  /// \brief collect does not block to repeatedly receive an amount of chars,
  /// and call a function to handle them, until all the message is read, with
  /// a timeout for all the message to be received
  ///
  /// \tparam type of time definition (std::chrono::seconds, for example)
  ///
  /// \param p_handler called to handle the amount of chars read
  ///
  /// \param p_timeout timeout for the message to be received
  ///
  /// \return
  template <typename t_timeout>
  std::future<status>
  receive_non_block(std::function<status(const char *, const char *)> p_handler,
                    t_timeout p_timeout) noexcept {

    return std::async(std::launch::async,
                      [this, p_handler, p_timeout]() -> status {
                        return receive_block(p_handler, p_timeout);
                      });
  }

  /// \brief receive_all_non_block receives all the message, which final size
  /// is unknown
  ///
  /// \tparam t_message is the type of the message
  /// <tt> std::back_inserter </tt> must be defined for \p p_message
  ///
  /// \details non blocking
  ///
  /// \return
  template <typename t_message>
  std::future<status> receive_non_block_all(t_message &p_all) noexcept {
    return std::async(std::launch::async, [this, &p_all]() -> status {
      return receive_block_all(p_all);
    });
  }

  template <typename t_message, typename t_timeout>
  std::future<status> receive_non_block_all(t_message &p_all,
                                            t_timeout p_timeout) noexcept {
    return std::async(std::launch::async,
                      [this, &p_all, p_timeout]() -> status {
                        return receive_block_all(p_all, p_timeout);
                      });
  }

  void receive_async(
      std::function<status(const char *, const char *)> p_handler) noexcept {
    std::async(std::launch::async,
               [this, p_handler]() -> void { receive_block(p_handler); });
  }

  template <typename t_timeout>
  void
  receive_async(std::function<status(const char *, const char *)> p_handler,
                t_timeout p_timeout) {

    std::async(std::launch::async, [this, p_handler, p_timeout]() -> void {
      receive_block(p_handler, p_timeout);
    });
  }

  //  template <typename t_message> void receive_async_all(t_message &p_all) {
  //    std::async(std::launch::async,
  //               [this, &p_all]() -> void { receive_block_all(p_all); });
  //  }

  //  template <typename t_message, typename t_timeout>
  //  void receive_async(t_message &p_all, t_timeout p_timeout) {
  //    std::async(std::launch::async, [this, &p_all, p_timeout]() -> void {
  //      receive_block_all(p_all, p_timeout);
  //    });
  //  }

private:
  //  template <typename t_iterator>
  //  status send(t_iterator p_begin, t_iterator p_end) noexcept {
  //    try {
  //      return m_connection.send(p_begin, p_end);
  //    } catch (const std::exception &_ex) {
  //      comm_log_error(logger, "error '", _ex.what(), "' while sending");
  //      return status::error_sending;
  //    }
  //  }

  //  template <typename t_iterator, typename t_timeout>
  //  status send(t_iterator p_begin, t_iterator p_end,
  //              t_timeout p_timeout) noexcept {
  //    using namespace std;
  //    try {
  //      future<status> _future =
  //          async(launch::async, [this, p_begin, p_end]() -> status {
  //            return send(p_begin, p_end);
  //          });
  //      future_status _future_status = _future.wait_for(p_timeout);
  //      if ((_future_status == future_status::timeout) ||
  //          (_future_status == future_status::deferred)) {
  //        return status::error_timeout;
  //      }
  //      return _future.get();
  //    } catch (const std::exception &_ex) {
  //      comm_log_error(logger, "error '", _ex.what(), "' while sending");
  //      return status::error_sending;
  //    }
  //  }

  //  template <typename t_timeout>
  //  status receive(std::function<status(const char *, const char *)>
  //  p_handler,
  //                 t_timeout p_timeout) noexcept {
  //    using namespace std;
  //    try {

  //      buffer _buffer;

  //      typename buffer::iterator _begin = _buffer.begin();
  //      std::pair<status, decltype(t_receive_buffer_size)> _res =
  //      {status::ok, 0};

  //      while (true) {
  //        std::future<status> _future =
  //            async(launch::async, [this, _begin]() -> status {
  //              return m_connection.receive(_begin, t_receive_buffer_size);
  //            });

  //        if (is_timeout(_future, p_timeout)) {
  //          comm_log_error(logger, "timeout");
  //          return status::error_timeout;
  //        }

  //        if (_res.first != status::ok) {
  //          if (_res.first == status::end_of_message) {
  //            comm_log_debug(logger, "end of message");
  //            p_handler(&(*_begin), std::next(_begin, _res.second));
  //            return status::ok;
  //          }

  //          comm_log_error(logger, "error ", _res.first, " while
  //          receiving"); return _res.first;
  //        }

  //        comm_log_debug(logger, "more ", _res.second, " bytes read");
  //        p_handler(&(*_begin), std::next(_begin, _res.second));
  //      }

  //    } catch (const std::exception &_ex) {
  //      comm_log_error(logger, "error '", _ex.what(), "' while receiving");
  //      return status::error_receiving;
  //    }
  //  }

  status receive(
      std::function<status(const char *, const char *)> p_handler) noexcept {
    using namespace std;
    try {

      buffer _buffer;

      typename buffer::iterator _begin = _buffer.begin();
      std::pair<status, decltype(t_receive_buffer_size)> _res = {status::ok, 0};

      while (true) {
        _res = m_connection.receive(_begin, t_receive_buffer_size);
        if (_res.first != status::ok) {
          comm_log_error(logger, "error receiving '", _res.first, "'");
          return _res.first;
        }
        if (_res.first == status::end_of_message) {
          comm_log_debug(logger, "end of message");
          return status::ok;
        }
        comm_log_debug(logger, "more ", _res.second, " bytes read");
        p_handler(&(*_begin), std::next(_begin, _res.second));
      }

    } catch (const std::exception &_ex) {
      comm_log_error(logger, "error '", _ex.what(), "' while receiving");
      return status::error_receiving;
    }
  }

private:
  connection m_connection;
};

} // namespace communication
} // namespace tenacitas

#endif
