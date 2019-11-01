/// \example
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <sstream>
#include <string>

#include <business/concurrent/dispatcher.h>
#include <business/concurrent/internal/log.h>
#include <business/concurrent/sleeping_loop.h>
#include <business/concurrent/traits.h>
#include <business/logger/cerr.h>
#include <business/tester/run.h>

using namespace tenacitas::business;
using namespace tenacitas::business;

// ############################## messages
struct start
{
  friend std::ostringstream& operator<<(std::ostringstream& p_out, const start&)
  {
    p_out << "start sent";
    return p_out;
  }
};

struct finish
{
  friend std::ostringstream& operator<<(std::ostringstream& p_out,
                                        const finish&)
  {
    p_out << "finish sent";
    return p_out;
  }
};

struct msg1
{
  friend std::ostringstream& operator<<(std::ostringstream& p_out, const msg1&)
  {
    p_out << "msg1 sent";
    return p_out;
  }
};

struct msg2
{
  friend std::ostringstream& operator<<(std::ostringstream& p_out, const msg2&)
  {
    p_out << "msg2 sent";
    return p_out;
  }
};

// ############################## agents
struct agent_1
{
  agent_1() { concurrent::dispatcher_t<start, logger::log>::publish(start()); }

  concurrent::result operator()(msg1&&)
  {
    concurrent_log_test(logger::log, "msg1 received");

    concurrent::dispatcher_t<msg2, logger::log>::publish(msg2());

    std::this_thread::sleep_for(std::chrono::milliseconds(40));

    concurrent::dispatcher_t<finish, logger::log>::publish(finish());
    return concurrent::result::dont_stop;
  }
};

struct agent_2
{
  agent_2() { concurrent::dispatcher_t<start, logger::log>::publish(start()); }

  concurrent::result operator()(msg2&&)
  {
    concurrent_log_test(logger::log, "msg2 received");

    concurrent::dispatcher_t<msg1, logger::log>::publish(msg1());

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    concurrent::dispatcher_t<finish, logger::log>::publish(finish());
    return concurrent::result::dont_stop;
  }
};

struct controler
{
  concurrent::result operator()(start&&)
  {

    {
      std::lock_guard<std::mutex> _lock(m_mutex_counter);
      ++m_counter;
    }
    concurrent_log_test(logger::log, "start received, counter = ", m_counter);
    return concurrent::result::dont_stop;
  }

  concurrent::result operator()(finish&&)
  {
    //    if (m_counter == 0) {
    //      concurrent_log_test(logger::log, "finisher received, counter = ",
    //      m_counter); m_cond_finish.notify_all(); return;
    //    }
    std::lock_guard<std::mutex> _lock(m_mutex_counter);
    --m_counter;
    if (m_counter == 0) {
      concurrent_log_test(
        logger::log, "finisher received, counter = ", m_counter);
      m_cond_finish.notify_all();
      return concurrent::result::stop;
    }
    concurrent_log_test(
      logger::log, "finisher received, counter = ", m_counter);
    return concurrent::result::dont_stop;
  }

  ~controler()
  {
    concurrent_log_test(logger::log, "------> waiting...");
    std::unique_lock<std::mutex> _lock(m_mutex_finish);
    m_cond_finish.wait(_lock);
    concurrent_log_test(logger::log, "------> done!");
  }

private:
  std::condition_variable m_cond_finish;
  std::mutex m_mutex_finish;
  std::mutex m_mutex_counter;
  uint16_t m_counter = { 0 };
};

// ############################## main
struct dispatcher_003
{

  dispatcher_003()
  {
    concurrent::dispatcher_t<start, logger::log>::subscribe(
      "start",
      [this](start&& p_start) -> concurrent::result {
        return m_controler(std::move(p_start));
      },
      std::chrono::milliseconds(500));

    concurrent::dispatcher_t<finish, logger::log>::subscribe(
      "finish",
      [this](finish&& p_finish) -> concurrent::result {
        return m_controler(std::move(p_finish));
      },
      std::chrono::milliseconds(500));

    concurrent::dispatcher_t<msg2, logger::log>::subscribe(
      "msg1", agent_2(), std::chrono::milliseconds(1500));
    concurrent::dispatcher_t<msg1, logger::log>::subscribe(
      "msg2", agent_1(), std::chrono::milliseconds(500));
  }

  bool operator()()
  {

    //    using namespace logger::business;
    //    using namespace concurrent::business;

    //    dispatcher_t<msg1, logger::log>::publish(msg1());

    return false;
  }

private:
  controler m_controler;
};

int
main(int argc, char** argv)
{
  logger::configure_cerr_log();
  run_test(dispatcher_003,
           argc,
           argv,
           "\nTesting asynchronous work collaboration, and ending with "
           "message. NOT WORKING")
}
