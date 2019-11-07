
#include <iostream>

#include <concurrent/business/traits.h>
#include <concurrent/business/sleeping_loop.h>
#include <logger/business/cout.h>
#include <tester/business/run.h>

using namespace tenacitas::logger::business;
using namespace tenacitas::concurrent::business;

typedef sleeping_loop_t<void, log> sleeping_loop;

struct cout_log_single
{

    bool operator()()
    {
        try {

            configure_cout_log();

            sleeping_loop _loop1(
              std::chrono::milliseconds(10000),
              []() {
                  log::debug(
                    "cout_log_single", __LINE__, "work! ", time(nullptr));
                  for (uint32_t _i = 0; _i < 100; ++_i) {
                      log::debug("cout_log_single", __LINE__, "ola! ", _i);
                      log::debug("cout_log_single", __LINE__, "como vai? ", _i);
                      log::info("cout_log_single", __LINE__, "vou bem!! ", _i);
                      log::info("cout_log_single", __LINE__, "e vc? ", _i);
                      log::warn("cout_log_single",
                                __LINE__,
                                "ótimo! novo emprego! ",
                                _i);
                      log::warn("cout_log_single", __LINE__, "que bom! ", _i);
                  }
                  return work_status::dont_stop;
              },
              std::chrono::milliseconds(1000));

            _loop1.run();

            log::debug("cout_log_single", __LINE__, "---- sleeping");
            std::this_thread::sleep_for(std::chrono::minutes(4));
            log::debug("cout_log_single", __LINE__, "---- waking up");

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
