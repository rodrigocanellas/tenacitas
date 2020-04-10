/// \example
#include <iostream>

#include <concurrent/sleeping_loop.h>
#include <concurrent/traits.h>
#include <logger/cerr/log.h>
#include <tester/test.h>

using namespace tenacitas;

typedef concurrent::_bus::sleeping_loop_t<void, logger::cerr::log> sleeping_loop;

struct cerr_log_single {
  bool operator()() {
    try {

      sleeping_loop _loop1(
          std::chrono::milliseconds(1000),
          []() {
            logger::cerr::log::debug("cerr_log_single", __LINE__,
                                     "================= work! ", time(nullptr));
            for (uint32_t _i = 0; _i < 5; ++_i) {
              logger::cerr::log::debug("cerr_log_single", __LINE__, "ola! ",
                                       _i);
              logger::cerr::log::debug("cerr_log_single", __LINE__,
                                       "como vai? ", _i);
              logger::cerr::log::info("cerr_log_single", __LINE__, "vou bem!! ",
                                      _i);
              logger::cerr::log::info("cerr_log_single", __LINE__, "e vc? ",
                                      _i);
              logger::cerr::log::warn("cerr_log_single", __LINE__,
                                      "ótimo! novo emprego! ", _i);
              logger::cerr::log::warn("cerr_log_single", __LINE__, "que bom! ",
                                      _i);
            }
            return concurrent::_bus::work_status::dont_stop;
          },
          std::chrono::milliseconds(1000));

      _loop1.run();

      logger::cerr::log::debug("cerr_log_single", __LINE__, "---- sleeping");
      std::this_thread::sleep_for(std::chrono::minutes(1));
      logger::cerr::log::debug("cerr_log_single", __LINE__, "---- waking up");

      return true;
    } catch (std::exception &_ex) {
      logger::cerr::log::fatal("ERRO logger::cerr::log::log_single: '",
                               _ex.what(), "'");
    }
    return false;
  }

  static std::string desc() { return "Single thread logging to 'cerr'"; }
  static std::string name() { return "cerr_log_single"; }
};

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();
  tester::_bus::test::run<cerr_log_single>(argc, argv);
}
