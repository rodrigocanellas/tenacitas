#include <iostream>
#include <sstream>
#include <string>

#include <concurrent/sleeping_loop.h>
#include <concurrent/traits.h>
#include <logger/file/log.h>
#include <tester/test.h>

using namespace tenacitas;

class file_log_creation {

public:
  explicit file_log_creation() = default;

  bool operator()() {

    try {
      logger::file::log::configure(".", "file_log_creation", 100,
                                   std::chrono::minutes(10));

      logger::file::log::set_debug();

      return true;
    } catch (std::exception &_ex) {
      logger::file::log::fatal("file_log_creation", __LINE__,
                               "ERRO file_log_creation: '", _ex.what(), "'");
    }
    return false;
  }
  static std::string desc() { return "'file_log' creation"; }
};

struct file_log_how_to {
  bool operator()() {

    try {
      logger::file::log::configure(".", "how_to", 10 * 1024,
                                   std::chrono::minutes(15));

      logger::file::log::set_debug();

      logger::file::log::debug("file_log_how_to", __LINE__, "hello! ", 309);
      logger::file::log::debug("file_log_how_to", __LINE__,
                               "how are you doing? ", 3.14);
      logger::file::log::info("file_log_how_to", __LINE__, "fine!! ", 'W');
      logger::file::log::info("file_log_how_to", __LINE__, "and you?");
      logger::file::log::warn("file_log_how_to", __LINE__,
                              "great! got a new job!! ", 6987.58f);
      logger::file::log::warn("file_log_how_to", __LINE__, "nice!! ", 10);
      return true;
    } catch (std::exception &_ex) {
      std::cerr << "ERRO cerr_log_creation: '" << _ex.what() << "'"
                << std::endl;
    }
    return false;
  }
  static std::string desc() { return "Simple 'file_log' usage"; }
};

class file_log_single {

public:
  typedef concurrent::sleeping_loop_t<void, tenacitas::logger::file::log>
      sleeping_loop;

  bool operator()() {
    try {
      using namespace tenacitas;

      logger::file::log::configure(".", "file_log_single", 10 * 1024,
                                   std::chrono::minutes(5));
      logger::file::log::set_debug();
      sleeping_loop _loop1(
          std::chrono::milliseconds(1000),
          []() {
            logger::file::log::debug("file_log_single", __LINE__,
                                     "================= work! ", time(nullptr));
            for (uint32_t _i = 0; _i < 5; ++_i) {
              logger::file::log::debug("file_log_single", __LINE__, "ola! ",
                                       _i);
              logger::file::log::debug("file_log_single", __LINE__,
                                       "como vai? ", _i);
              logger::file::log::info("file_log_single", __LINE__, "vou bem!! ",
                                      _i);
              logger::file::log::info("file_log_single", __LINE__, "e vc? ",
                                      _i);
              logger::file::log::warn("file_log_single", __LINE__,
                                      "ótimo! novo emprego! ", _i);
              logger::file::log::warn("file_log_single", __LINE__, "que bom! ",
                                      _i);
            }
            return status::ok;
          },
          std::chrono::milliseconds(1000));

      _loop1.start();

      logger::file::log::debug("file_log_single", __LINE__,
                               "---- sleeping 3 minutes");
      std::this_thread::sleep_for(std::chrono::minutes(3));
      logger::file::log::debug("file_log_single", __LINE__, "---- waking up");

      return true;
    } catch (std::exception &_ex) {
      logger::file::log::fatal("file_log_single", __LINE__,
                               "ERRO log::log_single: '", _ex.what(), "'");
    }
    return false;
  }

  static std::string desc() { return "Single thread logging to 'file'"; }
  static std::string name() { return "file_log_single"; }
};

class file_log_multi {

public:
  typedef concurrent::sleeping_loop_t<void, logger::file::log> sleeping_loop;
  file_log_multi() = default;

  bool operator()() {
    try {
      logger::file::log::configure("./", "file_log_multi", 1 * 1024 * 1024,
                                   std::chrono::minutes(3));
      logger::file::log::set_debug();

      sleeping_loop _loop1(
          std::chrono::milliseconds(1000),
          []() {
            for (uint32_t _i = 0; _i < 1000; ++_i) {
              logger::file::log::debug("file_log_multi", __LINE__, "ola! ", 33);
              logger::file::log::debug("file_log_multi", __LINE__, "como vai? ",
                                       _i);
              logger::file::log::info("file_log_multi", __LINE__, "vou bem!! ",
                                      _i);
              logger::file::log::info("file_log_multi", __LINE__, "e vc? ", _i);
              logger::file::log::warn("file_log_multi", __LINE__,
                                      "ótimo! novo emprego! ", _i);
              logger::file::log::warn("file_log_multi", __LINE__, "que bom! ",
                                      _i);
            }
            return status::ok;
          },
          std::chrono::milliseconds(1000));

      sleeping_loop _loop2(
          std::chrono::milliseconds(500),
          []() {
            for (uint32_t _i = 0; _i < 1500; ++_i) {
              logger::file::log::debug("file_log_multi", __LINE__, "aaa! ", 33);
              logger::file::log::debug("file_log_multi", __LINE__, "bbb? ", _i);
              logger::file::log::info("file_log_multi", __LINE__, "ccc!! ", _i);
              logger::file::log::info("file_log_multi", __LINE__, "ddd ", _i);
              logger::file::log::warn("file_log_multi", __LINE__, "eee! ", _i);
              logger::file::log::warn("file_log_multi", __LINE__, "fff! ", _i);
            }
            return status::ok;
          },
          std::chrono::milliseconds(1000));

      sleeping_loop _loop3(
          std::chrono::milliseconds(100),
          []() {
            for (uint32_t _i = 0; _i < 3000; ++_i) {
              logger::file::log::debug("file_log_multi", __LINE__,
                                       "abcdefghijklmnopqrstivwxyz! ", 33);
              logger::file::log::debug("file_log_multi", __LINE__,
                                       "abcdefghijklmnopqrstivwxyz? ", _i);
              logger::file::log::info("file_log_multi", __LINE__,
                                      "abcdefghijklmnopqrstivwxyz!! ", _i);
              logger::file::log::info("file_log_multi", __LINE__,
                                      "abcdefghijklmnopqrstivwxyz ", _i);
              logger::file::log::warn("file_log_multi", __LINE__,
                                      "abcdefghijklmnopqrstivwxyz! ", _i);
              logger::file::log::warn("file_log_multi", __LINE__,
                                      "abcdefghijklmnopqrstivwxyz! ", _i);
            }
            return status::ok;
          },
          std::chrono::milliseconds(1000));

      _loop1.start();
      _loop2.start();
      _loop3.start();

      logger::file::log::debug("file_log_multi", __LINE__, "---- sleeping");
      std::this_thread::sleep_for(std::chrono::seconds(50));
      logger::file::log::debug("file_log_multi", __LINE__, "---- waking up");

      return true;
    } catch (std::exception &_ex) {
      logger::file::log::fatal("file_log_multi", __LINE__,
                               "ERRO file_log_multi: '", _ex.what(), "'");
    }
    return false;
  }
  static std::string desc() { return "Multiple threads logging to 'file'"; }
  static std::string name() { return "file_log_multi"; }
};

int main(int argc, char **argv) {
  logger::file::log::set_debug();
  tester::test _tester(argc, argv);
  run_test(_tester, file_log_how_to);
  run_test(_tester, file_log_creation);
  run_test(_tester, file_log_single);
  run_test(_tester, file_log_multi);
}
