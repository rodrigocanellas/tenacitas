
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

    _status = _client.send(std::string("hello!!!"));
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

struct post_without_timeout {

  bool operator()() {
    typedef logger::cerr::log logger;
    typedef communication::tst::file_connection_takes_2_secs connection;
    typedef communication::status status;
    typedef communication::client_t<logger, connection> client;

    client _client;

    const std::string _file_name("./post_without_timeout.txt");
    status _status = _client.connect(_file_name);
    if (_status != status::ok) {
      comm_log_error(logger, "erro ", _status);
      return false;
    }

    std::future<status> _future = _client.post(
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
    return "'client_t' with a connection to a text file, using 'post' to write "
           "a message smaller than write buffer to the file, without "
           "timeout";
  }
};

struct post_with_timeout {

  bool operator()() {
    typedef logger::cerr::log logger;
    typedef communication::tst::file_connection_takes_2_secs connection;
    typedef communication::status status;
    typedef communication::client_t<logger, connection> client;

    client _client;

    const std::string _file_name("./post_with_timeout.txt");
    status _status = _client.connect(_file_name);
    if (_status != status::ok) {
      comm_log_error(logger, "erro ", _status);
      return false;
    }

    std::future<status> _future = _client.post(
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
    return "'client_t' with a connection to a text file, using 'post' to write "
           "a message smaller than write buffer to the file, with timeout";
  }
};

struct post_msg_greater_than_io_buffer {

  bool operator()() {
    typedef logger::cerr::log logger;
    typedef communication::tst::file_connection_takes_2_secs connection;
    typedef communication::status status;
    typedef communication::client_t<logger, connection> client;

    client _client;

    const std::string _file_name("./post_msg_greater_than_io_buffer.txt");
    status _status = _client.connect(_file_name);
    if (_status != status::ok) {
      comm_log_error(logger, "erro ", _status);
      return false;
    }

    std::future<status> _future =
        _client.post(std::string("012345678901234567890123456789012345678901234"
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
    return "'client_t' with a connection to a text file, using 'post' to write "
           "a message greather than the 'connection' write buffer, without "
           "timeout";
  }
};

struct read_all {

  bool operator()() {

    typedef logger::cerr::log logger;
    typedef communication::tst::file_connection_takes_2_secs connection;
    typedef communication::status status;
    typedef communication::client_t<logger, connection> client;

    const std::string _msg("01234567890123456789");
    {
      client _client;

      const std::string _file_name("./read_all.txt");
      status _status = _client.connect(_file_name);
      if (_status != status::ok) {
        comm_log_error(logger, "erro ", _status);
        return false;
      }

      _status = _client.send(_msg);
      if (_status != status::ok) {
        comm_log_error(logger, "erro ", _status);
        return false;
      }
    }
    {
      client _client;

      const std::string _file_name("./read_all.txt");
      status _status = _client.connect(_file_name);
      if (_status != status::ok) {
        comm_log_error(logger, "erro ", _status);
        return false;
      }

      std::pair<status, std::string> _res = _client.receive<std::string>();
      if (_res.first != status::ok) {
        comm_log_error(logger, "erro ", _res.first, " receiveing");
        return false;
      }

      if (_res.second != _msg) {
        comm_log_error(logger, "msg received '", _res.second,
                       "' is not equal to the one sent '", _msg, "'");
        return false;
      }
    }
    return true;
  }

  static std::string desc() {
    return "'client_t' with a connection to a text file, using 'post' to write "
           "a message smaller than write buffer to the file, but greater than "
           "the read buffer, and trying to read all the message";
  }
};

int main(int argc, char **argv) {
  tenacitas::logger::cerr::log::set_debug();
  tester::test _test(argc, argv);
  run_test(_test, send_msg_smaller_than_io_buffer);
  run_test(_test, post_without_timeout);
  run_test(_test, post_with_timeout);
  run_test(_test, post_msg_greater_than_io_buffer);
  run_test(_test, read_all);
}
