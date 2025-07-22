/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_TST_ASYNC_DISPATCHER_H
#define TENACITAS_TST_ASYNC_DISPATCHER_H

#include <iostream>
#include <string>

#include "tenacitas/src/async/dispatcher.h"
#include "tenacitas/src/async/handling_definition.h"
#include "tenacitas/src/async/result.h"
#include "tenacitas/src/container/circular_queue.h"
#include "tenacitas/src/format/fmt.h"
#include "tenacitas/src/log/cerr.h"
#include "tenacitas/src/program/options.h"
#include "tenacitas/src/traits/async/dispatcher.h"
#include "tenacitas/src/traits/log/logger.h"

using namespace tenacitas;
using namespace std::chrono_literals;

using logger = src::log::cerr;

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

using queue_1 = src::container::circular_queue<logger, event_1, 10>;

using queue_2 = src::container::circular_queue<logger, event_2, 5>;

// using handling = src::async::handling<logger, event_1, queue, handler>;
using dispatcher = src::async::dispatcher<logger, event_1, event_2>;

struct dispatcher_000 {
  static std::string desc() { return "Adding a handling and getting its id"; }

  bool operator()(const src::program::options &) {
    logger _logger;
    _logger.set_deb();

    dispatcher _dispatcher{_logger};

    auto _handler = [](event_1 &&) mutable {};

    using handling_1 =
        src::async::handling_definition<event_1, decltype(_handler), queue_1>;

    handling_1 _handling_1{"handling-000", std::move(_handler),
                           queue_1{_logger}, 0};

    auto _result{_dispatcher.add_handling<handling_1>(std::move(_handling_1))};

    if (_result != src::async::result::OK) {
      TNCT_LOG_ERR(_logger, src::format::fmt("result = ", _result));
      return false;
    }

    return true;
  }
};

struct dispatcher_001 {
  static std::string desc() {
    return "Checks the number of handlers in a handling";
  }

  bool operator()(const src::program::options &) {
    logger _logger;
    _logger.set_deb();

    dispatcher _dispatcher{_logger};

    src::async::handling_id _handling_id{"handling-001"};

    auto _handler = [](event_1 &&) mutable {};

    using handling_1 =
        src::async::handling_definition<event_1, decltype(_handler), queue_1>;

    auto _result{_dispatcher.add_handling<handling_1>(
        {_handling_id, std::move(_handler), queue_1{_logger}, 1})};

    if (_result != src::async::result::OK) {
      TNCT_LOG_ERR(_logger, src::format::fmt("result = ", _result));
      return false;
    }

    auto _amount_handlers_maybe{
        _dispatcher.get_amount_handlers<event_1>(_handling_id)};

    if (!_amount_handlers_maybe) {
      TNCT_LOG_ERR(
          _logger,
          src::format::fmt(
              "not possible to get the amount of handlers for handling ",
              _handling_id));
    }

    auto _amount_handlers{_amount_handlers_maybe.value()};
    TNCT_LOG_TST(_logger,
                 src::format::fmt("amount of handlers = ", _amount_handlers));

    return _amount_handlers == 1;
  }
};

struct dispatcher_002 {
  static std::string desc() { return "Checks the number of handlers"; }

