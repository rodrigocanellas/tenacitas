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

struct thread_pool_095 {

  bool operator()() {
    work _work;

    thread_pool _pool;

    _pool.add_work([&_work](msg &&p_msg) { return _work(std::move(p_msg)); },
                   std::chrono::milliseconds(200));

    msg _msg(18);
    concurrent_log_debug(logger::cerr::log, "adding msg ", _msg);
    _pool.handle(_msg);

    concurrent_log_debug(logger::cerr::log, "start pool");
    _pool.run();

    concurrent_log_debug(logger::cerr::log, "sleeping for 1 s");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    concurrent_log_debug(logger::cerr::log, "waking up");

    return true;
  }

  static std::string desc() { return "\nWork function taking too long"; }

  static std::string name() { return "thread_pool_095"; }
};

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();
  tester::test::run<thread_pool_095>(argc, argv);
}