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

  /// \brief send_block
  ///
  /// \tparam t_message
  ///
  /// \param p_message
  ///
  /// \details blocking
  ///
  /// \return
  template <typename t_message> inline status send_block(t_message p_message) {
    return send_block(p_message.begin(), p_message.end());
  }

  /// \brief send_block
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
  status send_block(t_iterator p_begin, t_iterator p_end) noexcept {
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

  /// \brief send_non_block
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
  std::future<status> send_non_block(t_iterator p_begin, t_iterator p_end) {
    using namespace std;
    try {
      return async(launch::async,
                   [this, p_begin, p_end]() { send(p_begin, p_end); });
    } catch (const exception &_ex) {
      comm_log_error(logger, "error '", _ex.what(), "' while posting");
      return future<status>();
    }
  }

  /// \brief send_non_block
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
  std::future<status> send_non_block(t_message p_message, t_timeout p_timeout) {
    using namespace std;
    typename t_message::const_iterator _begin = p_message.begin();
    typename t_message::const_iterator _end = p_message.end();
    return async([this, _begin, _end, p_timeout]() {
      return launch_send(_begin, _end, p_timeout);
    });
  }

  /// \brief send_non_block
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
  std::future<status> send_non_block(t_iterator p_begin, t_iterator p_end,
                                     t_timeout p_timeout) {
    using namespace std;
    return async([this, p_begin, p_end, p_timeout]() {
      return launch_send(p_begin, p_end, p_timeout);
    });
  }

  /// \brief receive_block receives all the message, which final size is unknown
  ///
  /// \tparam t_message is the type of the message
  /// <tt> std::back_inserter </tt> must be defined for \p p_message
  ///
  /// \details blocking
  ///
  /// \return
  template <typename t_message> status receive_all_block(t_message &p_all) {
    return receive_block([&p_all](buffer_const_iterator p_begin,
                                  buffer_const_iterator p_end) -> status {
      std::copy(p_begin, p_end, std::back_inserter(p_all));
      return status::ok;
    });
  }

  /// \brief receive_block receives all the message, which final size is unknown
  ///
  /// \tparam t_message is the type of the message
  /// <tt> std::back_inserter </tt> must be defined for \p p_message
  ///
  /// \details non blocking
  ///
  /// \return
  template <typename t_message>
  std::future<status> receive_all_non_block(t_message &p_all) {
    return std::async(std::launch::async, [this, &p_all]() -> status {
      return receive_all_block(p_all);
    });
  }

  template <typename t_message, typename t_timeout>
  std::future<status> receive_all_non_block(t_message &p_all,
                                            t_timeout p_timeout) {
    auto rall = [this, &p_all]() -> status { return receive_all_block(p_all); };

    auto launch_receive = [this, rall, p_timeout]() -> status {
      return launch(rall, p_timeout, status::error_timeout);
    };
    return std::async(std::launch::async, launch_receive);
  }

  /// \brief receive_block
  ///
  /// \param p_handler
  ///
  /// \details blocking
  ///
  /// \return
  status receive_block(
      std::function<status(buffer_const_iterator, buffer_const_iterator)>
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

  /// \brief receive_non_block
  ///
  /// \param p_handler
  ///
  /// \details non blocking
  ///
  /// \return
  std::future<status> receive_non_block(
      std::function<status(buffer_const_iterator, buffer_const_iterator)>
          p_handler) {
    using namespace std;
    try {
      return async(launch::async,
                   [this, p_handler]() { return receive_block(p_handler); });
    } catch (const exception &_ex) {
      comm_log_error(logger, "error '", _ex.what(), "' while collecting");
      return future<status>();
    }
  }

  /// \brief receive_non_block
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
  std::future<status> receive_non_block(
      std::function<status(buffer_const_iterator, buffer_const_iterator)>
          p_handler,
      t_timeout p_timeout) {
    using namespace std;
    return async([this, p_handler, p_timeout]() {
      return launch_receive(p_handler, p_timeout);
    });
  }

private:
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
      comm_log_error(logger, "error '", _ex.what(), "' while posting");
      return status::error_posting;
    }
  }

  /// \brief launch_receive launchs a receive function with timeout control
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
  template <typename t_timeout>
  status launch_receive(
      std::function<status(buffer_const_iterator, buffer_const_iterator)>
          p_handler,
      t_timeout p_timeout) {
    return launch([this, p_handler]() { return receive_block(p_handler); },
                  p_timeout, status::error_receiving);
  }

  /// \brief launch_receive launchs a send function with timeout control
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
  template <typename t_iterator, typename t_timeout>
  status launch_send(t_iterator p_begin, t_iterator p_end,
                     t_timeout p_timeout) {
    return launch(
        ([this, p_begin, p_end]() { return send_block(p_begin, p_end); }),
        p_timeout, status::error_timeout);
  }

private:
  connection m_connection;
};

} // namespace communication
} // namespace tenacitas

#endif
