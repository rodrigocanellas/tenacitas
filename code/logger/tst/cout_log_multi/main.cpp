/// \example
#include <iostream>

#include <concurrent/bus/sleeping_loop.h>
#include <concurrent/bus/traits.h>
#include <logger/cerr/cerr.h>
#include <tester/bus/test.h>

using namespace tenacitas::logger::business;
using namespace tenacitas::concurrent::business;

typedef sleeping_loop_t<void, log> sleeping_loop;

class cerr_log_multi
{

public:
  cerr_log_multi() = default;

  bool operator()()
  {
    try {
      configure_cerr_log();
      log::set_debug();

      sleeping_loop _loop1(
        std::chrono::milliseconds(1000),
        []() {
          for (uint32_t _i = 0; _i < 1000; ++_i) {
            log::debug("cerr_log_multi", __LINE__, "ola! ", 33);
            log::debug("cerr_log_multi", __LINE__, "como vai? ", _i);
            log::info("cerr_log_multi", __LINE__, "vou bem!! ", _i);
            log::info("cerr_log_multi", __LINE__, "e vc? ", _i);
            log::warn("cerr_log_multi", __LINE__, "ótimo! novo emprego! ", _i);
            log::warn("cerr_log_multi", __LINE__, "que bom! ", _i);
          }
          return work_status::dont_stop;
        },
        std::chrono::milliseconds(1000));

      sleeping_loop _loop2(
        std::chrono::milliseconds(500),
        []() {
          for (uint32_t _i = 0; _i < 1500; ++_i) {
            log::debug("cerr_log_multi", __LINE__, "aaa! ", 33);
            log::debug("cerr_log_multi", __LINE__, "bbb? ", _i);
            log::info("cerr_log_multi", __LINE__, "ccc!! ", _i);
            log::info("cerr_log_multi", __LINE__, "ddd ", _i);
            log::warn("cerr_log_multi", __LINE__, "eee! ", _i);
            log::warn("cerr_log_multi", __LINE__, "fff! ", _i);
          }
          return work_status::dont_stop;
        },
        std::chrono::milliseconds(1000));

      sleeping_loop _loop3(
        std::chrono::milliseconds(100),
        []() {
          for (uint32_t _i = 0; _i < 3000; ++_i) {
            log::debug(
              "cerr_log_multi", __LINE__, "abcdefghijklmnopqrstivwxyz! ", 33);
            log::debug(
              "cerr_log_multi", __LINE__, "abcdefghijklmnopqrstivwxyz? ", _i);
            log::info(
              "cerr_log_multi", __LINE__, "abcdefghijklmnopqrstivwxyz!! ", _i);
            log::info(
              "cerr_log_multi", __LINE__, "abcdefghijklmnopqrstivwxyz ", _i);
            log::warn(
              "cerr_log_multi", __LINE__, "abcdefghijklmnopqrstivwxyz! ", _i);
            log::warn(
              "cerr_log_multi", __LINE__, "abcdefghijklmnopqrstivwxyz! ", _i);
          }
          return work_status::dont_stop;
        },
        std::chrono::milliseconds(1000));

      _loop1.run();
      _loop2.run();
      _loop3.run();

      log::debug("cerr_log_multi", __LINE__, "---- sleeping");
      std::this_thread::sleep_for(std::chrono::seconds(50));
      log::debug("cerr_log_multi", __LINE__, "---- waking up");

      return true;
    } catch (std::exception& _ex) {
      std::cerr << "ERRO log::log_multi: '" << _ex.what() << "'" << std::endl;
    }
    return false;
  }
};

int
main(int argc, char** argv)
{
  run_test(cerr_log_multi, argc, argv, "Multiple threads logging to 'cout'");
}
