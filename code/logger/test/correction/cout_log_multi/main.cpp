/// \example
#include <iostream>

#include <business/concurrent/traits.h>
#include <business/concurrent/sleeping_loop.h>
#include <business/logger/cerr.h>
#include <business/tester/run.h>

using namespace tenacitas::business;

typedef concurrent::sleeping_loop_t<void, logger::log> sleeping_loop;

class cerr_log_multi
{

public:
  cerr_log_multi() = default;

  bool operator()()
  {
    try {
      logger::configure_cerr_log();
      logger::log::set_debug();

      sleeping_loop _loop1(
            std::chrono::milliseconds(1000),
            []() -> concurrent::result {
              for (uint32_t _i = 0; _i < 1000; ++_i) {
                logger::log::debug("cerr_log_multi", __LINE__, "ola! ", 33);
                logger::log::debug("cerr_log_multi", __LINE__, "como vai? ", _i);
                logger::log::info("cerr_log_multi", __LINE__, "vou bem!! ", _i);
                logger::log::info("cerr_log_multi", __LINE__, "e vc? ", _i);
                logger::log::warn("cerr_log_multi",
                __LINE__,
                "ótimo! novo emprego! ",
                _i);
                logger::log::warn("cerr_log_multi", __LINE__, "que bom! ", _i);
              }
              return concurrent::result::dont_stop;
            },
            std::chrono::milliseconds(1000));

      sleeping_loop _loop2(
            std::chrono::milliseconds(500),
            []() -> concurrent::result {
              for (uint32_t _i = 0; _i < 1500; ++_i) {
                logger::log::debug("cerr_log_multi", __LINE__, "aaa! ", 33);
                logger::log::debug("cerr_log_multi", __LINE__, "bbb? ", _i);
                logger::log::info("cerr_log_multi", __LINE__, "ccc!! ", _i);
                logger::log::info("cerr_log_multi", __LINE__, "ddd ", _i);
                logger::log::warn("cerr_log_multi", __LINE__, "eee! ", _i);
                logger::log::warn("cerr_log_multi", __LINE__, "fff! ", _i);
              }
              return concurrent::result::dont_stop;
            },
            std::chrono::milliseconds(1000));

      sleeping_loop _loop3(
            std::chrono::milliseconds(100),
            []() -> concurrent::result{
              for (uint32_t _i = 0; _i < 3000; ++_i) {
                logger::log::debug("cerr_log_multi",
                __LINE__,
                "abcdefghijklmnopqrstivwxyz! ",
                33);
                logger::log::debug("cerr_log_multi",
                __LINE__,
                "abcdefghijklmnopqrstivwxyz? ",
                _i);
                logger::log::info("cerr_log_multi",
                __LINE__,
                "abcdefghijklmnopqrstivwxyz!! ",
                _i);
                logger::log::info("cerr_log_multi",
                __LINE__,
                "abcdefghijklmnopqrstivwxyz ",
                _i);
                logger::log::warn("cerr_log_multi",
                __LINE__,
                "abcdefghijklmnopqrstivwxyz! ",
                _i);
                logger::log::warn("cerr_log_multi",
                __LINE__,
                "abcdefghijklmnopqrstivwxyz! ",
                _i);
              }
              return concurrent::result::dont_stop;
            },
            std::chrono::milliseconds(1000));

      _loop1.run();
      _loop2.run();
      _loop3.run();

      logger::log::debug("cerr_log_multi", __LINE__, "---- sleeping");
      std::this_thread::sleep_for(std::chrono::seconds(50));
      logger::log::debug("cerr_log_multi", __LINE__, "---- waking up");

      return true;
    } catch (std::exception& _ex) {
      std::cerr << "ERRO log::log_multi: '" << _ex.what() << "'"
                << std::endl;
    }
    return false;
  }
};

int
main(int argc, char** argv)
{
  run_test(cerr_log_multi, argc, argv, "Multiple threads logging to 'cout'");
}
