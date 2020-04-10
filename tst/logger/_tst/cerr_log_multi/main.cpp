/// \example
#include <iostream>

#include <concurrent/sleeping_loop.h>
#include <concurrent/traits.h>
#include <logger/cerr/log.h>
#include <tester/test.h>

using namespace tenacitas;

typedef concurrent::_bus::sleeping_loop_t<void, logger::cerr::log> sleeping_loop;

class cerr_log_multi {

public:
  cerr_log_multi() = default;

  bool operator()() {
    try {
      logger::cerr::log::set_debug();

      sleeping_loop _loop1(
          std::chrono::milliseconds(1000),
          []() {
            for (uint32_t _i = 0; _i < 1000; ++_i) {
              logger::cerr::log::debug("cerr_log_multi", __LINE__, "ola! ", 33);
              logger::cerr::log::debug("cerr_log_multi", __LINE__, "como vai? ",
                                       _i);
              logger::cerr::log::info("cerr_log_multi", __LINE__, "vou bem!! ",
                                      _i);
              logger::cerr::log::info("cerr_log_multi", __LINE__, "e vc? ", _i);
              logger::cerr::log::warn("cerr_log_multi", __LINE__,
                                      "ótimo! novo emprego! ", _i);
              logger::cerr::log::warn("cerr_log_multi", __LINE__, "que bom! ",
                                      _i);
            }
            return concurrent::_bus::work_status::dont_stop;
          },
          std::chrono::milliseconds(1000));

      sleeping_loop _loop2(
          std::chrono::milliseconds(500),
          []() {
            for (uint32_t _i = 0; _i < 1500; ++_i) {
              logger::cerr::log::debug("cerr_log_multi", __LINE__, "aaa! ", 33);
              logger::cerr::log::debug("cerr_log_multi", __LINE__, "bbb? ", _i);
              logger::cerr::log::info("cerr_log_multi", __LINE__, "ccc!! ", _i);
              logger::cerr::log::info("cerr_log_multi", __LINE__, "ddd ", _i);
              logger::cerr::log::warn("cerr_log_multi", __LINE__, "eee! ", _i);
              logger::cerr::log::warn("cerr_log_multi", __LINE__, "fff! ", _i);
            }
            return concurrent::_bus::work_status::dont_stop;
          },
          std::chrono::milliseconds(1000));

      sleeping_loop _loop3(
          std::chrono::milliseconds(100),
          []() {
            for (uint32_t _i = 0; _i < 3000; ++_i) {
              logger::cerr::log::debug("cerr_log_multi", __LINE__,
                                       "abcdefghijklmnopqrstivwxyz! ", 33);
              logger::cerr::log::debug("cerr_log_multi", __LINE__,
                                       "abcdefghijklmnopqrstivwxyz? ", _i);
              logger::cerr::log::info("cerr_log_multi", __LINE__,
                                      "abcdefghijklmnopqrstivwxyz!! ", _i);
              logger::cerr::log::info("cerr_log_multi", __LINE__,
                                      "abcdefghijklmnopqrstivwxyz ", _i);
              logger::cerr::log::warn("cerr_log_multi", __LINE__,
                                      "abcdefghijklmnopqrstivwxyz! ", _i);
              logger::cerr::log::warn("cerr_log_multi", __LINE__,
                                      "abcdefghijklmnopqrstivwxyz! ", _i);
            }
            return concurrent::_bus::work_status::dont_stop;
          },
          std::chrono::milliseconds(1000));

      _loop1.run();
      _loop2.run();
      _loop3.run();

      logger::cerr::log::debug("cerr_log_multi", __LINE__, "---- sleeping");
      std::this_thread::sleep_for(std::chrono::seconds(50));
      logger::cerr::log::debug("cerr_log_multi", __LINE__, "---- waking up");

      return true;
    } catch (std::exception &_ex) {
      std::cerr << "ERRO logger::cerr::log::log_multi: '" << _ex.what() << "'"
                << std::endl;
    }
    return false;
  }

  static std::string desc() { return "Multiple threads logging to 'cerr'"; }
  static std::string name() { return "cerr_log_multi"; }
};

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();
  tester::_bus::test::run<cerr_log_multi>(argc, argv);
}
