/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <ctime>
#include <functional>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <tuple>

#include <tenacitas/concurrent.h>
#include <tenacitas/logger.h>
#include <tenacitas/macros.h>
#include <tenacitas/tester.h>

/// TODO test when provider and breaker functions take too long, causing
/// timeout

using namespace tenacitas;

using namespace std::chrono_literals;

struct timeout_callback {
    inline void operator()(std::thread::id p_id) {
        WAR(m_log, "timeout for ", std::hash<std::thread::id>()(p_id));
    }

private:
    logger::cerr<> m_log{"timeout_callback"};
};

struct async_loop_000 {

    static std::string desc() {
        std::stringstream _stream;
        _stream << "no timeout, one param, no breaker";
        return _stream.str();
    }

    bool operator()() {

        m_log.set_debug_level();

        int16_t _i{0};
        auto _provider = [this, &_i]() -> int16_t {
            ++_i;
            DEB(m_log, "providing ", _i);
            return _i;
        };

        auto _worker = [this](int16_t p_i) -> void {
            DEB(m_log, "working with = ", p_i);
            std::this_thread::sleep_for(250ms);
        };

        concurrent::async_loop_t<logger::cerr<>, std::chrono::milliseconds, concurrent::use_breaker::no,
                int16_t>
                _loop(500ms, timeout_callback(), _worker, _provider);

        _loop.set_log_debug();

        _loop.start();

        std::this_thread::sleep_for(2s);

        _loop.stop();

        if (_i != 8) {
            ERR(m_log, "i should be 8, but it is ", _i);
            return false;
        }

        INF(m_log, "i is 8, as expected");

        return true;
    }

private:
    logger::cerr<> m_log{"async_loop_000"};
};

struct async_loop_001 {

    static std::string desc() {
        std::stringstream _stream;
        _stream << "no timeout, one param, breaker";
        return _stream.str();
    }

    bool operator()() {

        m_log.set_debug_level();

        int16_t _i{0};
        auto _provider = [this, &_i]() -> int16_t {
            ++_i;
            DEB(m_log, "providing ", _i);
            return _i;
        };

        auto _worker = [this](int16_t p_i) -> void {
            DEB(m_log, "working with = ", p_i);
            std::this_thread::sleep_for(250ms);
        };

        auto _breaker = [&_i]() -> bool {
            if (_i == 3) {
                return true;
            }
            return false;
        };

        concurrent::async_loop_t<logger::cerr<>, std::chrono::milliseconds, concurrent::use_breaker::yes,
                int16_t>
                _loop(500ms, timeout_callback(), _breaker, _worker, _provider);

        _loop.set_log_debug();

        _loop.start();

        std::this_thread::sleep_for(2s);

        _loop.stop();

        if (_i != 3) {
            ERR(m_log, "i should be 3, but it is ", _i);
            return false;
        }

        INF(m_log, "i is 3, as expected");

        return true;
    }

private:
    logger::cerr<> m_log{"async_loop_001"};
};

struct async_loop_002 {

    static std::string desc() {
        std::stringstream _stream;
        _stream << "no timeout, many params, no breaker";
        return _stream.str();
    }

    bool operator()() {

        m_log.set_debug_level();

        int16_t _i{0};
        float _f{0.0};
        auto _provider = [this, &_i, &_f]() -> std::tuple<int16_t, float> {
            ++_i;
            _f = _i * 2.5;
            DEB(m_log, "providing ", _i);
            return {_i, _f};
        };

        auto _worker = [this](int16_t p_i, float p_f) -> void {
            DEB(m_log, "working with = ", p_i, ", ", p_f);
            std::this_thread::sleep_for(250ms);
        };

        concurrent::async_loop_t<logger::cerr<>, std::chrono::milliseconds, concurrent::use_breaker::no,
                int16_t, float>
                _loop(500ms, timeout_callback(), _worker, _provider);

        _loop.set_log_debug();

        _loop.start();

        std::this_thread::sleep_for(2s);

        _loop.stop();

        if ((_i != 8) && (_f != (8 * 2.5))) {
            ERR(m_log, "i should be 8 and f should be 20.0, but they are they are ",
                _i, " and ", _f);
            return false;
        }

        INF(m_log, "i is 8 and f is 20, as expected");

        return true;
    }

private:
    logger::cerr<> m_log{"async_loop_002"};
};

