/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <iostream>

#include <tenacitas.lib/async/test/dispatcher_test.h>
#include <tenacitas.lib/async/test/handling_id_test.h>
#include <tenacitas.lib/async/test/handling_test.h>
#include <tenacitas.lib/async/test/handlings_test.h>
#include <tenacitas.lib/async/test/sleeping_loop_test.h>
// #include <tenacitas.lib/generic/tester.h>

#include <tenacitas.lib/async/dispatcher.h>
#include <tenacitas.lib/async/handling.h>
#include <tenacitas.lib/container/circular_queue.h>
#include <tenacitas.lib/log/cerr.h>

using namespace tenacitas::lib;

struct event_test {
  friend std::ostream &operator<<(std::ostream &p_out, const event_test &) {
    p_out << "event test";
    return p_out;
  }
};

using logger = log::cerr;
logger g_logger;

using dispatcher = async::dispatcher<log::cerr, event_test>;
dispatcher g_dispatcher(g_logger);

using queue = container::circular_queue<logger, event_test, 10>;

struct subscriber {
  subscriber(dispatcher &p_dispatcher) : m_dispatcher(p_dispatcher) {
    m_dispatcher.add_handling<event_test>(m_handling);
    event_test _event_test{};
    m_handling.increment_handlers(1U);
    m_dispatcher.publish(_event_test);
  }

private:
  struct handler {
    using event = event_test;
    void operator()(event &&p_event) {
      std::cout << "handler: " << p_event << std::endl;
    }
  };

  using handling =
      async::handling<logger, event_test, queue, handler>;

private:
  dispatcher &m_dispatcher;
  handling m_handling{"handling_1a", g_logger, handler{}};
};

int main(/*int argc, char **argv*/) {
  g_logger.set_tra();
  subscriber _subscriber(g_dispatcher);

  TNCT_LOG_TST(g_logger, "sleeping");
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  TNCT_LOG_TST(g_logger, "waking up");

  // generic::tester _tester(argc, argv);

  // run_test(_tester, handling_000);
  // run_test(_tester, handling_001);
  // run_test(_tester, handling_005);
  // run_test(_tester, handling_006);
  // run_test(_tester, handling_007);
  // run_test(_tester, handling_008);
  // run_test(_tester, handling_009);
  // run_test(_tester, handling_010);
  // run_test(_tester, handling_011);
  // run_test(_tester, handling_012);
  // run_test(_tester, handling_014);
  // run_test(_tester, handling_015);
  // run_test(_tester, handling_016);
  // run_test(_tester, sleeping_loop_000);
  // run_test(_tester, sleeping_loop_001);
  // // run_test(_tester, handlings_000);
  // // run_test(_tester, handlings_001);
  // // run_test(_tester, handlings_002);
  // // run_test(_tester, handlings_003);
  // // run_test(_tester, handlings_004);
  // // run_test(_tester, handlings_005);
  // // run_test(_tester, handlings_006);
  // // run_test(_tester, handlings_007);
  // // run_test(_tester, handlings_001);
  // // run_test(_tester, handlings_002);
  // // run_test(_tester, handlings_003);
  // // run_test(_tester, handlings_004);
  // // run_test(_tester, handlings_005);
  // run_test(_tester, dispatcher_000);
  // // run_test(_tester, dispatcher_001);
  // // run_test(_tester, dispatcher_002);
  // // run_test(_tester, dispatcher_003);
  // // run_test(_tester, dispatcher_004);
  // // run_test(_tester, dispatcher_005);
  // // run_test(_tester, dispatcher_006);
  // // run_test(_tester, dispatcher_007);
  // run_test(_tester, handling_id_000);
  // run_test(_tester, handling_id_001);
  // run_test(_tester, handling_id_002);
  // run_test(_tester, handling_id_003);
  // run_test(_tester, handling_id_004);
  // run_test(_tester, handling_id_005);
  // run_test(_tester, handling_id_006);
  // run_test(_tester, handling_id_007);

  return 0;
}
