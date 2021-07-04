
#include <tenacitas.lib/logger.h>
#include <tenacitas.lib/remote.h>
#include <tenacitas.lib/tester.h>

using namespace tenacitas;

struct test {
    static std::string desc() { return ""; }
    bool operator()() {

        try {
            remote::client<remote::protocol::TCP> _client;

            if (_client.connect("localhost", 15678)) {
                INF("connected to localhost:15678");
                return true;
            }
            ERR("fail to connect to localhost:15678");

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