struct async_loop_003 {

    static std::string desc() {
        std::stringstream _stream;
        _stream << "no timeout, many params, breaker";
        return _stream.str();
    }

    bool operator()() {

        m_log.set_debug_level();

        int16_t _i{0};
        float _f{0.0};
        auto _provider = [this, &_i, &_f]() -> std::tuple<int16_t, float> {
            ++_i;
            _f = _i * 2.5;
            DEB(m_log, "providing ", _i);
            return {_i, _f};
        };

        auto _worker = [this](int16_t p_i, float p_f) -> void {
            DEB(m_log, "working with = ", p_i, ", ", p_f);
            std::this_thread::sleep_for(250ms);
        };

        auto _breaker = [&_i]() -> bool {
            if (_i == 3) {
                return true;
            }
            return false;
        };

        concurrent::async_loop_t<logger::cerr<>, std::chrono::milliseconds, concurrent::use_breaker::yes,
                int16_t, float>
                _loop(500ms, timeout_callback(), _breaker, _worker, _provider);

        _loop.set_log_debug();

        _loop.start();

        std::this_thread::sleep_for(2s);

        _loop.stop();

        if ((_i != 3) && (_f != (3 * 2.5))) {
            ERR(m_log, "i should be 8 and f should be 7.5, but they are they are ",
                _i, " and ", _f);
            return false;
        }

        INF(m_log, "i is 8 and f is 3.75, as expected");

        return true;
    }

private:
    logger::cerr<> m_log{"async_loop_003"};
};

struct async_loop_004 {

    static std::string desc() {
        std::stringstream _stream;
        _stream << "no timeout, no params, no breaker";
        return _stream.str();
    }

    bool operator()() {

        m_log.set_debug_level();

        int16_t _i{0};

        auto _worker = [this, &_i]() -> void {
            DEB(m_log, "working with = ", ++_i);
            std::this_thread::sleep_for(250ms);
        };

        concurrent::async_loop_t<logger::cerr<>, std::chrono::milliseconds, concurrent::use_breaker::no, void>
                _loop(500ms, timeout_callback(), _worker);

        _loop.set_log_debug();

        _loop.start();

        std::this_thread::sleep_for(2s);

        _loop.stop();

        if (_i != 8) {
            ERR(m_log, "i should be 8, but it is ", _i);
            return false;
        }

        INF(m_log, "i is 8, as expected");

        return true;
    }

private:
    logger::cerr<> m_log{"async_loop_004"};
};

struct async_loop_005 {

    static std::string desc() {
        std::stringstream _stream;
        _stream << "no timeout, no params, breaker";
        return _stream.str();
    }

    bool operator()() {

        m_log.set_debug_level();

        int16_t _i{0};

        auto _worker = [this, &_i]() -> void {
            DEB(m_log, "working with = ", ++_i);
            std::this_thread::sleep_for(250ms);
        };

        auto _breaker = [&_i]() -> bool {
            if (_i == 3) {
                return true;
            }
            return false;
        };

        concurrent::async_loop_t<logger::cerr<>, std::chrono::milliseconds, concurrent::use_breaker::yes, void>
                _loop(500ms, timeout_callback(), _breaker, _worker);

        _loop.set_log_debug();

        _loop.start();

        std::this_thread::sleep_for(2s);

        _loop.stop();

        if (_i != 3) {
            ERR(m_log, "i should be 3, but it is ", _i);
            return false;
        }

        INF(m_log, "i is 3, as expected");

        return true;
    }

private:
    logger::cerr<> m_log{"async_loop_005"};
};

