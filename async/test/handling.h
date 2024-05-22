/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_ASYNC_TEST_HANDLING_H
#define TENACITAS_LIB_ASYNC_TEST_HANDLING_H

#include <limits>

#include <tenacitas.lib/async/handling_priority.h>
#include <tenacitas.lib/async/internal/handling.h>
#include <tenacitas.lib/async/result.h>
#include <tenacitas.lib/async/sleeping_loop.h>
#include <tenacitas.lib/container/circular_queue.h>
#include <tenacitas.lib/generic/fmt.h>
#include <tenacitas.lib/program/options.h>
#include <tenacitas.lib/generic/tester.h>
#include <tenacitas.lib/log/cerr.h>

using namespace tenacitas::lib;
using namespace std::chrono_literals;

struct handling_test {

protected:
  using logger = log::cerr;

  struct ev1 {
    ev1() : value(std ::numeric_limits<size_t>::max()) {}

    ev1(size_t p_value) : value(p_value) {}

    friend std::ostream &operator<<(std::ostream &p_out, const ev1 &p_ev1) {
      p_out << "ev1";
      if (p_ev1.value != std ::numeric_limits<size_t>::max()) {
        p_out << " - " << p_ev1.value;
      }
      return p_out;
    }
    size_t value;
  };

  using queue = container::circular_queue<logger, ev1>;

  using handling = async::internal::handling<logger, ev1>;

  using sleeping_loop = async::sleeping_loop<logger>;

  logger m_logger;
};

struct handling_000 : public handling_test {
  static std::string desc() {
    return "Creates a handling with no handler, adds an event and checks the "
           "number of events in the queue, which must be 1";
  }

  bool operator()(const program::options &) {
    handling::ptr _handling(handling::create<queue>(
        "h1", m_logger, [](auto &&) {}, 0, async::handling_priority::medium));

    auto _result(_handling->add_event(ev1()));
    if (_result != async::result::OK) {
      m_logger.err(generic ::fmt(_result));
      return false;
    }

    const auto _num_events(_handling->get_num_events());

    m_logger.tst(generic::fmt("# events = ", _num_events));

    return _num_events == 1;
  }
};

struct handling_001 : public handling_test {
  static std::string desc() {
    return "Creates a handling with no handler and checks if the handling is "
           "stopped, which must be false";
  }

  bool operator()(const program::options &) {
    handling::ptr _handling(handling::create<queue>(
        "h1", m_logger, [](auto &&) {}, 0, async::handling_priority::medium));

    const auto _is_stopped(_handling->is_stopped());

    m_logger.tst(generic::fmt("is stopped = ", _is_stopped));
    return !_is_stopped;
  }
};

struct handling_002 : public handling_test {
  static std::string desc() {
    return "Creates a handling with no handler and priority default and checks "
           "its priority, which must be 'handling_priority::medium";
  }

  bool operator()(const program::options &) {
    handling::ptr _handling(handling::create<queue>(
        "h1", m_logger, [](auto &&) {}, 0, async::handling_priority::medium,
        200));

    auto _priority(_handling->get_priority());

    m_logger.tst(generic::fmt("priority = ", _priority));

    return _priority == async::handling_priority::medium;
  }
};

struct handling_003 : public handling_test {
  static std::string desc() {
    return "Creates a handling with no handler and priority "
           "'handling_priority::high', and checks its priority, which must be "
           "'handling_priority::high'";
  }

  bool operator()(const program::options &) {
    handling::ptr _handling(handling::create<queue>(
        "h1", m_logger, [](auto &&) {}, 0, async::handling_priority::high));

    auto _priority(_handling->get_priority());

    m_logger.tst(generic::fmt("priority = ", _priority));

    return _priority == async::handling_priority::high;
  }
};

struct handling_004 : public handling_test {
  static std::string desc() {
    return "Creates a handling with no handler and priority "
           "'handling_priority::high', changes the priority to "
           "'handling_priority::lowest', and checks its priority, which must "
           "be 'handling_priority::lowest";
  }

  bool operator()(const program::options &) {
    handling::ptr _handling(handling::create<queue>(
        "h1", m_logger, [](auto &&) {}, 0, async::handling_priority::high));

    auto _priority(_handling->get_priority());

    m_logger.tst(generic::fmt("original priority = ", _priority));

    _handling->set_priority(async::handling_priority::lowest);

    _priority = _handling->get_priority();

    m_logger.tst(generic::fmt("new priority = ", _priority));

    return _priority == async::handling_priority::lowest;
  }
};

