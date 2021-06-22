
#include <tenacitas.lib/async.h>
#include <tenacitas.lib/logger.h>
#include <tenacitas.lib/tester.h>
#include <tenacitas.lib/type.h>

using namespace tenacitas;
using namespace std::chrono_literals;

struct executer_000 {
    static std::string desc() {
        return "Function with one parameter, no returning, with no timeout";
    }
    bool operator()() {

        auto _function = [](type::ptr<bool>, int &&p_i) -> void {
            TRA("i = ", p_i);
        };
        int _i {4};

        return async::execute(200ms, _function, std::move(_i));
    }
};

struct executer_001 {
    static std::string desc() {
        return "Function with one parameter, no returning, with timeout";
    }
    bool operator()() {

        auto _function = [](type::ptr<bool> p_bool, int &&p_i) -> void {
            std::this_thread::sleep_for(1s);
            if (p_bool) {
                WAR("TIMEOUT");
            } else {
                DEB("i = ", p_i);
            }
        };
        int _i {4};

        return !async::execute(200ms, _function, std::move(_i));
    }
};

struct executer_002 {
    static std::string desc() {
        return "Function with no parameter, no returning, with no timeout";
    }
    bool operator()() {

        auto _function = [](type::ptr<bool>) -> void { DEB("hello"); };

        return async::execute(200ms, _function);
    }
};

struct executer_003 {
    static std::string desc() {
        return "Function with no parameter, no returning, with timeout";
    }

    bool operator()() {

        auto _function = [](type::ptr<bool> p_bool) -> void {
            std::this_thread::sleep_for(1s);
            if (p_bool) {
                WAR("TIMEOUT");
            } else {
                DEB("hello");
            }
        };

        return !async::execute(200ms, _function);
    }
};

struct executer_004 {
    static std::string desc() {
        return "Function with 3 parameters, no returning, with no timeout";
    }

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
    static std::string desc() {
        return "Function with 3 parameters, no returning, with timeout";
    }

    bool operator()() {

        auto _function = [](type::ptr<bool> p_bool, int p_i,
                            std::string &&p_str, const char *p_char) -> void {
            std::this_thread::sleep_for(1s);
            if (p_bool) {
                WAR("TIMEOUT");
            } else {
                DEB(p_i, ',', p_str, ',', p_char);
            }
        };

        std::string _str {"hello"};

        return !async::execute(200ms, _function, 4, std::move(_str), "goodbye");
    }
};

// ----
struct executer_006 {
    static std::string desc() {
        return "Function with one parameter, with returning, with no timeout";
    }

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

struct executer_007 {
    static std::string desc() {
        return "Function with one parameter, with returning, with timeout";
    }

    bool operator()() {

        auto _function = [](type::ptr<bool> p_bool, int &&p_i) -> int16_t {
            std::this_thread::sleep_for(1s);
            if (p_bool) {
                WAR("timeout");
                return -1;
            } else {
                int16_t _res = p_i * 2;
                TRA("i = ", p_i, ", res = ", _res);
                return _res;
            }
        };
        int _i {4};

        std::optional<int16_t> _maybe =
            async::execute(200ms, _function, std::move(_i));

        if (_maybe) {
            ERR("function returned ", *_maybe, ", but it should not");
            return false;
        }
        return true;
    }
};

struct executer_008 {
    static std::string desc() {
        return "Function with no parameter, with returning, with no timeout";
    }

    bool operator()() {

        auto _function = [](type::ptr<bool>) -> int16_t {
            DEB("hello");
            return 94;
        };

        std::optional<int16_t> _maybe = async::execute(200ms, _function);
        if (!_maybe) {
            ERR("function timedout, but it should not");
            return false;
        }
        int16_t _value = *_maybe;
        if (_value != 94) {
            ERR("value returned should be 94, but it is ", _value);
            return false;
        }
        return true;
    }
};

struct executer_009 {
    static std::string desc() {
        return "Function with no parameter, with returning, with timeout";
    }

    bool operator()() {

        auto _function = [](type::ptr<bool> p_bool) -> int16_t {
            std::this_thread::sleep_for(1s);
            if (p_bool) {
                WAR("TIMEOUT");
                return -1;
            }
            return 53;
        };

        std::optional<int16_t> _maybe = async::execute(200ms, _function);
        if (_maybe) {
            ERR("function not timedout, as expected, and returned ", *_maybe);
            return false;
        }
        return true;
    }
};

struct executer_010 {
    static std::string desc() {
        return "Function with 2 parameters, with returning, with no timeout";
    }

    bool operator()() {

        auto _function = [](type::ptr<bool>, int16_t p_i, float p_f) -> float {
            return p_f * p_i;
        };

        std::optional<float> _maybe = async::execute(200ms, _function, 4, -2.5);
        if (!_maybe) {
            ERR("function timeout, when it should not");
            return false;
        }
        float _value = *_maybe;
        if (_value != static_cast<float>(-10)) {
            ERR("value should be ", static_cast<float>(-10), ", but it is ",
                _value);
        }
        return true;
    }
};

struct executer_011 {
    static std::string desc() {
        return "Function with 2 parameters, with returning, with timeout";
    }

    bool operator()() {

        auto _function = [](type::ptr<bool> p_bool, int16_t p_i,
                            float p_f) -> float {
            std::this_thread::sleep_for(1s);
            if (*p_bool) {
                WAR("timeout");
                return -1;
            }
            return p_f * p_i;
        };

        std::optional<float> _maybe = async::execute(200ms, _function, 4, -2.5);
        if (_maybe) {
            ERR("function should timeout, but it has not, and returned ",
                *_maybe);
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
    run_test(_tester, executer_007);
    run_test(_tester, executer_008);
    run_test(_tester, executer_009);
    run_test(_tester, executer_010);
    run_test(_tester, executer_011);
}
