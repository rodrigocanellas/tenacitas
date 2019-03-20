
#include <iostream>

#include <concurrent/bus/sleeping_loop.h>
#include <logger/bus/cerr.h>
#include <logger/bus/cout.h>
#include <tester/bus/run.h>

typedef tenacitas::concurrent::bus::sleeping_loop<void> sleeping_loop_t;

namespace tenacitas {
namespace logger {
namespace tst {
class cout_log_single
{

  public:
    cout_log_single() = default;

    bool operator()()
    {
        try {

            tenacitas::logger::bus::configure_cout_log();

            sleeping_loop_t _loop1(std::chrono::milliseconds(10000),
                                   []() {
                                       cerr_debug("work! ", time(nullptr));
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

            cerr_debug("---- sleeping");
            std::this_thread::sleep_for(std::chrono::minutes(1));
            cerr_debug("---- waking up");

            return true;
        } catch (std::exception& _ex) {
            cerr_fatal("ERRO cout_log_single: '", _ex.what(), "'");
        }
        return false;
    }
};
} // namespace tst
} // namespace logger
} // namespace tenacitas

int
main(int argc, char** argv)
{
    tenacitas::logger::bus::configure_cerr_log();
    cerr_set_debug();

    using namespace tenacitas::logger::tst;
    run_test(cout_log_single, argc, argv, "NO DESC");
}