struct handling_005 : public handling_test {
  static std::string desc() {
    return "Creates a handling with no handler and id 'dummy_abc', and checks "
           "its id, which must be 'dummy_abc'";
  }

  bool operator()(const program::options &) {
    handling::ptr _handling(handling::create<queue>(
        "dummy_abc", m_logger, [](auto &&) {}, 0,
        async::handling_priority::medium));

    auto _handling_id(_handling->get_id());

    m_logger.tst(generic::fmt("id = ", _handling_id));

    return _handling_id == "dummy_abc";
  }
};

struct handling_006 : public handling_test {
  static std::string desc() {
    return "Creates a handling with no handler, adds an event and checks the "
           "number of events in the queue, which must be 1, then clear the "
           "'handling, checks again for the number of events that must be 0";
  }

  bool operator()(const program::options &) {
    handling::ptr _handling(handling::create<queue>(
        "h1", m_logger, [](auto &&) {}, 0, async::handling_priority::medium));

    auto _result(_handling->add_event(ev1()));
    if (_result != async::result::OK) {
      m_logger.err(generic ::fmt(_result));
      return false;
    }

    auto _num_events(_handling->get_num_events());

    m_logger.tst(generic::fmt("# events before = ", _num_events));

    _handling->clear();

    _num_events = _handling->get_num_events();
    m_logger.tst(generic::fmt("# events after = ", _num_events));

    return _num_events == 0;
  }
};

struct handling_007 : public handling_test {
  static std::string desc() {
    return "Creates a handling with two handlers and checks for the number of "
           "handlers, which must be 2";
  }

  bool operator()(const program::options &) {
    handling::ptr _handling(handling::create<queue>(
        "h1", m_logger, [](auto &&) {}, 2, async::handling_priority::medium));

    auto _num_handlers(_handling->get_num_handlers());

    m_logger.tst(generic::fmt("# handlers = ", _num_handlers));

    return _num_handlers == 2;
  }
};

struct handling_008 : public handling_test {
  static std::string desc() {
    return "Creates a handling with two handlers and checks if it is stopped, "
           "which must be false";
  }

  bool operator()(const program::options &) {

    handling::ptr _handling(handling::create<queue>(
        "h1", m_logger, [](auto &&) {}, 2, async::handling_priority::medium));

    auto _is_stopped(_handling->is_stopped());

    m_logger.tst(generic::fmt("is stopped? ", _is_stopped));
    return !_is_stopped;
  }
};

struct handling_009 : public handling_test {
  static std::string desc() {
    return "Creates a handling with two handlers, stop it and checks if it is "
           "stopped, which must be true";
  }

  bool operator()(const program::options &) {
    handling::ptr _handling(handling::create<queue>(
        "h1", m_logger, [](auto &&) {}, 2, async::handling_priority::medium));

    auto _is_stopped(_handling->is_stopped());

    m_logger.tst(generic::fmt("is stopped before? ", _is_stopped));

    _handling->stop();

    _is_stopped = _handling->is_stopped();
    m_logger.tst(generic::fmt("is stopped after? ", _is_stopped));

    return _is_stopped;
  }
};

struct handling_010 : public handling_test {
  static std::string desc() {
    return "Creates a handling with two handlers, stop it, adds 2 events and "
           "checks for the number of events, which must be 2";
  }

  bool operator()(const program::options &) {
    handling::ptr _handling(handling::create<queue>(
        "h1", m_logger, [](auto &&) {}, 2, async::handling_priority::medium));

    _handling->stop();

    auto _result(_handling->add_event(ev1()));
    if (_result != async::result::OK) {
      m_logger.err(generic ::fmt(_result));
      return false;
    }

    _result = _handling->add_event(ev1());
    if (_result != async::result::OK) {
      m_logger.err(generic ::fmt(_result));
      return false;
    }

    auto _num_events(_handling->get_num_events());

    m_logger.tst(generic::fmt("# events = ", _num_events));

    return _num_events == 2;
  }
};

struct handling_011 : public handling_test {
  static std::string desc() {
    return "Creates a handling with 1 handler, adds 2 events, sleep for 200ms "
           "and checks for the number of events, which must be 0";
  }

