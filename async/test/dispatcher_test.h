/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_TEST_ASYNC_DISPATCHER_H
#define TENACITAS_LIB_TEST_ASYNC_DISPATCHER_H

#include <iostream>
#include <string>
#include <tuple>

#ifndef TENACITAS_LOG
#define TENACITAS_LOG
#endif

#include "tenacitas.lib/async/dispatcher.h"
#include "tenacitas.lib/traits/dispatcher.h"

#include "tenacitas.lib/container/circular_queue.h"
#include "tenacitas.lib/format/fmt.h"
#include "tenacitas.lib/log/cerr.h"
#include "tenacitas.lib/program/options.h"
#include "tenacitas.lib/traits/logger.h"

using namespace tenacitas::lib;
using namespace std::chrono_literals;

using logger = log::cerr;

struct event_1 {
  event_1(int16_t p_i = -9) : i(p_i) {}
  friend std::ostream &operator<<(std::ostream &out, const event_1 &p_event) {
    out << p_event.i;
    return out;
  }

  int16_t i;
};

struct event_2 {
  event_2(float p_f = 3.14f) : f(p_f) {}
  friend std::ostream &operator<<(std::ostream &out, const event_2 &p_event) {
    out << p_event.f;
    return out;
  }

  float f;
};

using queue_1 = container::circular_queue<logger, event_1, 10>;

using queue_2 = container::circular_queue<logger, event_2, 5>;

// using handling = async::handling<logger, event_1, queue, handler>;
using dispatcher = async::dispatcher<logger, event_1, event_2>;

struct dispatcher_000 {
  static std::string desc() { return "Adding a handling and getting its id"; }

  bool operator()(const program::options &) {
    logger _logger;
    _logger.set_deb();

    dispatcher _dispatcher{_logger};

    auto _handler = [this](event_1 &&p_event) { (*this)(std::move(p_event)); };

    auto _handling_id_maybe{
        _dispatcher.subscribe<event_1>(std::move(_handler), queue_1{_logger})};

    if (!_handling_id_maybe) {
      return false;
    }

    auto _handling_id{_handling_id_maybe.value()};

    TNCT_LOG_TST(_logger, format::fmt("handling id = ", _handling_id));

    return _handling_id == 1;
  }

  void operator()(event_1 &&) {}
};

struct dispatcher_001 {
  static std::string desc() {
    return "Checks the number of handlers in a handling";
  }

  bool operator()(const program::options &) {
    logger _logger;
    _logger.set_deb();

    dispatcher _dispatcher{_logger};

    auto _handler{[](event_1 &&) {}};

    auto _handling_id_maybe{
        _dispatcher.subscribe<event_1>(std::move(_handler), queue_1{_logger})};

    if (!_handling_id_maybe) {
      return false;
    }

    auto _handling_id{_handling_id_maybe.value()};

    TNCT_LOG_TST(_logger, format::fmt("handling id = ", _handling_id));

    auto _amount_handlers_maybe{
        _dispatcher.get_amount_handlers<event_1>(_handling_id)};

    if (!_amount_handlers_maybe) {
      TNCT_LOG_ERR(
          _logger,
          format::fmt(
              "not possible to get the amount of handlers for handling ",
              _handling_id));
    }

    auto _amount_handlers{_amount_handlers_maybe.value()};
    TNCT_LOG_TST(_logger,
                 format::fmt("amount of handlers = ", _amount_handlers));

    return _amount_handlers == 1;
  }

private:
  struct handler {
    using event = event_1;

    void operator()(event &&) {}
  };
};

struct dispatcher_002 {
  static std::string desc() {
    return "Checks the number of handlers in a handling after increasing";
  }

  bool operator()(const program::options &) {
    logger _logger;
    _logger.set_deb();

    dispatcher _dispatcher{_logger};

    auto _handling_id_maybe{
        _dispatcher.subscribe<event_1>([](event_1 &&) {}, queue_1{_logger})};

    if (!_handling_id_maybe) {
      return false;
    }

    auto _handling_id{_handling_id_maybe.value()};

    TNCT_LOG_TST(_logger, format::fmt("handling id = ", _handling_id));

    auto _amount_handlers_maybe{
        _dispatcher.get_amount_handlers<event_1>(_handling_id)};

    if (!_amount_handlers_maybe) {
      TNCT_LOG_ERR(
          _logger,
          format::fmt(
              "not possible to get the amount of handlers for handling ",
              _handling_id));
    }

    auto _amount_handlers{_amount_handlers_maybe.value()};
    TNCT_LOG_TST(_logger,
                 format::fmt("amount of handlers = ", _amount_handlers));

    if (_amount_handlers != 1) {
      TNCT_LOG_ERR(_logger,
                   format::fmt("amount of handlers should be 1, but it is ",
                               _amount_handlers));
      return false;
    }

    if (!_dispatcher.add_handlers<event_1>(_handling_id, 3)) {
      TNCT_LOG_ERR(_logger, format::fmt("error adding handlers to handling ",
                                        _handling_id));
      return false;
    }

    _amount_handlers_maybe =
        _dispatcher.get_amount_handlers<event_1>(_handling_id);

    if (!_amount_handlers_maybe) {
      TNCT_LOG_ERR(
          _logger,
          format::fmt(
              "not possible to get the amount of handlers for handling ",
              _handling_id));
    }
    _amount_handlers = _amount_handlers_maybe.value();
    TNCT_LOG_TST(_logger,
                 format::fmt("amount of handlers = ", _amount_handlers));

    if (_amount_handlers != 4) {
      TNCT_LOG_ERR(_logger,
                   format::fmt("amount of handlers should be 4, but it is ",
                               _amount_handlers));
      return false;
    }

    return true;
  }
};

struct dispatcher_003 {
  static std::string desc() {
    return "Trying to associate a handler to two handlings";
  }

  bool operator()(const program::options &) {
    logger _logger;
    try {
      _logger.set_deb();

      dispatcher _dispatcher{_logger};

      auto _handler = [&](event_1 &&) {};

      auto _handling_id_maybe{_dispatcher.subscribe<event_1>(
          std::move(_handler), queue_1{_logger})};

      if (!_handling_id_maybe) {
        TNCT_LOG_ERR(_logger, "handling not created, but it should");
        return false;
      }

      TNCT_LOG_TST(_logger, "passed 1st subscribe");
      _handling_id_maybe =
          _dispatcher.subscribe<event_1>(std::move(_handler), queue_1{_logger});

      if (_handling_id_maybe) {
        TNCT_LOG_ERR(_logger, "handling created, but it should not have been");
        return false;
      }

      return true;
    } catch (std::exception &_ex) {
      TNCT_LOG_ERR(_logger, _ex.what());
    }

    return false;
  }
};

