/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_ASYNC_TST_HANDLING_TEST_H
#define TNCT_ASYNC_TST_HANDLING_TEST_H

#include <limits>

#include "tnct/async/bus/sleeping_loop.h"
#include "tnct/async/dat/handling_name.h"
#include "tnct/async/internal/handling.h"
#include "tnct/async/internal/handling_id.h"
#include "tnct/container/dat/circular_queue.h"
#include "tnct/format/bus/fmt.h"
#include "tnct/log/cerr.h"
#include "tnct/log/cpt/macros.h"
#include "tnct/program/options.h"

using namespace tnct;
using namespace std::chrono_literals;

namespace tnct::async::tst
{
struct handling_test
{

protected:
  using logger = log::cerr;

  struct ev1
  {
    ev1() : value(std ::numeric_limits<size_t>::max())
    {
    }

    ev1(size_t p_value) : value(p_value)
    {
    }

    friend std::ostream &operator<<(std::ostream &p_out, const ev1 &p_ev1)
    {
      p_out << "ev1";
      if (p_ev1.value != std ::numeric_limits<size_t>::max())
      {
        p_out << " - " << p_ev1.value;
      }
      return p_out;
    }
    size_t value;
  };

  using event = ev1;

  using queue = container::dat::circular_queue<logger, event, 20>;

  using sleeping_loop = async::sleeping_loop<logger>;

  struct handler
  {
    using events_handled = std::tuple<event>;
    void operator()(event &&)
    {
    }
  };

  logger m_logger;
};

struct handling_000 : public handling_test
{
  static std::string desc()
  {
    return "Creates a handling with no handler, adds an event and checks the "
           "number of events in the queue, which must be 1";
  }

  bool operator()(const program::options &)
  {

    // using handler = handler_t<event>;

    using handling =
        async::internal::handling_concrete<logger, ev1, queue, handler>;

    try
    {
      m_logger.set_deb();

      handling _handling("handling-000", m_logger, handler{}, queue{m_logger});

      _handling.add_event(ev1());

      const auto _num_events(_handling.get_num_events());

      m_logger.tst(format::bus::fmt("# events = ", _num_events));

      return _num_events == 1;
    }
    catch (std::exception &_ex)
    {
      TNCT_LOG_ERR(m_logger, _ex.what());
    }
    return false;
  }
};

struct handling_001 : public handling_test
{
  static std::string desc()
  {
    return "Creates a handling with one handler and checks if the handling is "
           "stopped, which must be false";
  }

  bool operator()(const program::options &)
  {

    using handling =
        async::internal::handling_concrete<logger, ev1, queue, handler>;

    handling _handling("handling-001", m_logger, handler{}, queue{m_logger});

    const auto _is_stopped(_handling.is_stopped());

    m_logger.tst(format::bus::fmt("is stopped = ", _is_stopped));
    return !_is_stopped;
  }
};

struct handling_005 : public handling_test
{
  static std::string desc()
  {
    return "Creates a handling with no handler and id 5. Checks its id, which "
           "must be 5";
  }

  bool operator()(const program::options &)
  {

    using handling =
        async::internal::handling_concrete<logger, ev1, queue, handler>;

    handling _handling("handling-005", m_logger, handler{}, queue{m_logger});

    auto _handling_id{_handling.get_id()};

    m_logger.tst(format::bus::fmt("id = ", _handling_id));

    return _handling_id
           == async::internal::get_handling_id(
               async::dat::handling_name{"handling-005"});
  }
};

struct handling_006 : public handling_test
{
  static std::string desc()
  {
    return "Creates a handling with no handler, adds an event and checks the "
           "number of events in the queue, which must be 1, then clear the "
           "'handling, checks again for the number of events that must be 0";
  }

  bool operator()(const program::options &)
  {

    using handling =
        async::internal::handling_concrete<logger, ev1, queue, handler>;

    try
    {
      handling _handling("handling-006", m_logger, handler{}, queue{m_logger},
                         0);

      _handling.add_event(ev1());

      auto _num_events(_handling.get_num_events());

      if (_num_events != 1)
      {
        m_logger.tst(
            format::bus::fmt("# events before should be, but it is ", _num_events));
        return false;
      }

      _handling.clear();

      _num_events = _handling.get_num_events();
      m_logger.tst(format::bus::fmt("# events after = ", _num_events));

      return _num_events == 0;
    }
    catch (std::exception &_ex)
    {
      TNCT_LOG_ERR(m_logger, _ex.what());
    }
    return false;
  }
};

struct handling_007 : public handling_test
{
  static std::string desc()
  {
    return "Creates a handling with two handlers and checks for the number of "
           "handlers, which must be 2";
  }

