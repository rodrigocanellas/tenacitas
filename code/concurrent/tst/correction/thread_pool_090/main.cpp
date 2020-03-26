#include <concurrent/bus/internal/log.h>
#include <concurrent/bus/sleeping_loop.h>
#include <concurrent/bus/thread_pool.h>
#include <concurrent/bus/traits.h>
#include <logger/cerr/log.h>


#include <concurrent/test/msg_a.h>

#include <chrono>

using namespace tenacitas;

// typedef logger::cerr::log logger::cerr::log;

typedef tenacitas::concurrent::test::msg_a msg;
typedef concurrent::bus::thread_pool_t<msg, logger::cerr::log> thread_pool;
typedef concurrent::bus::sleeping_loop_t<void, logger::cerr::log> sleeping_loop;

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

struct thread_pool_090
{
  bool operator()()
  {

    work _work;

    thread_pool _pool;
    uint32_t _value = 0;

    sleeping_loop _loop(std::chrono::milliseconds(500),
                        [&_pool, &_value]() {
                          msg _msg(++_value);
                          concurrent_log( "adding msg ", _msg);
                          _pool.handle(std::move(_msg));
                          return concurrent::bus::work_status::dont_stop;
                        },
                        std::chrono::milliseconds(300));

    _pool.add_work([&_work](msg&& p_msg) { return _work(std::move(p_msg)); },
                   std::chrono::milliseconds(100));

    _pool.run();
    _loop.run();

    concurrent_log( "sleeping for 10 secs");
    std::this_thread::sleep_for(std::chrono::seconds(10));
    concurrent_log( "waking up after 10 secs");

    concurrent_log( "stopping the pool");
    _pool.stop();

    concurrent_log( "sleeping for 5 secs");
    std::this_thread::sleep_for(std::chrono::seconds(5));
    concurrent_log( "waking up after 5 secs");

    concurrent_log( "runnig the pool");
    _pool.run();

    concurrent_log( "sleeping for 4 secs");
    std::this_thread::sleep_for(std::chrono::seconds(4));
    concurrent_log( "waking up after 4 secs");

    concurrent_log(
                        "consumed = ",
                        _work.m_msg.counter(),
                        ", provided = ",
                        _value);
    if (_work.m_msg.counter() != _value) {
      concurrent_log_error(logger::cerr::log,
                           "Data value consumed should be equal to provided");
      return false;
    }

    return true;
  }
};
int
main(int argc, char** argv)
{
  logger::bus::configure_cerr_log();
  run_test(thread_pool_090,
           argc,
           argv,
           "\nA 'sleeping_loop' sending a message at each 500 ms, to a "
           "'thread_pool' with one consumer."

           "\nThe main thread will sleep for 10 secs, the 'thread_pool' will "
           "stop; main thread will sleep for 5 secs; 'thread_pool' will run "
           "again; main thread will sleep for 4 secs."

           "\nThe messages added while the pool was stopped are handled when "
           "the pool runs again."

           "\nThe amount of data consumed must be equal to the provided");
}
