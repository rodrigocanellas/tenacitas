
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

struct blocking__greater_io__no_timeout_control {

  bool operator()() {
    typedef logger::cerr::log logger;
    typedef communication::tst::file_connection_takes_2_secs connection;
    typedef communication::status status;
    typedef communication::client_t<logger, connection, 10> client;

    client _client;

    const std::string _file_name(
        "blocking__greater_io__no_timeout_control.txt");
    status _status = _client.connect(_file_name);
    if (_status != status::ok) {
      comm_log_error(logger, "erro ", _status);
      return false;
    }

    _status = _client.send_all(std::string("0123456789-0123456789-"));
    if (_status != status::ok) {
      comm_log_error(logger, "erro ", _status);
      return false;
    }

    return true;
  }

  static std::string desc() {
    return "blocking, msg greather than io buffer, no timeout control";
  }
};

struct blocking__greater_io__timeout_control__no_timeout {

  bool operator()() {
    typedef logger::cerr::log logger;
    typedef communication::tst::file_connection_takes_2_secs connection;
    typedef communication::status status;
    typedef communication::client_t<logger, connection, 10> client;

    client _client;

    const std::string _file_name(
        "blocking__greater_io__timeout_control__no_timeout.txt");
    status _status = _client.connect(_file_name);
    if (_status != status::ok) {
      comm_log_error(logger, "erro ", _status);
      return false;
    }

    _status = _client.send_all(std::string("0123456789-0123456789-"),
                               std::chrono::seconds(50));
    if (_status != status::ok) {
      comm_log_error(logger, "erro ", _status);
      return false;
    }

    return true;
  }

  static std::string desc() {
    return "blocking, msg greather than io buffer, with timeout control and "
           "no timeout";
  }
};

struct blocking__greater_io__timeout_control__with_timeout {

  bool operator()() {
    typedef logger::cerr::log logger;
    typedef communication::tst::file_connection_takes_2_secs connection;
    typedef communication::status status;
    typedef communication::client_t<logger, connection, 5> client;

    client _client;

    const std::string _file_name(
        "blocking__greater_io__timeout_control__with_timeout.txt");
    status _status = _client.connect(_file_name);
    if (_status != status::ok) {
      comm_log_error(logger, "erro ", _status);
      return false;
    }

    _status = _client.send_all(std::string("0123456789-0123456789-"),
                               std::chrono::seconds(5));
    if (_status != status::error_timeout) {
      comm_log_error(logger, "erro ", _status);
      return false;
    }

    return true;
  }

  static std::string desc() {
    return "blocking, msg greather than io buffer, with timeout control and "
           "with timeout";
  }
};

struct blocking__smaller_io__no_timeout_control {

  bool operator()() {
    typedef logger::cerr::log logger;
    typedef communication::tst::file_connection_takes_2_secs connection;
    typedef communication::status status;
    typedef communication::client_t<logger, connection, 200> client;

    client _client;

    const std::string _file_name(
        "blocking__smaller_io__no_timeout_control.txt");
    status _status = _client.connect(_file_name);
    if (_status != status::ok) {
      comm_log_error(logger, "erro ", _status);
      return false;
    }

    _status = _client.send_all(std::string("0123456789-0123456789-"));
    if (_status != status::ok) {
      comm_log_error(logger, "erro ", _status);
      return false;
    }

    return true;
  }

  static std::string desc() {
    return "blocking, msg smaller than io buffer, no timeout control";
  }
};

struct blocking__smaller_io__timeout_control__no_timeout {

  bool operator()() {
    typedef logger::cerr::log logger;
    typedef communication::tst::file_connection_takes_2_secs connection;
    typedef communication::status status;
    typedef communication::client_t<logger, connection, 200> client;

    client _client;

    const std::string _file_name(
        "blocking__smaller_io__timeout_control__no_timeout.txt");
    status _status = _client.connect(_file_name);
    if (_status != status::ok) {
      comm_log_error(logger, "erro ", _status);
      return false;
    }

    _status = _client.send_all(std::string("0123456789-0123456789-"),
                               std::chrono::seconds(5));
    if (_status != status::ok) {
      comm_log_error(logger, "erro ", _status);
      return false;
    }

    return true;
  }

  static std::string desc() {
    return "blocking, msg smaller than io buffer, with timeout control, but no "
           "timeout";
  }
};