struct async_loop_006 {

    static std::string desc() {
        std::stringstream _stream;
        _stream << "timeout, one param, no breaker";
        return _stream.str();
    }

    bool operator()() {
        try {
            const int16_t _max{9};
            const std::chrono::milliseconds _work_timeout{500};
            const std::chrono::milliseconds _work_normal_sleep{250ms};
            const std::chrono::milliseconds _work_timeout_sleep{
                _work_timeout.count() * 2};
            const std::chrono::milliseconds _wait{_max * _work_normal_sleep.count() +
                        3000};

            m_log.set_debug_level();

            auto _callback = [this](std::thread::id p_id) -> void {
                WAR(m_log, "timeout for ", std::hash<std::thread::id>()(p_id));
                m_cond.notify_one();
            };

            int16_t _i{0};
            auto _provider = [this, &_i]() -> std::tuple<int16_t> {
                ++_i;
                DEB(m_log, "providing ", _i);
                return {_i};
            };

            auto _worker = [this, _work_normal_sleep,
                    _work_timeout_sleep](int16_t p_i) -> void {
                DEB(m_log, "working with = ", p_i);
                if (p_i == _max) {
                    DEB(m_log, "causing timeout sleeping for ",
                        _work_timeout_sleep.count());
                    std::this_thread::sleep_for(_work_timeout_sleep);
                    return;
                }
                std::this_thread::sleep_for(_work_normal_sleep);
            };

            concurrent::async_loop_t<logger::cerr<>, std::chrono::milliseconds, concurrent::use_breaker::no,
                    int16_t>
                    _loop(_work_timeout, _callback, _worker, _provider);

            //    _loop.set_log_debug();

            _loop.start();

            {
                std::unique_lock<std::mutex> _lock{m_mutex};
                m_cond.wait_for(_lock, _wait);
                _loop.stop();
            }

            if (_i != _max) {
                ERR(m_log, "i should be ", _max, ", but it is ", _i);
                return false;
            }

            INF(m_log, "i is ", _max, " as expected");

            return true;
        } catch (std::exception &_ex) {
            FAT(m_log, _ex.what());
        }
        return false;
    }

private:
    logger::cerr<> m_log{"async_loop_006"};
    std::condition_variable m_cond;
    std::mutex m_mutex;
};

struct async_loop_006a {

    static std::string desc() {
        std::stringstream _stream;
        _stream << "same as async_loop_006, calling async_loop_006";
        return _stream.str();
    }

    bool operator()() {
        //    INF(m_log, "sleeping for 10s");
        //    std::this_thread::sleep_for(10s);
        async_loop_006 _al;
        return _al();
    }

private:
    logger::cerr<> m_log{"async_loop_006a"};
};

struct async_loop_006b {

    static std::string desc() {
        std::stringstream _stream;
        _stream << "same as async_loop_006, with code copied";
        return _stream.str();
    }

