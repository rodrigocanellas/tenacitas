#ifndef TENACITAS_COMMUNICATION_TST_CLIENT_TEST_H
#define TENACITAS_COMMUNICATION_TST_CLIENT_TEST_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <communication/client.h>
#include <communication/internal/log.h>
#include <communication/status.h>
#include <logger/cerr/log.h>

/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace communication {
/// \brief namespace for test
namespace tst {

using namespace tenacitas;

template <typename t_test> struct blocking__no_timeout_control_t {

  bool operator()() {
    typedef logger::cerr::log logger;
    typedef typename t_test::connection connection;
    typedef communication::status status;
    typedef communication::client_t<logger, connection, t_test::rec_buf_size,
                                    t_test::snd_buf_size>
        client;

    client _client;

    status _status = _client.connect(t_test::endpoint);
    if (_status != status::ok) {
      comm_log_error(logger, "erro ", _status);
      return false;
    }

    _status = _client.send_block_all(t_test::msg);
    if (_status != status::ok) {
      comm_log_error(logger, "erro ", _status);
      return false;
    }

    return true;
  }

  static std::string desc() { return t_test::desc; }
};

template <typename t_test> struct blocking__timeout_control_t {

  bool operator()() {
    typedef logger::cerr::log logger;
    typedef typename t_test::connection connection;
    typedef communication::status status;
    typedef communication::client_t<logger, connection, t_test::rec_buf_size,
                                    t_test::snd_buf_size>
        client;

    client _client;

    status _status = _client.connect(t_test::endpoint);
    if (_status != status::ok) {
      comm_log_error(logger, "erro ", _status);
      return false;
    }

    _status = _client.send_block_all(t_test::msg, t_test::timeout);

    if (t_test::is_timeout) {
      if (_status != status::error_timeout) {
        comm_log_error(logger, "erro ", _status);
        return false;
      }
    } else if (_status != status::ok) {
      comm_log_error(logger, "erro ", _status);
      return false;
    }

    return true;
  }

  static std::string desc() { return t_test::desc; }
};

template <typename t_test> struct non_blocking__no_timeout_control_t {
  bool operator()() {
    typedef logger::cerr::log logger;
    typedef typename t_test::connection connection;
    typedef communication::status status;
    typedef communication::client_t<logger, connection, t_test::rec_buf_size,
                                    t_test::snd_buf_size>
        client;

    client _client;

    status _status = _client.connect(t_test::endpoint);
    if (_status != status::ok) {
      comm_log_error(logger, "erro ", _status);
      return false;
    }

    std::future<status> _future = _client.send_non_block_all(t_test::msg);

    if (!_future.valid()) {
      comm_log_error(logger, "invalid future");
      return false;
    }

    _status = _future.get();

    if (_status != status::ok) {
      comm_log_error(logger, "erro ", _status);
      return false;
    }
    return true;
  }

  static std::string desc() { return t_test::desc; }
};

template <typename t_test> struct non_blocking__timeout_control_t {
  bool operator()() {
    typedef logger::cerr::log logger;
    typedef typename t_test::connection connection;
    typedef communication::status status;
    typedef communication::client_t<logger, connection, t_test::rec_buf_size,
                                    t_test::snd_buf_size>
        client;

    client _client;

    status _status = _client.connect(t_test::endpoint);
    if (_status != status::ok) {
      comm_log_error(logger, "erro ", _status);
      return false;
    }

    std::future<status> _future =
        _client.send_non_block_all(t_test::msg, t_test::timeout);

    if (!_future.valid()) {
      comm_log_error(logger, "invalid future");
      return false;
    }

    _status = _future.get();

    if (t_test::is_timeout) {
      if (_status != status::error_timeout) {
        comm_log_error(logger, "erro ", _status);
        return false;
      }
    } else if (_status != status::ok) {
      comm_log_error(logger, "erro ", _status);
      return false;
    }
    return true;
  }

  static std::string desc() {
    return "no blocking, msg greater than io buffer, with timeout control, "
           "but "
           "no timeout";
  }
};

template <typename t_test> struct async__no_timeout_control_t {

  bool operator()() {
    typedef logger::cerr::log logger;
    typedef typename t_test::connection connection;
    typedef communication::status status;
    typedef communication::client_t<logger, connection, t_test::rec_buf_size,
                                    t_test::snd_buf_size>
        client;

    client _client;

    status _status = _client.connect(t_test::endpoint);
    if (_status != status::ok) {
      comm_log_error(logger, "erro ", _status);
      return false;
    }

    _client.send_async_all(t_test::msg);

    return true;
  }

  static std::string desc() { return t_test::desc; }
};

template <typename t_test> struct async__timeout_control_t;

} // namespace tst
} // namespace communication
} // namespace tenacitas

#endif // CLIENT_TEST_H
