#include <concurrent/bus/internal/log.h>
#include <concurrent/bus/sleeping_loop.h>
#include <concurrent/bus/traits.h>
#include <concurrent/bus/thread_pool.h>
#include <concurrent/test/msg_a.h>
#include <logger/cerr/log.h>


#include <chrono>

using namespace tenacitas;
using namespace tenacitas;

typedef tenacitas::concurrent::test::msg_a msg;
typedef concurrent::bus::
thread_pool_t<msg, logger::cerr::log>
thread_pool;

using namespace tenacitas;

struct work
{
  concurrent::bus::work_status operator()(msg&& p_msg)
  {
    m_msg = p_msg;
    concurrent_log( "handling msg ", m_msg);
    return concurrent::bus::work_status::dont_stop;
  }
  msg m_msg;
};

struct thread_pool_095
{

  bool operator()()
  {
    work _work;

    thread_pool _pool;

    _pool.add_work(
          [&_work](msg&& p_msg) { return _work(std::move(p_msg)); },
    std::chrono::milliseconds(200));

    msg _msg(18);
    concurrent_log( "adding msg ", _msg);
    _pool.handle(_msg);

    concurrent_log( "start pool");
    _pool.run();

    concurrent_log( "sleeping for 1 s");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    concurrent_log( "waking up");

    return true;
  }
};

int
main(int argc, char** argv)
{
  logger::bus::configure_cerr_log();
  run_test(thread_pool_095, argc, argv, "\nWork function taking too long");
}
