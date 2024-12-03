/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_TEST_ASYNC_DISPATCHER_H
#define TENACITAS_LIB_TEST_ASYNC_DISPATCHER_H

#include <iostream>
#include <string>

#ifndef TENACITAS_LOG
#define TENACITAS_LOG
#endif

#include <tenacitas.lib/async/dispatcher.h>
#include <tenacitas.lib/async/handling_priority.h>
#include <tenacitas.lib/async/internal/handler_id.h>
#include <tenacitas.lib/async/sleeping_loop.h>

#include <tenacitas.lib/container/circular_queue.h>
#include <tenacitas.lib/format/fmt.h>
#include <tenacitas.lib/log/cerr.h>
#include <tenacitas.lib/parser/ini_file.h>
#include <tenacitas.lib/program/options.h>
#include <tenacitas.lib/traits/handler.h>
#include <tenacitas.lib/traits/logger.h>

using namespace tenacitas::lib;
using namespace std::chrono_literals;

using logger = log::cerr;

struct event_1 {
  event_1(int16_t p_i = -9) : i(p_i) {}
  friend std::ostream &operator<<(std::ostream &out, const event_1 &p_event) {
    out << "i = " << p_event.i;
    return out;
  }

  int16_t i;
};

struct event_2 {
  event_2(float p_f = 3.14f) : f(p_f) {}
  friend std::ostream &operator<<(std::ostream &out, const event_2 &p_event) {
    out << "f = " << p_event.f;
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

    if (!_dispatcher.increment_handlers<event_1>(_handling_id, 3)) {
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

      auto _is_stopped_maybe{_dispatcher.is_stopped<event_1>(_handling_2)};
      if (!_is_stopped_maybe) {
        TNCT_LOG_ERR(_logger, format::fmt("could not retrieve if handling ",
                                          _handling_2, " is stopped"));
        return false;
      }

      auto _is_stopped{_is_stopped_maybe.value()};
      if (_is_stopped) {
        TNCT_LOG_ERR(_logger, format::fmt("handling ", _handling_2,
                                          " is stopped, but it should not be"));
        return false;
      }
      TNCT_LOG_TST(_logger, format::fmt("handling ", _handling_2,
                                        "'s condition is ", _is_stopped));

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

      TNCT_LOG_TST(_logger, format::fmt("_handling_1b= ", _handling_id1b));

      auto _is_running{!_dispatcher.is_stopped<event_1>(_handling_id1a)};

      if (!_is_running) {
        TNCT_LOG_ERR(_logger, format::fmt("handling ", _handling_id1a,
                                          " is stopped, but it should not be"));
        return false;
      }

      _is_running = !_dispatcher.is_stopped<event_1>(_handling_id1b);
      if (!_is_running) {
        TNCT_LOG_ERR(_logger, format::fmt("handling ", _handling_id1b,
                                          " is stopped, but it should not be"));
        return false;
      }

      _dispatcher.stop<event_1>();

      _is_running = !_dispatcher.is_stopped<event_1>(_handling_id1a);

      if (_is_running) {
        TNCT_LOG_ERR(_logger, format::fmt("handling ", _handling_id1a,
                                          " is running, but it should not be"));
        return false;
      }

      _is_running = !_dispatcher.is_stopped<event_1>(_handling_id1b);

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

// struct dispatcher_test {

// protected:
//   using logger = log::cerr;

//   struct event_1 {
//     friend std::ostream &operator<<(std::ostream &p_out, const event_1 &)
//     {
//       return p_out;
//     }
//   };

//   struct handler_1a {
//     using event = event_1;
//     void operator()(event && /*p_event*/) {
//       std::cout << "handler 1a" << std::endl;
//     }
//   };

//   using queue_1a = container::circular_queue<logger, event_1, 10>;

//   using handling_1a =
//       async::handling<logger, event_1, queue_1a, handler_1a>;

//   struct handler_1b {
//     using event = event_1;
//     void operator()(event &&) {}
//   };

//   using queue_1b = container::circular_queue<logger, event_1, 100>;

//   using handling_1b =
//       async::handling<logger, event_1, queue_1b, handler_1b>;

//   struct event_2 {
//     friend std::ostream &operator<<(std::ostream &p_out, const event_2 &)
//     {
//       return p_out;
//     }
//   };

//   struct handler_2a {
//     using event = event_2;
//     void operator()(event &&) {}
//   };

//   using queue_2a = container::circular_queue<logger, event_2, 10>;

//   using handling_2a =
//       async::handling<logger, event_2, queue_2a, handler_2a>;

// protected:
//   logger m_logger;

// }; // namespace dispatcher_test

// struct dispatcher_000 : dispatcher_test {
//   static std::string desc() { return ""; }

//   bool operator()(const program::options &) {
//     using dispatcher = async::dispatcher<logger, event_1>;

//     m_logger.set_deb();

//     dispatcher _dispatcher{m_logger};
//     async::result _result{async::result::OK};

//     m_handling_1a.increment_handlers(1U);

//     // auto _notifier{[this](const event_1 &p_event) {
//     //   return m_handling_1a.add_event(p_event);
//     // }};
//     // _dispatcher.add_handling<event_1>(_notifier);

//     // handling_1a _handling_1a{"handling_1a", m_logger, handler_1a{}};
//     _dispatcher.add_handling<event_1>(m_handling_1a);

//     event_1 _event_1;
//     _dispatcher.publish(_event_1);

//     std::this_thread::sleep_for(1s);

//     return _result == async::result::OK;
//   }

// private:
//   handling_1a m_handling_1a{"handling_1a", m_logger, handler_1a{}};
// };

// struct dispatcher_test {

//   struct ev_0 {
//     ev_0() = default;

//     ev_0(uint16_t p_value) : value(p_value) {}

//     friend std::ostream &operator<<(std::ostream &p_out, const ev_0
//     &p_ev) {
//       p_out << "ev_0 = " << p_ev.value;
//       return p_out;
//     }

//     uint16_t value;
//   };

//   struct ev_1 {

//     ev_1() = default;
//     ev_1(char p_value) : value(p_value) {}

//     friend std::ostream &operator<<(std::ostream &p_out, const ev_1
//     &p_ev) {
//       p_out << "ev_1 = " << p_ev.value;
//       return p_out;
//     }

//     char value;
//   };

//   using logger = log::cerr;

//   template <traits::logger t_logger, traits::event t_event>
//   using queue = container::circular_queue<t_logger, t_event>;

//   logger m_logger;

//   using dispatcher = async::dispatcher<logger, ev_0, ev_1>;
// };

// struct dispatcher_000 : public dispatcher_test {
//   static std::string desc() { return "Declaring a 'dispatcher'"; }

//   bool operator()(const program::options &) {

//     m_logger.set_tra();
//     m_logger.tst("starting test_000");
//     dispatcher _dispatcher(m_logger);

//     m_logger.tst("finishing test_000");
//     return true;
//   }

// private:
//   struct subscriber {
//     using events_subscribed = std::tuple<ev_1>;

//     void handle(ev_1 &&) {}
//   };
// };

// struct dispatcher_001 : public dispatcher_test {
//   static std::string desc() {
//     return "Declaring a 'dispatcher' and subscring one subscriber ";
//   }

//   bool operator()(const program::options &) {

//     m_logger.set_tra();
//     m_logger.tst("starting test_001");
//     dispatcher _dispatcher(m_logger);

//     auto _result = _dispatcher.subscribe<subscriber, ev_1>(
//         "1", m_subscriber, async::handling_priority::medium, 1);
//     return _result == async::result::OK;
//   }

// private:
//   struct subscriber {
//     using events_subscribed = std::tuple<ev_1>;

//     void handle(ev_1 &&) {}
//   };

//   subscriber m_subscriber;
// };

// struct dispatcher_002 : public dispatcher_test {
//   static std::string desc() {
//     return "Declaring a 'dispatcher' and publishing one event";
//   }

//   bool operator()(const program::options &) {

//     m_logger.set_tra();
//     m_logger.tst("starting test_001");
//     dispatcher _dispatcher(m_logger);

//     auto _result(_dispatcher.publish<publisher, ev_1>(ev_1()));

//     return _result == async::result::OK;
//   }

// private:
//   struct publisher {
//     using events_published = std::tuple<ev_1>;
//   };
// };

// struct dispatcher_003 : public dispatcher_test {
//   static std::string desc() {
//     return "Declaring a 'dispatcher', adding two handlings with 2 "
//            "subscribers and clearing all";
//   }

//   bool operator()(const program::options &) {

//     m_logger.tst("starting test_001");
//     dispatcher _dispatcher(m_logger);

//     auto _result_1(_dispatcher.subscribe<subscriber_a, ev_1>(
//         "1", _subscriber_a, async::handling_priority::medium, 2));
//     if (_result_1 != async::result::OK) {
//       m_logger.err(generic::fmt("error: ", _result_1));
//       return false;
//     }

//     _result_1 = _dispatcher.subscribe<subscriber_b, ev_1>(
//         "2", _subscriber_b, async::handling_priority::medium, 2);
//     if (_result_1 != async::result::OK) {
//       m_logger.err(generic::fmt("error: ", _result_1));
//       return false;
//     }

//     return true;
//   }

// private:
//   struct subscriber_a {
//     using events_subscribed = std::tuple<ev_1>;

//     void handle(ev_1 &&) {}
//   };

//   struct subscriber_b {
//     using events_subscribed = std::tuple<ev_1>;

//     void handle(ev_1 &&) {}
//   };

//   subscriber_a _subscriber_a;

//   subscriber_b _subscriber_b;
// };

// struct dispatcher_004 : public dispatcher_test {
//   static std::string desc() {
//     return "Checking if 'clear' and 'clear(descrprition_id)' compile";
//   }

//   bool operator()(const program::options &) {

//     // m_logger.set_trace();
//     m_logger.tst("starting test_001");
//     dispatcher _dispatcher(m_logger);

//     _dispatcher.clear<ev_1>();

//     subscriber_a _subscriber_a;
//     auto _result(_dispatcher.subscribe<subscriber_a, ev_1>(
//         "1", _subscriber_a, async::handling_priority::medium, 1));
//     if (_result != async::result::OK) {
//       m_logger.err(generic::fmt("error: ", _result));
//       return false;
//     }

//     _dispatcher.clear<ev_1>("1");

//     return true;
//   }

// private:
//   struct subscriber_a {
//     using events_subscribed = std::tuple<ev_1>;

//     void handle(ev_1 &&) {}
//   };
// };

// struct dispatcher_005 : dispatcher_test {
//   static std::string desc() {
//     return "Checking if 'get_priority' and 'set_priority' compile";
//   }

//   bool operator()(const program::options &) {

//     // m_logger.set_trace();
//     m_logger.tst("starting test_001");
//     dispatcher _dispatcher(m_logger);

//     async::handling_id _id("1");

//     subscriber_a _subscriber_a;
//     auto _result(_dispatcher.subscribe<subscriber_a, ev_1>(
//         _id, _subscriber_a, async::handling_priority::medium, 1));
//     if (_result != async::result::OK) {
//       m_logger.err(generic::fmt("error: ", _result));
//       return false;
//     }

//     auto _priority(_dispatcher.get_priority<ev_1>(_id));
//     if (_priority != async::handling_priority::medium) {
//       m_logger.err(generic::fmt("pririty should be ",
//                                 async::handling_priority::medium, " but
//                                 it is
//                                 ", _priority));
//       return false;
//     }
//     _dispatcher.set_priority<ev_1>(_id, async::handling_priority::high);
//     _priority = _dispatcher.get_priority<ev_1>(_id);
//     if (_priority != async::handling_priority::high) {
//       m_logger.err(generic::fmt("pririty should be ",
//                                 async::handling_priority::high, " but it
//                                 is
//                                 ", _priority));
//       return false;
//     }

//     return true;
//   }

// private:
//   struct subscriber_a {
//     using events_subscribed = std::tuple<ev_1>;

//     void handle(ev_1 &&) {}
//   };
// };

// struct dispatcher_006 : dispatcher_test {
//   static std::string desc() {
//     return "Checking if 'get_num_events' and 'get_num_handling_handlers'
//     "
//            "'get_num_handlings' and 'clear' compile";
//   }

//   bool operator()(const program::options &) {

//     // m_logger.set_trace();
//     m_logger.tst("starting test_001");
//     dispatcher _dispatcher(m_logger);

//     async::handling_id _id("1");

//     subscriber_a _subscriber_a;
//     auto _result(
//         _dispatcher.subscribe<subscriber_a, ev_1>(_id, _subscriber_a,
//         1));
//     if (_result != async::result::OK) {
//       m_logger.err(generic::fmt("error: ", _result));
//       return false;
//     }

//     auto _num_events(_dispatcher.get_num_events<ev_1>(_id));
//     if (_num_events != 0) {
//       m_logger.err(
//           generic::fmt("num events should be 0, but it is ",
//           _num_events));
//       return false;
//     }

//     auto _num_handling_handlers(
//         _dispatcher.get_num_handling_handlers<ev_1>(_id).value());
//     if (_num_handling_handlers != 1) {
//       m_logger.err(generic::fmt("num handling handlers should be 1, but
//       it is
//       ",
//                                 _num_handling_handlers));
//       return false;
//     }

//     auto _num_handlings(_dispatcher.get_num_handlings<ev_1>());
//     if (_num_handlings != 1) {
//       m_logger.err(generic::fmt("num handlings should be 1, but it is ",
//                                 _num_handlings));
//       return false;
//     }

//     _dispatcher.clear<ev_1>(_id);

//     _dispatcher.clear<ev_1>();

//     _dispatcher.clear();

//     return true;
//   }

// private:
//   struct subscriber_a {
//     using events_subscribed = std::tuple<ev_1>;

//     void handle(ev_1 &&) {}
//   };
// };

// struct dispatcher_007 {

//   static std::string desc() {
//     return "Executes test configured in a .ini file passed as the
//     parameter "
//            "'--ini'. The configuration of the file can be found in the "
//            "'tenacitas.lib/async/test/dispatcher.h' file, inside "
//            "'dispatcher_007' class.";
//   }

//   /*
//   [PUBLISHER_1]
//   Use=True
//   IntervalInMilliSec=200
//   NumEvents=10000

//   [PUBLISHER_2]
//   Use=True
//   IntervalInMilliSec=1000
//   NumEvents=300

//   [PUBLISHER_3]
//   Use=False
//   IntervalInMilliSec=1000
//   NumEvents=300

//   [PUBLISHER_4]
//   Use=True
//   IntervalInMilliSec=1000
//   NumEvents=300

//   [PUBLISHER_5]
//   Use=False
//   IntervalInMilliSec=1000
//   NumEvents=300

//   [HANDLING_1]
//   Use=True
//   NumSubscribers=5
//   SleepToFakeWorkInMilliSec=300

//   [HANDLING_2]
//   Use=True
//   NumSubscribers=1
//   SleepToFakeWorkInMilliSec=5000

//   [HANDLING_3]
//   Use=False
//   NumSubscribers=10
//   SleepToFakeWorkInMilliSec=250

//   [HANDLING_4]
//   Use=True
//   NumSubscribers=5
//   SleepToFakeWorkInMilliSec=2500

//   [HANDLING_5]
//   Use=True
//   NumSubscribers=3
//   SleepToFakeWorkInMilliSec=1000
//   */

//   bool operator()(const program::options &p_options) {
//     logger _logger;
//     _logger.set_inf();

//     std::string
//     _ini_file_name{p_options.get_single_param("ini").value_or("")}; if
//     (_ini_file_name.empty()) {
//       _logger.err("parameter --ini expected");
//       return false;
//     }

//     amount_by_subscriber _amount_by_handling_by_subscriber;
//     dispatcher _dispatcher{_logger};
//     publishers _publishers;
//     subscribers _subscribers(
//         subscriber<0>(_dispatcher, _logger,
//         _amount_by_handling_by_subscriber), subscriber<1>(_dispatcher,
//         _logger, _amount_by_handling_by_subscriber),
//         subscriber<2>(_dispatcher, _logger,
//         _amount_by_handling_by_subscriber), subscriber<3>(_dispatcher,
//         _logger, _amount_by_handling_by_subscriber),
//         subscriber<4>(_dispatcher, _logger,
//         _amount_by_handling_by_subscriber));
//     publishers_cfg _publishers_cfg;
//     handlings_cfg _handlings_cfg;
//     std::condition_variable _cond;
//     std::mutex _mutex;
//     size_t _amount_events_handled(0);
//     event_handled_subscriber _event_handled_subscriber(
//         _cond, _amount_events_handled, _logger);

//     if (!configure(_ini_file_name, _logger, _subscribers, _publishers,
//                    _dispatcher, _publishers_cfg, _handlings_cfg)) {
//       return false;
//     }

//     auto _result(_dispatcher.subscribe<event_handled_subscriber,
//     event_handled>(
//         "ev-handled", _event_handled_subscriber, 1));
//     if (_result != async::result::OK) {
//       _logger.err(generic::fmt(_result));
//       return false;
//     }

//     size_t _amount_events_to_be_published(
//         calculate_amount_of_events_to_be_published(_publishers_cfg,
//                                                    _handlings_cfg));

//     for (auto &_publisher : _publishers) {
//       _publisher.start();
//     }

//     _logger.tst(generic::fmt("waiting for ",
//     _amount_events_to_be_published,
//                              " events to be handled"));
//     {
//       std::unique_lock<std::mutex> _lock(_mutex);
//       _cond.wait(_lock, [&]() -> bool {
//         return _amount_events_handled == _amount_events_to_be_published;
//       });
//     }

//     _logger.tst(generic::fmt(_amount_events_handled, " events were
//     handled"));

//     std::map<std::string, size_t> _amount_by_handling;
//     for (const auto &_handling_cfg : _handlings_cfg) {
//       _amount_by_handling.emplace(_handling_cfg.id, 0);
//     }

//     for (const auto &_value :
//     _amount_by_handling_by_subscriber.get_map()) {
//       auto _handling_id(_handlings_cfg[_value.first.first].id);
//       _logger.tst(generic::fmt("handling ", _handling_id, " subscriber ",
//                                _value.first.second, " handled ",
//                                _value.second, " events"));
//       _amount_by_handling[_handling_id] += _value.second;
//     }

//     for (const auto &_value : _amount_by_handling) {
//       _logger.tst(generic::fmt("handling ", _value.first, " handled ",
//                                _value.second, " events"));
//     }

//     return _amount_events_handled == _amount_events_to_be_published;
//   }

// private:
//   struct event_handled {
//     friend std::ostream &operator<<(std::ostream &p_out,
//                                     const event_handled &) {
//       p_out << "event_handled";
//       return p_out;
//     }
//   };

//   struct new_event {
//     friend std::ostream &operator<<(std::ostream &p_out, const new_event
//     &) {
//       p_out << "new event";
//       return p_out;
//     }
//   };

//   using logger = log::cerr;

//   using dispatcher = async::dispatcher<logger, new_event, event_handled>;

//   template <traits::logger t_logger, traits::event t_event>
//   using queue = container::circular_queue<t_logger, t_event>;

//   using sleeping_loop = async::sleeping_loop<logger>;

//   using ini_file = parser::ini_file<logger>;

//   struct publisher_cfg {
//     publisher_cfg() = default;
//     publisher_cfg(const publisher_cfg &) = delete;
//     publisher_cfg(publisher_cfg &&) = default;
//     publisher_cfg(std::string_view p_id, bool p_use,
//                   std::chrono::milliseconds p_interval, size_t
//                   p_num_events)
//         : id(p_id), use(p_use), interval(p_interval),
//         num_events(p_num_events) {
//     }

//     publisher_cfg &operator=(const publisher_cfg &) = delete;
//     publisher_cfg &operator=(publisher_cfg &&) = default;

//     friend std::ostream &operator<<(std::ostream &p_out,
//                                     const publisher_cfg &p_publisher_cfg)
//                                     {
//       p_out << "id = " << p_publisher_cfg.id
//             << ", interval (ms) = " << p_publisher_cfg.interval.count()
//             << ", # events = " << p_publisher_cfg.num_events << ", used?
//             "
//             << (p_publisher_cfg.use ? 'T' : 'F');
//       return p_out;
//     }

//     std::string id;
//     bool use = false;
//     std::chrono::milliseconds interval = 0ms;
//     size_t num_events = 0;
//   };

//   using publishers_cfg = std::vector<publisher_cfg>;

//   struct handling_cfg {
//     handling_cfg() = default;
//     handling_cfg(const handling_cfg &) = delete;
//     handling_cfg(handling_cfg &&) = default;

//     handling_cfg(std::string_view p_id, bool p_use, uint16_t
//     p_num_subscribers,
//                  std::chrono::milliseconds p_sleep)
//         : id(p_id), use(p_use), num_subscribers(p_num_subscribers),
//           sleep(p_sleep) {}

//     handling_cfg &operator=(const handling_cfg &) = delete;
//     handling_cfg &operator=(handling_cfg &&) = default;

//     friend std::ostream &operator<<(std::ostream &p_out,
//                                     const handling_cfg &p_handling_cfg) {
//       p_out << "id = " << p_handling_cfg.id
//             << ", # subscribers = " << p_handling_cfg.num_subscribers
//             << ", sleep = " << p_handling_cfg.sleep.count() << ", used? "
//             << (p_handling_cfg.use ? 'T' : 'F');
//       return p_out;
//     }

//     std::string id;
//     bool use = false;
//     uint16_t num_subscribers = std::numeric_limits<uint16_t>::max();
//     std::chrono::milliseconds sleep = 0ms;
//   };

//   using handlings_cfg = std::vector<handling_cfg>;

//   struct publisher {
//     using events_published = std::tuple<new_event>;

//     publisher(std::string_view p_id, logger &p_logger, dispatcher
//     &p_dispatcher,
//               size_t p_num_events, std::chrono::milliseconds p_interval)
//         : m_id(p_id), m_logger(p_logger), m_dispatcher(p_dispatcher),
//           m_num_events(p_num_events), m_interval(p_interval) {
//       // m_logger.tst(generic::fmt(
//       //     "publisher ", m_id, " will publish ", m_num_events,
//       //     " events at each ", p_interval.count(), " milliseconds"));
//     }

//     publisher(const publisher &) = delete;
//     publisher(publisher &&p_publisher)
//         : m_id(p_publisher.m_id), m_logger(p_publisher.m_logger),
//           m_dispatcher(p_publisher.m_dispatcher),
//           m_num_events(std::move(p_publisher.m_num_events)),
//           m_interval(std::move(p_publisher.m_interval)),
//           m_sleeping_loop(std::move(p_publisher.m_sleeping_loop)),
//           m_count_events(std::move(p_publisher.m_count_events)),
//           m_all_events_published(p_publisher.m_all_events_published) {}

//     publisher &operator=(const publisher &) = delete;
//     publisher &operator=(publisher &&) = delete;

//     void start() {
//       if (m_sleeping_loop) {
//         m_logger.tst("not starting sleeping_loop because it is started");
//         return;
//       }
//       m_sleeping_loop = std::make_unique<sleeping_loop>(
//           m_logger, [this]() { publish(); }, m_interval, "sl-" + m_id);

//       m_logger.tst(generic::fmt("publisher ", m_id, " starting"));
//       m_sleeping_loop->start();
//     }

//     constexpr bool all_events_published() const {
//       return m_all_events_published;
//     }

//     const std::string id() const { return m_id; }

//   private:
//     using sleeping_loop = async::sleeping_loop<logger>;

//   private:
//     void publish() {
//       if (m_count_events == m_num_events) {
//         m_logger.tst(generic::fmt(
//             "publisher ", m_id, " published ", m_count_events,
//             " events, and it was supposed to publish ", m_num_events));
//         m_all_events_published = true;
//         m_sleeping_loop->stop();
//         return;
//       }
//       auto _result(m_dispatcher.publish<publisher>(new_event()));
//       if (_result != async::result::OK) {
//         m_logger.err(generic::fmt(_result));
//       }

//       ++m_count_events;
//       if ((m_count_events % 10) == 0) {
//         m_logger.tst(generic::fmt("publisher ", m_id, " published event #
//         ",
//                                   m_count_events));
//       }
//     }

//   private:
//     std::string m_id;
//     logger &m_logger;
//     dispatcher &m_dispatcher;
//     size_t m_num_events{0};
//     std::chrono::milliseconds m_interval;

//     std::unique_ptr<sleeping_loop> m_sleeping_loop;
//     size_t m_count_events{0};
//     bool m_all_events_published{false};
//   };

//   using publishers = std::vector<publisher>;

//   struct amount_by_subscriber {
//     using map = std::map<std::pair<size_t, std::thread::id>, size_t>;

//     void increment(size_t p_handling_id, const std::thread::id
//     &p_subscriber)
//     {
//       std::lock_guard<std::mutex> _lock(m_mutex);
//       auto _ite(m_map.find(std::make_pair(p_handling_id, p_subscriber)));
//       if (_ite == m_map.end()) {
//         m_map.insert({std::make_pair(p_handling_id, p_subscriber), 1});
//       } else {
//         ++_ite->second;
//       }
//     }

//     const map &get_map() const { return m_map; }

//   private:
//     map m_map;
//     std::mutex m_mutex;
//   };

//   template <size_t t_id> struct subscriber {
//     using events_subscribed = std::tuple<new_event>;
//     using events_published = std::tuple<event_handled>;

//     subscriber(dispatcher &p_dispatcher, logger &p_logger,
//                amount_by_subscriber &p_amount_by_subscriber)
//         : m_dispatcher(p_dispatcher), m_logger(p_logger),
//           m_amount_by_subscriber(p_amount_by_subscriber) {}

//     subscriber(const subscriber &p_subscriber)
//         : m_dispatcher(p_subscriber.m_dispatcher),
//           m_logger(p_subscriber.m_logger),
//           m_amount_by_subscriber(p_subscriber.m_amount_by_subscriber) {}

//     void handle(new_event &&) {
//       std::this_thread::sleep_for(sleep);
//       auto _result(m_dispatcher.publish<subscriber>(event_handled()));
//       if (_result != async::result::OK) {
//         m_logger.err(generic::fmt(_result));
//       }

//       m_amount_by_subscriber.increment(t_id, std::this_thread::get_id());
//     }

//     std::chrono::milliseconds sleep = 0ms;

//   private:
//     dispatcher &m_dispatcher;
//     logger &m_logger;
//     amount_by_subscriber &m_amount_by_subscriber;
//   };

//   using subscribers = std::tuple<subscriber<0>, subscriber<1>,
//   subscriber<2>,
//                                  subscriber<3>, subscriber<4>>;

//   struct event_handled_subscriber {
//     using events_subscribed = std::tuple<event_handled>;

//     event_handled_subscriber(std::condition_variable &p_cond,
//                              size_t &p_amount_events_handled, logger
//                              &p_logger)
//         : m_cond(p_cond),
//         m_amount_events_handled(p_amount_events_handled),
//           m_logger(p_logger) {}

//     void handle(event_handled &&) {
//       ++m_amount_events_handled;
//       if ((m_amount_events_handled % 10) == 0) {
//         m_logger.tst(generic::fmt(m_amount_events_handled, " events
//         handled"));
//       }
//       m_cond.notify_all();
//     }

//   private:
//     std::condition_variable &m_cond;
//     size_t &m_amount_events_handled;
//     logger &m_logger;
//   };

// private:
//   std::optional<std::pair<publishers_cfg, handlings_cfg>>
//   read_cfg(std::string_view p_ini_file_name, logger &p_logger) {
//     ini_file _ini_file(p_logger);

//     std::optional<ini_file::sections> _maybe_sections(
//         _ini_file.read(p_ini_file_name));
//     if (!_maybe_sections) {
//       p_logger.err(generic::fmt("error reading '", p_ini_file_name,
//       '\'')); return std::nullopt;
//     }

//     ini_file::sections _sections(std::move(_maybe_sections.value()));

//     handlings_cfg _handlings_cfg;
//     publishers_cfg _publishers_cfg;

//     size_t _next_publisher(0);
//     size_t _next_handling(0);
//     for (const auto &_section_value : _sections) {
//       if (_section_value.first.starts_with("PUBLISHER")) {
//         if (_next_publisher == 5 /*_publishers_cfg.size()*/) {
//           p_logger.err(
//               generic::fmt("at most ", _publishers_cfg.size(),
//                            " PUBLISHER sections are allowed in ini
//                            file"));
//           return std::nullopt;
//         }
//         _publishers_cfg.push_back(read_publisher_cfg(_section_value));
//         _next_publisher++;
//       } else if (_section_value.first.starts_with("HANDLING")) {
//         if (_next_handling == 5 /*_handlings_cfg.size()*/) {
//           p_logger.err(
//               generic::fmt("at most ", _handlings_cfg.size(),
//                            " HANDLING sections are allowed in ini
//                            file"));
//           return std::nullopt;
//         }

//         _handlings_cfg.push_back(read_handling_cfg(_section_value));
//         _next_handling++;
//       } else {
//         p_logger.err(generic::fmt('\'', _section_value.first,
//                                   "' is not a valid section name"));
//         return std::nullopt;
//       }
//     }
//     return {
//         std::make_pair(std::move(_publishers_cfg),
//         std::move(_handlings_cfg))};
//   }

//   handling_cfg
//   read_handling_cfg(const ini_file::sections::value_type
//   &p_section_value) {
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
//   read_publisher_cfg(const ini_file::sections::value_type
//   &p_section_value) {
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

//   void print_cfg(const publishers_cfg &p_publishers_cfg,
//                  const handlings_cfg &p_handlings_cfg, logger &p_logger)
//                  {
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

//   void create_publishers(const publishers_cfg &p_publishers_cfg,
//                          publishers &p_publishers, dispatcher
//                          &p_dispatcher, logger &p_logger) {
//     for (const publisher_cfg &_publisher_cfg : p_publishers_cfg) {
//       if (_publisher_cfg.use) {
//         publisher _publisher(
//             _publisher_cfg.id, p_logger, p_dispatcher,
//             _publisher_cfg.num_events,
//             std::chrono::milliseconds(_publisher_cfg.interval));

//         p_publishers.push_back(std::move(_publisher));
//       }
//     }
//   }

//   bool create_subscribers(const handlings_cfg &p_handlings_cfg,
//                           subscribers &p_subscribers, dispatcher
//                           &p_dispatcher, logger &p_logger) {
//     for (size_t _i = 0; _i < p_handlings_cfg.size(); ++_i) {
//       if (p_handlings_cfg[_i].use) {
//         async::handling_id _handling_id(p_handlings_cfg[_i].id.c_str());
//         async::result _result(async::result::OK);
//         switch (_i) {
//         case 0: {
//           std::get<0>(p_subscribers).sleep = p_handlings_cfg[_i].sleep;
//           _result = p_dispatcher.subscribe<subscriber<0>, new_event>(
//               _handling_id, std::get<0>(p_subscribers),
//               async::handling_priority::medium,
//               p_handlings_cfg[_i].num_subscribers);
//           if (_result != async::result::OK) {
//             p_logger.err(generic::fmt(_result));
//             return false;
//           }
//         } break;
//         case 1: {
//           std::get<1>(p_subscribers).sleep = p_handlings_cfg[_i].sleep;
//           _result = p_dispatcher.subscribe<subscriber<1>, new_event>(
//               _handling_id, std::get<1>(p_subscribers),
//               async::handling_priority::medium,
//               p_handlings_cfg[_i].num_subscribers);
//           if (_result != async::result::OK) {
//             p_logger.err(generic::fmt(_result));
//             return false;
//           }
//         } break;
//         case 2: {
//           std::get<2>(p_subscribers).sleep = p_handlings_cfg[_i].sleep;
//           _result = p_dispatcher.subscribe<subscriber<2>, new_event>(
//               _handling_id, std::get<2>(p_subscribers),
//               async::handling_priority::medium,
//               p_handlings_cfg[_i].num_subscribers);
//           if (_result != async::result::OK) {
//             p_logger.err(generic::fmt(_result));
//             return false;
//           }
//         } break;
//         case 3: {
//           std::get<3>(p_subscribers).sleep = p_handlings_cfg[_i].sleep;
//           _result = p_dispatcher.subscribe<subscriber<3>, new_event>(
//               _handling_id, std::get<3>(p_subscribers),
//               async::handling_priority::medium,
//               p_handlings_cfg[_i].num_subscribers);
//           if (_result != async::result::OK) {
//             p_logger.err(generic::fmt(_result));
//             return false;
//           }
//         } break;
//         case 4: {
//           std::get<4>(p_subscribers).sleep = p_handlings_cfg[_i].sleep;
//           _result = p_dispatcher.subscribe<subscriber<4>, new_event>(
//               _handling_id, std::get<4>(p_subscribers),
//               async::handling_priority::medium,
//               p_handlings_cfg[_i].num_subscribers);
//           if (_result != async::result::OK) {
//             p_logger.err(generic::fmt(_result));
//             return false;
//           }
//         } break;
//         }
//       }
//     }
//     return true;
//   }

//   bool configure(std::string_view p_ini_file_name, logger &p_logger,
//                  subscribers &p_subscribers, publishers &p_publishers,
//                  dispatcher &p_dispatcher, publishers_cfg
//                  &p_publishers_cfg, handlings_cfg &p_handlings_cfg) {
//     auto _maybe(read_cfg(p_ini_file_name, p_logger));
//     if (!_maybe) {
//       return false;
//     }
//     auto _pair(std::move(_maybe.value()));
//     print_cfg(_pair.first, _pair.second, p_logger);

//     if (!create_subscribers(_pair.second, p_subscribers, p_dispatcher,
//                             p_logger)) {
//       return false;
//     }

//     create_publishers(_pair.first, p_publishers, p_dispatcher, p_logger);

//     p_publishers_cfg = std::move(_pair.first);
//     p_handlings_cfg = std::move(_pair.second);

//     return true;
//   }

//   size_t calculate_amount_of_events_to_be_published(
//       const publishers_cfg &p_publishers_cfg,
//       const handlings_cfg &p_handlings_cfg) {
//     size_t _amount_events_to_be_published(0);
//     for (const auto &_publisher_cfg : p_publishers_cfg) {
//       if (_publisher_cfg.use) {
//         _amount_events_to_be_published += _publisher_cfg.num_events;
//       }
//     }

//     size_t _amount_handlings(0);
//     for (const auto &_handling_cfg : p_handlings_cfg) {
//       if (_handling_cfg.use) {
//         ++_amount_handlings;
//       }
//     }
//     return _amount_events_to_be_published * _amount_handlings;
//   }
// };

#endif
