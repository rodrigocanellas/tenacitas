
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>

#include <business/concurrent/traits.h>
#include <business/calendar/epoch.h>
#include <business/concurrent/internal/log.h>
#include <business/concurrent/sleeping_loop.h>
#include <business/concurrent/traits.h>
#include <business/logger/cerr.h>
#include <business/tester/run.h>

using namespace tenacitas::business;
using namespace tenacitas::business;

struct msg
{

  friend std::ostream& operator<<(std::ostream&, const msg&);

  msg() = default;

  msg(int32_t p_i, double p_f)
    : m_i(p_i)
    , m_f(p_f)
  {}

  msg(msg&&) noexcept = default;
  msg(const msg&) = default;

  msg& operator=(msg&& p_msg) noexcept
  {
    if (this != &p_msg) {
      m_i = std::move(p_msg.m_i);
      m_f = std::move(p_msg.m_f);
    }
    return *this;
  }

  msg& operator=(const msg&) = delete;

  ~msg() = default;

  int32_t m_i = 0;
  double m_f = 0.0;
};

std::ostream&
operator<<(std::ostream& p_out, const msg& p_msg)
{
  p_out << p_msg.m_i << "-" << p_msg.m_f << "'";
  return p_out;
}

typedef concurrent::
sleeping_loop_t<msg, logger::log>
loop;

struct provide_1
{
  std::pair<bool, msg> operator()()
  {
    ++m_i;
    m_f *= 2;
    std::pair<bool, msg> result(true, msg(m_i, m_f));
    return result;
  }

private:
  int32_t m_i = 0;
  double m_f = 1.0;
};

struct provide_2
{
  std::pair<bool, msg> operator()()
  {
    m_i += 100;
    m_f *= 3;
    std::pair<bool, msg> result(true, msg(m_i, m_f));
    return result;
  }

private:
  int32_t m_i = 0;
  double m_f = 1.0;
};

struct provide_3
{
  std::pair<bool, msg> operator()()
  {
    m_i += 1000;
    m_f *= 4;
    std::pair<bool, msg> result(true, msg(m_i, m_f));
    return result;
  }

private:
  int32_t m_i = 0;
  double m_f = 1.0;
};

struct work_1
{
  concurrent::result operator()(msg&& p_msg)
  {
    m_msg = std::move(p_msg);
    concurrent_log_test(logger::log, "1: ", m_msg);
    return concurrent::result::dont_stop;
  }
  msg m_msg;
};

struct work_2
{
  concurrent::result operator()(msg&& p_msg)
  {
    m_msg = std::move(p_msg);
    concurrent_log_test(logger::log, "2: ", m_msg);
    return concurrent::result::dont_stop;
  }
  msg m_msg;
};

struct work_3
{
  concurrent::result operator()(msg&& p_msg)
  {
    m_msg = std::move(p_msg);
    concurrent_log_test(logger::log, "3: ", m_msg);
    return concurrent::result::dont_stop;
  }
  msg m_msg;
};

struct sleeping_loop_012
{

  bool operator()()
  {
    provide_1 _provide_1;
    provide_2 _provide_2;
    provide_3 _provide_3;
    work_1 _work_1;
    work_2 _work_2;
    work_3 _work_3;

    loop _loop_1(
          std::chrono::milliseconds(1000),
          [&_work_1](msg&& p_msg) -> concurrent::result { return _work_1(std::move(p_msg)); },
    std::chrono::milliseconds(100),
        [&_provide_1]() -> std::pair<bool, msg> { return _provide_1(); });

    loop _loop_2(
          std::chrono::milliseconds(500),
          [&_work_2](msg&& p_msg) -> concurrent::result { return _work_2(std::move(p_msg)); },
    std::chrono::milliseconds(100),
        [&_provide_2]() -> std::pair<bool, msg> { return _provide_2(); });

    loop _loop_3(
          std::chrono::milliseconds(2000),
          [&_work_3](msg&& p_msg) -> concurrent::result { return _work_3(std::move(p_msg)); },
    std::chrono::milliseconds(100),
        [&_provide_3]() -> std::pair<bool, msg> { return _provide_3(); });

    _loop_1.run();
    _loop_2.run();
    _loop_3.run();

    std::this_thread::sleep_for(std::chrono::seconds(10));

    concurrent_log_test(logger::log, "stopping");
    _loop_1.stop();
    _loop_2.stop();
    _loop_3.stop();

    concurrent_log_test(logger::log, "data 1 = ", _work_1.m_msg);
    if ((_work_1.m_msg.m_i != 10) && (_work_1.m_msg.m_f != 1024.0)) {
      return false;
    }

    concurrent_log_test(logger::log, "data 2 = ", _work_2.m_msg);
    if ((_work_2.m_msg.m_i != 2000) &&
        (_work_2.m_msg.m_f != 3486784401.0)) {
      return false;
    }

    concurrent_log_test(logger::log, "data 3 = ", _work_3.m_msg);
    if ((_work_3.m_msg.m_i != 5000) && (_work_3.m_msg.m_f != 1024.0)) {
      return false;
    }

    return true;
  }
};

int
main(int argc, char** argv)
{
  logger::configure_cerr_log();
  run_test(
        sleeping_loop_012,
        argc,
        argv,
        "3 'sleeping_loops':\n'1' with 1000 ms of interval, work time out of 50 "
        "ms, and work function too simple to cause time out.\n'2' with interval "
        "of 500 ms, work time out of 100 ms, and work won't cause time out.\n'3' "
        "with interval of 2000 ms, work time out of 75 ms, and work also too "
        "simple.\nAll 3 use the same provider. \nAll will be started, the main "
        "thread will sleep for 12 secs, all the loops will be stopped by calling "
        "'stop', the main thread will "
        "sleep for 3 secs.");
}