  bool operator()(const program::options &)
  {
    using handling =
        async::internal::handling_concrete<logger, ev1, queue, handler>;

    handling _handling("handling-007", m_logger, handler{}, queue{m_logger}, 2);

    auto _num_handlers(_handling.get_amount_handlers());

    m_logger.tst(format::bus::fmt("# handlers = ", _num_handlers));

    return _num_handlers == 2;
  }
};

struct handling_008 : public handling_test
{
  static std::string desc()
  {
    return "Creates a handling with two handlers and checks if it is stopped, "
           "which must be false";
  }

  bool operator()(const program::options &)
  {

    using handling =
        async::internal::handling_concrete<logger, ev1, queue, handler>;

    handling _handling("handling-008", m_logger, handler{}, queue{m_logger}, 2);

    auto _is_stopped(_handling.is_stopped());

    m_logger.tst(format::bus::fmt("is stopped? ", _is_stopped));
    return !_is_stopped;
  }
};

struct handling_009 : public handling_test
{
  static std::string desc()
  {
    return "Creates a handling with two handlers, stop it and checks if it is "
           "stopped, which must be true";
  }

  bool operator()(const program::options &)
  {

    using handling =
        async::internal::handling_concrete<logger, ev1, queue, handler>;

    handling _handling("handling-009", m_logger, handler{}, queue{m_logger}, 2);

    auto _is_stopped(_handling.is_stopped());

    m_logger.tst(format::bus::fmt("is stopped before? ", _is_stopped));

    _handling.stop();

    _is_stopped = _handling.is_stopped();
    m_logger.tst(format::bus::fmt("is stopped after? ", _is_stopped));

    return _is_stopped;
  }
};

struct handling_010 : public handling_test
{
  static std::string desc()
  {
    return "Creates a handling with two handlers, stop it, adds 2 events and "
           "checks for the number of events, which must be 2";
  }

  bool operator()(const program::options &)
  {

    using handling =
        async::internal::handling_concrete<logger, ev1, queue, handler>;
    try
    {
      handling _handling("handling-010", m_logger, handler{}, queue{m_logger},
                         2);

      _handling.stop();

      _handling.add_event(ev1());

      _handling.add_event(ev1());

      auto _num_events(_handling.get_num_events());

      m_logger.tst(format::bus::fmt("# events = ", _num_events));

      return _num_events == 2;
    }
    catch (std::exception &_ex)
    {
      TNCT_LOG_ERR(m_logger, _ex.what());
    }
    return false;
  }
};

struct handling_011 : public handling_test
{
  static std::string desc()
  {
    return "Creates a handling with 1 handler, adds 2 events, sleep for 200ms "
           "and checks for the number of events, which must be 0";
  }

  bool operator()(const program::options &)
  {

    using handling =
        async::internal::handling_concrete<logger, ev1, queue, handler>;

    try
    {
      handling _handling("handling-011", m_logger, handler{}, queue{m_logger},
                         2);

      _handling.add_event(ev1());

      _handling.add_event(ev1());

      std::this_thread::sleep_for(200ms);

      auto _num_events(_handling.get_num_events());

      m_logger.tst(format::bus::fmt("# events = ", _num_events));

      return _num_events == 0;
    }
    catch (std::exception &_ex)
    {
      TNCT_LOG_ERR(m_logger, _ex.what());
    }
    return false;
  }
};

struct handling_014 : public handling_test
{
  static std::string desc()
  {
    return "Creates a handling with " + std::to_string(m_amount_handlers)
           + " handlers that sleeps for 20 "
             "milliseconds, adds "
           + std::to_string(m_num_events)
           + " events, count the events handled by all the handlers and waits "
             "for all "
             "the events to be handled, the sum of events handled must be "
           + std::to_string(m_amount_handlers + 1) + " and handlers must be "
           + std::to_string(m_num_events);
  }

