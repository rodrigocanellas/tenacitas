/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

/// \example dispatcher/dispatcher_000/main.cpp

#include <chrono>
#include <condition_variable>
#include <mutex>

#include "tenacitas.lib/async/dispatcher.h"
#include "tenacitas.lib/async/exp/dispatcher/event.h"
#include "tenacitas.lib/async/exp/dispatcher/event_handled.h"
#include "tenacitas.lib/async/exp/dispatcher/handler.h"
#include "tenacitas.lib/async/exp/dispatcher/logger.h"
#include "tenacitas.lib/async/exp/dispatcher/publisher.h"
#include "tenacitas.lib/async/exp/dispatcher/results.h"
#include "tenacitas.lib/container/circular_queue.h"
#include "tenacitas.lib/format/fmt.h"

using namespace std::chrono_literals;
using namespace tenacitas::lib;

using logger = async::exp::logger;

using event_a = async::exp::event<'a'>;

using event_handled = async::exp::event_handled;

using dispatcher = async::dispatcher<logger, event_a, event_handled>;

using queue_event = container::circular_queue<logger, event_a, 20>;

using queue_event_handled =
    container::circular_queue<logger, event_handled, 200>;

using publisher = async::exp::publisher<'a', dispatcher>;

using handler_1 = async::exp::handler<'a', 1, dispatcher>;
using handler_2 = async::exp::handler<'a', 2, dispatcher>;

using results = async::exp::results;

int main() {
  logger _logger;

  std::condition_variable _cond_all_handled;
  std::mutex _mutex_all_handled;

  dispatcher _dispatcher(_logger);

  results _results;

  const size_t _total_to_be_published{10};
  const size_t _total_to_be_handled{20};
  size_t _total_handled{0};

  _dispatcher.add_handling<event_handled>(
      [&](event_handled &&p_event) {
        TNCT_LOG_TST(_logger, format::fmt("event handled: ", p_event))
        ++_total_handled;
        _results.increment(p_event.event_id, p_event.handling_id,
                           p_event.handler_type_id, p_event.handler_id);
        if (_total_handled >= _total_to_be_handled) {
          _cond_all_handled.notify_all();
        }
      },
      queue_event_handled{_logger});

  publisher _publisher{_dispatcher, _logger, 750ms, _total_to_be_published,
                       "pub 1"};

  _dispatcher.subscribe<event_a>(handler_1{_logger, _dispatcher, 1, 1s},
                                 queue_event{_logger}, 3);

  _dispatcher.subscribe<event_a>(handler_2{_logger, _dispatcher, 2, 300ms},
                                 queue_event{_logger}, 1);

  _publisher.start();

  {
    std::unique_lock<std::mutex> _lock{_mutex_all_handled};
    _cond_all_handled.wait(_lock);
  }

  TNCT_LOG_TST(_logger,
               format::fmt("# events published ", _publisher.num_events()));

  std::cout << _results.report() << std::endl;
}
