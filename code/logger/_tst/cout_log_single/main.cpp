
#include <iostream>

#include <concurrent/_bus/sleeping_loop.h>
#include <concurrent/_bus/traits.h>
#include <logger/cout/log.h>
#include <tester/_bus/test.h>

using namespace tenacitas;

typedef concurrent::_bus::sleeping_loop_t<void, logger::cout::log> sleeping_loop;

struct cout_log_single {

  bool operator()() {
    try {

      sleeping_loop _loop1(
          std::chrono::milliseconds(1000),
          []() {
            logger::cout::log::debug("cout_log_single", __LINE__,
                                     "================= work! ", time(nullptr));
            for (uint32_t _i = 0; _i < 5; ++_i) {
              logger::cout::log::debug("cout_log_single", __LINE__, "ola! ",
                                       _i);
              logger::cout::log::debug("cout_log_single", __LINE__,
                                       "como vai? ", _i);
              logger::cout::log::info("cout_log_single", __LINE__, "vou bem!! ",
                                      _i);
              logger::cout::log::info("cout_log_single", __LINE__, "e vc? ",
                                      _i);
              logger::cout::log::warn("cout_log_single", __LINE__,
                                      "ótimo! novo emprego! ", _i);
              logger::cout::log::warn("cout_log_single", __LINE__, "que bom! ",
                                      _i);
            }
            return concurrent::_bus::work_status::dont_stop;
          },
          std::chrono::milliseconds(1000));

      _loop1.run();

      logger::cout::log::debug("cout_log_single", __LINE__, "---- sleeping");
      std::this_thread::sleep_for(std::chrono::minutes(1));
      logger::cout::log::debug("cout_log_single", __LINE__, "---- waking up");

      return true;
    } catch (std::exception &_ex) {
      std::cerr << "ERRO logger::cout::log::logger::cout::log_single: '"
                << _ex.what() << "'" << std::endl;
    }
    return false;
  }

  static std::string desc() {
    return "Single thread logger::cout::logging to 'cout'";
  }
  static std::string name() { return "cout_log_single"; }
};

int main(int argc, char **argv) {
  logger::cout::log::set_debug();
  tester::_bus::test::run<cout_log_single>(argc, argv);
}
