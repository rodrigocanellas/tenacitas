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

#include "tenacitas.lib/async/dispatcher.h"
#include "tenacitas.lib/async/handling_priority.h"
#include "tenacitas.lib/async/sleeping_loop.h"

#include "tenacitas.lib/container/circular_queue.h"
#include "tenacitas.lib/format/fmt.h"
#include "tenacitas.lib/log/cerr.h"
#include "tenacitas.lib/parser/ini_file.h"
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

struct dispatcher_009 {
  static std::string desc() {
    return "Executes test configured in a .ini file passed as the parameter "
           "'--ini_file'. The configuration of the file can be found in the "
           "'tenacitas.lib/async/test/dispatcher.h' file, inside "
           "'dispatcher_009' class.";
  }

  /*
[PUBLISHER_1]
Use=True
IntervalInMilliSec=200
NumEvents=10000

[PUBLISHER_2]
Use=True
IntervalInMilliSec=1000
NumEvents=300

[PUBLISHER_3]
Use=False
IntervalInMilliSec=1000
NumEvents=300

[PUBLISHER_4]
Use=True
IntervalInMilliSec=1000
NumEvents=300

[PUBLISHER_5]
Use=False
IntervalInMilliSec=1000
NumEvents=300

[HANDLING_1]
Use=True
NumSubscribers=5
SleepToFakeWorkInMilliSec=300

[HANDLING_2]
Use=True
NumSubscribers=1
SleepToFakeWorkInMilliSec=5000

[HANDLING_3]
Use=False
NumSubscribers=10
SleepToFakeWorkInMilliSec=250

[HANDLING_4]
Use=True
NumSubscribers=5
SleepToFakeWorkInMilliSec=2500

[HANDLING_5]
Use=True
NumSubscribers=3
SleepToFakeWorkInMilliSec=1000
*/

  bool operator()(const program::options &p_options) {
    logger _logger;

    try {
      _logger.set_deb();

      // ini file
      auto _ini_file_name{p_options.get_single_param("ini_file").value_or("")};
      if (_ini_file_name.empty()) {
        TNCT_LOG_ERR(_logger, "'ini_file' parameter not found");
        return false;
      }
      auto _maybe_ini_file_sections{load_ini(_ini_file_name, _logger)};
      if (!_maybe_ini_file_sections) {
        TNCT_LOG_ERR(_logger, format::fmt("error loading ", _ini_file_name));
        return false;
      }
      auto _ini_file_section{std::move(_maybe_ini_file_sections.value())};

      std::optional<std::pair<publishers_cfg, handlings_cfg>> _maybe_cfs{
          load_cfgs(_logger, _ini_file_section)};

      if (!_maybe_cfs) {
        TNCT_LOG_ERR(_logger,
                     "could not load pubisher and handlings configuration");
        return true;
      }

      auto _cfgs{std::move(_maybe_cfs.value())};

      auto _publishers_cfg{std::move(_cfgs.first)};

      auto _handlings_cfg{std::move(_cfgs.second)};

      print_cfg(_publishers_cfg, _handlings_cfg, _logger);

      size_t _num_events_to_be_publised{
          calculate_num_events_to_be_published(_publishers_cfg)};

      TNCT_LOG_TST(_logger, format::fmt("num events to be published = ",
                                        _num_events_to_be_publised));

      dispatcher _dispatcher(_logger);

      subscribe(_dispatcher, _logger, _handlings_cfg);

      publishers _publishers;

      amounts_events_published _amounts_events_published;

      auto _start{std::chrono::high_resolution_clock::now()};
      publish(_dispatcher, _publishers, _logger, _amounts_events_published,
              _publishers_cfg);

      std::unique_lock<std::mutex> _lock{m_mutex};
      m_wait.wait(_lock, [&]() {
        size_t _amount_events_published_so_far{0};
        for (const auto _amount : _amounts_events_published) {
          _amount_events_published_so_far += _amount;
        }
        if (_amount_events_published_so_far >= _num_events_to_be_publised) {
          return true;
        }
        return false;
      });

      auto _finish{std::chrono::high_resolution_clock::now()};

      TNCT_LOG_TST(_logger,
                   format::fmt("total time = ", (_finish - _start).count()));
      return true;
    } catch (std::exception &_ex) {
      TNCT_LOG_ERR(_logger, _ex.what());
    }

    return false;
  }

private:
  static constexpr size_t m_limit{5};

