
#include <iostream>

#include <concurrent/business/sleeping_loop.h>
#include <logger/business/cout.h>
#include <tester/business/run.h>

typedef tenacitas::concurrent::business::sleeping_loop<void> sleeping_loop_t;

struct cout_log_single
{

    bool operator()()
    {
        try {

            tenacitas::logger::business::configure_cout_log();

            sleeping_loop_t _loop1(std::chrono::milliseconds(10000),
                                   []() {
                                       cout_debug("work! ", time(nullptr));
                                       for (uint32_t _i = 0; _i < 100; ++_i) {
                                           cout_debug("ola! ", _i);
                                           cout_debug("como vai? ", _i);
                                           cout_info("vou bem!! ", _i);
                                           cout_info("e vc? ", _i);
                                           cout_warn("ótimo! novo emprego! ",
                                                     _i);
                                           cout_warn("que bom! ", _i);
                                       }
                                       return true;
                                   },
                                   std::chrono::milliseconds(1000));

            _loop1.run();

            cout_debug("---- sleeping");
            std::this_thread::sleep_for(std::chrono::minutes(4));
            cout_debug("---- waking up");

            return true;
        } catch (std::exception& _ex) {
            std::cerr << "ERRO cout_log_single: '" << _ex.what() << "'"
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
