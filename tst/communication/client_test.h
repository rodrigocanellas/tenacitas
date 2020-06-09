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
  typedef communication::client_t<logger, connection, test::rec_buf_size>
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

    comm_log_debug(logger, "blocking");

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

    comm_log_debug(logger, "non blocking");

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

/// \brief
///
/// \tparam t_test must define
/// connection
/// rec_buf_size
/// snd_buf_size
/// is_blocking
/// is_non_blocking
/// is_async
/// is_timeout_control
/// send_first
/// all
/// timeout
/// is_timeout
/// endpoint
/// send_def send definition, like the ones used with \p send_t
/// rcv_msg
template <typename t_test> struct receive_t {
  typedef t_test test;
  typedef logger::cerr::log logger;
  typedef typename test::connection connection;
  typedef communication::status status;
  typedef communication::client_t<logger, connection, test::rec_buf_size>
      client;

  bool operator()() {
    status _status = m_client.connect(test::endpoint);
    if (_status != status::ok) {
      comm_log_error(logger, "erro ", _status);
      return false;
    }

    if (test::send_first) {
      send_t<typename test::send_def> _send;
      if (!_send()) {
        comm_log_error(logger, "error sending");
        return false;
      }
    }

    if (test::is_blocking) {
      comm_log_debug(logger, "blocking");
      return blocking();
    }

    if (test::is_non_blocking) {
      comm_log_debug(logger, "non blocking");
      return non_blocking();
    }

    if (test::is_async) {
      comm_log_debug(logger, "async");
      return async();
    }
    return true;
  }

  static std::string desc() { return test::desc; }

private:
  struct handler {
    status operator()(const char *p_begin, const char *p_end) {
      comm_log_debug(logger, std::string(p_begin, p_end));
      return status::ok;
    }
  };

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

    status _status = status::ok;

    if (test::all) {
      comm_log_debug(logger, "receive all the message");
      std::string _all;

      if (test::is_timeout_control) {
        comm_log_debug(logger, "timeout control");
        _status = m_client.receive_block_all(_all, test::timeout);

        if (test::is_timeout) {
          comm_log_debug(logger, "timeout is expected");
          if (_status == status::error_timeout) {
            comm_log_debug(logger, "timeout as expected");
            _status = status::ok;
          } else {
            comm_log_error(logger, "status = ", _status,
                           ", but timeout was expected");
            _status = status::error_receiving;
          }
        } else {
          comm_log_debug(logger, "no timeout is expected");
          if (_status == status::ok) {
            comm_log_debug(logger, "no timeout, as expected");
            if (_all != test::rcv_msg) {
              comm_log_error(logger, "msg received is '", _all, "', but '",
                             test::rcv_msg, "' was expected");
              _status = status::error_receiving;
            } else {
              comm_log_debug(logger, "msg received = '", _all, "'");
            }
          } else {
            comm_log_error(logger, "error '", _status, "'");
          }
        }
      } else {
        comm_log_debug(logger, "no timeout control");
        _status = m_client.receive_block_all(_all);
        if (_status == status::ok) {
          if (_all != test::rcv_msg) {
            comm_log_error(logger, "msg received is '", _all, "', but '",
                           test::rcv_msg, "' was expected");
            _status = status::error_receiving;
          } else {
            comm_log_debug(logger, "msg received = '", _all, "'");
          }
        } else {
          comm_log_error(logger, "error '", _status, "'");
        }
      }
    } else {
      comm_log_debug(logger, "receive with handler");

      if (test::is_timeout_control) {
        comm_log_debug(logger, "timeout control");
        _status = m_client.receive_block(m_handler, test::timeout);

        if (test::is_timeout) {
          comm_log_debug(logger, "timeout expected");
          if (_status == status::error_timeout) {
            comm_log_debug(logger, "timeout as expected");
            _status = status::ok;
          } else {
            comm_log_error(logger, "status = ", _status,
                           ", but timeout was expected");
            _status = status::error_receiving;
          }
        } else {
          comm_log_debug(logger, "no timeout expected");
          if (_status == status::ok) {
            comm_log_debug(logger, "no timeout, as expected");
          } else {
            comm_log_error(logger, "error '", _status, "'");
          }
        }
      } else {
        comm_log_debug(logger, "no timeout control");
        _status = m_client.receive_block(m_handler);
        if (_status != status::ok) {
          comm_log_error(logger, "error '", _status, "'");
        }
      }
    }

    return (_status == status::ok);
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
    status _status = status::ok;

    if (test::all) {
      comm_log_debug(logger, "receive all the message");

      std::string _all;

      if (test::is_timeout_control) {
        comm_log_debug(logger, "timeout control");

        std::future<status> _future =
            m_client.receive_non_block_all(_all, test::timeout);

        if (!_future.valid()) {
          comm_log_error(logger, "invalid future");
          _status = status::error_receiving;
        } else {
          comm_log_debug(logger, "valid future");
          _status = _future.get();
          if (test::is_timeout) {
            comm_log_debug(logger, "timeout is expected");
            if (_status == status::error_timeout) {
              comm_log_debug(logger, "timeout, as expected");
              _status = status::ok;
            } else {
              comm_log_error(logger, "timeout expected, but '", _status,
                             "' was returned");
              _status = status::error_receiving;
            }
          } else {
            comm_log_debug(logger, "no timeout is expected");
            if (_status == status::ok) {
              comm_log_debug(logger, "no timeout, as expected");
              if (_all != test::rcv_msg) {
                comm_log_error(logger, "msg received is '", _all, "', but '",
                               test::rcv_msg, "' was expected");
                _status = status::error_receiving;
              } else {
                comm_log_debug(logger, "msg received = '", _all, "'");
              }
            } else {
              comm_log_error(logger, "error '", _status, "'");
            }
          }
        }
      } else {
        comm_log_debug(logger, "no timeout control");
        std::future<status> _future = m_client.receive_non_block_all(_all);
        if (!_future.valid()) {
          comm_log_error(logger, "invalid future");
          _status = status::error_receiving;
        } else {
          comm_log_debug(logger, "valid future");
          _status = _future.get();

          if (_status == status::ok) {
            if (_all != test::rcv_msg) {
              comm_log_error(logger, "msg received is '", _all, "', but '",
                             test::rcv_msg, "' was expected");
              _status = status::error_receiving;
            } else {
              comm_log_debug(logger, "msg received = '", _all, "'");
            }
          } else {
            comm_log_error(logger, "error '", _status, "'");
          }
        }
      }

    } else {
      comm_log_debug(logger, "receive the message in parts");

      if (test::is_timeout_control) {
        comm_log_debug(logger, "timeout control");

        std::future<status> _future =
            m_client.receive_non_block(m_handler, test::timeout);

        if (!_future.valid()) {
          comm_log_error(logger, "invalid future");
          _status = status::error_receiving;
        } else {
          comm_log_debug(logger, "valid future");
          _status = _future.get();

          if (test::is_timeout) {
            comm_log_debug(logger, "timeout is expected");
            if (_status == status::error_timeout) {
              comm_log_debug(logger, "timeout, as expected");
              _status = status::ok;
            } else {
              comm_log_error(logger, "timeout was expected, but '", _status,
                             "' was returned");
              _status = status::error_receiving;
            }
          } else {
            comm_log_debug(logger, "timeout is not expected");
            if (_status == status::ok) {
              comm_log_debug(logger, "no timeout, as expected");
            } else {
              comm_log_error(logger, "error '", _status, "'");
            }
          }
        }
      } else {
        comm_log_debug(logger, "no timeout control");

        std::future<status> _future = m_client.receive_non_block(m_handler);

        if (!_future.valid()) {
          comm_log_error(logger, "invalid future");
          _status = status::error_receiving;
        } else {
          comm_log_debug(logger, "valid future");
          _status = _future.get();
          if (_status != status::ok) {
            comm_log_error(logger, "error '", _status, "'");
          }
        }
      }
    }

    return (_status == status::ok);
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

    //    if (test::all) {
    //      comm_log_debug(logger, "receive all the message");
    //      std::string _all;

    //      if (test::is_timeout_control) {
    //        comm_log_debug(logger, "timeout control");
    //        m_client.receive_async_all(_all, test::timeout);
    //      }
    //      else {
    //        comm_log_debug(logger, "no timeout control");
    //        m_client.receive_async_all(_all);
    //      }
    //    }
    //    else {
    comm_log_debug(logger, "receive msg in parts");
    if (test::is_timeout_control) {
      comm_log_debug(logger, "timeout control");
      m_client.receive_async(m_handler, test::timeout);
    } else {
      comm_log_debug(logger, "no timeout control");
      m_client.receive_async(m_handler);
    }

    //    }
    return true;
  }

private:
  client m_client;
  handler m_handler;
};

} // namespace tst
} // namespace communication
} // namespace tenacitas

#endif // CLIENT_TEST_H
