
#include <iostream>

#include <concurrent/business/sleeping_loop.h>
#include <logger/business/cerr.h>
#include <tester/business/run.h>

typedef tenacitas::concurrent::business::sleeping_loop<void> sleeping_loop_t;

struct cerr_log_single
{
    bool operator()()
    {
        try {

            tenacitas::logger::business::configure_cerr_log();

            sleeping_loop_t _loop1(std::chrono::milliseconds(10000),
                                   []() {
                                       cerr_debug("work! ", time(nullptr));
                                       for (uint32_t _i = 0; _i < 100; ++_i) {
                                           cerr_debug("ola! ", _i);
                                           cerr_debug("como vai? ", _i);
                                           cerr_info("vou bem!! ", _i);
                                           cerr_info("e vc? ", _i);
                                           cerr_warn("ótimo! novo emprego! ",
                                                     _i);
                                           cerr_warn("que bom! ", _i);
                                       }
                                       return true;
                                   },
                                   std::chrono::milliseconds(1000));

            _loop1.run();

            cerr_debug("---- sleeping");
            std::this_thread::sleep_for(std::chrono::minutes(1));
            cerr_debug("---- waking up");

            return true;
        } catch (std::exception& _ex) {
            cerr_fatal("ERRO cerr_log_single: '", _ex.what(), "'");
        }
        return false;
    }
};

int
main(int argc, char** argv)
{
    run_test(cerr_log_single, argc, argv, "Single thread logging to 'cerr'");
}
