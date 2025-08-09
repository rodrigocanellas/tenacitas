/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_TST_ASYNC_DISPATCHER_H
#define TENACITAS_TST_ASYNC_DISPATCHER_H

#include <iostream>
#include <string>

#include "tenacitas/src/async/dispatcher.h"
#include "tenacitas/src/async/result.h"
#include "tenacitas/src/async/traits/is_dispatcher.h"
#include "tenacitas/src/container/circular_queue.h"
#include "tenacitas/src/format/fmt.h"
#include "tenacitas/src/log/cerr.h"
#include "tenacitas/src/program/options.h"
#include "tenacitas/src/log/traits/logger.h"

using namespace tenacitas;
using namespace std::chrono_literals;

using logger = src::log::cerr;

namespace tenacitas::src::async::tst
{

struct event_1
{
  event_1(int16_t p_i = -9) : i(p_i)
  {
  }
  friend std::ostream &operator<<(std::ostream &out, const event_1 &p_event)
  {
    out << p_event.i;
    return out;
  }

  int16_t i;
};

struct event_2
{
  event_2(float p_f = 3.14f) : f(p_f)
  {
  }
  friend std::ostream &operator<<(std::ostream &out, const event_2 &p_event)
  {
    out << p_event.f;
    return out;
  }

  float f;
};

using queue_1 = src::container::circular_queue<logger, event_1, 10>;

using queue_2 = src::container::circular_queue<logger, event_2, 5>;

// using handling = src::async::handling<logger, event_1, queue, handler>;
using dispatcher = src::async::dispatcher<logger, event_1, event_2>;

struct dispatcher_000
{
  static std::string desc()
  {
    return "Adding a handling and getting its id";
  }

  bool operator()(const src::program::options &)
  {
    logger _logger;
    _logger.set_deb();

    dispatcher _dispatcher{_logger};

    auto _handler = [](event_1 &&) mutable {};

    auto _result{_dispatcher.add_handling<event_1, queue_1, decltype(_handler)>(
        "handling-000", queue_1{_logger}, std::move(_handler),
        src::async::handling_priority::medium, 0)};

    if (_result != src::async::result::OK)
    {
      TNCT_LOG_ERR(_logger, src::format::fmt("result = ", _result));
      return false;
    }

    return true;
  }
};

struct dispatcher_001
{
  static std::string desc()
  {
    return "Checks the number of handlers in a handling";
  }

  bool operator()(const src::program::options &)
  {
    logger _logger;
    _logger.set_deb();

    dispatcher _dispatcher{_logger};

    src::async::handling_name _handling_name{"handling-001"};

    auto _handler = [](event_1 &&) mutable {};

    auto _result{_dispatcher.add_handling<event_1>(
        _handling_name, queue_1{_logger}, std::move(_handler))};

    if (_result != src::async::result::OK)
    {
      TNCT_LOG_ERR(_logger, src::format::fmt("result = ", _result));
      return false;
    }

    auto _amount_handlers_maybe{
        _dispatcher.get_amount_handlers<event_1>(_handling_name)};

    if (!_amount_handlers_maybe)
    {
      TNCT_LOG_ERR(
          _logger,
          src::format::fmt(
              "not possible to get the amount of handlers for handling ",
              _handling_name));
    }

    auto _amount_handlers{_amount_handlers_maybe.value()};
    TNCT_LOG_TST(_logger,
                 src::format::fmt("amount of handlers = ", _amount_handlers));

    return _amount_handlers == 1;
  }
};

struct dispatcher_002
{
  static std::string desc()
  {
    return "Checks the number of handlers";
  }

  bool operator()(const src::program::options &)
  {
    logger _logger;
    _logger.set_deb();

    dispatcher _dispatcher{_logger};

    src::async::handling_name _handling_name{"handling-002"};

    auto _handler = [](event_1 &&) mutable {};

    auto _result{_dispatcher.add_handling<event_1>(
        _handling_name, queue_1{_logger}, std::move(_handler),
        src::async::handling_priority::medium, 4)};

    if (_result != src::async::result::OK)
    {
      TNCT_LOG_ERR(_logger, src::format::fmt("result = ", _result));
      return false;
    }

    TNCT_LOG_TST(_logger, src::format::fmt("handling id = ", _handling_name));

    auto _amount_handlers_maybe{
        _dispatcher.get_amount_handlers<event_1>(_handling_name)};

    if (!_amount_handlers_maybe)
    {
      TNCT_LOG_ERR(
          _logger,
          src::format::fmt(
              "not possible to get the amount of handlers for handling ",
              _handling_name));
    }

    auto _amount_handlers{_amount_handlers_maybe.value()};
    TNCT_LOG_TST(_logger,
                 src::format::fmt("amount of handlers = ", _amount_handlers));

    if (_amount_handlers != 4)
    {
      TNCT_LOG_ERR(_logger, src::format::fmt(
                                "amount of handlers should be 1, but it is ",
                                _amount_handlers));
      return false;
    }

    return true;
  }
};

struct dispatcher_003
{
  static std::string desc()
  {
    return "Trying to associate a handler to two handlings";
  }

