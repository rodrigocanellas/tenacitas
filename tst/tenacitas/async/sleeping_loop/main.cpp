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

        DEB(m_log, "id = ", m_id);

        typedef async::sleeping_loop loop;

        auto _operation = [this](std::shared_ptr<bool>) -> void {
            DEB(m_log, "loop1");
        };

        loop _loop(m_id, _operation, 100ms, 1s);

        return true;
    }

  private:
    logger::cerr<> m_log{"sleeping_loop_000"};
    number::id m_id;
};

struct sleeping_loop_001 {
    typedef async::sleeping_loop loop;

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

        DEB(m_log, "id = ", m_id);

        operation1 _op(&m_cond);

        loop _loop(
            m_id, [&_op](std::shared_ptr<bool> p_bool) { return _op(p_bool); },
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
            ERR(m_log, "counter should be ", m_amount, ", but it is ",
                _op.counter);
            return false;
        }

        INF(m_log, "counter should be ", m_amount, ", and it really is ",
            _op.counter);

        return true;
    }

  private:
    struct operation1 {

        operation1(std::condition_variable *p_cond) : m_cond(p_cond) {}

        void operator()(std::shared_ptr<bool>) {
            if (counter < m_amount) {
                ++counter;
                DEB(m_log, "counter = ", counter);
                std::this_thread::sleep_for(m_sleep);
            } else {
                m_cond->notify_one();
            }
        }

        value counter{0};

      private:
        std::condition_variable *m_cond;
        logger::cerr<> m_log{"operation1"};
    };

  private:
    std::mutex m_mutex;
    std::condition_variable m_cond;

    static constexpr value m_interval_secs{1};
    static constexpr value m_amount{14};
    static constexpr value m_timeout{400};
    static constexpr std::chrono::milliseconds m_sleep{200};
    logger::cerr<> m_log{"sleeping_loop_001"};
    number::id m_id;
};

// struct sleeping_loop_002 {
//  static std::string desc() {
//    return std::string("sleep interval of 2 seconds, work timeout of 500ms, "
//                       "timeout when a counter reaches 2, not giving time for
//                       " "not even one sleep loop");
//  }

//  bool operator()() {

//    DEB(m_log, "id = ", m_id);

//    typedef async::sleeping_loop<int16_t, float> loop;
//    int16_t _i{0};
//    int16_t _value{0};

//    std::function<std::optional<std::tuple<int16_t, float>>()> _provider =
//        [&_i]() -> std::optional<std::tuple<int16_t, float>> {
//      ++_i;
//      return {{_i, 2.5 * _i}};
//    };

//    auto _worker = [this, &_value](std::shared_ptr<bool> p_stop, int16_t
//    &&p_i,
//                                   float &&p_f) {
//      DEB(m_log, "entering with ", p_i, " and ", p_f);

//      if (*p_stop) {
//        DEB(m_log, "must stop");
//        m_cond.notify_one();
//        return;
//      }

//      if (p_i == m_max) {
//        _value = p_i;
//        std::this_thread::sleep_for(
//            std::chrono::milliseconds(m_timeout.count() * 2));
//        if (*p_stop) {
//          DEB(m_log, "must stop");
//          m_cond.notify_one();
//          return;
//        }
//      } else {
//        std::this_thread::sleep_for(
//            std::chrono::milliseconds(m_timeout.count() / 2));
//        if (*p_stop) {
//          DEB(m_log, "must stop");
//          m_cond.notify_one();
//          return;
//        }
//        INF(m_log, p_i, " - ", p_f);
//      }
//      DEB(m_log, "exiting");
//    };

//    loop _loop(m_id, _worker, m_timeout, _provider, 2s);

//    _loop.start();

//    {
//      std::unique_lock<std::mutex> _lock(m_mutex);
//      m_cond.wait(_lock);
//    }

//    DEB(m_log, "stopping the sleeping loop");
//    _loop.stop();

//    if (_value != m_max) {
//      ERR(m_log, "value = ", _value, " but it should be ", m_max);
//      return false;
//    }

//    INF(m_log, "value = ", _value, " and it is correct as ", m_max,
//        " was expected");

//    return true;
//  }

// private:
//  std::mutex m_mutex;
//  std::condition_variable m_cond;
//  static constexpr int16_t m_max{2};
//  static constexpr std::chrono::milliseconds m_timeout{500};
//  logger::cerr<> m_log{"sleeping_loop_002"};
//  number::id m_id;
//};

// struct sleeping_loop_003 {
//  static std::string desc() {
//    return std::string("sleep interval of 2s, work timeout of 3s, "
//                       "timeout when a counter reaches 38");
//  }

//  bool operator()() {

//    DEB(m_log, "id = ", m_id);

//    typedef async::sleeping_loop<int16_t, float> loop;
//    int16_t _i{0};
//    int16_t _value{0};

//    auto _provider = [&_i]() -> std::optional<std::tuple<int16_t, float>> {
//      ++_i;
//      return {std::tuple<int16_t, float>{_i, 2.5 * _i}};
//    };

//    auto _worker = [this, &_value](std::shared_ptr<bool> p_stop, int16_t
//    &&p_i,
//                                   float &&p_f) {
//      DEB(m_log, "worker called with ", p_i, " and ", p_f);
//      if (*p_stop) {
//        DEB(m_log, "must stop");
//        m_cond.notify_one();
//        return;
//      }

//      if (p_i == m_max) {
//        _value = p_i;
//        DEB(m_log, "going to sleep for ", m_timeout.count() * 2, "ms");
//        std::this_thread::sleep_for(
//            std::chrono::milliseconds(m_timeout.count() * 2));
//        if (*p_stop) {
//          DEB(m_log, "must stop");
//          m_cond.notify_one();
//          return;
//        }

//      } else {
//        DEB(m_log, "going to sleep for ", m_timeout.count() / 2, "ms");
//        std::this_thread::sleep_for(
//            std::chrono::milliseconds(m_timeout.count() / 2));
//        if (*p_stop) {
//          DEB(m_log, "must stop");
//          m_cond.notify_one();
//          return;
//        }

//        INF(m_log, p_i, " - ", p_f);
//      }
//    };

//    loop _loop(m_id, _worker, m_timeout, _provider, 2000ms);

//    _loop.start();

//    {
//      std::unique_lock<std::mutex> _lock(m_mutex);
//      m_cond.wait(_lock);
//    }

//    DEB(m_log, "stopping the loop");

//    _loop.stop();

//    if (_value != m_max) {
//      ERR(m_log, "value = ", _value, " but it should be ", m_max);
//      return false;
//    }

//    INF(m_log, "value = ", _value, " and it is correct as ", m_max,
//        " was expected");

//    return true;
//  }

// private:
//  std::mutex m_mutex;
//  std::condition_variable m_cond;
//  static constexpr int16_t m_max{38};
//  static constexpr std::chrono::milliseconds m_timeout{3000};
//  logger::cerr<> m_log{"sleeping_loop_003"};
//  number::id m_id;
//};

int main(int argc, char **argv) {
    logger::set_debug_level();

    tester::test<> _tester(argc, argv);

    run_test(_tester, sleeping_loop_000);
    run_test(_tester, sleeping_loop_001);
    //  run_test(_tester, sleeping_loop_002);
    //  run_test(_tester, sleeping_loop_003);
}