struct dispatcher_004 {
  static std::string desc() { return "Retrieves if a handling is stopped"; }
  bool operator()(const program::options &) {
    logger _logger;
    try {
      dispatcher _dispatcher{_logger};

      auto _handler_1 = [&](event_1 &&) {};

      auto _handling_id_maybe{_dispatcher.subscribe<event_1>(
          std::move(_handler_1), queue_1{_logger})};

      if (!_handling_id_maybe) {
        TNCT_LOG_ERR(_logger, "handling 1 not created, but it should");
        return false;
      }

      auto _handling_1{_handling_id_maybe.value()};

      TNCT_LOG_TST(_logger,
                   format::fmt("_handling_1 = ", _handling_id_maybe.value()));

      auto _is_stopped_maybe{_dispatcher.is_stopped<event_1>(_handling_1)};
      if (!_is_stopped_maybe) {
        TNCT_LOG_ERR(_logger, format::fmt("could not retrieve if handling ",
                                          _handling_1, " is stopped"));
        return false;
      }

      auto _is_stopped{_is_stopped_maybe.value()};
      if (_is_stopped) {
        TNCT_LOG_ERR(_logger, format::fmt("handling ", _handling_1,
                                          " is stopped, but it should not be"));
        return false;
      }
      TNCT_LOG_TST(_logger,
                   format::fmt("handling ", _handling_1, "'s condition is ",
                               (_is_stopped ? "t" : "f")));
      return true;
    } catch (std::exception &_ex) {
      TNCT_LOG_ERR(_logger, _ex.what());
    }
    return false;
  }
};

struct dispatcher_005 {
  static std::string desc() { return "Stops one handling of an event"; }
  bool operator()(const program::options &) {
    logger _logger;
    try {
      _logger.set_deb();
      dispatcher _dispatcher{_logger};

      auto _handler_1 = [&](event_1 &&) {};

      auto _handling_id_maybe{_dispatcher.subscribe<event_1>(
          std::move(_handler_1), queue_1{_logger})};

      if (!_handling_id_maybe) {
        TNCT_LOG_ERR(_logger, "handling 1 not created, but it should");
        return false;
      }

      TNCT_LOG_TST(_logger,
                   format::fmt("_handling_1 = ", _handling_id_maybe.value()));

      auto _handler_2 = [&](event_2 &&) {};

      _handling_id_maybe = _dispatcher.subscribe<event_2>(std::move(_handler_2),
                                                          queue_2{_logger});

      if (!_handling_id_maybe) {
        TNCT_LOG_ERR(_logger, "handling 2 not created, but it should");
        return false;
      }

      auto _handling_2{_handling_id_maybe.value()};

      TNCT_LOG_TST(_logger, format::fmt("_handling_2 = ", _handling_2));

      if (!_dispatcher.stop<event_2>(_handling_2)) {
        TNCT_LOG_ERR(_logger,
                     format::fmt("could not stop handling ", _handling_2));
        return false;
      }

      auto _is_stopped_maybe{_dispatcher.is_stopped<event_2>(_handling_2)};
      if (!_is_stopped_maybe) {
        TNCT_LOG_ERR(_logger, format::fmt("could not retrieve if handling ",
                                          _handling_2, " is stopped"));
        return false;
      }

      auto _is_stopped{_is_stopped_maybe.value()};
      if (!_is_stopped) {
        TNCT_LOG_ERR(_logger, format::fmt("handling ", _handling_2,
                                          " is not stopped, but it should be"));
        return false;
      }
      TNCT_LOG_TST(_logger,
                   format::fmt("handling ", _handling_2, "'s condition is ",
                               (_is_stopped ? "stopped" : "not stopped")));

      return true;
    } catch (std::exception &_ex) {
      TNCT_LOG_ERR(_logger, _ex.what());
    }
    return false;
  }
};

struct dispatcher_006 {
  static std::string desc() { return "Stops all handlings of an event"; }
  bool operator()(const program::options &) {
    logger _logger;
    _logger.set_deb();
    try {
      dispatcher _dispatcher{_logger};

      auto _handler_1a = [&](event_1 &&) {};

      auto _handling_id1a_maybe{_dispatcher.subscribe<event_1>(
          std::move(_handler_1a), queue_1{_logger})};

      if (!_handling_id1a_maybe) {
        TNCT_LOG_ERR(_logger, "handling 1 not created, but it should");
        return false;
      }

      auto _handling_id1a{_handling_id1a_maybe.value()};

      TNCT_LOG_TST(_logger, format::fmt("_handling_1a = ", _handling_id1a));

      auto _handler_1b = [&](event_1 &&) {};

      auto _handling_id1b_maybe{_dispatcher.subscribe<event_1>(
          std::move(_handler_1b), queue_1{_logger})};

      if (!_handling_id1b_maybe) {
        TNCT_LOG_ERR(_logger, "handling 1 not created, but it should");
        return false;
      }

      auto _handling_id1b{_handling_id1b_maybe.value()};

      TNCT_LOG_TST(_logger, format::fmt("_handling_1b = ", _handling_id1b));

      auto _is_running{
          !(_dispatcher.is_stopped<event_1>(_handling_id1a).value())};

      if (!_is_running) {
        TNCT_LOG_ERR(_logger, format::fmt("handling ", _handling_id1a,
                                          " is stopped, but it should not be"));
        return false;
      }

      _is_running = !_dispatcher.is_stopped<event_1>(_handling_id1b).value();
      if (!_is_running) {
        TNCT_LOG_ERR(_logger, format::fmt("handling ", _handling_id1b,
                                          " is stopped, but it should not be"));
        return false;
      }

      _dispatcher.stop<event_1>();

      _is_running = !_dispatcher.is_stopped<event_1>(_handling_id1a).value();

      if (_is_running) {
        TNCT_LOG_ERR(_logger, format::fmt("handling ", _handling_id1a,
                                          " is running, but it should not be"));
        return false;
      }

      _is_running = !_dispatcher.is_stopped<event_1>(_handling_id1b).value();

      if (_is_running) {
        TNCT_LOG_ERR(_logger, format::fmt("handling ", _handling_id1b,
                                          " is running, but it should not be"));
        return false;
      }

    } catch (std::exception &_ex) {
      TNCT_LOG_ERR(_logger, _ex.what());
    }
    return true;
  }
};

struct dispatcher_007 {
  static std::string desc() {
    return "Publish an event and checks for the modified value of the event "
           "attribute";
  }

  bool operator()(const program::options &) {
    try {
      logger _logger;
      dispatcher _dispatcher(_logger);

      event_1 _event;

      TNCT_LOG_TST(_logger, format::fmt("event was ", _event));

      auto _handler{[&](event_1 &&p_event) {
        TNCT_LOG_TST(_logger, format::fmt("event 1 = ", p_event));
        _event = p_event;
      }};

      auto _handling_1{
          _dispatcher.subscribe<event_1>(std::move(_handler), queue_1{_logger})
              .value()};

      TNCT_LOG_TST(_logger, format::fmt("handling 1 = ", _handling_1));

      _dispatcher.publish(event_1{53});

      std::this_thread::sleep_for(100ms);

      TNCT_LOG_TST(_logger, format::fmt("now event is ", _event));

    } catch (...) {
    }

    return true;
  }
};