  bool operator()(const program::options &)
  {
    try
    {
      m_logger.set_deb();

      using handling =
          async::internal::handling_concrete<logger, ev1, queue, handler>;

      handling _handling("handling-014", m_logger, handler{this},
                         queue{m_logger}, m_amount_handlers);

      for (num_events _i = 0; _i < m_num_events; ++_i)
      {
        _handling.add_event(ev1(_i));
      }

      TNCT_LOG_TST(m_logger, format::bus::fmt("number of events = ",
                                         _handling.get_num_events()));

      {
        std::unique_lock<std::mutex> _lock(m_mutex_wait);
        m_logger.tst(format::bus::fmt("thread main ", std::this_thread::get_id(),
                                 " starts to wait"));
        m_cond_wait.wait(_lock, [&]()
                         { return m_current_num_events == m_num_events; });
      }

      TNCT_LOG_TST(m_logger,
                   format::bus::fmt("thread main ", std::this_thread::get_id(),
                               ", done waiting"));

      for (const auto &_value : m_events_by_handler)
      {
        m_logger.tst(format::bus::fmt("thread ", _value.first,
                                 " # events = ", _value.second));
      }

      return (m_current_num_events == m_num_events)
             && all_events_handled(m_events_by_handler);
    }
    catch (std::exception &_ex)
    {
      TNCT_LOG_ERR(m_logger, _ex.what());
    }
    return false;
  }

private:
  using num_events        = uint16_t;
  using events_by_handler = std::map<std::thread::id, num_events>;
  static constexpr num_events m_num_events{1000};
  static constexpr uint16_t   m_amount_handlers{4};

  struct handler
  {
    handler(handling_014 *p_owner) : m_owner(p_owner)
    {
    }
    using event          = ev1;
    using events_handled = std::tuple<event>;

    void operator()(event &&p_event)
    {

      {
        std::lock_guard<std::mutex> _lock(m_owner->m_mutex_count_all_events);
        ++m_owner->m_current_num_events;
        if (m_owner->m_events_by_handler.find(std::this_thread ::get_id())
            == m_owner->m_events_by_handler.end())
        {
          m_owner->m_events_by_handler[std::this_thread ::get_id()] = 1;
        }
        else
        {
          ++m_owner->m_events_by_handler[std::this_thread ::get_id()];
        }
      }
      m_owner->m_logger.tst(format::bus::fmt("handler ",
                                        std ::this_thread ::get_id(),
                                        " handling event ", p_event.value));
      m_owner->m_cond_wait.notify_all();

      // m_owner->m_logger.tst(format::bus::fmt("thread ", std ::this_thread
      // ::get_id(),
      //                                   " all notified"));

      std::this_thread::sleep_for(200ms);

      // m_owner->m_logger.tst(
      //     format::bus::fmt("thread ", std ::this_thread ::get_id(), ", woke
      //     up"));
    }
    handling_014 *m_owner{nullptr};
  };

private:
  bool all_events_handled(const events_by_handler &p_events_by_handler)
  {
    num_events _total(0);
    for (const auto &_value : p_events_by_handler)
    {
      // m_logger.tst(format::bus::fmt("thread ", _value.first, " # ",
      // _value.second));
      _total += _value.second;
    }
    // m_logger.tst(format::bus::fmt("total = ", _total));
    return _total >= m_num_events;
  }
  std::mutex              m_mutex_count_all_events;
  num_events              m_current_num_events{0};
  events_by_handler       m_events_by_handler;
  std::condition_variable m_cond_wait;
  std::mutex              m_mutex_wait;
};

struct handling_015 : public handling_test
{
  static std::string desc()
  {
    return "Creates a handling with 1 handler that sleeps for 1 second after "
           "handling the event, adds "
           + std::to_string(m_num_events / 2)
           + " events, sleeps for 2 seconds, move the handling to another, "
             "send "
           + std::to_string(m_num_events / 2)
           + " events, count the events handled and waits for all the events "
             "to "
             "be handled, the sum of events handled must be "
           + std::to_string(m_num_events);
  }

