
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

// *****************************************************************************
struct test_000_def {
  typedef communication::tst::file_connection_takes_2_secs connection;
  static const int16_t rec_buf_size = 8192;

  static const bool is_blocking = true;
  static const bool is_non_blocking = false;
  static const bool is_async = false;
  static const std::string snd_msg;
  static const std::string endpoint;
  static const bool is_timeout_control = false;
  static constexpr std::chrono::seconds timeout = std::chrono::seconds(0);
  static const bool is_timeout = false;
  static const std::string desc;
};

const std::string test_000_def::snd_msg = "0123456789-0123456789-";

const std::string test_000_def::endpoint = "test000.txt";

const std::string test_000_def::desc = "blocking, no timeout control";

typedef send_t<test_000_def> test000;

// *****************************************************************************
struct test_001_def {
  typedef communication::tst::file_connection_takes_2_secs connection;
  static const int16_t rec_buf_size = 8192;

  static const bool is_blocking = true;
  static const bool is_non_blocking = false;
  static const bool is_async = false;
  static const std::string snd_msg;
  static const std::string endpoint;
  static const bool is_timeout_control = true;
  static constexpr std::chrono::seconds timeout = std::chrono::seconds(50);
  static const bool is_timeout = false;
  static const std::string desc;
};

const std::string test_001_def::snd_msg = "0123456789-0123456789-";

const std::string test_001_def::endpoint = "test001.txt";

const std::string test_001_def::desc = "blocking, timeout control, no timeout";

typedef send_t<test_001_def> test001;

// *****************************************************************************
struct test_002_def {
  typedef communication::tst::file_connection_takes_2_secs connection;
  static const int16_t rec_buf_size = 8192;

  static const bool is_blocking = true;
  static const bool is_non_blocking = false;
  static const bool is_async = false;
  static const std::string snd_msg;
  static const std::string endpoint;
  static const bool is_timeout_control = true;
  static constexpr std::chrono::seconds timeout = std::chrono::seconds(1);
  static const bool is_timeout = true;
  static const std::string desc;
};

const std::string test_002_def::snd_msg = "0123456789-0123456789-";

const std::string test_002_def::endpoint = "test002.txt";

const std::string test_002_def::desc =
    "blocking, timeout control, with timeout";

typedef send_t<test_002_def> test002;

// *****************************************************************************
struct test_003_def {
  typedef communication::tst::file_connection_takes_2_secs connection;
  static const int16_t rec_buf_size = 8192;

  static const bool is_blocking = false;
  static const bool is_non_blocking = true;
  static const bool is_async = false;
  static const std::string snd_msg;
  static const std::string endpoint;
  static const bool is_timeout_control = false;
  static constexpr std::chrono::seconds timeout = std::chrono::seconds(0);
  static const bool is_timeout = false;
  static const std::string desc;
};

const std::string test_003_def::snd_msg = "0123456789-0123456789-";

const std::string test_003_def::endpoint = "test003.txt";

const std::string test_003_def::desc = "non blocking, no timeout control";

typedef send_t<test_003_def> test003;

// *****************************************************************************
struct test_004_def {
  typedef communication::tst::file_connection_takes_2_secs connection;
  static const int16_t rec_buf_size = 8192;

  static const bool is_blocking = false;
  static const bool is_non_blocking = true;
  static const bool is_async = false;
  static const std::string snd_msg;
  static const std::string endpoint;
  static const bool is_timeout_control = true;
  static constexpr std::chrono::seconds timeout = std::chrono::seconds(50);
  static const bool is_timeout = false;
  static const std::string desc;
};

const std::string test_004_def::snd_msg = "0123456789-0123456789-";

const std::string test_004_def::endpoint = "test004.txt";

const std::string test_004_def::desc =
    "non blocking, timeout control, no timeout";

typedef send_t<test_004_def> test004;

// *****************************************************************************
struct test_005_def {
  typedef communication::tst::file_connection_takes_2_secs connection;
  static const int16_t rec_buf_size = 8192;

  static const bool is_blocking = false;
  static const bool is_non_blocking = true;
  static const bool is_async = false;
  static const std::string snd_msg;
  static const std::string endpoint;
  static const bool is_timeout_control = true;
  static constexpr std::chrono::seconds timeout = std::chrono::seconds(1);
  static const bool is_timeout = true;
  static const std::string desc;
};

const std::string test_005_def::snd_msg = "0123456789-0123456789-";

const std::string test_005_def::endpoint = "test005.txt";

const std::string test_005_def::desc =
    "non blocking, timeout control, with timeout";

typedef send_t<test_005_def> test005;

// *****************************************************************************
struct test_006_def {
  typedef communication::tst::file_connection_takes_2_secs connection;
  static const int16_t rec_buf_size = 8192;

  static const bool is_blocking = false;
  static const bool is_non_blocking = false;
  static const bool is_async = true;
  static const std::string snd_msg;
  static const std::string endpoint;
  static const bool is_timeout_control = false;
  static constexpr std::chrono::seconds timeout = std::chrono::seconds(0);
  static const bool is_timeout = false;
  static const std::string desc;
};

const std::string test_006_def::snd_msg = "0123456789-0123456789-";

const std::string test_006_def::endpoint = "test006.txt";

const std::string test_006_def::desc = "async, no timeout control";