struct dispatcher_008 {
  static std::string desc() {
    return "Publish an event passing the paramenter, and checks for the "
           "modified value of the event "
           "attribute";
  }

  bool operator()(const program::options &) {
    try {
      logger _logger;
      dispatcher _dispatcher(_logger);

      event_1 _event;

      TNCT_LOG_TST(_logger, format::fmt("event was ", _event));

      auto _handler{[&](event_1 &&p_event) {
        TNCT_LOG_TST(_logger, format::fmt("event 1 = ", p_event));
        _event = p_event;
      }};

      auto _handling_1{
          _dispatcher.subscribe<event_1>(std::move(_handler), queue_1{_logger})
              .value()};

      TNCT_LOG_TST(_logger, format::fmt("handling 1 = ", _handling_1));

      _dispatcher.publish<event_1>(53);

      std::this_thread::sleep_for(100ms);

      TNCT_LOG_TST(_logger, format::fmt("now event is ", _event));

    } catch (...) {
    }

    return true;
  }
};

struct dispatcher_011 {
  static std::string desc() { return ""; }

  bool operator()(const program::options &) {
    using dispatcher =
        async::dispatcher<logger, event_a, event_b, event_c, event_d>;

    logger _logger;

    dispatcher _dispatcher(_logger);

    foo(_dispatcher);

    return false;
  }

private:
  struct event_a {
    event_a() = default;
    event_a(const event_a &) = default;
    event_a(event_a &&) = default;
    friend std::ostream &operator<<(std::ostream &p_out, const event_a &) {
      return p_out;
    }
  };

  struct event_b {
    event_b() = default;
    event_b(const event_b &) = default;
    event_b(event_b &&) = default;
    friend std::ostream &operator<<(std::ostream &p_out, const event_b &) {
      return p_out;
    }
  };

  struct event_c {
    event_c() = default;
    event_c(const event_c &) = default;
    event_c(event_c &&) = default;
    friend std::ostream &operator<<(std::ostream &p_out, const event_c &) {
      return p_out;
    }
  };
  struct event_d {
    event_d() = default;
    event_d(const event_d &) = default;
    event_d(event_d &&) = default;
    friend std::ostream &operator<<(std::ostream &p_out, const event_d &) {
      return p_out;
    }
  };

  using logger = log::cerr;

private:
  template <traits::dispatcher<logger, event_a, event_b, event_c> t_dispatcher>
  void foo(t_dispatcher &) {}
};

struct dispatcher_012 {
  static std::string desc() { return "Testing 'traits::dispatcher'"; }

  bool operator()(const program::options &) {

    // the code below does not compile

    // logger _logger;

    // dispatcher _dispatcher(_logger);

    // foo<dispatcher>(_dispatcher);

    return false;
  }

private:
  struct event_a {
    friend std::ostream &operator<<(std::ostream &p_out, const event_a &) {
      return p_out;
    }
  };

  struct event_b {
    friend std::ostream &operator<<(std::ostream &p_out, const event_b &) {
      return p_out;
    }
  };

  struct event_c {
    friend std::ostream &operator<<(std::ostream &p_out, const event_c &) {
      return p_out;
    }
  };
  struct event_d {
    friend std::ostream &operator<<(std::ostream &p_out, const event_d &) {
      return p_out;
    }
  };
  struct event_e {
    friend std::ostream &operator<<(std::ostream &p_out, const event_e &) {
      return p_out;
    }
  };

  using logger = log::cerr;
  using dispatcher = async::dispatcher<logger, event_a, event_b, event_d>;

private:
  template <traits::dispatcher<logger, event_a, event_b, event_c, event_e>
                t_dispatcher>

  // requires(std::tuple_size_v<dispatcher::events> >=
  //          std::tuple_size_v<std::tuple<event_a, event_b, event_c, event_e>>)

  void foo(t_dispatcher &) {}
};

// struct dispatcher_009 {
//   static std::string desc() {
//     return "Executes test configured in a .ini file passed as the parameter "
//            "'--ini_file'. The configuration of the file can be found in the "
//            "'tenacitas.lib/async/test/dispatcher.h' file, inside "
//            "'dispatcher_009' class.";
//   }

//   /*
// [PUBLISHER_1]
// Use=True
// IntervalInMilliSec=200
// NumEvents=10000

// [PUBLISHER_2]
// Use=True
// IntervalInMilliSec=1000
// NumEvents=300

// [PUBLISHER_3]
// Use=False
// IntervalInMilliSec=1000
// NumEvents=300

// [PUBLISHER_4]
// Use=True
// IntervalInMilliSec=1000
// NumEvents=300

// [PUBLISHER_5]
// Use=False
// IntervalInMilliSec=1000
// NumEvents=300

// [HANDLING_1]
// Use=True
// NumSubscribers=5
// SleepToFakeWorkInMilliSec=300

// [HANDLING_2]
// Use=True
// NumSubscribers=1
// SleepToFakeWorkInMilliSec=5000

// [HANDLING_3]
// Use=False
// NumSubscribers=10
// SleepToFakeWorkInMilliSec=250

// [HANDLING_4]
// Use=True
// NumSubscribers=5
// SleepToFakeWorkInMilliSec=2500

// [HANDLING_5]
// Use=True
// NumSubscribers=3
// SleepToFakeWorkInMilliSec=1000
// */

//   bool operator()(const program::options &p_options) {
//     logger _logger;

//     try {
//       _logger.set_deb();

//       // ini file
//       auto
//       _ini_file_name{p_options.get_single_param("ini_file").value_or("")}; if
//       (_ini_file_name.empty()) {
//         TNCT_LOG_ERR(_logger, "'ini_file' parameter not found");
//         return false;
//       }

//       std::optional<std::pair<publishers_cfg, handlings_cfg>> _maybe_cfs{
//           load_cfgs(_ini_file_name, _logger)};

//       if (!_maybe_cfs) {
//         TNCT_LOG_ERR(_logger,
//                      "could not load pubishers and handlings configuration");
//         return true;
//       }

//       auto _cfgs{std::move(_maybe_cfs.value())};

//       auto _publishers_cfg{std::move(_cfgs.first)};

//       auto _handlings_cfg{std::move(_cfgs.second)};

//       print_cfg(_publishers_cfg, _handlings_cfg, _logger);

//       const size_t _num_events_to_be_published{
//           calculate_num_events_to_be_published(_logger, _publishers_cfg)};

//       const size_t
//       _num_events_to_be_handled{calculate_num_events_to_be_handled(
//           _logger, _publishers_cfg, _handlings_cfg)};

//       size_t _amount_events_published{0};
//       size_t _amount_events_handled{0};

