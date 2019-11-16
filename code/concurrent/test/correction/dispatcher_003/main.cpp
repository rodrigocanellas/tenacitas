/// \example
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <sstream>
#include <string>

#include <calendar/business/epoch.h>
#include <concurrent/business/dispatcher.h>
#include <concurrent/business/internal/log.h>
#include <concurrent/business/sleeping_loop.h>
#include <concurrent/business/traits.h>
#include <logger/business/cerr.h>
#include <tester/business/run.h>

using namespace tenacitas;
namespace ns_con = tenacitas::concurrent::business;
namespace ns_log = tenacitas::logger::business;
namespace ns_cal = tenacitas::calendar::business;

// ############################## loops
typedef ns_con::sleeping_loop_t<void, ns_log::log> sync_loop;
typedef ns_con::sleeping_loop_t<void, ns_log::log> rx_loop;

// ############################## messages

struct end
{};

struct msg_tpdo
{
  msg_tpdo() { ++counter; }

  friend std::ostream& operator<<(std::ostream& p_out,
                                  const msg_tpdo& /*p_msg*/)
  {
    p_out << ns_cal::epoch::microsecs();
    return p_out;
  }
  static uint16_t counter;
};
uint16_t msg_tpdo::counter = 0;
;

struct sync
{
  ns_con::work_status operator()()
  {
    concurrent_log_test(ns_log::log, "sync");
    return ns_con::work_status::dont_stop;
  }
};

// ############################## publishers
struct rx
{
  ns_con::work_status operator()()
  {
    concurrent_log_test(ns_log::log, "rx");
    ns_con::dispatcher_t<msg_tpdo, ns_log::log>::publish(msg_tpdo());
    ns_con::dispatcher_t<msg_tpdo, ns_log::log>::publish(msg_tpdo());
    ns_con::dispatcher_t<msg_tpdo, ns_log::log>::publish(msg_tpdo());
    ns_con::dispatcher_t<msg_tpdo, ns_log::log>::publish(msg_tpdo());
    ns_con::dispatcher_t<msg_tpdo, ns_log::log>::publish(msg_tpdo());
    ns_con::dispatcher_t<msg_tpdo, ns_log::log>::publish(msg_tpdo());
    ns_con::dispatcher_t<msg_tpdo, ns_log::log>::publish(msg_tpdo());
    ns_con::dispatcher_t<msg_tpdo, ns_log::log>::publish(msg_tpdo());
    ns_con::dispatcher_t<msg_tpdo, ns_log::log>::publish(msg_tpdo());
    ns_con::dispatcher_t<msg_tpdo, ns_log::log>::publish(msg_tpdo());
    ns_con::dispatcher_t<msg_tpdo, ns_log::log>::publish(msg_tpdo());
    ns_con::dispatcher_t<msg_tpdo, ns_log::log>::publish(msg_tpdo());
    ns_con::dispatcher_t<msg_tpdo, ns_log::log>::publish(msg_tpdo());

    return concurrent::business::work_status::dont_stop;
  }
};

// ############################## handler
struct tx
{
  tx()
    : m_id(++m_counter)
  {}

  ns_con::work_status operator()(msg_tpdo&& p_msg)
  {
    //    if (p_msg.counter > 20) {
    //      concurrent_log_test(
    //        ns_log::log, "stoping because counter = ", p_msg.counter);
    //      return ns_con::work_status::stop;
    //    }
    concurrent_log_test(logger::business::log, m_id, "|", p_msg);

    return ns_con::work_status::dont_stop;
  }

private:
  uint16_t m_id = 0;
  static uint16_t m_counter;
};
uint16_t tx::m_counter = 0;

// ############################## main
struct dispatcher_003
{

  dispatcher_003() {}

  bool operator()()
  {
    //    sync_loop _sync(
    //      std::chrono::milliseconds(100), sync(),
    //      std::chrono::milliseconds(2));

    rx_loop _rx(
      std::chrono::milliseconds(40), rx(), std::chrono::milliseconds(15));

    ns_con::dispatcher_t<msg_tpdo, ns_log::log>::subscribe(
      "tx", tx(), std::chrono::milliseconds(10));

    //    _sync.run();
    _rx.run();

    std::this_thread::sleep_for(std::chrono::milliseconds(122));

    return true;
  }
};

int
main(int argc, char** argv)
{
  logger::business::configure_cerr_log();
  run_test(dispatcher_003,
           argc,
           argv,
           "\nTest inspired in a case brought by Fabio Costa")
}
