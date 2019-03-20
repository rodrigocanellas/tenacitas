
#include <iostream>

#include <concurrent/bus/sleeping_loop.h>
#include <logger/bus/cerr.h>
#include <tester/bus/run.h>

typedef tenacitas::concurrent::bus::sleeping_loop<void> sleeping_loop_t;

namespace tenacitas {
namespace logger {
namespace tst {
class cerr_log_single
{

  public:
    cerr_log_single() = default;

    bool operator()()
    {
        try {

            tenacitas::logger::bus::configure_cerr_log();

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
            std::this_thread::sleep_for(std::chrono::minutes(4));
            cerr_debug("---- waking up");

            return true;
        } catch (std::exception& _ex) {
            std::cerr << "ERRO cerr_log_single: '" << _ex.what() << "'"
                      << std::endl;
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
    run_test(cerr_log_single, argc, argv, "NO DESC");
}
