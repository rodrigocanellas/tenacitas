/// \example
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <sstream>
#include <string>

#include <concurrent/_bus/dispatcher.h>
#include <concurrent/_bus/internal/log.h>
#include <concurrent/_bus/sleeping_loop.h>
#include <concurrent/_bus/traits.h>
#include <logger/cerr/log.h>
#include <tester/_bus/test.h>

using namespace tenacitas;
namespace ns_con = tenacitas::concurrent::_bus;
namespace ns_log = tenacitas::logger::cerr;

// ############################## loops
typedef ns_con::sleeping_loop_t<void, ns_log::log> sync_loop;
typedef ns_con::sleeping_loop_t<void, ns_log::log> rx_loop;

// ############################## messages

struct end {};

struct msg_tpdo {
  msg_tpdo() { ++counter; }

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const msg_tpdo & /*p_msg*/) {
    p_out << static_cast<uint64_t>(
        std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch())
            .count());
    return p_out;
  }
  static uint16_t counter;
};
uint16_t msg_tpdo::counter = 0;
;

struct sync {
  ns_con::work_status operator()() {
    concurrent_log_debug(ns_log::log, "sync");
    return ns_con::work_status::dont_stop;
  }
};

// ############################## publishers
struct rx {
  ns_con::work_status operator()() {
    concurrent_log_debug(ns_log::log, "rx");
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

    return concurrent::_bus::work_status::dont_stop;
  }
};

// ############################## handler
struct tx {
  tx() : m_id(++m_counter) {}

  ns_con::work_status operator()(msg_tpdo &&p_msg) {
    //    if (p_msg.counter > 20) {
    //      concurrent_log_debug(
    //        ns_log::log, "stoping because counter = ", p_msg.counter);
    //      return ns_con::work_status::stop;
    //    }
    concurrent_log_debug(ns_log::log, m_id, "|", p_msg);

    return ns_con::work_status::dont_stop;
  }

private:
  uint16_t m_id = 0;
  static uint16_t m_counter;
};
uint16_t tx::m_counter = 0;

// ############################## main
struct dispatcher_003 {

  dispatcher_003() {}

  bool operator()() {
    //    sync_loop _sync(
    //      std::chrono::milliseconds(100), sync(),
    //      std::chrono::milliseconds(2));

    rx_loop _rx(std::chrono::milliseconds(40), rx(),
                std::chrono::milliseconds(15));

    ns_con::dispatcher_t<msg_tpdo, ns_log::log>::subscribe(
        "tx", tx(), std::chrono::milliseconds(10));

    //    _sync.run();
    _rx.run();

    std::this_thread::sleep_for(std::chrono::milliseconds(122));

    return true;
  }

  static const std::string desc() {
    return "\nTest inspired in a case brought by Fabio Costa";
  }

  static const std::string name() { return "dispatcher_003"; }
};

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();
  tester::_bus::test::run<dispatcher_003>(argc, argv);
}