  bool operator()(const program::options &) {
    handling::ptr _handling(handling::create<queue>(
        "h1", m_logger, [&](auto &&) { m_logger.tst("event handled"); }, 1,
        async::handling_priority::medium));
    auto _result(_handling->add_event(ev1()));
    if (_result != async::result::OK) {
      m_logger.err(generic ::fmt(_result));
      return false;
    }

    _result = _handling->add_event(ev1());
    if (_result != async::result::OK) {
      m_logger.err(generic ::fmt(_result));
      return false;
    }

    std::this_thread::sleep_for(200ms);

    auto _num_events(_handling->get_num_events());

    m_logger.tst(generic::fmt("# events = ", _num_events));

    return _num_events == 0;
  }
};

struct handling_012 : public handling_test {
  static std::string desc() {
    return "Creates a handling with 3 handlers, check for the number of "
           "handlers that must be 3, then increment the number of handler by "
           "2, and checks for the number of handlers, which must be 5";
  }

  bool operator()(const program::options &) {
    handling::ptr _handling(handling::create<queue>(
        "h1", m_logger, [&](auto &&) { m_logger.tst("event handled"); }, 3,
        async::handling_priority::medium));

    auto _num_handlers(_handling->get_num_handlers());
    m_logger.tst(generic::fmt("# handlers = ", _num_handlers));

    _handling->increment_handlers(2);
    _num_handlers = _handling->get_num_handlers();
    m_logger.tst(generic::fmt("# handlers = ", _num_handlers));

    return _num_handlers == 5;
  }
};

struct handling_014 : public handling_test {
  static std::string desc() {
    return "Creates a handling with 1 handler that sleeps for 200 "
           "milliseconds, adds " +
           std::to_string(m_num_events) +
           " events, increment the number of handlers by " +
           std::to_string(m_amount_handlers_to_add) +
           ", count the events handled by all the handlers and waits for all "
           "the events to be handled, the sum of events handled must by the " +
           std::to_string(m_amount_handlers_to_add + 1) + " handlers must be " +
           std::to_string(m_num_events);
  }

  bool operator()(const program::options &) {
    std::mutex _mutex_wait;
    std::condition_variable _cond_wait;
    std::mutex _mutex_count_all_events;

    events_by_handler _events_by_handler;

    num_events _num_events(0);

    auto _handler([&](auto &&p_event) {
      {
        std::lock_guard<std::mutex> _lock(_mutex_count_all_events);
        ++_num_events;
        if (_events_by_handler.find(std::this_thread ::get_id()) ==
            _events_by_handler.end()) {
          _events_by_handler[std::this_thread ::get_id()] = 1;
        } else {
          ++_events_by_handler[std::this_thread ::get_id()];
        }
      }
      m_logger.tst(generic::fmt("thread ", std ::this_thread ::get_id(),
                                ", handling event ", p_event.value));
      _cond_wait.notify_all();

      std::this_thread::sleep_for(200ms);
    });

    handling::ptr _handling(handling::create<queue>(
        "h014", m_logger, _handler, 1, async::handling_priority::medium));

    async::result _result(async::result::OK);
    for (num_events _i = 0; _i < m_num_events; ++_i) {
      _result = _handling->add_event(ev1(_i));
      if (_result != async::result::OK) {
        m_logger.err(generic ::fmt(_result));
        return false;
      }
    }

    _handling->increment_handlers(m_amount_handlers_to_add);

    {
      std::unique_lock<std::mutex> _lock(_mutex_wait);
      m_logger.tst(generic::fmt("still waiting "));
      _cond_wait.wait(_lock, [&]() { return _num_events == m_num_events; });
    }

    for (const auto &_value : _events_by_handler) {
      m_logger.tst(
          generic::fmt("thread ", _value.first, " # events = ", _value.second));
    }

    return (_num_events == m_num_events) &&
           all_events_handled(_events_by_handler);
  }

private:
  using num_events = uint16_t;
  using events_by_handler = std::map<std::thread::id, num_events>;
  static constexpr num_events m_num_events{1000};
  static constexpr uint16_t m_amount_handlers_to_add{3};

private:
  bool all_events_handled(const events_by_handler &p_events_by_handler) {
    num_events _total(0);
    for (const auto &_value : p_events_by_handler) {
      // m_logger.tst(generic::fmt("thread ", _value.first, " # ",
      // _value.second));
      _total += _value.second;
    }
    // m_logger.tst(generic::fmt("total = ", _total));
    return _total >= m_num_events;
  }
};

