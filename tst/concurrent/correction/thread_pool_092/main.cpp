#include <concurrent/internal/log.h>
#include <concurrent/sleeping_loop.h>
#include <concurrent/thread_pool.h>
#include <concurrent/traits.h>
#include <concurrent/_tst/msg_a.h>
#include <logger/cerr/log.h>
#include <tester/test.h>

#include <chrono>

using namespace tenacitas;

typedef tenacitas::concurrent::_tst::msg_a msg;
typedef concurrent::_bus::thread_pool_t<msg, logger::cerr::log> thread_pool;

using namespace tenacitas;

struct work {
  concurrent::_bus::work_status operator()(msg &&p_msg) {
    m_msg = p_msg;
    concurrent_log_debug(logger::cerr::log, "handling msg ", m_msg);
    return concurrent::_bus::work_status::dont_stop;
  }
  msg m_msg;
};

struct thread_pool_092 {
  bool operator()() {
    work _work;

    thread_pool _pool_1;
    _pool_1.add_work([&_work](msg &&p_msg) { return _work(std::move(p_msg)); },
                     std::chrono::milliseconds(500));

    for (uint16_t _i = 0; _i < 20; ++_i) {
      msg _msg(_i);
      concurrent_log_debug(logger::cerr::log, "adding msg ", _msg);
      _pool_1.handle(_msg);
    }

    thread_pool _pool_2(std::move(_pool_1));

    _pool_2.run();

    std::this_thread::sleep_for(std::chrono::seconds(1));

    concurrent_log_debug(logger::cerr::log,
                         "consumed = ", _work.m_msg.counter());
    if (_work.m_msg.counter() != 19) {
      concurrent_log_error(logger::cerr::log,
                           "Data value consumed should be equal to 19");
      return false;
    }

    return true;
  }

  static std::string desc() {
    return "\n20 messages added to a 'thread_pool' with one consumer; "
           "'thread_pool' moved; new 'thread_pool' started";
  }

  static std::string name() { return "thread_pool_092"; }
};

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();
  tester::test::run<thread_pool_092>(argc, argv);
}