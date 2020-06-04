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

/// \brief
///
/// \tparam t_test must define
/// connection
/// rec_buf_size
/// snd_buf_size
/// is_blocking
/// is_non_blocking
/// is_async
/// snd_msg
/// is_timeout_control
/// timeout
/// is_timeout
/// endpoint
template <typename t_test> struct send_t {
  typedef t_test test;
  typedef logger::cerr::log logger;
  typedef typename test::connection connection;
  typedef communication::status status;
  typedef communication::client_t<logger, connection, test::rec_buf_size,
                                  test::snd_buf_size>
      client;

  bool operator()() {

    status _status = m_client.connect(test::endpoint);
    if (_status != status::ok) {
      comm_log_error(logger, "erro ", _status);
      return false;
    }

    if (test::is_blocking) {
      return blocking();
    }

    if (test::is_non_blocking) {
      return non_blocking();
    }

    if (test::is_async) {
      return async();
    }
    return false;
  }

  static std::string desc() { return t_test::desc; }

private:
  bool blocking() {
    if (test::is_non_blocking) {
      comm_log_error(logger,
                     "teste defined as 'is_blocking' and as 'is_non_blocking'");
      return false;
    }
    if (test::is_async) {
      comm_log_error(logger,
                     "teste defined as 'is_blocking' and as 'is_async'");
      return false;
    }

    comm_log_debug(logger, "non bloking");

    status _status = status::ok;
    if (test::is_timeout_control) {
      comm_log_debug(logger, "with timeout control");
      _status = m_client.send_block_all(test::snd_msg, test::timeout);
      if (test::is_timeout) {
        comm_log_debug(logger, "with timeout");
        if (_status != status::error_timeout) {
          comm_log_error(logger, "erro ", _status);
          return false;
        }
      } else {
        comm_log_debug(logger, "with no timeout");
        if (_status != status::ok) {
          comm_log_error(logger, "erro ", _status);
          return false;
        }
      }
    } else {
      comm_log_debug(logger, "with no timeout control");
      _status = m_client.send_block_all(test::snd_msg);
      if (_status != status::ok) {
        comm_log_error(logger, "erro ", _status);
        return false;
      }
    }
    return true;
  }

  bool non_blocking() {

    if (test::is_blocking) {
      comm_log_error(logger,
                     "teste defined as 'is_non_blocking' and as 'is_blocking'");
      return false;
    }
    if (test::is_async) {
      comm_log_error(logger,
                     "teste defined as 'is_non_blocking' and as 'is_async'");
      return false;
    }

    comm_log_debug(logger, "non bloking");

    status _status = status::ok;
    if (test::is_timeout_control) {
      comm_log_debug(logger, "with timeout control");
      std::future<status> _future =
          m_client.send_non_block_all(test::snd_msg, test::timeout);

      if (!_future.valid()) {
        comm_log_error(logger, "invalid future");
        return false;
      }

      _status = _future.get();
      if (test::is_timeout) {
        comm_log_debug(logger, "with timeout");
        if (_status != status::error_timeout) {
          comm_log_error(logger, "erro ", _status);
          return false;
        }
      } else {
        comm_log_debug(logger, "with no timeout");
        if (_status != status::ok) {

          comm_log_error(logger, "erro ", _status);
          return false;
        }
      }
    } else {
      comm_log_debug(logger, "no timeout control");
      std::future<status> _future = m_client.send_non_block_all(test::snd_msg);

      if (!_future.valid()) {
        comm_log_error(logger, "invalid future");
        return false;
      }

      _status = _future.get();

      if (_status != status::ok) {
        comm_log_error(logger, "erro ", _status);
        return false;
      }
    }
    return true;
  }

  bool async() {
    if (test::is_blocking) {
      comm_log_error(logger,
                     "teste defined as 'is_async' and as 'is_blocking'");
      return false;
    }
    if (test::is_non_blocking) {
      comm_log_error(logger,
                     "teste defined as 'is_async' and as 'is_non_blocking'");
      return false;
    }

    comm_log_debug(logger, "async");

    if (test::is_timeout_control) {
      comm_log_debug(logger, "with timeout control");
      m_client.send_async_all(test::snd_msg, test::timeout);
    } else {
      comm_log_debug(logger, "with no timeout control");
      m_client.send_async_all(test::snd_msg);
    }

    return true;
  }

private:
  client m_client;
};

// template <typename t_test> struct send_blocking__no_timeout_control_t {
//  typedef t_test test;
//  bool operator()() {
//    typedef logger::cerr::log logger;
//    typedef typename t_test::connection connection;
//    typedef communication::status status;
//    typedef communication::client_t<logger, connection, t_test::rec_buf_size,
//                                    t_test::snd_buf_size>
//        client;

//    client _client;

//    status _status = _client.connect(t_test::endpoint);
//    if (_status != status::ok) {
//      comm_log_error(logger, "erro ", _status);
//      return false;
//    }

