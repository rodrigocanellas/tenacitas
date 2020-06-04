
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
  static const int16_t snd_buf_size = 10;
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

const std::string test_000_def::desc =
    "blocking, msg greather than out buffer, no timeout control";

typedef send_t<test_000_def> test000;

// *****************************************************************************
struct test_001_def {
  typedef communication::tst::file_connection_takes_2_secs connection;
  static const int16_t rec_buf_size = 8192;
  static const int16_t snd_buf_size = 10;
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

const std::string test_001_def::desc =
    "blocking, msg greather than out buffer, timeout control, no timeout";

typedef send_t<test_001_def> test001;

// *****************************************************************************
struct test_002_def {
  typedef communication::tst::file_connection_takes_2_secs connection;
  static const int16_t rec_buf_size = 8192;
  static const int16_t snd_buf_size = 10;
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
    "blocking, msg greather than out buffer, timeout control, with timeout";

typedef send_t<test_002_def> test002;

// *****************************************************************************
struct test_003_def {
  typedef communication::tst::file_connection_takes_2_secs connection;
  static const int16_t rec_buf_size = 8192;
  static const int16_t snd_buf_size = 100;
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

const std::string test_003_def::snd_msg = "0123456789-0123456789-";

const std::string test_003_def::endpoint = "test003.txt";

const std::string test_003_def::desc =
    "blocking, msg smaller than out buffer, no timeout control";

typedef send_t<test_003_def> test003;

// *****************************************************************************
struct test_004_def {
  typedef communication::tst::file_connection_takes_2_secs connection;
  static const int16_t rec_buf_size = 8192;
  static const int16_t snd_buf_size = 100;
  static const bool is_blocking = true;
  static const bool is_non_blocking = false;
  static const bool is_async = false;
  static const std::string snd_msg;
  static const std::string endpoint;
  static const bool is_timeout_control = true;
  static constexpr std::chrono::seconds timeout = std::chrono::seconds(3);
  static const bool is_timeout = false;
  static const std::string desc;
};

const std::string test_004_def::snd_msg = "0123456789-0123456789-";

const std::string test_004_def::endpoint = "test004.txt";

const std::string test_004_def::desc =
    "blocking, msg smaller than out buffer, timeout control, no timeout";

typedef send_t<test_004_def> test004;

// *****************************************************************************
struct test_005_def {
  typedef communication::tst::file_connection_takes_2_secs connection;
  static const int16_t rec_buf_size = 8192;
  static const int16_t snd_buf_size = 10;
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

const std::string test_005_def::snd_msg = "0123456789-0123456789-";

const std::string test_005_def::endpoint = "test005.txt";

const std::string test_005_def::desc =
    "non blocking, msg greather than out buffer, no timeout control";

typedef send_t<test_005_def> test005;

// *****************************************************************************
struct test_006_def {
  typedef communication::tst::file_connection_takes_2_secs connection;
  static const int16_t rec_buf_size = 8192;
  static const int16_t snd_buf_size = 10;
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

const std::string test_006_def::snd_msg = "0123456789-0123456789-";

const std::string test_006_def::endpoint = "test006.txt";

const std::string test_006_def::desc =
    "non blocking, msg greather than out buffer, timeout control, no timeout";

typedef send_t<test_006_def> test006;

// *****************************************************************************
struct test_007_def {
  typedef communication::tst::file_connection_takes_2_secs connection;
  static const int16_t rec_buf_size = 8192;
  static const int16_t snd_buf_size = 10;
  static const bool is_blocking = false;
  static const bool is_non_blocking = true;
  static const bool is_async = false;
  static const std::string snd_msg;
  static const std::string endpoint;
  static const bool is_timeout_control = true;
  static constexpr std::chrono::seconds timeout = std::chrono::seconds(3);
  static const bool is_timeout = true;
  static const std::string desc;
};

const std::string test_007_def::snd_msg = "0123456789-0123456789-";

const std::string test_007_def::endpoint = "test007.txt";

const std::string test_007_def::desc =
    "non blocking, msg greather than out buffer, timeout control, with timeout";

typedef send_t<test_007_def> test007;

// *****************************************************************************
struct test_008_def {
  typedef communication::tst::file_connection_takes_2_secs connection;
  static const int16_t rec_buf_size = 8192;
  static const int16_t snd_buf_size = 100;
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

const std::string test_008_def::snd_msg = "0123456789-0123456789-";

const std::string test_008_def::endpoint = "test008.txt";

const std::string test_008_def::desc =
    "non blocking, msg smaller than out buffer, no timeout control";

typedef send_t<test_008_def> test008;

// *****************************************************************************
struct test_009_def {
  typedef communication::tst::file_connection_takes_2_secs connection;
  static const int16_t rec_buf_size = 8192;
  static const int16_t snd_buf_size = 100;
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

const std::string test_009_def::snd_msg = "0123456789-0123456789-";

const std::string test_009_def::endpoint = "test009.txt";

const std::string test_009_def::desc =
    "non blocking, msg smaller than out buffer, timeout control, no timeout";

typedef send_t<test_009_def> test009;

// *****************************************************************************
struct test_010_def {
  typedef communication::tst::file_connection_takes_2_secs connection;
  static const int16_t rec_buf_size = 8192;
  static const int16_t snd_buf_size = 10;
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

const std::string test_010_def::snd_msg = "0123456789-0123456789-";

const std::string test_010_def::endpoint = "test010.txt";

const std::string test_010_def::desc =
    "async, msg greather than out buffer, no timeout control";

typedef send_t<test_010_def> test010;

// *****************************************************************************
struct test_011_def {
  typedef communication::tst::file_connection_takes_2_secs connection;
  static const int16_t rec_buf_size = 8192;
  static const int16_t snd_buf_size = 10;
  static const bool is_blocking = false;
  static const bool is_non_blocking = false;
  static const bool is_async = true;
  static const std::string snd_msg;
  static const std::string endpoint;
  static const bool is_timeout_control = true;
  static constexpr std::chrono::seconds timeout = std::chrono::seconds(50);
  static const bool is_timeout = true;
  static const std::string desc;
};

const std::string test_011_def::snd_msg = "0123456789-0123456789-";

const std::string test_011_def::endpoint = "test011.txt";

const std::string test_011_def::desc =
    "async, msg greather than out buffer, timeout control, no timeout";

typedef send_t<test_011_def> test011;

// *****************************************************************************
struct test_012_def {
  typedef communication::tst::file_connection_takes_2_secs connection;
  static const int16_t rec_buf_size = 8192;
  static const int16_t snd_buf_size = 10;
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

const std::string test_012_def::snd_msg = "01223456789-01223456789-";

const std::string test_012_def::endpoint = "test012.txt";

const std::string test_012_def::desc =
    "async, msg greather than out buffer, timeout control, with timeout";

typedef send_t<test_012_def> test012;

// *****************************************************************************
struct test_013_def {
  typedef communication::tst::file_connection_takes_2_secs connection;
  static const int16_t rec_buf_size = 8192;
  static const int16_t snd_buf_size = 100;
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

const std::string test_013_def::snd_msg = "01323456789-01323456789-";

const std::string test_013_def::endpoint = "test013.txt";

const std::string test_013_def::desc =
    "async, msg smaller than out buffer, no timeout control";

typedef send_t<test_013_def> test013;

// *****************************************************************************
struct test_014_def {
  typedef communication::tst::file_connection_takes_2_secs connection;
  static const int16_t rec_buf_size = 8192;
  static const int16_t snd_buf_size = 100;
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

const std::string test_014_def::snd_msg = "01423456789-01423456789-";

const std::string test_014_def::endpoint = "test014.txt";

const std::string test_014_def::desc =
    "async, msg smaller than out buffer, timeout control, no timeout";

typedef send_t<test_014_def> test014;

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
  run_test(_test, test009);
  run_test(_test, test010);
  run_test(_test, test011);
  run_test(_test, test012);
  run_test(_test, test013);
  run_test(_test, test014);
}
