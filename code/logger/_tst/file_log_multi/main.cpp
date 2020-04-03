
#include <iostream>

#include <concurrent/_bus/sleeping_loop.h>
#include <concurrent/_bus/traits.h>
#include <logger/file/log.h>
#include <tester/_bus/test.h>

using namespace tenacitas;

typedef concurrent::_bus::sleeping_loop_t<void, logger::file::log> sleeping_loop;

class file_log_multi {

public:
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
            return concurrent::_bus::work_status::dont_stop;
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
            return concurrent::_bus::work_status::dont_stop;
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
            return concurrent::_bus::work_status::dont_stop;
          },
          std::chrono::milliseconds(1000));

      _loop1.run();
      _loop2.run();
      _loop3.run();

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
  tester::_bus::test::run<file_log_multi>(argc, argv);
}
