
#include <chrono>
#include <cstdint>
#include <future>
#include <iostream>
#include <string>

#include <communication/client.h>
#include <communication/client_test.h>
#include <communication/file_connection.h>
#include <communication/internal/log.h>
#include <communication/status.h>
#include <logger/cerr/log.h>
#include <tester/test.h>

#define REC_BUF_SIZE 8192
#define SND_BUF_SIZE 8192

using namespace tenacitas;
/// \brief namespace of the organization
namespace tenacitas {
/// \brief namespace of the project
namespace communication {
/// \brief namespace for test
namespace tst {

struct blocking__greater_io__no_timeout_control_test {
  typedef communication::tst::file_connection_takes_2_secs connection;
  static const int16_t rec_buf_size = 8192;
  static const int16_t snd_buf_size = 10;
  static const std::string endpoint;
  static const std::string msg;
  static const std::string desc;
};

const std::string blocking__greater_io__no_timeout_control_test::msg =
    "0123456789-0123456789-";

const std::string blocking__greater_io__no_timeout_control_test::endpoint =
    "blocking__greater_io__no_timeout_control.txt";

const std::string blocking__greater_io__no_timeout_control_test::desc =
    "blocking, msg greather than io buffer, no timeout control";

typedef blocking__no_timeout_control_t<
    blocking__greater_io__no_timeout_control_test>
    blocking__greater_io__no_timeout_control;

// *****************************************************************************
struct blocking__greater_io__timeout_control__no_timeout_test {
  typedef communication::tst::file_connection_takes_2_secs connection;
  static const int16_t rec_buf_size = 8192;
  static const int16_t snd_buf_size = 10;
  static constexpr std::chrono::seconds timeout = std::chrono::seconds(50);
  static const bool is_timeout = false;
  static const std::string desc;
  static const std::string endpoint;
  static const std::string msg;
};

const std::string blocking__greater_io__timeout_control__no_timeout_test::msg =
    "0123456789-0123456789-";

const std::string
    blocking__greater_io__timeout_control__no_timeout_test::endpoint =
        "blocking__greater_io__timeout_control__no_timeout";

const std::string blocking__greater_io__timeout_control__no_timeout_test::desc =
    "blocking, msg greather than io buffer, with timeout control and "
    "with timeout";

typedef blocking__timeout_control_t<
    blocking__greater_io__timeout_control__no_timeout_test>
    blocking__greater_io__timeout_control__no_timeout;

// *****************************************************************************
struct blocking__greater_io__timeout_control__with_timeout_test {
  typedef communication::tst::file_connection_takes_2_secs connection;
  static const int16_t rec_buf_size = 8192;
  static const int16_t snd_buf_size = 5;
  static constexpr std::chrono::seconds timeout = std::chrono::seconds(5);
  static const bool is_timeout = true;
  static const std::string endpoint;
  static const std::string desc;
  static const std::string msg;
};

const std::string
    blocking__greater_io__timeout_control__with_timeout_test::msg =
        "0123456789-0123456789-";

const std::string
    blocking__greater_io__timeout_control__with_timeout_test::endpoint =
        "blocking__greater_io__timeout_control__with_timeout.txt";

const std::string
    blocking__greater_io__timeout_control__with_timeout_test::desc =
        "blocking, msg greather than io buffer, with timeout control and "
        "with timeout";

typedef blocking__timeout_control_t<
    blocking__greater_io__timeout_control__with_timeout_test>
    blocking__greater_io__timeout_control__with_timeout;

// *****************************************************************************
struct blocking__smaller_io__no_timeout_control_test {
  typedef communication::tst::file_connection_takes_2_secs connection;
  static const int16_t rec_buf_size = 8192;
  static const int16_t snd_buf_size = 200;
  static const std::string endpoint;
  static const std::string desc;
  static const std::string msg;
};

const std::string blocking__smaller_io__no_timeout_control_test::msg =
    "0123456789-0123456789-";

const std::string blocking__smaller_io__no_timeout_control_test::endpoint =
    "blocking__smaller_io__no_timeout_control.txt";

const std::string blocking__smaller_io__no_timeout_control_test::desc =
    "blocking, msg smaller than io buffer, no timeout control";

typedef blocking__no_timeout_control_t<
    blocking__smaller_io__no_timeout_control_test>
    blocking__smaller_io__no_timeout_control;

// *****************************************************************************
struct blocking__smaller_io__timeout_control__no_timeout_test {
  typedef communication::tst::file_connection_takes_2_secs connection;
  static const int16_t rec_buf_size = 8192;
  static const int16_t snd_buf_size = 200;
  static constexpr std::chrono::seconds timeout = std::chrono::seconds(5);
  static const bool is_timeout = false;
  static const std::string desc;
  static const std::string endpoint;
  static const std::string msg;
};

const std::string blocking__smaller_io__timeout_control__no_timeout_test ::msg =
    "0123456789-0123456789-";

const std::string
    blocking__smaller_io__timeout_control__no_timeout_test ::endpoint =
        "blocking__smaller_io__timeout_control__no_timeout.txt";

const std::string
    blocking__smaller_io__timeout_control__no_timeout_test ::desc =
        "blocking, msg smaller than io buffer, with timeout control, but no "
        "timeout";

typedef blocking__timeout_control_t<
    blocking__smaller_io__timeout_control__no_timeout_test>
    blocking__smaller_io__timeout_control__no_timeout;

// *****************************************************************************
struct non_blocking__greater_io__no_timeout_control_test {
  typedef communication::tst::file_connection_takes_2_secs connection;
  static const int16_t rec_buf_size = 8192;
  static const int16_t snd_buf_size = 10;
  static const std::string desc;
  static const std::string endpoint;
  static const std::string msg;
};

const std::string non_blocking__greater_io__no_timeout_control_test::msg =
    "0123456789-0123456789-";

const std::string non_blocking__greater_io__no_timeout_control_test::endpoint =
    "non_blocking__greater_io__no_timeout_control.txt";

const std::string non_blocking__greater_io__no_timeout_control_test::desc =
    "no blocking, msg greater than io buffer, no timeout control";

typedef non_blocking__no_timeout_control_t<
    non_blocking__greater_io__no_timeout_control_test>
    non_blocking__greater_io__no_timeout_control;

// *****************************************************************************
struct non_blocking__greater_io__timeout_control__no_timeout_test {
  typedef communication::tst::file_connection_takes_2_secs connection;
  static const int16_t rec_buf_size = 8192;
  static const int16_t snd_buf_size = 10;
  static constexpr std::chrono::seconds timeout = std::chrono::seconds(25);
  static const bool is_timeout = false;
  static const std::string desc;
  static const std::string endpoint;
  static const std::string msg;
};

const std::string
    non_blocking__greater_io__timeout_control__no_timeout_test::msg =
        "0123456789-0123456789-";

const std::string
    non_blocking__greater_io__timeout_control__no_timeout_test::endpoint =
        "non_blocking__greater_io__timeout_control__no_timeout.txt";

const std::string
    non_blocking__greater_io__timeout_control__no_timeout_test::desc =
        "no blocking, msg greater than io buffer, with timeout control, but no "
        "timeout";

typedef non_blocking__timeout_control_t<
    non_blocking__greater_io__timeout_control__no_timeout_test>
    non_blocking__greater_io__timeout_control__no_timeout;

// *****************************************************************************
struct non_blocking__greater_io__timeout_control__with_timeout_test {
  typedef communication::tst::file_connection_takes_2_secs connection;
  static const int16_t rec_buf_size = 8192;
  static const int16_t snd_buf_size = 10;
  static constexpr std::chrono::seconds timeout = std::chrono::seconds(4);
  static const bool is_timeout = true;
  static const std::string desc;
  static const std::string endpoint;
  static const std::string msg;
};

const std::string
    non_blocking__greater_io__timeout_control__with_timeout_test::msg =
        "0123456789-0123456789-";

const std::string
    non_blocking__greater_io__timeout_control__with_timeout_test::endpoint =
        "non_blocking__greater_io__timeout_control__with_timeout.txt";

const std::string
    non_blocking__greater_io__timeout_control__with_timeout_test::desc =
        "no blocking, msg greater than io buffer, with timeout control, but no "
        "timeout";

typedef non_blocking__timeout_control_t<
    non_blocking__greater_io__timeout_control__with_timeout_test>
    non_blocking__greater_io__timeout_control__with_timeout;

// *****************************************************************************
struct non_blocking__smaller_io__no_timeout_control_test {
  typedef communication::tst::file_connection_takes_2_secs connection;
  static const int16_t rec_buf_size = 8192;
  static const int16_t snd_buf_size = 100;
  static const std::string desc;
  static const std::string endpoint;
  static const std::string msg;
};

const std::string non_blocking__smaller_io__no_timeout_control_test::msg =
    "0123456789-0123456789-";

const std::string non_blocking__smaller_io__no_timeout_control_test::endpoint =
    "non_blocking__smaller_io__no_timeout_control.txt";

const std::string non_blocking__smaller_io__no_timeout_control_test::desc =
    "no blocking, msg smaller than io buffer, without timeout control";

typedef non_blocking__no_timeout_control_t<
    non_blocking__smaller_io__no_timeout_control_test>
    non_blocking__smaller_io__no_timeout_control;

// *****************************************************************************
struct non_blocking__smaller_io__timeout_control__no_timeout_test {
  typedef communication::tst::file_connection_takes_2_secs connection;
  static const int16_t rec_buf_size = 8192;
  static const int16_t snd_buf_size = 100;
  static constexpr std::chrono::seconds timeout = std::chrono::seconds(1);
  static const bool is_timeout = false;
  static const std::string desc;
  static const std::string endpoint;
  static const std::string msg;
};

const std::string
    non_blocking__smaller_io__timeout_control__no_timeout_test::msg =
        "0123456789-0123456789-";

const std::string
    non_blocking__smaller_io__timeout_control__no_timeout_test::endpoint =
        "non_blocking__smaller_io__timeout_control__no_timeout.txt";

const std::string
    non_blocking__smaller_io__timeout_control__no_timeout_test::desc =
        "no blocking, msg smaller than io buffer, with timeout control, but no "
        "timeout";

typedef non_blocking__timeout_control_t<
    non_blocking__smaller_io__timeout_control__no_timeout_test>
    non_blocking__smaller_io__timeout_control__no_timeout;

} // namespace tst
} // namespace communication
} // namespace tenacitas

int main(int argc, char **argv) {

  using namespace tenacitas::communication::tst;

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
