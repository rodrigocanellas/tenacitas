
#include <tenacitas.lib/async.h>
#include <tenacitas.lib/logger.h>
#include <tenacitas.lib/tester.h>
#include <tenacitas.lib/type.h>

using namespace tenacitas;
using namespace std::chrono_literals;

struct executer_000 {
    static std::string desc() { return ""; }
    bool operator()() {

        auto _function = [](type::ptr<bool>, int &&p_i) -> void {
            std::cout << "i = " << p_i << '\n';
        };
        int _i {4};

        return async::execute(200ms, _function, std::move(_i));
    }
};

struct executer_001 {
    static std::string desc() { return ""; }
    bool operator()() {

        auto _function = [](type::ptr<bool> p_bool, int &&p_i) -> void {
            std::this_thread::sleep_for(1s);
            if (p_bool) {
                std::cout << "TIMEOUT\n";
            } else {
                std::cout << "i = " << p_i << '\n';
            }
        };
        int _i {4};

        return !async::execute(200ms, _function, std::move(_i));
    }
};

struct executer_002 {
    static std::string desc() { return ""; }
    bool operator()() {

        auto _function = [](type::ptr<bool>) -> void {
            std::cout << "hello!\n";
        };

        return async::execute(200ms, _function);
    }
};

struct executer_003 {
    static std::string desc() { return ""; }
    bool operator()() {

        auto _function = [](type::ptr<bool> p_bool) -> void {
            std::this_thread::sleep_for(1s);
            if (p_bool) {
                std::cout << "TIMEOUT\n";
            } else {
                std::cout << "hello\n";
            }
        };

        return !async::execute(200ms, _function);
    }
};

struct executer_004 {
    static std::string desc() { return ""; }
    bool operator()() {

        auto _function = [](type::ptr<bool>, int p_i, std::string &&p_str,
                            const char *p_char) -> void {
            std::cout << p_i << ',' << p_str << ',' << p_char << '\n';
        };

        std::string _str {"hello"};

        return async::execute(200ms, _function, 4, std::move(_str), "goodbye");
    }
};

struct executer_005 {
    static std::string desc() { return ""; }
    bool operator()() {

        auto _function = [](type::ptr<bool> p_bool, int p_i,
                            std::string &&p_str, const char *p_char) -> void {
            std::this_thread::sleep_for(1s);
            if (p_bool) {
                std::cout << "TIMEOUT\n";
            } else {
                std::cout << p_i << ',' << p_str << ',' << p_char << '\n';
            }
        };

        std::string _str {"hello"};

        return !async::execute(200ms, _function, 4, std::move(_str), "goodbye");
    }
};

int main(int argc, char **argv) {
    logger::set_info_level();
    tester::test _tester(argc, argv);

    run_test(_tester, executer_000);
    run_test(_tester, executer_001);
    run_test(_tester, executer_002);
    run_test(_tester, executer_003);
    run_test(_tester, executer_004);
    run_test(_tester, executer_005);
}