  bool operator()(const src::program::options &)
  {
    logger _logger;
    try
    {
      _logger.set_deb();

      dispatcher _dispatcher{_logger};

      auto _handler_1 = [](event_1 &&) mutable {};
      auto _result{_dispatcher.add_handling<event_1>(
          "handling-003-1", queue_1{_logger}, std::move(_handler_1),
          src::async::handling_priority::medium, 0)};

      if (_result != src::async::result::OK)
      {
        TNCT_LOG_ERR(_logger, src::format::fmt("result = ", _result));
        return false;
      }

      TNCT_LOG_TST(_logger, "passed 1st subscribe");
      auto _handler_2 = [](event_1 &&) mutable {};
      _result         = _dispatcher.add_handling<event_1>(
          "handling-003-2", queue_1{_logger}, std::move(_handler_2),
          src::async::handling_priority::medium, 0);

      if (_result != src::async::result::ERROR_HANDLER_ALREADY_IN_USE)
      {
        TNCT_LOG_ERR(_logger, src::format::fmt("result = ", _result));
        return false;
      }

      return true;
    }
    catch (std::exception &_ex)
    {
      TNCT_LOG_ERR(_logger, _ex.what());
    }

    return false;
  }
};

struct dispatcher_007
{
  static std::string desc()
  {
    return "Publish an event and checks for the modified value of the event "
           "attribute";
  }

  bool operator()(const src::program::options &)
  {
    try
    {
      logger     _logger;
      dispatcher _dispatcher(_logger);

      event_1 _event;

      TNCT_LOG_TST(_logger, src::format::fmt("event was ", _event));

      auto _handler_1 = [&](event_1 &&p_event) mutable { _event = p_event; };

      auto _result{_dispatcher.add_handling<event_1>(
          "handling-007", queue_1{_logger}, std::move(_handler_1),
          src::async::handling_priority::medium, 1)};

      if (_result != src::async::result::OK)
      {
        TNCT_LOG_ERR(_logger, src::format::fmt("result = ", _result));
        return false;
      }

      _result = _dispatcher.publish(event_1{53});
      if (_result != src::async::result::OK)
      {
        TNCT_LOG_ERR(_logger, src::format::fmt(_result));
        return false;
      }

      std::this_thread::sleep_for(100ms);

      TNCT_LOG_TST(_logger, src::format::fmt("now event is ", _event));
    }
    catch (...)
    {
    }

    return true;
  }
};

struct dispatcher_008
{
  static std::string desc()
  {
    return "Publish an event passing the parameter, and checks for the "
           "modified value of the event attribute";
  }

  bool operator()(const src::program::options &)
  {
    try
    {
      logger     _logger;
      dispatcher _dispatcher(_logger);

      event_1 _event;

      TNCT_LOG_TST(_logger, src::format::fmt("event was ", _event));

      auto _handler_1 = [&](event_1 &&p_event) mutable { _event = p_event; };

      auto _result{_dispatcher.add_handling<event_1>(
          "handling-008", queue_1{_logger}, std::move(_handler_1),
          src::async::handling_priority::medium, 1)};

      if (_result != src::async::result::OK)
      {
        TNCT_LOG_ERR(_logger, src::format::fmt("result = ", _result));
        return false;
      }

      _result =
          _dispatcher.publish<event_1>(static_cast<decltype(event_1::i)>(53));
      if (_result != src::async::result::OK)
      {
        TNCT_LOG_ERR(_logger, src::format::fmt(_result));
        return false;
      }

      std::this_thread::sleep_for(100ms);

      TNCT_LOG_TST(_logger, src::format::fmt("now event is ", _event));
    }
    catch (...)
    {
    }

    return true;
  }

private:
  struct handler
  {
    handler(event_1 &p_event) : m_event(p_event)
    {
    }

    using events_handled = std::tuple<event_1>;

    void operator()(event_1 &&p_event)
    {
      m_event = p_event;
    }

  private:
    event_1 &m_event;
  };
};

struct dispatcher_009
{
  static std::string desc()
  {
    return "A method depends on a src::async::traits::dispatcher with 3 "
           "events, and the concrete src::async::dispatcher is defined with "
           "those 3 and 1 more";
  }

  bool operator()(const src::program::options &)
  {
    using dispatcher =
        src::async::dispatcher<logger, event_a, event_b, event_c, event_d>;

    logger _logger;

    dispatcher _dispatcher(_logger);

    foo(_dispatcher);

    return true;
  }

private:
  struct event_a
  {
    friend std::ostream &operator<<(std::ostream &p_out, const event_a &)
    {
      return p_out;
    }
  };

  struct event_b
  {
    friend std::ostream &operator<<(std::ostream &p_out, const event_b &)
    {
      return p_out;
    }
  };

  struct event_c
  {
    friend std::ostream &operator<<(std::ostream &p_out, const event_c &)
    {
      return p_out;
    }
  };
  struct event_d
  {
    friend std::ostream &operator<<(std::ostream &p_out, const event_d &)
    {
      return p_out;
    }
  };

  using logger = src::log::cerr;

private:
  template <
      src::async::traits::is_dispatcher<event_a, event_b, event_c, event_d>
          t_dispatcher>
  void foo(t_dispatcher &)
  {
  }
};

} // namespace tenacitas::src::async::tst

#endif