  struct ev_a {
    ev_a() {}
    ev_a(const ev_a &) = default;
    ev_a(ev_a &) = default;
    ev_a &operator=(const ev_a &) = default;
    ev_a &operator=(ev_a &) = default;
    ~ev_a() = default;

    friend std::ostream &operator<<(std::ostream &p_out, const ev_a &) {
      p_out << "ev_a";
      return p_out;
    }
  };

  struct ev_b {
    ev_b() {}
    ev_b(const ev_b &) = default;
    ev_b(ev_b &) = default;
    ev_b &operator=(const ev_b &) = default;
    ev_b &operator=(ev_b &) = default;
    ~ev_b() = default;

    friend std::ostream &operator<<(std::ostream &p_out, const ev_b &) {
      p_out << "ev_b";
      return p_out;
    }
  };

  struct ev_c {
    ev_c() {}
    ev_c(const ev_c &) = default;
    ev_c(ev_c &) = default;
    ev_c &operator=(const ev_c &) = default;
    ev_c &operator=(ev_c &) = default;
    ~ev_c() = default;

    friend std::ostream &operator<<(std::ostream &p_out, const ev_c &) {
      p_out << "ev_c";
      return p_out;
    }
  };

  struct ev_d {
    ev_d() {}
    ev_d(const ev_d &) = default;
    ev_d(ev_d &) = default;
    ev_d &operator=(const ev_d &) = default;
    ev_d &operator=(ev_d &) = default;
    ~ev_d() = default;

    friend std::ostream &operator<<(std::ostream &p_out, const ev_d &) {
      p_out << "ev_d";
      return p_out;
    }
  };

  struct ev_e {
    ev_e() {}
    ev_e(const ev_e &) = default;
    ev_e(ev_e &) = default;
    ev_e &operator=(const ev_e &) = default;
    ev_e &operator=(ev_e &) = default;
    ~ev_e() = default;

    friend std::ostream &operator<<(std::ostream &p_out, const ev_e &) {
      p_out << "ev_e";
      return p_out;
    }
  };

  using events = std::tuple<ev_a, ev_b, ev_c, ev_d, ev_e>;

  using dispatcher = async::dispatcher<logger, ev_a, ev_b, ev_c, ev_d, ev_e>;

  struct publisher_cfg {
    publisher_cfg() = default;
    publisher_cfg(bool p_use, std::chrono::milliseconds p_interval,
                  size_t p_num_events)
        : use(p_use), interval(p_interval), num_events(p_num_events) {}

    publisher_cfg(const publisher_cfg &p_publisher_cfg) = default;

    publisher_cfg(publisher_cfg &&p_publisher_cfg) = default;

    publisher_cfg &operator=(const publisher_cfg &) = default;
    publisher_cfg &operator=(publisher_cfg &&) = default;

    ~publisher_cfg() = default;

    friend std::ostream &operator<<(std::ostream &p_out,
                                    const publisher_cfg &p_publisher_cfg) {
      p_out << "id = " << p_publisher_cfg.id
            << ", interval (ms) = " << p_publisher_cfg.interval.count()
            << ", # events = " << p_publisher_cfg.num_events << ", used? "
            << (p_publisher_cfg.use ? 'T' : 'F');
      return p_out;
    }

    std::string id;
    bool use{false};
    std::chrono::milliseconds interval{std::chrono::milliseconds::zero()};
    size_t num_events{0};
  };

  struct handling_cfg {
    handling_cfg() = default;
    handling_cfg(std::string_view p_id, bool p_use, size_t p_num_subscribers,
                 std::chrono::milliseconds p_sleep)
        : id(p_id), use(p_use), num_subscribers(p_num_subscribers),
          sleep(p_sleep) {}

    handling_cfg(const handling_cfg &p_handling_cfg) = default;

    handling_cfg(handling_cfg &&p_handling_cfg) = default;

    handling_cfg &operator=(const handling_cfg &) = default;
    handling_cfg &operator=(handling_cfg &&) = default;

    ~handling_cfg() = default;

    friend std::ostream &operator<<(std::ostream &p_out,
                                    const handling_cfg &p_handling_cfg) {
      p_out << "id = " << p_handling_cfg.id
            << ", # subscribers = " << p_handling_cfg.num_subscribers
            << ", sleep = " << p_handling_cfg.sleep.count() << ", used? "
            << (p_handling_cfg.use ? 'T' : 'F');
      return p_out;
    }

