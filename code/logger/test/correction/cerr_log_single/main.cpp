/// \example
#include <iostream>

#include <concurrent/business/sleeping_loop.h>
#include <logger/business/cerr.h>
#include <tester/business/run.h>

using namespace tenacitas;

typedef concurrent::business::sleeping_loop_t<void, logger::business::log>
  sleeping_loop;

struct cerr_log_single
{
    bool operator()()
    {
        using namespace tenacitas::logger::business;
        try {

            configure_cerr_log();

            std::function<bool(void)> _work = []() {
                log::debug(
                  "cerr_log_single", __LINE__, "work! ", time(nullptr));
                for (uint32_t _i = 0; _i < 100; ++_i) {
                    log::debug("cerr_log_single", __LINE__, "ola! ", _i);
                    log::debug("cerr_log_single", __LINE__, "como vai? ", _i);
                    log::info("cerr_log_single", __LINE__, "vou bem!! ", _i);
                    log::info("cerr_log_single", __LINE__, "e vc? ", _i);
                    log::warn(
                      "cerr_log_single", __LINE__, "ótimo! novo emprego! ", _i);
                    log::warn("cerr_log_single", __LINE__, "que bom! ", _i);
                }
                return true;
            };
            sleeping_loop _loop1(std::chrono::milliseconds(10000),
                                 std::move(_work),
                                 std::chrono::milliseconds(1000));

            _loop1.run();

            log::debug("cerr_log_single", __LINE__, "---- sleeping");
            std::this_thread::sleep_for(std::chrono::minutes(1));
            log::debug("cerr_log_single", __LINE__, "---- waking up");

            return true;
        } catch (std::exception& _ex) {
            log::fatal("ERRO log::log_single: '", _ex.what(), "'");
        }
        return false;
    }
};

int
main(int argc, char** argv)
{
    run_test(cerr_log_single, argc, argv, "Single thread logging to 'cerr'");
}
