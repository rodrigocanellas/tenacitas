
#include <iostream>

#include <business/concurrent/traits.h>
#include <business/concurrent/sleeping_loop.h>
#include <business/logger/cout.h>
#include <business/tester/run.h>


using namespace tenacitas::business;

typedef concurrent::sleeping_loop_t<void, logger::log> sleeping_loop;

struct cout_log_single
{

  bool operator()()
  {
    try {

      logger::configure_cout_log();

      sleeping_loop _loop1(
            std::chrono::milliseconds(10000),
            []() -> concurrent::result{
              logger::log::debug(
              "cout_log_single", __LINE__, "work! ", time(nullptr));
              for (uint32_t _i = 0; _i < 100; ++_i) {
                logger::log::debug("cout_log_single", __LINE__, "ola! ", _i);
                logger::log::debug("cout_log_single", __LINE__, "como vai? ", _i);
                logger::log::info("cout_log_single", __LINE__, "vou bem!! ", _i);
                logger::log::info("cout_log_single", __LINE__, "e vc? ", _i);
                logger::log::warn("cout_log_single",
                __LINE__,
                "ótimo! novo emprego! ",
                _i);
                logger::log::warn("cout_log_single", __LINE__, "que bom! ", _i);
              }
              return concurrent::result::dont_stop;
            },
            std::chrono::milliseconds(1000));

      _loop1.run();

      logger::log::debug("cout_log_single", __LINE__, "---- sleeping");
      std::this_thread::sleep_for(std::chrono::minutes(4));
      logger::log::debug("cout_log_single", __LINE__, "---- waking up");

      return true;
    } catch (std::exception& _ex) {
      std::cerr << "ERRO log::log_single: '" << _ex.what() << "'"
                << std::endl;
    }
    return false;
  }
};

int
main(int argc, char** argv)
{
  run_test(cout_log_single, argc, argv, "Single thread logging to 'cout'");
}
