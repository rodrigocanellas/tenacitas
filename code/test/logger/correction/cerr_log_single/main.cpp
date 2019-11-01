/// \example
#include <iostream>

#include <business/concurrent/traits.h>
#include <business/concurrent/sleeping_loop.h>
#include <business/logger/cerr.h>
#include <business/tester/run.h>

using namespace tenacitas::business;

typedef concurrent::sleeping_loop_t<void, logger::log> sleeping_loop;

struct cerr_log_single
{
  bool operator()()
  {

    try {

      logger::configure_cerr_log();

      std::function<concurrent::result(void)> _work = []() -> concurrent::result {
          logger::log::debug(
            "cerr_log_single", __LINE__, "work! ", time(nullptr));
          for (uint32_t _i = 0; _i < 100; ++_i) {
        logger::log::debug("cerr_log_single", __LINE__, "ola! ", _i);
        logger::log::debug("cerr_log_single", __LINE__, "como vai? ", _i);
        logger::log::info("cerr_log_single", __LINE__, "vou bem!! ", _i);
        logger::log::info("cerr_log_single", __LINE__, "e vc? ", _i);
        logger::log::warn(
              "cerr_log_single", __LINE__, "ótimo! novo emprego! ", _i);
        logger::log::warn("cerr_log_single", __LINE__, "que bom! ", _i);
      }
      return concurrent::result::dont_stop;
    };
    sleeping_loop _loop1(std::chrono::milliseconds(10000),
                         std::move(_work),
                         std::chrono::milliseconds(1000));

    _loop1.run();

    logger::log::debug("cerr_log_single", __LINE__, "---- sleeping");
    std::this_thread::sleep_for(std::chrono::minutes(1));
    logger::log::debug("cerr_log_single", __LINE__, "---- waking up");

    return true;
  } catch (std::exception& _ex) {
    logger::log::fatal("ERRO logger::log::log_single: '", _ex.what(), "'");
  }
  return false;
}
};

int
main(int argc, char** argv)
{
  run_test(cerr_log_single, argc, argv, "Single thread logging to 'cerr'");
}