typedef send_t<test_006_def> test006;

// *****************************************************************************
struct test_007_def {
  typedef communication::tst::file_connection_takes_2_secs connection;
  static const int16_t rec_buf_size = 8192;

  static const bool is_blocking = false;
  static const bool is_non_blocking = false;
  static const bool is_async = true;
  static const std::string snd_msg;
  static const std::string endpoint;
  static const bool is_timeout_control = true;
  static constexpr std::chrono::seconds timeout = std::chrono::seconds(50);
  static const bool is_timeout = false;
  static const std::string desc;
};

const std::string test_007_def::snd_msg = "0123456789-0123456789-";

const std::string test_007_def::endpoint = "test007.txt";

const std::string test_007_def::desc =
    "async, timeout control, no timeout, but it is impossible to detect "
    "timeout, only log";

typedef send_t<test_007_def> test007;

// *****************************************************************************
struct test_008_def {
  typedef communication::tst::file_connection_takes_2_secs connection;
  static const int16_t rec_buf_size = 8192;

  static const bool is_blocking = false;
  static const bool is_non_blocking = false;
  static const bool is_async = true;
  static const std::string snd_msg;
  static const std::string endpoint;
  static const bool is_timeout_control = true;
  static constexpr std::chrono::seconds timeout = std::chrono::seconds(3);
  static const bool is_timeout = true;
  static const std::string desc;
};

const std::string test_008_def::snd_msg = "00823456789-00823456789-";

const std::string test_008_def::endpoint = "test008.txt";

const std::string test_008_def::desc =
    "async, timeout control, with timeout, but it is impossible to detect "
    "timeout, only log";

typedef send_t<test_008_def> test008;

// *****************************************************************************
struct test_015_def {
  typedef communication::tst::file_connection_takes_2_secs connection;
  typedef test_003_def send_def;
  static const bool send_first = true;
  static const int16_t rec_buf_size = 100;

  static const bool is_blocking = true;
  static const bool is_non_blocking = false;
  static const bool is_async = false;
  static const std::string rcv_msg;
  static const std::string endpoint;
  static const bool is_timeout_control = false;
  static constexpr std::chrono::microseconds timeout = std::chrono::seconds(3);
  static const bool is_timeout = false;
  static const bool all = true;
  static const std::string desc;
};

const std::string test_015_def::rcv_msg = send_def::snd_msg;

const std::string test_015_def::endpoint = send_def::endpoint;

const std::string test_015_def::desc =
    "send: " + test_015_def::send_def::desc +
    "; receive: blocking, in buffer greather than msg, read all the message, "
    "no timeout control";

typedef receive_t<test_015_def> test015;

////
///*****************************************************************************
// struct test_016_def {
//  typedef communication::tst::file_connection_takes_2_secs connection;
//  typedef test_003_def send_def;
//  static const int16_t rec_buf_size = 100;
//
//  static const bool is_blocking = true;
//  static const bool is_non_blocking = false;
//  static const bool is_async = false;
//  static const std::string rcv_msg;
//  static const std::string endpoint;
//  static const bool is_timeout_control = true;
//  static constexpr std::chrono::seconds timeout = std::chrono::seconds(50);
//  static const bool is_timeout = false;
//  static const bool send_first = true;
//  static const bool all = true;
//  static const std::string desc;
//};

// const std::string test_016_def::rcv_msg = send_def::snd_msg;

// const std::string test_016_def::endpoint = send_def::endpoint;

// const std::string test_016_def::desc =
//    "send: " + test_016_def::send_def::desc +
//    "; receive: blocking, in buffer greather than msg, read all the message, "
//    "timeout control, no timeout";

// typedef receive_t<test_016_def> test016;

////
///*****************************************************************************
// struct test_017_def {
//  typedef communication::tst::file_connection_takes_2_secs connection;
//  typedef test_003_def send_def;
//  static const int16_t rec_buf_size = 100;
//
//  static const bool is_blocking = true;
//  static const bool is_non_blocking = false;
//  static const bool is_async = false;
//  static const std::string rcv_msg;
//  static const std::string endpoint;
//  static const bool is_timeout_control = true;
//  static constexpr std::chrono::seconds timeout = std::chrono::seconds();
//  static const bool is_timeout = true;
//  static const bool send_first = true;
//  static const bool all = true;
//  static const std::string desc;
//};

// const std::string test_017_def::rcv_msg = send_def::snd_msg;

// const std::string test_017_def::endpoint = send_def::endpoint;

// const std::string test_017_def::desc =
//    "send: " + test_017_def::send_def::desc +
//    "; receive: blocking, in buffer greather than msg, read all the message, "
//    "timeout control, with timeout";

// typedef receive_t<test_017_def> test017;

} // namespace tst
} // namespace communication
} // namespace tenacitas

int main(int argc, char **argv) {

  using namespace tenacitas::communication::tst;

  tenacitas::logger::cerr::log::set_debug();

  tester::test _test(argc, argv);
  run_test(_test, test000);
  run_test(_test, test001);
  run_test(_test, test002);
  run_test(_test, test003);
  run_test(_test, test004);
  run_test(_test, test005);
  run_test(_test, test006);
  run_test(_test, test007);
  run_test(_test, test008);
  //  run_test(_test, test015);
  //  run_test(_test, test016);
}