//    _status = _client.send_block_all(t_test::snd_msg);
//    if (_status != status::ok) {
//      comm_log_error(logger, "erro ", _status);
//      return false;
//    }

//    return true;
//  }

//  static std::string desc() { return t_test::desc; }
//};

// template <typename t_test> struct send_blocking__timeout_control_t {
//  typedef t_test test;
//  bool operator()() {
//    typedef logger::cerr::log logger;
//    typedef typename t_test::connection connection;
//    typedef communication::status status;
//    typedef communication::client_t<logger, connection, t_test::rec_buf_size,
//                                    t_test::snd_buf_size>
//        client;

//    client _client;

//    status _status = _client.connect(t_test::endpoint);
//    if (_status != status::ok) {
//      comm_log_error(logger, "erro ", _status);
//      return false;
//    }

//    _status = _client.send_block_all(t_test::snd_msg, t_test::timeout);

//    if (t_test::is_timeout) {
//      if (_status != status::error_timeout) {
//        comm_log_error(logger, "erro ", _status);
//        return false;
//      }
//    } else if (_status != status::ok) {
//      comm_log_error(logger, "erro ", _status);
//      return false;
//    }

//    return true;
//  }

//  static std::string desc() { return t_test::desc; }
//};

// template <typename t_test> struct send_non_blocking__no_timeout_control_t {
//  typedef t_test test;
//  bool operator()() {
//    typedef logger::cerr::log logger;
//    typedef typename t_test::connection connection;
//    typedef communication::status status;
//    typedef communication::client_t<logger, connection, t_test::rec_buf_size,
//                                    t_test::snd_buf_size>
//        client;

//    client _client;

//    status _status = _client.connect(t_test::endpoint);
//    if (_status != status::ok) {
//      comm_log_error(logger, "erro ", _status);
//      return false;
//    }

//    std::future<status> _future = _client.send_non_block_all(t_test::snd_msg);

//    if (!_future.valid()) {
//      comm_log_error(logger, "invalid future");
//      return false;
//    }

//    _status = _future.get();

//    if (_status != status::ok) {
//      comm_log_error(logger, "erro ", _status);
//      return false;
//    }
//    return true;
//  }

//  static std::string desc() { return t_test::desc; }
//};

// template <typename t_test> struct send_non_blocking__timeout_control_t {
//  typedef t_test test;
//  bool operator()() {
//    typedef logger::cerr::log logger;
//    typedef typename t_test::connection connection;
//    typedef communication::status status;
//    typedef communication::client_t<logger, connection, t_test::rec_buf_size,
//                                    t_test::snd_buf_size>
//        client;

//    client _client;

//    status _status = _client.connect(t_test::endpoint);
//    if (_status != status::ok) {
//      comm_log_error(logger, "erro ", _status);
//      return false;
//    }

//    std::future<status> _future =
//        _client.send_non_block_all(t_test::snd_msg, t_test::timeout);

//    if (!_future.valid()) {
//      comm_log_error(logger, "invalid future");
//      return false;
//    }

//    _status = _future.get();

//    if (t_test::is_timeout) {
//      if (_status != status::error_timeout) {
//        comm_log_error(logger, "erro ", _status);
//        return false;
//      }
//    } else if (_status != status::ok) {
//      comm_log_error(logger, "erro ", _status);
//      return false;
//    }
//    return true;
//  }

//  static std::string desc() {
//    return "no blocking, msg greater than io buffer, with timeout control, "
//           "but "
//           "no timeout";
//  }
//};

// template <typename t_test> struct send_async__no_timeout_t {
//  typedef t_test test;
//  bool operator()() {
//    typedef logger::cerr::log logger;
//    typedef typename t_test::connection connection;
//    typedef communication::status status;
//    typedef communication::client_t<logger, connection, t_test::rec_buf_size,
//                                    t_test::snd_buf_size>
//        client;

//    client _client;

//    status _status = _client.connect(t_test::endpoint);
//    if (_status != status::ok) {
//      comm_log_error(logger, "erro ", _status);
//      return false;
//    }

//    _client.send_async_all(t_test::snd_msg);

//    return true;
//  }

//  static std::string desc() { return t_test::desc; }
//};

// template <typename t_test> struct send_async__timeout_t {
//  typedef t_test test;
//  bool operator()() {
//    typedef logger::cerr::log logger;
//    typedef typename t_test::connection connection;
//    typedef communication::status status;
//    typedef communication::client_t<logger, connection, t_test::rec_buf_size,
//                                    t_test::snd_buf_size>
//        client;

//    client _client;

//    status _status = _client.connect(t_test::endpoint);
//    if (_status != status::ok) {
//      comm_log_error(logger, "erro ", _status);
//      return false;
//    }

//    _client.send_async_all(t_test::snd_msg, t_test::timeout);

//    return true;
//  }

//  static std::string desc() { return t_test::desc; }
//};

