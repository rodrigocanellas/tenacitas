
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

struct send_msg_greater_than_io_buffer_no_block {

  bool operator()() {
    typedef logger::cerr::log logger;
    typedef communication::tst::file_connection_takes_2_secs connection;
    typedef communication::status status;
    typedef communication::client_t<logger, connection, 15> client;

    client _client;

    const std::string _file_name(
        "./send_msg_greater_than_io_buffer_no_block.txt");
    status _status = _client.connect(_file_name);
    if (_status != status::ok) {
      comm_log_error(logger, "erro ", _status);
      return false;
    }

    std::future<status> _future = _client.send_all_no_block(
        std::string("012345678901234567890123456789012345678901234"
                    "5678901234567890123456789"),
        std::chrono::seconds(30));
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

struct send_msg_greater_than_io_buffer_block {

  bool operator()() {
    typedef logger::cerr::log logger;
    typedef communication::tst::file_connection_takes_2_secs connection;
    typedef communication::status status;
    typedef communication::client_t<logger, connection, 15> client;

    client _client;

    const std::string _file_name("./send_msg_greater_than_io_buffer_block.txt");
    status _status = _client.connect(_file_name);
    if (_status != status::ok) {
      comm_log_error(logger, "erro ", _status);
      return false;
    }

    _status = _client.send_all_block(
        std::string("012345678901234567890123456789012345678901234"
                    "5678901234567890123456789"));

    if (_status != status::ok) {
      comm_log_error(logger, "erro ", _status);
      return false;
    }

    return true;
  }

  static std::string desc() {
    return "'client_t' with a connection to a text file, using "
           "'send_all_block' to write a message greather than the 'connection' "
           "write buffer";
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
    return "'client_t' with a connection to a text file, using "
           "'send_some_block' to write a message smaller than write buffer to "
           "the file, but greater than the read buffer, and trying to read all "
           "the message";
  }
};

struct send_some_async {
  bool operator()() {
    typedef logger::cerr::log logger;
    typedef communication::tst::file_connection_takes_2_secs connection;
    typedef communication::status status;

    comm_log_debug(logger, "starting 'send_some_async'");

    const std::string _file_name{"send_some_async.txt"};

    const std::string _msg(
        "0123456789-0123456789-0123456789-0123456789-0123456789-0123456789");
    {

      typedef communication::client_t<logger, connection, 22> client;
      client _client;

      status _status = _client.connect(_file_name);
      if (_status != status::ok) {
        comm_log_error(logger, "erro ", _status);
        return false;
      }

      _client.send_some_async(_msg.begin(), _msg.end());

      comm_log_debug(logger, "msg sent = '", _msg, "'");
    }

    comm_log_debug(logger, "sleeping...");
    std::this_thread::sleep_for(std::chrono::seconds(4));
    comm_log_debug(logger, "woke up...");

    {
      typedef communication::client_t<logger, connection> client;
      client _client;

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
    return "'client_t' with a connection to a text file, using "
           "'send_block_async' to write a message. Main thread will sleep for "
           "2 seconds, and another 'client_t' will be created to receive the "
           "data from the file.";
  }
};

int main(int argc, char **argv) {
  tenacitas::logger::cerr::log::set_debug();
  tester::test _test(argc, argv);
  run_test(_test, send_msg_smaller_than_io_buffer);
  run_test(_test, send_without_timeout);
  run_test(_test, send_with_timeout);
  run_test(_test, send_msg_greater_than_io_buffer_no_block);
  run_test(_test, receive_all);
  run_test(_test, send_some_async);
  run_test(_test, send_msg_greater_than_io_buffer_block);
}
