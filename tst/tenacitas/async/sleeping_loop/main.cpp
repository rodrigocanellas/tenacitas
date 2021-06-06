/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <cmath>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>

#include <tenacitas/async.h>
#include <tenacitas/logger.h>
#include <tenacitas/number.h>
#include <tenacitas/tester.h>

using namespace tenacitas;
using namespace std::chrono_literals;

struct sleeping_loop_000 {

    static const std::string desc() { return "'sleeping_loop' creation test"; }

    bool operator()() {

        typedef async::sleeping_loop_t<> loop;

        auto _operation = [](std::shared_ptr<bool>) -> void { DEB("loop1"); };

        loop _loop(_operation, 100ms, 1s);

        return true;
    }
};

struct sleeping_loop_001 {
    typedef async::sleeping_loop_t<> loop;

    typedef uint16_t value;

    static const std::string desc() {
        std::stringstream _stream;
        _stream
            << "'sleeping_loop' with interval of " << m_interval_secs
            << "s, operation timeout of " << m_timeout
            << "ms, increments a counter, and just prints so there will be no "
            << "timeout."

            << "\nCounter should be " << m_amount;
        return _stream.str();
    }

    bool operator()() {

        operation1 _op(&m_cond);

        loop _loop([&_op](std::shared_ptr<bool> p_bool) { return _op(p_bool); },
                   std::chrono::milliseconds(m_timeout),
                   std::chrono::seconds(m_interval_secs));

        _loop.start();

        {
            std::unique_lock<std::mutex> _lock(m_mutex);
            m_cond.wait(_lock,
                        [&_op]() -> bool { return _op.counter >= m_amount; });
        }

        _loop.stop();

        if (_op.counter != m_amount) {
            ERR("counter should be ", m_amount, ", but it is ", _op.counter);
            return false;
        }

        INF("counter should be ", m_amount, ", and it really is ", _op.counter);

        return true;
    }

private:
    struct operation1 {

        operation1(std::condition_variable *p_cond)
            : m_cond(p_cond) {}

        void operator()(std::shared_ptr<bool>) {
            if (counter < m_amount) {
                ++counter;
                DEB("counter = ", counter);
                std::this_thread::sleep_for(m_sleep);
            } else {
                m_cond->notify_one();
            }
        }

        value counter {0};

    private:
        std::condition_variable *m_cond;
    };

private:
    std::mutex m_mutex;
    std::condition_variable m_cond;

    static constexpr value m_interval_secs {1};
    static constexpr value m_amount {14};
    static constexpr value m_timeout {400};
    static constexpr std::chrono::milliseconds m_sleep {200};
};

int main(int argc, char **argv) {
    logger::set_debug_level();

    tester::test<> _tester(argc, argv);

    run_test(_tester, sleeping_loop_000);
    run_test(_tester, sleeping_loop_001);
}