//       TNCT_LOG_TST(_logger, format::fmt("num events to be published = ",
//                                         _num_events_to_be_published,
//                                         ", num_events_to_be_handled = ",
//                                         _num_events_to_be_handled));

//       dispatcher _dispatcher(_logger);

//       subscribe(_dispatcher, _logger, _handlings_cfg);

//       publishers _publishers;

//       std::condition_variable _cond;
//       std::mutex _mutex;

//       handle_events_published(_dispatcher, _logger, _cond,
//                               _amount_events_published,
//                               _num_events_to_be_published, _publishers);

//       events_handled _events_handled;

//       handle_events_handled(_dispatcher, _logger, _amount_events_handled,
//                             _num_events_to_be_handled, _cond,
//                             _events_handled);

//       auto _start{std::chrono::high_resolution_clock::now()};
//       publish(_dispatcher, _publishers, _logger, _publishers_cfg);

//       std::unique_lock<std::mutex> _lock{_mutex};
//       _cond.wait(_lock, [&]() {
//         if ((_amount_events_handled >= _num_events_to_be_handled) &&
//             (_amount_events_published >= _num_events_to_be_published)) {
//           return true;
//         }
//         return false;
//       });

//       auto _finish{std::chrono::high_resolution_clock::now()};

//       TNCT_LOG_TST(_logger,
//                    format::fmt("total time = ", (_finish - _start).count()));

//       TNCT_LOG_TST(_logger,
//                    format::fmt(_num_events_to_be_handled,
//                                " events were to be handled, and ",
//                                _amount_events_handled, " events were
//                                handled"));

//       report(_events_handled, _logger);
//       return true;
//     } catch (std::exception &_ex) {
//       TNCT_LOG_ERR(_logger, _ex.what());
//     }

//     return false;
//   }

// private:
//   static constexpr size_t m_limit{5};

//   struct ev_a {
//     ev_a() {}
//     ev_a(const ev_a &) = default;
//     ev_a(ev_a &) = default;
//     ev_a &operator=(const ev_a &) = default;
//     ev_a &operator=(ev_a &) = default;
//     ~ev_a() = default;

//     friend std::ostream &operator<<(std::ostream &p_out, const ev_a &) {
//       p_out << "ev_a";
//       return p_out;
//     }
//   };

//   struct ev_b {
//     ev_b() {}
//     ev_b(const ev_b &) = default;
//     ev_b(ev_b &) = default;
//     ev_b &operator=(const ev_b &) = default;
//     ev_b &operator=(ev_b &) = default;
//     ~ev_b() = default;

//     friend std::ostream &operator<<(std::ostream &p_out, const ev_b &) {
//       p_out << "ev_b";
//       return p_out;
//     }
//   };

//   struct ev_c {
//     ev_c() {}
//     ev_c(const ev_c &) = default;
//     ev_c(ev_c &) = default;
//     ev_c &operator=(const ev_c &) = default;
//     ev_c &operator=(ev_c &) = default;
//     ~ev_c() = default;

//     friend std::ostream &operator<<(std::ostream &p_out, const ev_c &) {
//       p_out << "ev_c";
//       return p_out;
//     }
//   };

//   struct ev_d {
//     ev_d() {}
//     ev_d(const ev_d &) = default;
//     ev_d(ev_d &) = default;
//     ev_d &operator=(const ev_d &) = default;
//     ev_d &operator=(ev_d &) = default;
//     ~ev_d() = default;

//     friend std::ostream &operator<<(std::ostream &p_out, const ev_d &) {
//       p_out << "ev_d";
//       return p_out;
//     }
//   };

//   struct ev_e {
//     ev_e() {}
//     ev_e(const ev_e &) = default;
//     ev_e(ev_e &) = default;
//     ev_e &operator=(const ev_e &) = default;
//     ev_e &operator=(ev_e &) = default;
//     ~ev_e() = default;

//     friend std::ostream &operator<<(std::ostream &p_out, const ev_e &) {
//       p_out << "ev_e";
//       return p_out;
//     }
//   };

//   struct event_published {
//     friend std::ostream &operator<<(std::ostream &p_out,
//                                     const event_published &) {
//       return p_out;
//     }
//   };

//   using handling_name = std::string;
//   using handler_id = generic::id<0>;

//   struct event_handled {
//     event_handled(handling_name p_handling_id) : handling(p_handling_id) {}

//     event_handled() = default;
//     event_handled(const event_handled &) = default;
//     event_handled(event_handled &&) = default;
//     event_handled &operator=(const event_handled &) = default;
//     event_handled &operator=(event_handled &&) = default;

//     friend std::ostream &operator<<(std::ostream &p_out,
//                                     const event_handled &) {
//       return p_out;
//     }

//     handling_name handling;
//   };

//   using events_handled = std::map<handling_name, size_t>;

//   using events = std::tuple<ev_a, ev_b, ev_c, ev_d, ev_e>;

//   using dispatcher = async::dispatcher<logger, ev_a, ev_b, ev_c, ev_d, ev_e,
//                                        event_published, event_handled>;

//   struct publisher_cfg {
//     publisher_cfg() = default;
//     publisher_cfg(bool p_use, std::chrono::milliseconds p_interval,
//                   size_t p_num_events)
//         : use(p_use), interval(p_interval), num_events(p_num_events) {}

//     publisher_cfg(const publisher_cfg &p_publisher_cfg) = default;

//     publisher_cfg(publisher_cfg &&p_publisher_cfg) = default;

//     publisher_cfg &operator=(const publisher_cfg &) = default;
//     publisher_cfg &operator=(publisher_cfg &&) = default;

//     ~publisher_cfg() = default;

//     friend std::ostream &operator<<(std::ostream &p_out,
//                                     const publisher_cfg &p_publisher_cfg) {
//       p_out << "id = " << p_publisher_cfg.id
//             << ", interval (ms) = " << p_publisher_cfg.interval.count()
//             << ", # events = " << p_publisher_cfg.num_events << ", used? "
//             << (p_publisher_cfg.use ? 'T' : 'F');
//       return p_out;
//     }

//     std::string id;
//     bool use{false};
//     std::chrono::milliseconds interval{std::chrono::milliseconds::zero()};
//     size_t num_events{0};
//   };

//   struct handling_cfg {
//     handling_cfg() = default;
//     handling_cfg(std::string_view p_id, bool p_use, size_t p_num_subscribers,
//                  std::chrono::milliseconds p_sleep)
//         : id(p_id), use(p_use), num_subscribers(p_num_subscribers),
//           sleep(p_sleep) {}

//     handling_cfg(const handling_cfg &p_handling_cfg) = default;

//     handling_cfg(handling_cfg &&p_handling_cfg) = default;

//     handling_cfg &operator=(const handling_cfg &) = default;
//     handling_cfg &operator=(handling_cfg &&) = default;

//     ~handling_cfg() = default;

