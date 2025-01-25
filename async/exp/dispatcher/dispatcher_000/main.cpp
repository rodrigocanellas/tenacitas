/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

/// \example dispatcher/dispatcher_000/main.cpp

#include <chrono>

#include "tenacitas.lib/async/dispatcher.h"
#include "tenacitas.lib/format/fmt.h"

#include "tenacitas.lib/async/exp/dispatcher/event.h"
#include "tenacitas.lib/async/exp/dispatcher/logger.h"
#include "tenacitas.lib/async/exp/dispatcher/publisher.h"

using namespace std::chrono_literals;
using namespace tenacitas::lib;

using event_a = async::exp::event<'a'>;

using dispatcher = async::dispatcher<async::exp::logger, event_a>;

int main() {
  async::exp::logger _logger;

  dispatcher _dispatcher(_logger);

  async::exp::publisher<event_a, dispatcher> _publisher{_dispatcher, _logger,
                                                        100ms, 10, "pub 1"};

  _publisher.start();

  std::this_thread::sleep_for(2s);

  TNCT_LOG_TST(_logger,
               format::fmt("# events published ", _publisher.num_events()));
}
