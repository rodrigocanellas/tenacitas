/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

// #include <iostream>

#include "tenacitas.lib/async/test/dispatcher_test.h"
#include "tenacitas.lib/async/test/handling_id_test.h"
#include "tenacitas.lib/async/test/handling_test.h"
// #include "tenacitas.lib/async/test/handlings_test.h"
#include "tenacitas.lib/async/test/sleeping_loop_test.h"
// #include "tenacitas.lib/test/test.h"

// #include "tenacitas.lib/async/dispatcher.h"
// #include "tenacitas.lib/async/handling.h"
// #include "tenacitas.lib/container/circular_queue.h"
#include "tenacitas.lib/log/cerr.h"
#include "tenacitas.lib/tester/test.h"

using namespace tenacitas::lib;

// struct event_test {
//   friend std::ostream &operator<<(std::ostream &p_out, const event_test &) {
//     p_out << "event test";
//     return p_out;
//   }
// };

// using logger = log::cerr;
// logger g_logger;

// using dispatcher = async::dispatcher<log::cerr, event_test>;
// dispatcher g_dispatcher(g_logger);

// using queue = container::circular_queue<logger, event_test, 10>;

// struct subscriber {
//   subscriber(dispatcher &p_dispatcher) : m_dispatcher(p_dispatcher) {
//     m_dispatcher.add_handling<event_test>(m_handling);
//     event_test _event_test{};
//     m_handling.increment_handlers(1U);
//     m_dispatcher.publish(_event_test);
//   }

// private:
//   struct handler {
//     using event = event_test;
//     void operator()(event &&p_event) {
//       std::cout << "handler: " << p_event << std::endl;
//     }
//   };

//   using handling =
//       async::handling<logger, event_test, queue, handler>;

// private:
//   dispatcher &m_dispatcher;
//   handling m_handling{"handling_1a", g_logger, handler{}};
// };

int main(int argc, char **argv) {
  tester::test _test(argc, argv);

  run_test(_test, handling_000);
  run_test(_test, handling_001);
  run_test(_test, handling_005);
  run_test(_test, handling_006);
  run_test(_test, handling_007);
  run_test(_test, handling_008);
  run_test(_test, handling_009);
  run_test(_test, handling_010);
  run_test(_test, handling_011);
  run_test(_test, handling_012);
  run_test(_test, handling_014);
  run_test(_test, handling_015);
  run_test(_test, handling_016);
  run_test(_test, sleeping_loop_000);
  run_test(_test, sleeping_loop_001);
  // run_test(_test, handlings_000);
  // // run_test(_test, handlings_001);
  // // run_test(_test, handlings_002);
  // // run_test(_test, handlings_003);
  // // run_test(_test, handlings_004);
  // // run_test(_test, handlings_005);
  // // run_test(_test, handlings_006);
  // // run_test(_test, handlings_007);
  // // run_test(_test, handlings_001);
  // // run_test(_test, handlings_002);
  // // run_test(_test, handlings_003);
  // // run_test(_test, handlings_004);
  // // run_test(_test, handlings_005);
  run_test(_test, dispatcher_000);
  run_test(_test, dispatcher_001);
  run_test(_test, dispatcher_002);
  run_test(_test, dispatcher_003);
  run_test(_test, dispatcher_004);
  run_test(_test, dispatcher_005);
  run_test(_test, dispatcher_006);
  // // run_test(_test, dispatcher_007);
  run_test(_test, handling_id_000);
  run_test(_test, handling_id_001);
  run_test(_test, handling_id_002);
  run_test(_test, handling_id_004);
  run_test(_test, handling_id_005);
  run_test(_test, handling_id_006);

  return 0;
}