//     friend std::ostream &operator<<(std::ostream &p_out,
//                                     const handling_cfg &p_handling_cfg) {
//       p_out << "id = " << p_handling_cfg.id
//             << ", # subscribers = " << p_handling_cfg.num_subscribers
//             << ", sleep = " << p_handling_cfg.sleep.count() << ", used? "
//             << (p_handling_cfg.use ? 'T' : 'F');
//       return p_out;
//     }

//     std::string id{0};
//     bool use{false};
//     size_t num_subscribers{0};
//     std::chrono::milliseconds sleep{std::chrono::milliseconds::zero()};
//   };

//   using ini_file = parser::ini_file<logger>;
//   using publishers_cfg = std::array<publisher_cfg, m_limit>;
//   using handlings_cfg = std::array<handling_cfg, m_limit>;

//   template <traits::event t_event> struct handler {
//     handler(dispatcher &p_dispatcher, std::chrono::milliseconds p_sleep,
//             handling_name p_handling_idx)
//         : m_dispatcher(p_dispatcher), m_sleep(p_sleep),
//           m_handling_name(p_handling_idx) {}

//     void operator()(t_event && /*p_event*/) {
//       std::this_thread::sleep_for(m_sleep);
//       m_dispatcher.publish(event_handled{m_handling_name});
//     }

//   private:
//     dispatcher &m_dispatcher;
//     std::chrono::milliseconds m_sleep{std::chrono::milliseconds::zero()};
//     handling_name m_handling_name;
//   };

//   using amounts_events_published = std::array<size_t, m_limit>;

//   template <traits::event t_event> struct publisher {};

//   using sleeping_loop = async::sleeping_loop<logger>;
//   using sleeping_loop_ptr = std::unique_ptr<sleeping_loop>;

//   using publishers = std::array<sleeping_loop_ptr, m_limit>;

// private:
//   void handle_events_published(dispatcher &p_dispatcher, logger &p_logger,
//                                std::condition_variable &p_cond,
//                                size_t &p_amounts_events_published,
//                                size_t p_num_events_to_be_published,
//                                publishers &p_publishers) {

//     auto _handle_event_published = [&](auto &&) {
//       if (p_amounts_events_published >= p_num_events_to_be_published) {
//         stop_publishing(p_publishers);
//         p_cond.notify_all();
//         return;
//       }
//       ++p_amounts_events_published;
//     };

//     using event_published_queue =
//         container::circular_queue<logger, event_published, 100>;

//     auto _result{p_dispatcher.subscribe<event_published,
//     event_published_queue,
//                                         decltype(_handle_event_published)>(
//         std::move(_handle_event_published),
//         event_published_queue{p_logger})};

//     TNCT_LOG_TST(p_logger, format::fmt("handling id for event published = ",
//                                        _result.value()));
//   }

//   void report(const events_handled &p_events_handled, logger &p_logger) {
//     std::stringstream _stream;
//     _stream << '\n';
//     for (const events_handled::value_type &_value : p_events_handled) {
//       _stream << "handling " << _value.first << ", handled = " <<
//       _value.second
//               << " events" << std::endl;
//     }
//     TNCT_LOG_TST(p_logger, _stream.str());
//   }

//   std::optional<std::pair<publishers_cfg, handlings_cfg>>
//   load_cfgs(const std::string &p_ini_file_name, logger &p_logger) {

//     ini_file _ini_file(p_logger);
//     auto _maybe_ini_file_sections{_ini_file.read(p_ini_file_name)};
//     if (!_maybe_ini_file_sections) {
//       TNCT_LOG_ERR(p_logger, format::fmt("error loading ", p_ini_file_name));
//       return std::nullopt;
//     }

//     auto _ini_file_section{std::move(_maybe_ini_file_sections.value())};

//     handlings_cfg _handlings_cfg;
//     publishers_cfg _publishers_cfg;

//     size_t _next_publisher(0);
//     size_t _next_handling(0);
//     for (const auto &_section_value : _ini_file_section) {
//       if (_section_value.first.starts_with("PUBLISHER")) {
//         if (_next_publisher == 5 /*_publishers_cfg.size()*/) {
//           TNCT_LOG_ERR(
//               p_logger,
//               format::fmt("at most ", _publishers_cfg.size(),
//                           " PUBLISHER sections are allowed in ini file"));
//           return std::nullopt;
//         }
//         _publishers_cfg[_next_publisher] =
//         read_publisher_cfg(_section_value); _next_publisher++;
//       } else if (_section_value.first.starts_with("HANDLING")) {
//         if (_next_handling == 5 /*_handlings_cfg.size()*/) {
//           TNCT_LOG_ERR(
//               p_logger,
//               format::fmt("at most ", _handlings_cfg.size(),
//                           " HANDLING sections are allowed in ini file"));
//           return std::nullopt;
//         }

//         _handlings_cfg[_next_handling] = read_handling_cfg(_section_value);
//         _next_handling++;
//       } else {
//         TNCT_LOG_ERR(p_logger, format::fmt('\'', _section_value.first,
//                                            "' is not a valid section name"));
//         return std::nullopt;
//       }
//     }
//     return {
//         std::make_pair(std::move(_publishers_cfg),
//         std::move(_handlings_cfg))};
//   }

//   handling_cfg
//   read_handling_cfg(const ini_file::sections::value_type &p_section_value) {
//     handling_cfg _handling_cfg;
//     _handling_cfg.id = p_section_value.first;
//     for (const auto &_property : p_section_value.second) {
//       if (_property.first == "Use") {
//         if (_property.second == "True") {
//           _handling_cfg.use = true;
//         }
//       } else if (_property.first == "SleepToFakeWorkInMilliSec") {
//         _handling_cfg.sleep = decltype(handling_cfg::sleep)(
//             static_cast<size_t>(std::stoi(_property.second)));
//       } else if (_property.first == "NumSubscribers") {
//         _handling_cfg.num_subscribers =
//             static_cast<decltype(handling_cfg::num_subscribers)>(
//                 std::stoi(_property.second));
//       }
//     }
//     return _handling_cfg;
//   }

//   publisher_cfg
//   read_publisher_cfg(const ini_file::sections::value_type &p_section_value) {
//     publisher_cfg _publisher_cfg;
//     _publisher_cfg.id = p_section_value.first;
//     for (const auto &_property : p_section_value.second) {
//       if (_property.first == "Use") {
//         if (_property.second == "True") {
//           _publisher_cfg.use = true;
//         }
//       } else if (_property.first == "IntervalInMilliSec") {
//         _publisher_cfg.interval = decltype(publisher_cfg::interval)(
//             static_cast<size_t>(std::stoi(_property.second)));
//       } else if (_property.first == "NumEvents") {
//         _publisher_cfg.num_events =
//             static_cast<decltype(publisher_cfg::num_events)>(
//                 std::stoi(_property.second));
//       }
//     }
//     return _publisher_cfg;
//   }

