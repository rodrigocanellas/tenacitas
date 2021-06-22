
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
            TRA("i = ", p_i);
        };
        int _i {4};

        return async::execute(200ms, _function, std::move(_i));
        /*
main.cpp:19:16: error: no matching function for call to 'execute'
async.h:261:1: note: candidate template ignored:
substitution failure [with
t_time = std::chrono::duration<long, std::ratio<1, 1000>>,
t_function = (lambda at
/home/rodrigo/development/tenacitas.lib/tst/async/executer/main.cpp:14:26),
t_params = <int>]:
no type named 'type' in
'std::invoke_result<(lambda at
/home/rodrigo/development/tenacitas.lib/tst/async/executer/main.cpp:14:26),
int>'
*/
    }
};

struct executer_001 {
    static std::string desc() { return ""; }
    bool operator()() {

        auto _function = [](type::ptr<bool> p_bool, int &&p_i) -> void {
            std::this_thread::sleep_for(1s);
            if (p_bool) {
                DEB("hello");
            } else {
                DEB("i = ", p_i);
            }
        };
        int _i {4};

        return !async::execute(200ms, _function, std::move(_i));
    }
};

struct executer_002 {
    static std::string desc() { return ""; }
    bool operator()() {

        auto _function = [](type::ptr<bool>) -> void { DEB("hello"); };

        return async::execute(200ms, _function);
    }
};

struct executer_003 {
    static std::string desc() { return ""; }
    bool operator()() {

        auto _function = [](type::ptr<bool> p_bool) -> void {
            std::this_thread::sleep_for(1s);
            if (p_bool) {
                DEB("TIMEOUT");
            } else {
                DEB("hello");
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
            DEB(p_i, ',', p_str, ',', p_char);
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
                DEB("TIMEOUT");
            } else {
                DEB(p_i, ',', p_str, ',', p_char);
            }
        };

        std::string _str {"hello"};

        return !async::execute(200ms, _function, 4, std::move(_str), "goodbye");
    }
};

struct executer_006 {
    static std::string desc() { return ""; }
    bool operator()() {

        auto _function = [](type::ptr<bool>, int16_t &&p_i) -> int16_t {
            const int16_t _res = 2 * p_i;
            TRA("i = ", p_i, ", _res = ", _res);
            return _res;
        };

        int _i {4};

        std::optional<int16_t> _ret =
            async::execute(200ms, _function, std::move(_i));

        if (!_ret) {
            ERR("timeout, when it should not");
            return false;
        }
        int16_t _value = _ret.value();
        if (_value != 8) {
            ERR("return should be 8, but it is ", _value);
            return false;
        }
        return true;
    }
};

int main(int argc, char **argv) {
    logger::set_trace_level();
    tester::test _tester(argc, argv);

    run_test(_tester, executer_000);
    run_test(_tester, executer_001);
    run_test(_tester, executer_002);
    run_test(_tester, executer_003);
    run_test(_tester, executer_004);
    run_test(_tester, executer_005);
    run_test(_tester, executer_006);
}