    std::string id{0};
    bool use{false};
    size_t num_subscribers{0};
    std::chrono::milliseconds sleep{std::chrono::milliseconds::zero()};
  };

  using ini_file = parser::ini_file<logger>;
  using publishers_cfg = std::array<publisher_cfg, m_limit>;
  using handlings_cfg = std::array<handling_cfg, m_limit>;

  template <traits::event t_event> struct handler {
    handler(std::chrono::milliseconds p_sleep, std::condition_variable &p_wait)
        : m_sleep(p_sleep), m_wait(p_wait) {}
    void operator()(t_event && /*p_event*/) {
      std::this_thread::sleep_for(m_sleep);
      m_wait.notify_all();
    }

  private:
    std::chrono::milliseconds m_sleep{std::chrono::milliseconds::zero()};
    std::condition_variable &m_wait;
  };

  using amounts_events_published = std::array<size_t, m_limit>;

  template <traits::event t_event> struct publisher {};

  using sleeping_loop = async::sleeping_loop<logger>;
  using sleeping_loop_ptr = std::unique_ptr<sleeping_loop>;

  using publishers = std::array<sleeping_loop_ptr, m_limit>;

private:
  std::optional<typename ini_file::sections>
  load_ini(const std::string &p_ini_file, logger &p_logger) {
    ini_file _ini_file(p_logger);
    return _ini_file.read(p_ini_file);
  }

  std::optional<std::pair<publishers_cfg, handlings_cfg>>
  load_cfgs(logger &p_logger,
            const typename ini_file::sections &p_ini_file_section) {
    handlings_cfg _handlings_cfg;
    publishers_cfg _publishers_cfg;

    size_t _next_publisher(0);
    size_t _next_handling(0);
    for (const auto &_section_value : p_ini_file_section) {
      if (_section_value.first.starts_with("PUBLISHER")) {
        if (_next_publisher == 5 /*_publishers_cfg.size()*/) {
          TNCT_LOG_ERR(
              p_logger,
              format::fmt("at most ", _publishers_cfg.size(),
                          " PUBLISHER sections are allowed in ini file"));
          return std::nullopt;
        }
        _publishers_cfg[_next_publisher] = read_publisher_cfg(_section_value);
        _next_publisher++;
      } else if (_section_value.first.starts_with("HANDLING")) {
        if (_next_handling == 5 /*_handlings_cfg.size()*/) {
          TNCT_LOG_ERR(
              p_logger,
              format::fmt("at most ", _handlings_cfg.size(),
                          " HANDLING sections are allowed in ini file"));
          return std::nullopt;
        }

        _handlings_cfg[_next_handling] = read_handling_cfg(_section_value);
        _next_handling++;
      } else {
        TNCT_LOG_ERR(p_logger, format::fmt('\'', _section_value.first,
                                           "' is not a valid section name"));
        return std::nullopt;
      }
    }
    return {
        std::make_pair(std::move(_publishers_cfg), std::move(_handlings_cfg))};
  }

  handling_cfg
  read_handling_cfg(const ini_file::sections::value_type &p_section_value) {
    handling_cfg _handling_cfg;
    _handling_cfg.id = p_section_value.first;
    for (const auto &_property : p_section_value.second) {
      if (_property.first == "Use") {
        if (_property.second == "True") {
          _handling_cfg.use = true;
        }
      } else if (_property.first == "SleepToFakeWorkInMilliSec") {
        _handling_cfg.sleep = decltype(handling_cfg::sleep)(
            static_cast<size_t>(std::stoi(_property.second)));
      } else if (_property.first == "NumSubscribers") {
        _handling_cfg.num_subscribers =
            static_cast<decltype(handling_cfg::num_subscribers)>(
                std::stoi(_property.second));
      }
    }
    return _handling_cfg;
  }

  publisher_cfg
  read_publisher_cfg(const ini_file::sections::value_type &p_section_value) {
    publisher_cfg _publisher_cfg;
    _publisher_cfg.id = p_section_value.first;
    for (const auto &_property : p_section_value.second) {
      if (_property.first == "Use") {
        if (_property.second == "True") {
          _publisher_cfg.use = true;
        }
      } else if (_property.first == "IntervalInMilliSec") {
        _publisher_cfg.interval = decltype(publisher_cfg::interval)(
            static_cast<size_t>(std::stoi(_property.second)));
      } else if (_property.first == "NumEvents") {
        _publisher_cfg.num_events =
            static_cast<decltype(publisher_cfg::num_events)>(
                std::stoi(_property.second));
      }
    }
    return _publisher_cfg;
  }