//   template <size_t t_index>
//   void subscribe(dispatcher &p_dispatcher, logger &p_logger,
//                  const handlings_cfg &p_handlings_cfg) {
//     if (!p_handlings_cfg[t_index].use) {
//       TNCT_LOG_INF(p_logger,
//                    format::fmt("handler ", t_index, " will be not used"));
//       return;
//     }
//     using event_type = std::tuple_element_t<t_index, events>;
//     using handler_type = handler<event_type>;
//     using queue_type = container::circular_queue<logger, event_type, 50>;

//     handler_type _handler{p_dispatcher, p_handlings_cfg[t_index].sleep,
//                           p_handlings_cfg[t_index].id};

//     auto _handling_id{p_dispatcher.subscribe<event_type>(
//         std::move(_handler), async::handling_priority::medium,
//         queue_type{p_logger}, p_handlings_cfg[t_index].num_subscribers)};

//     TNCT_LOG_TST(p_logger, format::fmt("handling id for event ",
//                                        typeid(event_type).name(), " = ",
//                                        _handling_id.value()));
//   }

//   void subscribe(dispatcher &p_dispatcher, logger &p_logger,
//                  const handlings_cfg &p_handlings_cfg) {
//     subscribe<0>(p_dispatcher, p_logger, p_handlings_cfg);
//     subscribe<1>(p_dispatcher, p_logger, p_handlings_cfg);
//     subscribe<2>(p_dispatcher, p_logger, p_handlings_cfg);
//     subscribe<3>(p_dispatcher, p_logger, p_handlings_cfg);
//     subscribe<4>(p_dispatcher, p_logger, p_handlings_cfg);
//   }

//   size_t calculate_num_events_to_be_published(
//       logger &p_logger, const publishers_cfg &p_publishers_cfg) const {
//     size_t _result{0};
//     for (const auto &_cfg : p_publishers_cfg) {
//       if (_cfg.use) {
//         TNCT_LOG_TST(p_logger,
//                      format::fmt("publihser ", _cfg.id, ", will publish ",
//                                  _cfg.num_events, " events"));
//         _result += _cfg.num_events;
//       }
//     }
//     return _result;
//   }

//   template <size_t t_index>
//   void publish(dispatcher &p_dispatcher, publishers &p_publishers,
//                logger &p_logger, const publishers_cfg &p_publishers_cfg) {
//     using sleeping_loop = async::sleeping_loop<logger>;
//     using event_type = std::tuple_element_t<t_index, events>;

//     if (!p_publishers_cfg[t_index].use) {
//       TNCT_LOG_INF(p_logger,
//                    format::fmt("publisher ", t_index, " will be not used"));
//       return;
//     }

//     auto _publisher = [&]() {
//       p_dispatcher.publish(event_type{});
//       p_dispatcher.publish(event_published{});
//     };

//     p_publishers[t_index] = std::make_unique<sleeping_loop>(
//         p_logger, _publisher, p_publishers_cfg[t_index].interval);

//     p_publishers[t_index]->start();
//   }

//   void publish(dispatcher &p_dispatcher, publishers &p_publishers,
//                logger &p_logger, const publishers_cfg &p_publishers_cfg) {
//     publish<0>(p_dispatcher, p_publishers, p_logger, p_publishers_cfg);
//     publish<1>(p_dispatcher, p_publishers, p_logger, p_publishers_cfg);
//     publish<2>(p_dispatcher, p_publishers, p_logger, p_publishers_cfg);
//     publish<3>(p_dispatcher, p_publishers, p_logger, p_publishers_cfg);
//     publish<4>(p_dispatcher, p_publishers, p_logger, p_publishers_cfg);
//   }

//   void stop_publishing(publishers &p_publishers) {
//     for (auto &_publisher : p_publishers) {
//       _publisher->stop();
//     }
//   }

//   void print_cfg(const publishers_cfg &p_publishers_cfg,
//                  const handlings_cfg &p_handlings_cfg, logger &p_logger) {
//     std::stringstream _stream;
//     _stream << '\n';
//     for (const auto &_publisher_cfg : p_publishers_cfg) {
//       _stream << _publisher_cfg << '\n';
//     }
//     p_logger.tst(_stream.str());

//     _stream.str("");
//     _stream << '\n';
//     for (const auto &_handling_cfg : p_handlings_cfg) {
//       _stream << _handling_cfg << '\n';
//     }
//     p_logger.tst(_stream.str());
//   }

//   void handle_events_handled(dispatcher &p_dispatcher, logger &p_logger,
//                              size_t &p_amount_events_handled,
//                              const size_t &p_num_events_to_be_handled,
//                              std::condition_variable &p_cond,
//                              events_handled &p_events_handled) {
//     auto _handle_event_handled = [&](auto &&p_event_handled) {
//       ++p_amount_events_handled;

//       events_handled::iterator _ite{
//           p_events_handled.find(p_event_handled.handling)};
//       if (_ite != p_events_handled.end()) {
//         ++_ite->second;
//       } else {
//         p_events_handled.insert({p_event_handled.handling, 1});
//       }

//       if (p_amount_events_handled >= p_num_events_to_be_handled) {
//         p_cond.notify_all();
//       }
//     };

//     using event_handled_queue =
//         container::circular_queue<logger, event_handled, 100>;

//     auto _result{p_dispatcher.subscribe<event_handled, event_handled_queue,
//                                         decltype(_handle_event_handled)>(
//         std::move(_handle_event_handled), event_handled_queue{p_logger})};

//     TNCT_LOG_TST(p_logger, format::fmt("handling id for event handled = ",
//                                        _result.value()));
//   }

//   size_t calculate_num_events_to_be_handled(
//       logger &p_logger, const publishers_cfg &p_publishers_cfg,
//       const handlings_cfg &p_handlings_cfg) const {
//     size_t _result{0};

//     const size_t _cfgs{p_publishers_cfg.size()};

//     for (size_t _i = 0; _i < _cfgs; ++_i) {
//       if (p_handlings_cfg[_i].use && p_publishers_cfg[_i].use) {
//         const auto _aux{p_publishers_cfg[_i].num_events *
//                         p_handlings_cfg[_i].num_subscribers};
//         TNCT_LOG_TST(p_logger, format::fmt("handling ",
//         p_handlings_cfg[_i].id,
//                                            " will handled ", _aux, "
//                                            events"));
//         _result += _aux;
//       }
//     }
//     return _result;
//   }
// };

// struct dispatcher_010 {
//   static std::string desc() { return "dispatcher_010"; }

//   bool operator()(const program::options &p_options) {
//     logger _logger;

//     try {
//       _logger.set_deb();

//       // ini file
//       auto
//       _ini_file_name{p_options.get_single_param("ini_file").value_or("")}; if
//       (_ini_file_name.empty()) {
//         TNCT_LOG_ERR(_logger, "'ini_file' parameter not found");
//         return false;
//       }