struct handling_015 : public handling_test {
  static std::string desc() {
    return "Creates a handling with 1 handler that sleeps for 1 second after "
           "handling the event, adds " +
           std::to_string(m_num_events / 2) +
           " events, sleeps for 2 seconds, move the handling to another, "
           "send " +
           std::to_string(m_num_events / 2) +
           " events, count the events handled and waits for all the events to "
           "be handled, the sum of events handled must be " +
           std::to_string(m_num_events);
  }

  bool operator()(const program::options &) {
    std::mutex _mutex_wait;
    std::condition_variable _cond_wait;

    num_events _num_events(0);

    auto _handler([&](auto &&p_event) {
      ++_num_events;
      m_logger.tst(generic::fmt("handling event ", p_event.value));
      _cond_wait.notify_all();

      std::this_thread::sleep_for(200ms);
    });

    handling::ptr _handling(handling::create<queue>(
        "h015", m_logger, _handler, 1, async::handling_priority::medium));

    num_events _i = 0;
    auto _result(async::result::OK);
    for (; _i < m_num_events / 2; ++_i) {
      _result = _handling->add_event(ev1(_i));
      if (_result != async::result::OK) {
        m_logger.err(generic ::fmt(_result));
        return false;
      }
    }

    m_logger.tst("sleeping...");
    std::this_thread::sleep_for(2s);
    m_logger.tst("waking up...");

    // m_logger.set_tra();

    handling::ptr _handling_1(std::move(_handling));

    for (num_events _j = _i; _j < m_num_events; ++_j) {
      _result = _handling_1->add_event(ev1(_j));
      if (_result != async::result::OK) {
        m_logger.err(generic ::fmt(_result));
        return false;
      }
    }

    {
      std::unique_lock<std::mutex> _lock(_mutex_wait);
      m_logger.tst(generic::fmt("still waiting "));
      _cond_wait.wait(_lock, [&]() { return _num_events == m_num_events; });
    }

    return (_num_events == m_num_events);
  }

private:
  using num_events = uint16_t;

  static constexpr num_events m_num_events{100};
};

struct handling_016 : public handling_test {
  static std::string desc() {
    return "Creates a handling with 1 handler, adds " +
           std::to_string(m_num_events) +
           " events, count the events handled and waits for all the events to  "
           "be handled, the sum of events handled must be " +
           std::to_string(m_num_events);
  }

  bool operator()(const program::options &) {
    // m_logger.set_tra();

    uint16_t _handled(0);
    uint16_t _added(0);

    std::mutex _mutex;
    std::condition_variable m_cond;

    auto _handler([&](auto &&p_event) {
      m_logger.tst(
          generic::fmt("handling event # ", ++_handled, ": '", p_event, '\''));
      std::this_thread::sleep_for(175ms);
      m_cond.notify_one();
    });

    handling::ptr _handling(handling::create<queue>(
        "h1", m_logger, _handler, 1, async::handling_priority::highest,
        m_num_events + 10));

    sleeping_loop _sleeping_loop(
        m_logger,
        [&]() {
          if (_added != m_num_events) {
            ev1 _ev(_added);
            m_logger.tst(
                generic::fmt("adding event # ", ++_added, ": '", _ev, '\''));
            auto _result(_handling->add_event(std::move(_ev)));
            if (_result != async::result::OK) {
              m_logger.err(generic ::fmt(_result));
            }
          }
        },
        50ms);

    _sleeping_loop.start();

    {
      std::unique_lock<std::mutex> _lock(_mutex);
      m_cond.wait(_lock, [&]() {
        if (_handled == m_num_events) {
          m_logger.tst(generic::fmt(_handled, " = ", m_num_events,
                                    ", stopping event generation"));
          _sleeping_loop.stop();
          return true;
        }
        if (_added == m_num_events) {
          if (!_sleeping_loop.is_stopped()) {
            m_logger.tst(generic::fmt(_added, " = ", m_num_events,
                                      " stopping event generation"));
            _sleeping_loop.stop();
          }
        }
        m_logger.tst(generic::fmt(
            "# events handled = ", _handled,
            ", # events in queue = ", _handling->get_num_events()));
        return false;
      });
    }

    m_logger.tst(generic::fmt("# events added = ", _added,
                              ", # events handled = ", _handled));

    return (_handled == m_num_events) && (_added == m_num_events);
  }

private:
  static constexpr uint16_t m_num_events{1000};
};

#endif