  template <size_t t_index>
  void subscribe(dispatcher &p_dispatcher, logger &p_logger,
                 const handlings_cfg &p_handlings_cfg) {
    using event_type = std::tuple_element_t<t_index, events>;
    using handler_type = handler<event_type>;
    using queue_type = container::circular_queue<logger, event_type, 50>;

    handler_type _handler{p_handlings_cfg[t_index].sleep, m_wait};
    auto _handling_id{p_dispatcher.subscribe<event_type>(
        std::move(_handler), async::handling_priority::medium,
        queue_type{p_logger}, p_handlings_cfg[t_index].num_subscribers)};
    TNCT_LOG_TST(p_logger,
                 format::fmt("handling ev ", typeid(event_type).name(), " = ",
                             _handling_id.value()));
  }

  void subscribe(dispatcher &p_dispatcher, logger &p_logger,
                 const handlings_cfg &p_handlings_cfg) {
    subscribe<0>(p_dispatcher, p_logger, p_handlings_cfg);
    subscribe<1>(p_dispatcher, p_logger, p_handlings_cfg);
    subscribe<2>(p_dispatcher, p_logger, p_handlings_cfg);
    subscribe<3>(p_dispatcher, p_logger, p_handlings_cfg);
    subscribe<4>(p_dispatcher, p_logger, p_handlings_cfg);
  }

  size_t
  calculate_num_events_to_be_published(publishers_cfg &p_publishers_cfg) {
    size_t _result{0};
    for (const auto &_cfg : p_publishers_cfg) {
      if (_cfg.use) {
        _result += _cfg.num_events;
      }
    }
    return _result;
  }

  template <size_t t_index>
  void publish(dispatcher &p_dispatcher, publishers &p_publishers,
               logger &p_logger,
               amounts_events_published &p_amounts_events_published,
               const publishers_cfg &p_publishers_cfg) {
    using sleeping_loop = async::sleeping_loop<logger>;
    using event_type = std::tuple_element_t<t_index, events>;

    if (!p_publishers_cfg[t_index].use) {
      TNCT_LOG_INF(p_logger,
                   format::fmt("publisher ", t_index, " will be not used"));
    }

    auto _publisher = [&]() {
      if (p_amounts_events_published[t_index] <
          p_publishers_cfg[t_index].num_events) {
        p_dispatcher.publish(event_type{});
        ++p_amounts_events_published[t_index];
      }
    };

    p_amounts_events_published[t_index] = 0;

    p_publishers[t_index] = std::make_unique<sleeping_loop>(
        p_logger, _publisher, p_publishers_cfg[t_index].interval);

    p_publishers[t_index]->start();
  }

  void publish(dispatcher &p_dispatcher, publishers &p_publishers,
               logger &p_logger,
               amounts_events_published &p_amounts_events_published,
               const publishers_cfg &p_publishers_cfg) {
    publish<0>(p_dispatcher, p_publishers, p_logger, p_amounts_events_published,
               p_publishers_cfg);
    publish<1>(p_dispatcher, p_publishers, p_logger, p_amounts_events_published,
               p_publishers_cfg);
    publish<2>(p_dispatcher, p_publishers, p_logger, p_amounts_events_published,
               p_publishers_cfg);
    publish<3>(p_dispatcher, p_publishers, p_logger, p_amounts_events_published,
               p_publishers_cfg);
    publish<4>(p_dispatcher, p_publishers, p_logger, p_amounts_events_published,
               p_publishers_cfg);
  }

  void print_cfg(const publishers_cfg &p_publishers_cfg,
                 const handlings_cfg &p_handlings_cfg, logger &p_logger) {
    std::stringstream _stream;
    _stream << '\n';
    for (const auto &_publisher_cfg : p_publishers_cfg) {
      _stream << _publisher_cfg << '\n';
    }
    p_logger.tst(_stream.str());

    _stream.str("");
    _stream << '\n';
    for (const auto &_handling_cfg : p_handlings_cfg) {
      _stream << _handling_cfg << '\n';
    }
    p_logger.tst(_stream.str());
  }

private:
  std::condition_variable m_wait;
  std::mutex m_mutex;
};

#endif
