
#include <tenacitas.lib/logger.h>
#include <tenacitas.lib/remote.h>
#include <tenacitas.lib/tester.h>

using namespace tenacitas;

struct test {
    static std::string desc() { return ""; }

    bool operator()() {

        try {
            remote::server<remote::protocol::TCP> _server;
            INF("server created");
            _server.start_sync(15678);
            return true;
        } catch (std::exception &_ex) {
            FAT(_ex.what());
        }
        return false;
    }
};

int main(int argc, char **argv) {
    logger::set_trace_level();
    tester::test _test(argc, argv);
    run_test(_test, test);
}
