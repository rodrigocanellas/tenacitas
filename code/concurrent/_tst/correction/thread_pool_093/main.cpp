#include <concurrent/_bus/internal/log.h>
#include <concurrent/_bus/sleeping_loop.h>
#include <concurrent/_bus/thread_pool.h>
#include <concurrent/_bus/traits.h>
#include <concurrent/_tst/msg_a.h>
#include <logger/cerr/log.h>
#include <tester/_bus/test.h>

#include <chrono>

using namespace tenacitas;

typedef tenacitas::concurrent::_tst::msg_a msg;
typedef concurrent::_bus::thread_pool_t<msg, logger::cerr::log> thread_pool;
typedef concurrent::_bus::sleeping_loop_t<void, logger::cerr::log> sleeping_loop;

struct work {
  concurrent::_bus::work_status operator()(msg &&p_msg) {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    m_msg = p_msg;
    concurrent_log_debug(logger::cerr::log, "handling msg ", m_msg);
    return concurrent::_bus::work_status::dont_stop;
  }
  msg m_msg;
};

struct thread_pool_093 {
  bool operator()() {
    work _work;
    thread_pool _pool_1;
    _pool_1.add_work([&_work](msg &&p_msg) { return _work(std::move(p_msg)); },
                     std::chrono::milliseconds(501));

    for (uint16_t _i = 0; _i < 200; ++_i) {
      msg _msg(_i);
      concurrent_log_debug(logger::cerr::log, "adding msg ", _msg);
      _pool_1.handle(_msg);
    }

    concurrent_log_debug(logger::cerr::log, "starting pool");
    _pool_1.run();

    concurrent_log_debug(logger::cerr::log, "sleeping for 2 s");
    std::this_thread::sleep_for(std::chrono::seconds(2));
    concurrent_log_debug(logger::cerr::log, "waking up");

    concurrent_log_debug(logger::cerr::log, "stopping pool");
    _pool_1.stop();

    concurrent_log_debug(logger::cerr::log, "moving pool");
    thread_pool _pool_2(std::move(_pool_1));

    concurrent_log_debug(logger::cerr::log, "sleeping for 3 s");
    std::this_thread::sleep_for(std::chrono::seconds(3));

    concurrent_log_debug(logger::cerr::log, "starting new pool");
    _pool_2.run();

    std::this_thread::sleep_for(std::chrono::seconds(10));

    concurrent_log_debug(logger::cerr::log,
                         "consumed = ", _work.m_msg.counter());
    if (_work.m_msg.counter() != 199) {
      concurrent_log_error(logger::cerr::log,
                           "Data value consumed should be equal to 199");
      return false;
    }

    return true;
  }

  static std::string desc() {
    return "\n200 messages added to a 'thread_pool' with one consumer, each "
           "sleeping for 50 ms; 'thread_pool' started; main thread sleeps for "
           "2 s; "
           "'thread_pool' stopped; 'thread_pool' moved; main thread sleeps for "
           "2 s; "
           "new 'thread_pool' started";
  }

  static std::string name() { return "thread_pool_093"; }
};

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();
  tester::_bus::test::run<thread_pool_093>(argc, argv);
}