//       publishers_cfg _publishers_cfg;
//       handlings_cfg _handlings_cfg;
//       {
//         std::optional<std::pair<publishers_cfg, handlings_cfg>> _maybe_cfg{
//             read_cfg(_logger, _ini_file_name)};
//         if (!_maybe_cfg) {
//           TNCT_LOG_ERR(_logger, format::fmt("error reading ",
//           _ini_file_name)); return false;
//         }

//         std::pair<publishers_cfg, handlings_cfg>
//         _pair_cfg{_maybe_cfg.value()};

//         _publishers_cfg = std::move(_pair_cfg.first);
//         _handlings_cfg = std::move(_pair_cfg.second);
//       }

//       print_cfg(_logger, _publishers_cfg, _handlings_cfg);

//       return true;
//     } catch (std::exception &_ex) {
//       TNCT_LOG_ERR(_logger, _ex.what());
//     }
//     return false;
//   }

// private:
//   using logger = log::cerr;

//   // struct ev_a {
//   //   ev_a() {}
//   //   ev_a(const ev_a &) = default;
//   //   ev_a(ev_a &) = default;
//   //   ev_a &operator=(const ev_a &) = default;
//   //   ev_a &operator=(ev_a &) = default;
//   //   ~ev_a() = default;

//   //   friend std::ostream &operator<<(std::ostream &p_out, const ev_a &) {
//   //     p_out << "ev_a";
//   //     return p_out;
//   //   }
//   // };

//   // struct ev_b {
//   //   ev_b() {}
//   //   ev_b(const ev_b &) = default;
//   //   ev_b(ev_b &) = default;
//   //   ev_b &operator=(const ev_b &) = default;
//   //   ev_b &operator=(ev_b &) = default;
//   //   ~ev_b() = default;

//   //   friend std::ostream &operator<<(std::ostream &p_out, const ev_b &) {
//   //     p_out << "ev_b";
//   //     return p_out;
//   //   }
//   // };

//   // struct ev_c {
//   //   ev_c() {}
//   //   ev_c(const ev_c &) = default;
//   //   ev_c(ev_c &) = default;
//   //   ev_c &operator=(const ev_c &) = default;
//   //   ev_c &operator=(ev_c &) = default;
//   //   ~ev_c() = default;

//   //   friend std::ostream &operator<<(std::ostream &p_out, const ev_c &) {
//   //     p_out << "ev_c";
//   //     return p_out;
//   //   }
//   // };

//   // struct ev_d {
//   //   ev_d() {}
//   //   ev_d(const ev_d &) = default;
//   //   ev_d(ev_d &) = default;
//   //   ev_d &operator=(const ev_d &) = default;
//   //   ev_d &operator=(ev_d &) = default;
//   //   ~ev_d() = default;

//   //   friend std::ostream &operator<<(std::ostream &p_out, const ev_d &) {
//   //     p_out << "ev_d";
//   //     return p_out;
//   //   }
//   // };

//   // struct ev_e {
//   //   ev_e() {}
//   //   ev_e(const ev_e &) = default;
//   //   ev_e(ev_e &) = default;
//   //   ev_e &operator=(const ev_e &) = default;
//   //   ev_e &operator=(ev_e &) = default;
//   //   ~ev_e() = default;

//   //   friend std::ostream &operator<<(std::ostream &p_out, const ev_e &) {
//   //     p_out << "ev_e";
//   //     return p_out;
//   //   }
//   // };

//   using handling_id = generic::id<0>;
//   using publisher_id = generic::id<1>;
//   using handler_id = generic::id<2>;

//   // template <traits::event t_event> struct event_handled {
//   //   event_handled() = default;
//   //   event_handled(const handling_id &p_handling, const handler_id
//   &p_handler)
//   //       : handling(p_handling), handler(p_handler) {}

//   //   friend std::ostream &operator<<(std::ostream &p_out,
//   //                                   const event_handled &p_event_handled)
//   {
//   //     p_out << '(' << p_event_handled.handling << ',' <<
//   //     p_event_handled.handler
//   //           << ')';
//   //     return p_out;
//   //   }

//   //   handling_id handling;
//   //   handler_id handler;
//   // };

//   // struct totals {
//   //   totals(logger &p_logger) : m_logger(p_logger) {}

//   //   template <traits::event t_event>
//   //   void increment(const handling_cfg_id &p_handling_cfg_id,
//   //                  const handler_id &p_handler_id) {
//   //     using event_total_type = event_total<t_event>;

//   //     constexpr auto _idx{
//   //         traits::tuple_find<events_totals, event_total_type>()};
//   //     if constexpr (_idx == -1) {
//   //       TNCT_LOG_ERR(m_logger,
//   //                    format::fmt("could not find event_total for event ",
//   //                                typeid(t_event).name()));
//   //       return;
//   //     }

//   //     event_total_type &_event_total{std::get<_idx>(m_events_totals)};

//   //     handling_total &_handling_total{_event_total.second};

//   //     handling_total::iterator _handling_total_ite{
//   //         _handling_total.find(p_handling_cfg_id)};
//   //     if (_handling_total_ite == _handling_total.end()) {
//   //       TNCT_LOG_ERR(m_logger,
//   //                    format::fmt("could not find handling_total for event
//   ",
//   //                                typeid(t_event).name(), " and handling ",
//   //                                p_handling_cfg_id));

//   //       return;
//   //     }

//   //     handler_total &_handler_total{_handling_total_ite->second};
//   //     handler_total::iterator _handler_total_ite{
//   //         _handler_total.find(p_handler_id)};

//   //     if (_handler_total_ite == _handler_total.end()) {
//   //       TNCT_LOG_ERR(m_logger,
//   //                    format::fmt("could not find handler_total for event
//   ",
//   //                                typeid(t_event).name(), ", handling ",
//   //                                p_handling_cfg_id, " and handler ",
//   //                                p_handler_id));

//   //       return;
//   //     }

//   //     ++_handler_total_ite->second;
//   //   }

//   // private:
//   //   using handler_total = std::map<handler_id, size_t>;
//   //   using handling_total = std::map<handling_cfg_id, handler_total>;
//   //   template <traits::event t_event>
//   //   using event_total = std::map<t_event, handling_total>;

//   //   using events_totals =
//   //       std::tuple<event_total<ev_a>, event_total<ev_b>,
//   event_total<ev_c>,
//   //                  event_total<ev_d>, event_total<ev_e>>;

//   // private:
//   //   logger &m_logger;
//   //   events_totals m_events_totals;
//   // };

//   // using dispatcher = async::dispatcher<logger, ev_a, ev_b, ev_c, ev_d,
//   ev_e,
//   //                                      event_handled<ev_a>,
//   //                                      event_handled<ev_b>,
//   //                                      event_handled<ev_c>,
//   //                                      event_handled<ev_d>,
//   //                                      event_handled<ev_e>>;