  bool operator()(const src::program::options &) {
    logger _logger;
    _logger.set_deb();

    dispatcher _dispatcher{_logger};

    src::async::handling_id _handling_id{"handling-002"};

    auto _handler = [](event_1 &&) mutable {};

    using handling_1 =
        src::async::handling_definition<event_1, decltype(_handler), queue_1>;

    auto _result{_dispatcher.add_handling(
        handling_1{_handling_id, std::move(_handler), queue_1{_logger}, 4})};

    if (_result != src::async::result::OK) {
      TNCT_LOG_ERR(_logger, src::format::fmt("result = ", _result));
      return false;
    }

    TNCT_LOG_TST(_logger, src::format::fmt("handling id = ", _handling_id));

    auto _amount_handlers_maybe{
        _dispatcher.get_amount_handlers<event_1>(_handling_id)};

    if (!_amount_handlers_maybe) {
      TNCT_LOG_ERR(
          _logger,
          src::format::fmt(
              "not possible to get the amount of handlers for handling ",
              _handling_id));
    }

    auto _amount_handlers{_amount_handlers_maybe.value()};
    TNCT_LOG_TST(_logger,
                 src::format::fmt("amount of handlers = ", _amount_handlers));

    if (_amount_handlers != 4) {
      TNCT_LOG_ERR(_logger,
                   src::format::fmt("amount of handlers should be 1, but it is ",
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

  bool operator()(const src::program::options &) {
    logger _logger;
    try {
      _logger.set_deb();

      dispatcher _dispatcher{_logger};

      auto _handler_1 = [](event_1 &&) mutable {};
      using handling_1 =
          src::async::handling_definition<event_1, decltype(_handler_1), queue_1>;
      auto _result{_dispatcher.add_handling(handling_1{
          "handling-003-1", std::move(_handler_1), queue_1{_logger}, 0})};

      if (_result != src::async::result::OK) {
        TNCT_LOG_ERR(_logger, src::format::fmt("result = ", _result));
        return false;
      }

      TNCT_LOG_TST(_logger, "passed 1st subscribe");
      auto _handler_2 = [](event_1 &&) mutable {};
      using handling_2 =
          src::async::handling_definition<event_1, decltype(_handler_2), queue_1>;
      _result = _dispatcher.add_handling<handling_2>(
          {"handling-003-2", std::move(_handler_2), queue_1{_logger}, 0});

      if (_result != src::async::result::ERROR_HANDLER_ALREADY_IN_USE) {
        TNCT_LOG_ERR(_logger, src::format::fmt("result = ", _result));
        return false;
      }

      return true;
    } catch (std::exception &_ex) {
      TNCT_LOG_ERR(_logger, _ex.what());
    }

    return false;
  }
};

struct dispatcher_007 {
  static std::string desc() {
    return "Publish an event and checks for the modified value of the event "
           "attribute";
  }

  bool operator()(const src::program::options &) {
    try {
      logger _logger;
      dispatcher _dispatcher(_logger);

      event_1 _event;

      TNCT_LOG_TST(_logger, src::format::fmt("event was ", _event));

      auto _handler_1 = [&](event_1 &&p_event) mutable { _event = p_event; };
      using handling_1 =
          src::async::handling_definition<event_1, decltype(_handler_1), queue_1>;

      auto _result{_dispatcher.add_handling<handling_1>(
          {"handling-007", std::move(_handler_1), queue_1{_logger}, 1})};

      if (_result != src::async::result::OK) {
        TNCT_LOG_ERR(_logger, src::format::fmt("result = ", _result));
        return false;
      }

      _result = _dispatcher.publish(event_1{53});
      if (_result != src::async::result::OK) {
        TNCT_LOG_ERR(_logger, src::format::fmt(_result));
        return false;
      }

      std::this_thread::sleep_for(100ms);

      TNCT_LOG_TST(_logger, src::format::fmt("now event is ", _event));

    } catch (...) {
    }

    return true;
  }
};

struct dispatcher_008 {
  static std::string desc() {
    return "Publish an event passing the parameter, and checks for the "
           "modified value of the event attribute";
  }

  bool operator()(const src::program::options &) {
    try {
      logger _logger;
      dispatcher _dispatcher(_logger);

      event_1 _event;

      TNCT_LOG_TST(_logger, src::format::fmt("event was ", _event));

      auto _handler_1 = [&](event_1 &&p_event) mutable { _event = p_event; };
      using handling_1 =
          src::async::handling_definition<event_1, decltype(_handler_1), queue_1>;

      auto _result{_dispatcher.add_handling<handling_1>(
          {"handling-008", std::move(_handler_1), queue_1{_logger}, 1})};

      if (_result != src::async::result::OK) {
        TNCT_LOG_ERR(_logger, src::format::fmt("result = ", _result));
        return false;
      }

      _result =
          _dispatcher.publish<event_1>(static_cast<decltype(event_1::i)>(53));
      if (_result != src::async::result::OK) {
        TNCT_LOG_ERR(_logger, src::format::fmt(_result));
        return false;
      }

      std::this_thread::sleep_for(100ms);

      TNCT_LOG_TST(_logger, src::format::fmt("now event is ", _event));

    } catch (...) {
    }

    return true;
  }

private:
  struct handler {
    handler(event_1 &p_event) : m_event(p_event) {}

    using events_handled = std::tuple<event_1>;

    void operator()(event_1 &&p_event) { m_event = p_event; }

  private:
    event_1 &m_event;
  };
};

struct dispatcher_011 {
  static std::string desc() {
    return "A method depends on a src::traits::async::dispatcher with 3 "
           "events, and the concrete src::async::dispatcher is defined with "
           "those 3 and 1 more";
  }

  bool operator()(const src::program::options &) {
    using dispatcher =
        src::async::dispatcher<logger, event_a, event_b, event_c, event_d>;

    logger _logger;

    dispatcher _dispatcher(_logger);

    foo(_dispatcher);

    return true;
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

  using logger = src::log::cerr;

private:
  template <src::traits::async::dispatcher<event_a, event_b, event_c, event_d>
                t_dispatcher>
  void foo(t_dispatcher &) {}
};

struct dispatcher_012 {
  static std::string desc() { return "Testing 'src::traits::async::dispatcher'"; }

  bool operator()(const src::program::options &) {

    // the code below does not compile

    // logger _logger;

    // dispatcher _dispatcher(_logger);

    // foo<dispatcher>(_dispatcher);

    return true;
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

  using logger = src::log::cerr;
  using dispatcher = src::async::dispatcher<logger, event_a, event_b, event_d>;

private:
  template <src::traits::async::dispatcher<event_a, event_b, event_c, event_e>
                t_dispatcher>

  // requires(std::tuple_size_v<dispatcher::events> >=
  //          std::tuple_size_v<std::tuple<event_a, event_b, event_c,
  //          event_e>>)

  void foo(t_dispatcher &) {}
};

#endif