// template <typename t_test> struct receive_blocking__all__no_timeout_control_t
// {
//  bool operator()() {
//    typedef logger::cerr::log logger;
//    typedef typename t_test::connection connection;
//    typedef communication::status status;
//    typedef communication::client_t<logger, connection, t_test::rec_buf_size,
//                                    t_test::snd_buf_size>
//        client;

//    client _client;

//    if (t_test::send_first) {
//      send_blocking__no_timeout_control_t<t_test> _send;
//      if (!_send()) {
//        return false;
//      }
//    }

//    status _status = _client.connect(t_test::endpoint);
//    if (_status != status::ok) {
//      comm_log_error(logger, "erro ", _status);
//      return false;
//    }

//    std::string _all;
//    _status = _client.receive_block_all(_all);

//    if (_status != status::ok) {
//      comm_log_error(logger, "status = ", _status);
//      return false;
//    }

//    if (_all != t_test::rec_msg) {
//      comm_log_error(logger, "msg received should be '", t_test::rec_msg,
//                     "', but it was '", _all, "'");
//      return false;
//    }

//    comm_log_debug(logger, "msg received = '", _all, "'");
//    return true;
//  }

//  static std::string desc() { return t_test::desc; }
//};

// template <typename t_test> struct receive_blocking__all__timeout_control_t {
//  bool operator()() {
//    typedef logger::cerr::log logger;
//    typedef typename t_test::connection connection;
//    typedef communication::status status;
//    typedef communication::client_t<logger, connection, t_test::rec_buf_size,
//                                    t_test::snd_buf_size>
//        client;

//    client _client;

//    if (t_test::send_first) {
//      send_blocking__no_timeout_control_t<t_test> _send;
//      if (!_send()) {
//        return false;
//      }
//    }

//    status _status = _client.connect(t_test::endpoint);
//    if (_status != status::ok) {
//      comm_log_error(logger, "erro ", _status);
//      return false;
//    }

//    std::string _all;
//    _status = _client.receive_block_all(_all, t_test::timeout);

//    if (t_test::is_timeout) {
//      if (_status != status::error_timeout) {
//        comm_log_error(logger, "timeout expected, but it was not");
//        return false;
//      }
//      comm_log_debug(logger, "timeout, as expected");
//      return true;
//    }

//    if (_status != status::ok) {
//      comm_log_error(logger, "status = ", _status);
//      return false;
//    }

//    if (_all != t_test::rec_msg) {
//      comm_log_error(logger, "msg received should be '", t_test::rec_msg,
//                     "', but it was '", _all, "'");
//      return false;
//    }

//    comm_log_debug(logger, "msg received = '", _all, "'");
//    return true;
//  }

//  static std::string desc() { return t_test::desc; }
//};

// template <typename t_test>
// struct receive_blocking__handler__no_timeout_control_t {
//  bool operator()() {
//    typedef logger::cerr::log logger;
//    typedef typename t_test::connection connection;
//    typedef communication::status status;
//    typedef communication::client_t<logger, connection, t_test::rec_buf_size,
//                                    t_test::snd_buf_size>
//        client;

//    client _client;

//    if (t_test::send_first) {
//      send_blocking__no_timeout_control_t<t_test> _send;
//      if (!_send()) {
//        return false;
//      }
//    }

//    status _status = _client.connect(t_test::endpoint);
//    if (_status != status::ok) {
//      comm_log_error(logger, "erro ", _status);
//      return false;
//    }

//    std::string _all;
//    auto _handler = [&_all](const char *p_begin, const char &p_end) -> status
//    {
//      std::string _aux(p_begin, p_end);
//      comm_log_debug(logger, _aux);
//      _all += _aux;
//    };

//    _status = _client.receive_block(_handler);

//    if (_status != status::ok) {
//      comm_log_error(logger, "status = ", _status);
//      return false;
//    }

//    if (_all != t_test::rec_msg) {
//      comm_log_error(logger, "msg received should be '", t_test::rec_msg,
//                     "', but it was '", _all, "'");
//      return false;
//    }

//    comm_log_debug(logger, "msg received = '", _all, "'");
//    return true;
//  }

//  static std::string desc() { return t_test::desc; }
//};

// template <typename t_test> struct
// receive_blocking__handler__timeout_control_t;

// template <typename t_test>
// struct receive_non_blocking__all__no_timeout_control_t;

// template <typename t_test> struct
// receive_non_blocking__all__timeout_control_t;

// template <typename t_test>
// struct receive_non_blocking__handler__no_timeout_control_t;

// template <typename t_test>
// struct receive_non_blocking__handler__timeout_control_t;

// template <typename t_test> struct receive_async__all_t;

// template <typename t_test> struct receive_async__all__timeout_t;

// template <typename t_test> struct receive_async__handler_t;

// template <typename t_test> struct receive_async__handler__timeout_t;

} // namespace tst
} // namespace communication
} // namespace tenacitas

#endif // CLIENT_TEST_H