//   struct publisher_cfg {
//     publisher_cfg(bool p_used, std::chrono::milliseconds p_interval,
//                   size_t p_num_events)
//         : use(p_used), interval(p_interval), num_events(p_num_events) {}
//     publisher_cfg() = default;
//     publisher_cfg(const publisher_cfg &) = default;
//     publisher_cfg(publisher_cfg &&) = default;
//     publisher_cfg &operator=(const publisher_cfg &) = default;
//     publisher_cfg &operator=(publisher_cfg &&) = default;

//     friend std::ostream &operator<<(std::ostream &p_out,
//                                     const publisher_cfg &p_publisher_cfg) {
//       p_out << '(' << p_publisher_cfg.id << ','
//             << (p_publisher_cfg.use ? 'Y' : 'N') << ',' <<
//             p_publisher_cfg.event
//             << ',' << p_publisher_cfg.interval.count() << "ms,"
//             << p_publisher_cfg.num_events << ')';
//       return p_out;
//     }

//     publisher_id id;
//     bool use{false};
//     std::string event;
//     std::chrono::milliseconds interval{80ms};
//     size_t num_events{0};
//   };

//   using publishers_cfg = std::vector<publisher_cfg>;

//   struct handling_cfg {
//     handling_cfg() = default;
//     handling_cfg(bool p_use, std::string_view p_event, size_t p_num_handlers,
//                  std::chrono::milliseconds p_sleep)
//         : use(p_use), event(p_event), num_handlers(p_num_handlers),
//           sleep(p_sleep) {}

//     handling_cfg(const handling_cfg &p_handling_cfg) = default;

//     handling_cfg(handling_cfg &&p_handling_cfg) = default;

//     handling_cfg &operator=(const handling_cfg &) = default;
//     handling_cfg &operator=(handling_cfg &&) = default;

//     ~handling_cfg() = default;

//     friend std::ostream &operator<<(std::ostream &p_out,
//                                     const handling_cfg &p_handling_cfg) {
//       p_out << '(' << p_handling_cfg.id << ',' << p_handling_cfg.event << ','
//             << p_handling_cfg.num_handlers << ','
//             << p_handling_cfg.sleep.count() << ','
//             << (p_handling_cfg.use ? 'T' : 'F') << ')';
//       return p_out;
//     }

//     handling_id id;
//     bool use{false};
//     std::string event;
//     size_t num_handlers{0};
//     std::chrono::milliseconds sleep{std::chrono::milliseconds::zero()};
//   };

//   // using publishers_cfg =
//   //     std::tuple<publisher_cfg<ev_a>, publisher_cfg<ev_b>,
//   //     publisher_cfg<ev_c>,
//   //                publisher_cfg<ev_d>, publisher_cfg<ev_e>>;
//   using handlings_cfg = std::vector<handling_cfg>;

//   // template <traits::event t_event> struct handler {
//   //   handler(dispatcher &p_dispather, logger &p_logger,
//   //           const handling_id &p_handling_id)
//   //       : m_dispather(p_dispather), m_logger(p_logger),
//   //         handling_id(p_handling_id) {}

//   //   void operator()(t_event &&) {
//   //     m_dispather.publish<t_event>(handling_id, id);
//   //   }

//   // private:
//   //   dispatcher &m_dispather;
//   //   logger &m_logger;

//   //   handling_id handling_id;
//   //   handler_id id;
//   // };
//   using ini_file = parser::ini_file<logger>;

// private:
//   std::optional<std::pair<publishers_cfg, handlings_cfg>>
//   read_cfg(logger &p_logger, std::string_view p_ini_file_name) {
//     ini_file _ini_file(p_logger);

//     auto _maybe_ini_file_sections{_ini_file.read(p_ini_file_name)};
//     if (!_maybe_ini_file_sections) {
//       TNCT_LOG_ERR(p_logger, format::fmt("error loading ", p_ini_file_name));
//       return std::nullopt;
//     }

//     auto _ini_file_section{std::move(_maybe_ini_file_sections.value())};

//     handlings_cfg _handlings_cfg;
//     publishers_cfg _publishers_cfg;

//     for (const auto &_section_value : _ini_file_section) {
//       if (_section_value.first.starts_with("PUBLISHER")) {
//         _publishers_cfg.push_back(read_publisher_cfg(_section_value));
//       } else if (_section_value.first.starts_with("HANDLING")) {
//         _handlings_cfg.push_back(read_handling_cfg(_section_value));
//       } else {
//         TNCT_LOG_ERR(p_logger, format::fmt('\'', _section_value.first,
//                                            "' is not a valid section name"));
//         return std::nullopt;
//       }
//     }
//     return {
//         std::make_pair(std::move(_publishers_cfg),
//         std::move(_handlings_cfg))};
//   }

//   handling_cfg
//   read_handling_cfg(const ini_file::sections::value_type &p_section_value) {
//     handling_cfg _handling_cfg;

//     for (const auto &_property : p_section_value.second) {
//       if ((_property.first == "Use") && (_property.second == "True")) {
//         _handling_cfg.use = true;
//       } else if (_property.first == "Event") {
//         _handling_cfg.event = _property.second;
//       } else if (_property.first == "NumSubscribers") {
//         _handling_cfg.num_handlers =
//             static_cast<decltype(handling_cfg::num_handlers)>(
//                 std::stoi(_property.second));
//       } else if (_property.first == "SleepToFakeWorkInMilliSec") {
//         _handling_cfg.sleep = decltype(handling_cfg::sleep)(
//             static_cast<size_t>(std::stoi(_property.second)));
//       }
//     }
//     return _handling_cfg;
//   }

//   publisher_cfg
//   read_publisher_cfg(const ini_file::sections::value_type &p_section_value) {
//     publisher_cfg _publisher_cfg;

//     for (const auto &_property : p_section_value.second) {
//       if ((_property.first == "Use") && (_property.second == "True")) {
//         _publisher_cfg.use = true;
//       } else if (_property.first == "Event") {
//         _publisher_cfg.event = _property.second;
//       } else if (_property.first == "IntervalInMilliSec") {
//         _publisher_cfg.interval = decltype(publisher_cfg::interval)(
//             static_cast<size_t>(std::stoi(_property.second)));
//       } else if (_property.first == "NumEvents") {
//         _publisher_cfg.num_events =
//             static_cast<decltype(publisher_cfg::num_events)>(
//                 std::stoi(_property.second));
//       }
//     }
//     return _publisher_cfg;
//   }

//   void print_cfg(logger &p_logger, const publishers_cfg &p_publishers_cfg,
//                  const handlings_cfg &p_handlings_cfg) {
//     std::stringstream _stream;
//     _stream << '\n';
//     for (const publisher_cfg &_publisher_cfg : p_publishers_cfg) {
//       _stream << _publisher_cfg << std::endl;
//     }
//     _stream << '\n';
//     for (const handling_cfg &_handling_cfg : p_handlings_cfg) {
//       _stream << _handling_cfg << std::endl;
//     }
//     TNCT_LOG_TST(p_logger, _stream.str());
//   }
// };
#endif