struct non_blocking__greater_io__no_timeout_control {
  bool operator()() {
    typedef logger::cerr::log logger;
    typedef communication::tst::file_connection_takes_2_secs connection;
    typedef communication::status status;
    typedef communication::client_t<logger, connection, 10> client;

    client _client;

    const std::string _file_name(
        "non_blocking__greater_io__no_timeout_control.txt");
    status _status = _client.connect(_file_name);
    if (_status != status::ok) {
      comm_log_error(logger, "erro ", _status);
      return false;
    }

    //    std::string _msg("ABCDE-ABCDE-ABCDE-ABCDE-ABCDE-");
    //    std::future<status> _future = _client.post_all(_msg);

    std::future<status> _future =
        _client.post_all(std::string("ABCDE-ABCDE-ABCDE-ABCDE-ABCDE-"));

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

  static std::string desc() {
    return "no blocking, msg greater than io buffer, no timeout control";
  }
};

struct non_blocking__greater_io__timeout_control__no_timeout {
  bool operator()() {
    typedef logger::cerr::log logger;
    typedef communication::tst::file_connection_takes_2_secs connection;
    typedef communication::status status;
    typedef communication::client_t<logger, connection, 10> client;

    client _client;

    const std::string _file_name(
        "non_blocking__greater_io__timeout_control__no_timeout.txt");
    status _status = _client.connect(_file_name);
    if (_status != status::ok) {
      comm_log_error(logger, "erro ", _status);
      return false;
    }

    //    std::string _msg("ABCDE-ABCDE-ABCDE-ABCDE-ABCDE-");
    //    std::future<status> _future = _client.post_all(_msg);

    std::future<status> _future =
        _client.post_all(std::string("ABCDE-ABCDE-ABCDE-ABCDE-ABCDE-"),
                         std::chrono::seconds(25));

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

  static std::string desc() {
    return "no blocking, msg greater than io buffer, with timeout control, but "
           "no timeout";
  }
};

struct non_blocking__greater_io__timeout_control__with_timeout {
  bool operator()() {
    typedef logger::cerr::log logger;
    typedef communication::tst::file_connection_takes_2_secs connection;
    typedef communication::status status;
    typedef communication::client_t<logger, connection, 10> client;

    client _client;

    const std::string _file_name(
        "non_blocking__greater_io__timeout_control__no_timeout.txt");
    status _status = _client.connect(_file_name);
    if (_status != status::ok) {
      comm_log_error(logger, "erro ", _status);
      return false;
    }

    //    std::string _msg("ABCDE-ABCDE-ABCDE-ABCDE-ABCDE-");
    //    std::future<status> _future = _client.post_all(_msg);

    std::future<status> _future = _client.post_all(
        std::string("ABCDE-ABCDE-ABCDE-ABCDE-ABCDE-"), std::chrono::seconds(4));

    if (!_future.valid()) {
      comm_log_error(logger, "invalid future");
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
    return "no blocking, msg greater than io buffer, with timeout control, but "
           "no timeout";
  }
};

struct non_blocking__smaller_io__no_timeout_control {
  bool operator()() {
    typedef logger::cerr::log logger;
    typedef communication::tst::file_connection_takes_2_secs connection;
    typedef communication::status status;
    typedef communication::client_t<logger, connection, 100> client;

    client _client;

    const std::string _file_name(
        "non_blocking__smaller_io__no_timeout_control.txt");
    status _status = _client.connect(_file_name);
    if (_status != status::ok) {
      comm_log_error(logger, "erro ", _status);
      return false;
    }

    std::future<status> _future =
        _client.post_all(std::string("ABCDE-ABCDE-ABCDE-ABCDE-ABCDE-"));

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

  static std::string desc() {
    return "no blocking, msg smaller than io buffer, without timeout control";
  }
};

struct non_blocking__smaller_io__timeout_control__no_timeout {
  bool operator()() {
    typedef logger::cerr::log logger;
    typedef communication::tst::file_connection_takes_2_secs connection;
    typedef communication::status status;
    typedef communication::client_t<logger, connection, 100> client;

    client _client;

    const std::string _file_name(
        "non_blocking__smaller_io__no_timeout_control.txt");
    status _status = _client.connect(_file_name);
    if (_status != status::ok) {
      comm_log_error(logger, "erro ", _status);
      return false;
    }

    std::future<status> _future = _client.post_all(
        std::string("ABCDE-ABCDE-ABCDE-ABCDE-ABCDE-"), std::chrono::seconds(1));

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

  static std::string desc() {
    return "no blocking, msg smaller than io buffer, with timeout control, but "
           "no timeout";
  }
};

int main(int argc, char **argv) {
  tenacitas::logger::cerr::log::set_debug();
  tester::test _test(argc, argv);
  run_test(_test, blocking__greater_io__no_timeout_control);
  run_test(_test, blocking__greater_io__timeout_control__no_timeout);
  run_test(_test, blocking__greater_io__timeout_control__with_timeout);
  run_test(_test, blocking__smaller_io__no_timeout_control);
  run_test(_test, blocking__smaller_io__timeout_control__no_timeout);
  run_test(_test, non_blocking__greater_io__no_timeout_control);
  run_test(_test, non_blocking__greater_io__timeout_control__no_timeout);
  run_test(_test, non_blocking__greater_io__timeout_control__with_timeout);
  run_test(_test, non_blocking__smaller_io__no_timeout_control);
  run_test(_test, non_blocking__smaller_io__timeout_control__no_timeout);
}
