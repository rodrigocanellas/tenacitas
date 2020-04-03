/// \example
#include <iostream>

#include <concurrent/_bus/sleeping_loop.h>
#include <concurrent/_bus/traits.h>
#include <logger/file/log.h>
#include <tester/_bus/test.h>

using namespace tenacitas;

typedef concurrent::_bus::sleeping_loop_t<void, tenacitas::logger::file::log>
    sleeping_loop;

class file_log_single {

public:
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
            return concurrent::_bus::work_status::dont_stop;
          },
          std::chrono::milliseconds(1000));

      _loop1.run();

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

int main(int argc, char **argv) {
  logger::file::log::set_debug();
  tester::_bus::test::run<file_log_single>(argc, argv);
}