  bool operator()(const program::options &)
  {
    try
    {
      m_logger.set_deb();

      using handling =
          async::internal::handling_concrete<logger, ev1, queue, handler>;

      handling _handling("handling-015", m_logger, handler{this},
                         queue{m_logger}, 1);
      // _handling.increment_handlers(1U);

      num_events _i = 0;

      for (; _i < (m_num_events / 2); ++_i)
      {
        _handling.add_event(ev1(_i));
      }

      m_logger.tst("sleeping...");
      std::this_thread::sleep_for(2s);
      m_logger.tst("waking up...");

      // m_logger.set_tra();

      handling _handling_1(std::move(_handling));

      for (num_events _j = _i; _j < m_num_events; ++_j)
      {
        _handling_1.add_event(ev1(_j));
      }

      {
        std::unique_lock<std::mutex> _lock(m_mutex_wait);
        m_logger.tst(format::bus::fmt("still waiting "));
        m_cond_wait.wait(_lock, [&]()
                         { return m_current_num_events == m_num_events; });
      }

      return (m_current_num_events == m_num_events);
    }
    catch (std::exception &_ex)
    {
      TNCT_LOG_ERR(m_logger, _ex.what());
    }
    return false;
  }

private:
  struct handler
  {
    using event          = ev1;
    using events_handled = std::tuple<event>;
    handler(handling_015 *p_owner) : m_owner(p_owner)
    {
    }
    void operator()(event &&p_event)
    {
      ++m_owner->m_current_num_events;
      m_owner->m_logger.tst(format::bus::fmt("handling event ", p_event.value));
      m_owner->m_cond_wait.notify_all();

      std::this_thread::sleep_for(200ms);
    }
    handling_015 *m_owner{nullptr};
  };

private:
  using num_events = uint16_t;
  std::mutex              m_mutex_wait;
  std::condition_variable m_cond_wait;

  num_events m_current_num_events{0};

  static constexpr num_events m_num_events{100};
};

struct handling_016 : public handling_test
{
  static std::string desc()
  {
    return "Creates a handling with 1 handler, adds "
           + std::to_string(m_num_events)
           + " events, count the events handled and waits for all the events "
             "to  "
             "be handled, the sum of events handled must be "
           + std::to_string(m_num_events);
  }

  bool operator()(const program::options &)
  {
    // m_logger.set_tra();

    try
    {

      uint16_t _added(0);

      std::mutex _mutex;

      using handling =
          async::internal::handling_concrete<logger, ev1, queue, handler>;

      handling _handling("handling-016", m_logger, handler{this},
                         queue{m_logger}, 10);

      sleeping_loop _sleeping_loop(
          m_logger,
          [&]()
          {
            if (_added != m_num_events)
            {
              ev1 _ev(_added);
              m_logger.tst(
                  format::bus::fmt("adding event # ", ++_added, ": '", _ev, '\''));
              _handling.add_event(std::move(_ev));
            }
          },
          50ms);

      _sleeping_loop.start();

      {
        std::unique_lock<std::mutex> _lock(_mutex);
        m_cond.wait(
            _lock,
            [&]()
            {
              if (m_handled == m_num_events)
              {
                m_logger.tst(format::bus::fmt(m_handled, " = ", m_num_events,
                                         ", stopping event generation"));
                _sleeping_loop.stop();
                return true;
              }
              if (_added == m_num_events)
              {
                if (!_sleeping_loop.is_stopped())
                {
                  m_logger.tst(format::bus::fmt(_added, " = ", m_num_events,
                                           " stopping event generation"));
                  _sleeping_loop.stop();
                }
              }
              m_logger.tst(format::bus::fmt(
                  "# events handled = ", m_handled,
                  ", # events in queue = ", _handling.get_num_events()));
              return false;
            });
      }

      m_logger.tst(format::bus::fmt("# events added = ", _added,
                               ", # events handled = ", m_handled));

      return (m_handled == m_num_events) && (_added == m_num_events);
    }
    catch (std::exception &_ex)
    {
      TNCT_LOG_ERR(m_logger, _ex.what());
    }
    return false;
  }

private:
  struct handler
  {
    using event          = ev1;
    using events_handled = std::tuple<event>;
    handler(handling_016 *p_owner) : m_owner(p_owner)
    {
    }
    void operator()(event &&p_event)
    {
      m_owner->m_logger.tst(format::bus::fmt(
          "handling event # ", ++m_owner->m_handled, ": '", p_event, '\''));
      std::this_thread::sleep_for(500ms);
      m_owner->m_cond.notify_one();
    }

    handling_016 *m_owner{nullptr};
  };

private:
  static constexpr uint16_t m_num_events{1000};
  std::condition_variable   m_cond;
  uint16_t                  m_handled{0};
};
} // namespace tnct::async::tst
#endif