    bool operator()() {

        //    INF(m_log, "sleeping for 10s");
        //    std::this_thread::sleep_for(10s);

        const int16_t _max{9};
        const std::chrono::milliseconds _work_timeout{500};
        const std::chrono::milliseconds _work_normal_sleep{250ms};
        const std::chrono::milliseconds _work_timeout_sleep{_work_timeout.count() *
                    2};
        const std::chrono::milliseconds _wait{_max * _work_normal_sleep.count() +
                    3000};

        m_log.set_debug_level();

        auto _callback = [this](std::thread::id p_id) -> void {
            WAR(m_log, "timeout for ", std::hash<std::thread::id>()(p_id));
            m_cond.notify_one();
        };

        int16_t _i{0};
        auto _provider = [this, &_i]() -> std::tuple<int16_t> {
            ++_i;
            DEB(m_log, "providing ", _i);
            return {_i};
        };

        auto _worker = [this, _work_normal_sleep,
                _work_timeout_sleep](int16_t p_i) -> void {
            DEB(m_log, "working with = ", p_i);
            if (p_i == _max) {
                DEB(m_log, "causing timeout sleeping for ",
                    _work_timeout_sleep.count());
                std::this_thread::sleep_for(_work_timeout_sleep);
                return;
            }
            std::this_thread::sleep_for(_work_normal_sleep);
        };

        concurrent::async_loop_t<logger::cerr<>, std::chrono::milliseconds, concurrent::use_breaker::no,
                int16_t>
                _loop(_work_timeout, _callback, _worker, _provider);

        //    _loop.set_log_debug();

        _loop.start();

        {
            std::unique_lock<std::mutex> _lock{m_mutex};
            m_cond.wait_for(_lock, _wait);
            _loop.stop();
        }

        if (_i != _max) {
            ERR(m_log, "i should be ", _max, ", but it is ", _i);
            return false;
        }

        INF(m_log, "i is ", _max, " as expected");

        return true;
    }

private:
    logger::cerr<> m_log{"async_loop_006a"};
    std::condition_variable m_cond;
    std::mutex m_mutex;
};

struct async_loop_007 {

    static std::string desc() {
        std::stringstream _stream;
        _stream << "timeout, 2 params, no breaker";
        return _stream.str();
    }

    bool operator()() {

        const int16_t _max{6};
        const std::chrono::milliseconds _work_timeout{500};
        const std::chrono::milliseconds _work_normal_sleep{250ms};
        const std::chrono::milliseconds _work_timeout_sleep{_work_timeout.count() *
                    2};
        const std::chrono::milliseconds _wait{_max * _work_normal_sleep.count() +
                    3000};

        m_log.set_debug_level();

        auto _callback = [this](std::thread::id p_id) -> void {
            WAR(m_log, "timeout for ", std::hash<std::thread::id>()(p_id));
            m_cond.notify_one();
        };

        int16_t _i{0};
        auto _provider = [this, &_i]() -> std::tuple<int16_t, float> {
            ++_i;
            float _f(_i * 2.5);
            DEB(m_log, "providing ", _i);
            return {_i, _f};
        };

        auto _worker = [this, _work_normal_sleep,
                _work_timeout_sleep](int16_t p_i, float p_f) -> void {
            DEB(m_log, "working with = ", p_i, ", ", p_f);
            if (p_i == _max) {
                DEB(m_log, "causing timeout");
                std::this_thread::sleep_for(_work_timeout_sleep);
                return;
            }
            std::this_thread::sleep_for(_work_normal_sleep);
        };

        concurrent::async_loop_t<logger::cerr<>, std::chrono::milliseconds, concurrent::use_breaker::no,
                int16_t, float>
                _loop(_work_timeout, _callback, _worker, _provider);

        _loop.set_log_debug();

        _loop.start();

        {
            std::unique_lock<std::mutex> _lock{m_mutex};
            m_cond.wait_for(_lock, _wait);
            _loop.stop();
        }

        if (_i != _max) {
            ERR(m_log, "i should be ", _max, ", but it is ", _i);
            return false;
        }

        INF(m_log, "i is ", _max, " as expected");

        return true;
    }

private:
    logger::cerr<> m_log{"async_loop_007"};
    std::condition_variable m_cond;
    std::mutex m_mutex;
};

struct async_loop_008 {

    static std::string desc() {
        std::stringstream _stream;
        _stream << "timeout, no params, no breaker";
        return _stream.str();
    }

