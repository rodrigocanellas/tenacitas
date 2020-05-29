
#include <chrono>
#include <cstdint>
#include <future>
#include <iostream>
#include <string>

#include <communication/client.h>
#include <communication/file_connection.h>
#include <communication/internal/log.h>
#include <communication/status.h>
#include <logger/cerr/log.h>
#include <tester/test.h>

using namespace tenacitas;

struct send_msg_smaller_than_io_buffer {

  bool operator()() {
    typedef logger::cerr::log logger;
    typedef communication::tst::file_connection_takes_2_secs connection;
    typedef communication::status status;
    typedef communication::client_t<logger, connection> client;

    client _client;

    const std::string _file_name("send_msg_smaller_than_io_buffer.txt");
    status _status = _client.connect(_file_name);
    if (_status != status::ok) {
      comm_log_error(logger, "erro ", _status);
      return false;
    }

    _status = _client.send_all_block(std::string("hello!!!"));
    if (_status != status::ok) {
      comm_log_error(logger, "erro ", _status);
      return false;
    }

    return true;
  }

  static std::string desc() {
    return "'client_t' with a connection to a text file, using 'send' to write "
           "a message smaller than write buffer to the file";
  }
};

struct send_without_timeout {

  bool operator()() {
    typedef logger::cerr::log logger;
    typedef communication::tst::file_connection_takes_2_secs connection;
    typedef communication::status status;
    typedef communication::client_t<logger, connection> client;

    client _client;

    const std::string _file_name("./send_without_timeout.txt");
    status _status = _client.connect(_file_name);
    if (_status != status::ok) {
      comm_log_error(logger, "erro ", _status);
      return false;
    }

    std::future<status> _future = _client.send_all_no_block(
        std::string("how are you without timeout?"), std::chrono::seconds(3));
    if (!_future.valid()) {
      comm_log_error(logger, "invalid 'future'");
      return false;
    }
    _status = _future.get();
    if (_status != status::ok) {
      comm_log_error(logger, "erro ", _status);
      return false;
    }

    return true;
  }

  static std::string desc() {
    return "'client_t' with a connection to a text file, using "
           "'send_non_block' to write a message smaller than write buffer to "
           "the file, without timeout";
  }
};

struct send_with_timeout {

  bool operator()() {
    typedef logger::cerr::log logger;
    typedef communication::tst::file_connection_takes_2_secs connection;
    typedef communication::status status;
    typedef communication::client_t<logger, connection> client;

    client _client;

    const std::string _file_name("./send_with_timeout.txt");
    status _status = _client.connect(_file_name);
    if (_status != status::ok) {
      comm_log_error(logger, "erro ", _status);
      return false;
    }

    std::future<status> _future = _client.send_all_no_block(
        std::string("how are you with timeout?"), std::chrono::seconds(1));
    if (!_future.valid()) {
      comm_log_error(logger, "invalid 'future'");
      return false;
    }
    _status = _future.get();
    if (_status != status::error_timeout) {
      comm_log_error(logger, "erro ", _status);
      return false;
    }

    return true;
  }

  static std::string desc() {
    return "'client_t' with a connection to a text file, using "
           "'send_non_block' to write a message smaller than write buffer to "
           "the file, with timeout";
  }
};

struct send_msg_greater_than_io_buffer {

  bool operator()() {
    typedef logger::cerr::log logger;
    typedef communication::tst::file_connection_takes_2_secs connection;
    typedef communication::status status;
    typedef communication::client_t<logger, connection> client;

    client _client;

    const std::string _file_name("./send_msg_greater_than_io_buffer.txt");
    status _status = _client.connect(_file_name);
    if (_status != status::ok) {
      comm_log_error(logger, "erro ", _status);
      return false;
    }

    std::future<status> _future = _client.send_all_no_block(
        std::string("012345678901234567890123456789012345678901234"
                    "5678901234567890123456789"),
        std::chrono::seconds(6));
    if (!_future.valid()) {
      comm_log_error(logger, "invalid 'future'");
      return false;
    }
    _status = _future.get();
    if (_status != status::ok) {
      comm_log_error(logger, "erro ", _status);
      return false;
    }

    return true;
  }

  static std::string desc() {
    return "'client_t' with a connection to a text file, using "
           "'send_non_block' to write a message greather than the 'connection' "
           "write buffer, without "
           "timeout";
  }
};

struct receive_all {

  bool operator()() {

    typedef logger::cerr::log logger;
    typedef communication::tst::file_connection_takes_2_secs connection;
    typedef communication::status status;
    typedef communication::client_t<logger, connection> client;

    const std::string _msg("01234567890123456789");
    {
      client _client;

      const std::string _file_name("./receive_all.txt");
      status _status = _client.connect(_file_name);
      if (_status != status::ok) {
        comm_log_error(logger, "erro ", _status);
        return false;
      }

      _status = _client.send_all_block(_msg);
      if (_status != status::ok) {
        comm_log_error(logger, "erro ", _status);
        return false;
      }

      comm_log_debug(logger, "msg sent = '", _msg, "'");
    }
    {
      client _client;

      const std::string _file_name("./receive_all.txt");
      status _status = _client.connect(_file_name);
      if (_status != status::ok) {
        comm_log_error(logger, "erro ", _status);
        return false;
      }

      std::string _all;
      _status = _client.receive_all_block(_all);
      if (_status != status::ok) {
        comm_log_error(logger, "erro ", _status, " receiving");
        return false;
      }

      if (_all != _msg) {
        comm_log_error(logger, "msg received '", _all,
                       "' is not equal to the one sent '", _msg, "'");
        return false;
      }

      comm_log_debug(logger, "msg received = '", _all, "'");
    }
    return true;
  }

  static std::string desc() {
    return "'client_t' with a connection to a text file, using 'send_block' to "
           "write a message smaller than write buffer to the file, but greater "
           "than the read buffer, and trying to read all the message";
  }
};

int main(int argc, char **argv) {
  tenacitas::logger::cerr::log::set_debug();
  tester::test _test(argc, argv);
  run_test(_test, send_msg_smaller_than_io_buffer);
  run_test(_test, send_without_timeout);
  run_test(_test, send_with_timeout);
  run_test(_test, send_msg_greater_than_io_buffer);
  run_test(_test, receive_all);
}
