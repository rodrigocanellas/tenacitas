
#include <string>

#include <tenacitas.lib/async.h>
#include <tenacitas.lib/logger.h>
#include <tenacitas.lib/tester.h>

using namespace tenacitas;

struct reader_000 {
    static std::string desc() {
        return "";
    }

    bool operator()() {
        return true;
    }
};


int main(int argc, char** argv) {
    logger::set_trace_level();
    tester::test _test(argc, argv);

    run_test(_test, reader_000);
}