    bool operator()() {
        m_log.set_debug_level();

        const int16_t _max{23};
        const std::chrono::milliseconds _work_timeout{500};
        const std::chrono::milliseconds _work_normal_sleep{250ms};
        const std::chrono::milliseconds _work_timeout_sleep{_work_timeout.count() *
                    2};
        const std::chrono::milliseconds _wait{_max * _work_normal_sleep.count() +
                    3000};

        m_log.set_debug_level();

        auto _callback = [this](std::thread::id p_id) -> void {
            WAR(m_log, "timeout for ", std::hash<std::thread::id>()(p_id));
            m_cond.notify_one();
        };

        int16_t _i{0};

        auto _worker = [this, _work_normal_sleep, _work_timeout_sleep,
                &_i]() -> void {
            DEB(m_log, "working with = ", _i);
            if (_i == _max) {
                DEB(m_log, "causing timeout");
                std::this_thread::sleep_for(_work_timeout_sleep);
                return;
            }
            ++_i;
            std::this_thread::sleep_for(_work_normal_sleep);
        };

        concurrent::async_loop_t<logger::cerr<>, std::chrono::milliseconds, concurrent::use_breaker::no, void>
                _loop(_work_timeout, _callback, _worker);

        _loop.set_log_debug();

        _loop.start();

        {
            std::unique_lock<std::mutex> _lock{m_mutex};
            m_cond.wait_for(_lock, _wait);
            _loop.stop();
        }

        if (_i != _max) {
            ERR(m_log, "i should be ", _max, ", but it is ", _i);
            return false;
        }

        INF(m_log, "i is ", _max, " as expected");

        return true;
    }

private:
    logger::cerr<> m_log{"async_loop_008"};
    std::condition_variable m_cond;
    std::mutex m_mutex;
};

struct async_loop_009 {

    static std::string desc() {
        std::stringstream _stream;
        _stream << "timeout, no params, no breaker";
        return _stream.str();
    }

    bool operator()() {
        m_log.set_debug_level();

        const int16_t _max{249};
        const std::chrono::milliseconds _work_timeout{500};
        const std::chrono::milliseconds _work_normal_sleep{250ms};
        const std::chrono::milliseconds _work_timeout_sleep{_work_timeout.count() *
                    2};
        const std::chrono::milliseconds _wait{_max * _work_normal_sleep.count() +
                    3000};

        m_log.set_debug_level();

        auto _callback = [this](std::thread::id p_id) -> void {
            WAR(m_log, "timeout for ", std::hash<std::thread::id>()(p_id));
            m_cond.notify_one();
        };

        int16_t _i{0};

        auto _worker = [this, _work_normal_sleep, _work_timeout_sleep,
                &_i]() -> void {
            DEB(m_log, "working with = ", _i);
            if (_i == _max) {
                DEB(m_log, "causing timeout");
                std::this_thread::sleep_for(_work_timeout_sleep);
                return;
            }
            ++_i;
            std::this_thread::sleep_for(_work_normal_sleep);
        };

        concurrent::async_loop_t<logger::cerr<>, std::chrono::milliseconds, concurrent::use_breaker::no, void>
                _loop(_work_timeout, _callback, _worker);

        _loop.set_log_debug();

        _loop.start();

        {
            std::unique_lock<std::mutex> _lock{m_mutex};
            m_cond.wait_for(_lock, _wait);
            _loop.stop();
        }

        if (_i != _max) {
            ERR(m_log, "i should be ", _max, ", but it is ", _i);
            return false;
        }

        INF(m_log, "i is ", _max, " as expected");

        return true;
    }

private:
    logger::cerr<> m_log{"async_loop_009"};
    std::condition_variable m_cond;
    std::mutex m_mutex;
};

int main(int argc, char **argv) {
    logger::set_debug_level();

    tenacitas::tester::test _tester(argc, argv);
    run_test(_tester, async_loop_000);
    run_test(_tester, async_loop_001);
    run_test(_tester, async_loop_002);
    run_test(_tester, async_loop_003);
    run_test(_tester, async_loop_004);
    run_test(_tester, async_loop_005);
    run_test(_tester, async_loop_006);
    run_test(_tester, async_loop_006a);
    run_test(_tester, async_loop_006b);
    run_test(_tester, async_loop_007);
    run_test(_tester, async_loop_008);
    run_test(_tester, async_loop_009);
}
