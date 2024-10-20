/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_ASYNC_TEST_HANDLINGS_H
#define TENACITAS_LIB_ASYNC_TEST_HANDLINGS_H

#include <tenacitas.lib/async/handling.h>
#include <tenacitas.lib/async/internal/handlings.h>
#include <tenacitas.lib/async/result.h>
#include <tenacitas.lib/async/sleeping_loop.h>
#include <tenacitas.lib/container/circular_queue.h>
#include <tenacitas.lib/generic/fmt.h>
#include <tenacitas.lib/log/cerr.h>
#include <tenacitas.lib/log/logger.h>
#include <tenacitas.lib/program/options.h>

using namespace tenacitas::lib;
using namespace std::chrono_literals;
using namespace std::placeholders;

struct handlings_000 {
  static std::string desc() {
    return "Simple 'async::handlings' creation";
  }

  bool operator()(const program::options &) {
    logger _logger;
    handling_1a _handling_1a{"handling_1a", _logger, handler_1a{}};

    handling_1b _handling_1b{"handling_1b", _logger, handler_1b{}};

    using handlings =
        async::handlings<logger, event_1, handling_1a, handling_1b>;
    handlings _handlings{std::move(_handling_1a), std::move(_handling_1b)};

    return true;
  }

private:
  using logger = log::cerr;
  struct event_1 {
    friend std::ostream &operator<<(std::ostream &p_out, const event_1 &) {
      return p_out;
    }
  };
  using queue_1a = container::circular_queue<logger, event_1, 10>;

  using queue_1b = container::circular_queue<logger, event_1, 200>;

  struct handler_1a {
    using event = event_1;
    void operator()(event_1 &&) {}
  };

  struct handler_1b {
    using event = event_1;
    void operator()(event_1 &&) {}
  };

  using handling_1a =
      async::handling<logger, event_1, queue_1a, handler_1a>;

  using handling_1b =
      async::handling<logger, event_1, queue_1b, handler_1b>;
};

struct handlings_001 {
  static std::string desc() {
    return "Breaking 'traits::all_handlings_are_for_same_event' requirement";
  }

  bool operator()(const program::options &) {
    /// \attention THIS DOES NOT COMPILE, AS EXPECTED
    // using handlings =
    //     async::handlings<logger, event_1, handling_1, handling_2>;

    return true;
  }

private:
  using logger = log::cerr;

  struct event_1 {
    friend std::ostream &operator<<(std::ostream &p_out, const event_1 &) {
      return p_out;
    }
  };
  using queue_1 = container::circular_queue<logger, event_1, 10>;

  struct handler_1 {
    using event = event_1;
    void operator()(event_1 &&) {}
  };

  using handling_1 =
      async::handling<logger, event_1, queue_1, handler_1>;

  struct event_2 {
    friend std::ostream &operator<<(std::ostream &p_out, const event_2 &) {
      return p_out;
    }
  };

  struct handler_2 {
    using event = event_2;
    void operator()(event_2 &&) {}
  };

  using queue_2 = container::circular_queue<logger, event_2, 200>;

  using handling_2 =
      async::handling<logger, event_2, queue_2, handler_2>;
};

struct handlings_002 {
  static std::string desc() {
    return "Not compiling because there there is a 'traits::handler' that is "
           "associated to two differente 'async::handlings'";
  }

  bool operator()(const program::options &) {
    // logger _logger;
    // /// \attention THIS DOES NOT COMPILE, AS EXPECTED
    // using handlings =
    //     async::handlings<logger, event_1, handling_1a,
    //     handling_1b>;

    // handling_1a _handling_1a{"handling_1a", _logger, handler_1{}, 1,
    //                          async::handling_priority::medium};

    // handling_1b _handling_1b{"handling_1b", _logger, handler_1{}, 3,
    //                          async::handling_priority::medium};

    // handlings _handlings{std::move(_handling_1a), std::move(_handling_1b)};

    return true;
  }

private:
  using logger = log::cerr;

  struct event_1 {
    friend std::ostream &operator<<(std::ostream &p_out, const event_1 &) {
      return p_out;
    }
  };

  struct handler_1 {
    using event = event_1;
    void operator()(event_1 &&) {}
  };

  using queue_1a = container::circular_queue<logger, event_1, 10>;

  using handling_1a =
      async::handling<logger, event_1, queue_1a, handler_1>;

  using queue_1b = container::circular_queue<logger, event_1, 30>;

  using handling_1b =
      async::handling<logger, event_1, queue_1b, handler_1>;
};

struct handlings_003 {
  static std::string desc() {
    return "Defining one handling with 1 handler, retrieving the number of "
           "handlers defined for this handling and it should be 1";
  }

  bool operator()(const program::options &) {
    logger _logger;

    using handlings = async::handlings<logger, event_1, handling_1>;

    handling_1 _handling_1{"handling_1", _logger, handler_1{}};
    _handling_1.increment_handlers(1U);

    handlings _handlings{std::move(_handling_1)};

    size_t _num_handlers{0};

    async::result _result{
        _handlings.get_num_handlers<handling_1>(_num_handlers)};

    if (_result != async::result::OK) {
      TNCT_LOG_ERR(_logger, generic::fmt("result = ", _result));
      return false;
    }
    if (_num_handlers != 1U) {
      TNCT_LOG_ERR(_logger,
                   generic::fmt("number of handlers should be 1 but it is = ",
                                _num_handlers));
      return false;
    }

    TNCT_LOG_TST(
        _logger,
        generic::fmt("number of handlers should be 1 and it is actually = ",
                     _num_handlers))
    return (_num_handlers == 1);
  }

private:
  using logger = log::cerr;

  struct event_1 {
    friend std::ostream &operator<<(std::ostream &p_out, const event_1 &) {
      return p_out;
    }
  };

  struct handler_1 {
    using event = event_1;
    void operator()(event_1 &&) {}
  };

  using queue_1 = container::circular_queue<logger, event_1, 10>;

  using handling_1 =
      async::handling<logger, event_1, queue_1, handler_1>;
};

struct handlings_004 {
  static std::string desc() {
    return "Defining one handling with 1 handler, retrieving the number of "
           "handlers defined for this handling and it should be 1. Then adds 2 "
           "more handlers, retrieve the number of handlers again and it must "
           "be 3";
  }

  bool operator()(const program::options &) {
    logger _logger;

    using handlings = async::handlings<logger, event_1, handling_1>;

    handling_1 _handling_1{"handling_1", _logger, handler_1{}};
    _handling_1.increment_handlers(1U);

    handlings _handlings{std::move(_handling_1)};

    size_t _num_handlers{0};

    async::result _result{
        _handlings.get_num_handlers<handling_1>(_num_handlers)};

    if (_result != async::result::OK) {
      TNCT_LOG_ERR(_logger, generic::fmt("result = ", _result));
      return false;
    }
    if (_num_handlers != 1U) {
      TNCT_LOG_ERR(_logger,
                   generic::fmt("number of handlers should be 1 but it is = ",
                                _num_handlers));
      return false;
    }

    TNCT_LOG_TST(
        _logger,
        generic::fmt("number of handlers should be 1 and it is actually = ",
                     _num_handlers))

    _result = _handlings.increment_handlers<handling_1>(2U);
    if (_result != async::result::OK) {
      TNCT_LOG_ERR(_logger, generic::fmt("result = ", _result));
      return false;
    }

    _result = _handlings.get_num_handlers<handling_1>(_num_handlers);

    if (_result != async::result::OK) {
      TNCT_LOG_ERR(_logger, generic::fmt("result = ", _result));
      return false;
    }
    if (_num_handlers != 3U) {
      TNCT_LOG_ERR(_logger,
                   generic::fmt("number of handlers should be 3 but it is = ",
                                _num_handlers));
      return false;
    }

    TNCT_LOG_TST(
        _logger,
        generic::fmt("number of handlers should be 3 and it is actually = ",
                     _num_handlers))

    return (_num_handlers == 3);
  }

private:
  using logger = log::cerr;

  struct event_1 {
    friend std::ostream &operator<<(std::ostream &p_out, const event_1 &) {
      return p_out;
    }
  };

  struct handler_1 {
    using event = event_1;
    void operator()(event_1 &&) {}
  };

  using queue_1 = container::circular_queue<logger, event_1, 10>;

  using handling_1 =
      async::handling<logger, event_1, queue_1, handler_1>;
};

struct handlings_005 {
  static std::string desc() {
    return "Defining one handling with 1 handler and adds an event. The event "
           "will be initialized with '-1', the handler will add '-2'";
  }

  bool operator()(const program::options &) {
    logger _logger;

    using handlings = async::handlings<logger, event_1, handling_1>;

    int16_t _i{0};
    handling_1 _handling_1{"handling_1", _logger, handler_1{_logger, &_i}};
    _handling_1.increment_handlers(1U);

    handlings _handlings{std::move(_handling_1)};

    async::result _result{_handlings.add_event<handling_1>(event_1{-1})};
    if (_result != async::result::OK) {
      TNCT_LOG_ERR(_logger, generic::fmt("error: ", _result));
      return false;
    }

    std::this_thread::sleep_for(3s);

    if (_i != -3) {
      TNCT_LOG_ERR(_logger, generic::fmt("i should be -3, but it is ", _i));
      return false;
    }

    return true;
  }

private:
  using logger = log::cerr;

  struct event_1 {
    event_1(){};
    event_1(int16_t p_i) : i(p_i) {}
    event_1(const event_1 &) = default;
    event_1(event_1 &&) = default;
    event_1 &operator=(const event_1 &p_event) {
      i = p_event.i;
      return *this;
    }

    friend std::ostream &operator<<(std::ostream &p_out,
                                    const event_1 &p_event_1) {
      p_out << "i = " << p_event_1.i;
      return p_out;
    }
    int16_t i{0};
  };

  struct handler_1 {
    using event = event_1;
    handler_1(logger &p_logger, int16_t *p_i) : m_logger(p_logger), m_i(p_i) {}
    // handler_1(const handler_1 &p_handler_1)
    //     : m_logger(p_handler_1.m_logger)
    //     , m_i(p_handler_1.m_i)
    // {}
    void operator()(event_1 &&p_event) {
      std::this_thread::sleep_for(2s);
      *m_i = p_event.i - 2;
      TNCT_LOG_TST(m_logger, generic::fmt("*m_i is now ", *m_i));
    }

  private:
    logger &m_logger;
    int16_t *m_i;
  };

  using queue_1 = container::circular_queue<logger, event_1, 10>;

  using handling_1 =
      async::handling<logger, event_1, queue_1, handler_1>;
};

struct handlings_006 {
  static std::string desc() {
    return "Defining one handling with 1 handler, adds an event, then stops "
           "the handling and check is the handling is actually stopped";
  }

  bool operator()(const program::options &) {
    logger _logger;

    using handlings = async::handlings<logger, event_1, handling_1>;

    uint16_t _counter{0};
    handling_1 _handling_1{"handling_1", _logger, handler_1{_logger}};
    _handling_1.increment_handlers(1U);

    handlings _handlings{std::move(_handling_1)};

    while (!_handlings.is_stopped<handling_1>()) {
      async::result _result{
          _handlings.add_event<handling_1>(event_1{_counter})};
      if (_result != async::result::OK) {
        TNCT_LOG_ERR(_logger, generic::fmt("error: ", _result));
        return false;
      }

      if (_counter == 14) {
        auto _result{_handlings.stop<handling_1>()};
        if (_result != async::result::OK) {
          TNCT_LOG_ERR(_logger,
                       generic::fmt("result is ", _result,
                                    ", but is should be ", async::result::OK));
          return false;
        }
        if (!_handlings.is_stopped<handling_1>()) {
          TNCT_LOG_ERR(
              _logger,
              generic::fmt("handlings should be stopped, but it is not"));
          return false;
        }
        TNCT_LOG_TST(_logger,
                     generic::fmt("handlings is stopped, as it should"))
      } else {
        ++_counter;
      }
    }
    return true;
  }

private:
  using logger = log::cerr;

  struct event_1 {
    event_1(){};
    event_1(uint16_t p_i) : i(p_i) {}
    event_1(const event_1 &) = default;
    event_1(event_1 &&) = default;
    event_1 &operator=(const event_1 &p_event) {
      i = p_event.i;
      return *this;
    }

    friend std::ostream &operator<<(std::ostream &p_out,
                                    const event_1 &p_event_1) {
      p_out << "i = " << p_event_1.i;
      return p_out;
    }
    uint16_t i{0};
  };

  struct handler_1 {
    using event = event_1;
    handler_1(logger &p_logger) : m_logger(p_logger) {}

    void operator()(event_1 &&p_event) {
      std::this_thread::sleep_for(800ms);

      TNCT_LOG_TST(m_logger, generic::fmt("p_event.i = ", p_event.i));
    }

  private:
    logger &m_logger;
  };

  using queue_1 = container::circular_queue<logger, event_1, 10>;

  using handling_1 =
      async::handling<logger, event_1, queue_1, handler_1>;
};

struct handlings_007 {
  static std::string desc() { return "Just use the << operator"; }

  bool operator()(const program::options &) {
    logger _logger;

    using handlings =
        async::handlings<logger, event_1, handling_1a, handling_1b>;

    handling_1a _handling_1a{"handling_1a", _logger, handler_1a{_logger}};
    handling_1b _handling_1b{"handling_1b", _logger, handler_1b{_logger}};

    handlings _handlings{std::move(_handling_1a), std::move(_handling_1b)};

    TNCT_LOG_TST(_logger, generic::fmt(_handlings));

    return true;
  }

private:
  using logger = log::cerr;

  struct event_1 {
    event_1(){};
    event_1(uint16_t p_i) : i(p_i) {}
    event_1(const event_1 &) = default;
    event_1(event_1 &&) = default;
    event_1 &operator=(const event_1 &p_event) {
      i = p_event.i;
      return *this;
    }

    friend std::ostream &operator<<(std::ostream &p_out,
                                    const event_1 &p_event_1) {
      p_out << "i = " << p_event_1.i;
      return p_out;
    }
    uint16_t i{0};
  };

  struct handler_1a {
    using event = event_1;
    handler_1a(logger &p_logger) : m_logger(p_logger) {}

    void operator()(event_1 &&p_event) {
      std::this_thread::sleep_for(800ms);

      TNCT_LOG_TST(m_logger, generic::fmt("*m_i is now ", p_event.i));
    }

  private:
    logger &m_logger;
  };

  using queue_1a = container::circular_queue<logger, event_1, 10>;

  using handling_1a =
      async::handling<logger, event_1, queue_1a, handler_1a>;

  struct handler_1b {
    using event = event_1;
    handler_1b(logger &p_logger) : m_logger(p_logger) {}

    void operator()(event_1 &&p_event) {
      TNCT_LOG_TST(m_logger, generic::fmt("p_event * 2 is ", p_event.i * 2));
    }

  private:
    logger &m_logger;
  };

  using queue_1b = container::circular_queue<logger, event_1, 30>;

  using handling_1b =
      async::handling<logger, event_1, queue_1b, handler_1b>;
};
// struct handlings_base {
//   handlings_base() { m_logger.set_deb(); }

//   using logger = log::cerr;

//   struct ev1 {
//     friend std::ostream &operator<<(std::ostream &p_out, const ev1 &) {
//       p_out << "ev1";
//       return p_out;
//     }
//   };

//   struct ev2 {
//     friend std::ostream &operator<<(std::ostream &p_out, const ev2 &) {
//       p_out << "ev2";
//       return p_out;
//     }
//   };

//   template <traits::event t_event>
//   using queue = container::circular_queue<logger, t_event>;

//   template <traits::event t_event>
//   using handlings_t = async::handlings<logger, t_event>;

//   struct subscriber_1 {
//     using events_subscribed = std::tuple<ev1>;

//     subscriber_1(logger &p_logger) : m_logger(p_logger) {}

//     void handle(ev1 &&) { m_logger.tst("sub_1 - ev1"); }

//   private:
//     logger &m_logger;
//   };

//   struct subscriber_2 {
//     using events_subscribed = std::tuple<ev1>;

//     subscriber_2(logger &p_logger) : m_logger(p_logger) {}

//     void handle(ev1 &&) { m_logger.tst("sub_2 - ev1"); }

//   private:
//     logger &m_logger;
//   };

//   log::cerr m_logger;
// };

// struct handlings_000 : public handlings_base {
//   static std::string desc() {
//     return "Simple creation and addition of two event handlings";
//   }

//   bool operator()(const program::options &) {

//     handlings_t<ev1> _eql(m_logger);
//     subscriber_1 _sub_1(m_logger);

//     auto _result = _eql.add_handling<subscriber_1>(
//         "1", _sub_1, 0, async::handling_priority::medium, 10);

//     if (_result != async::result::OK) {
//       m_logger.err(generic::fmt("error: ", _result));
//       return false;
//     }

//     subscriber_2 _sub_2(m_logger);
//     _result = _eql.add_handling<subscriber_2>(
//         "2", _sub_2, 0, async::handling_priority::medium, 10);

//     if (_result != async::result::OK) {
//       m_logger.err(generic::fmt("error: ", _result));
//       return false;
//     }

//     m_logger.tst(generic::fmt("events handlings: ", _eql));

//     return true;
//   }
// };

// struct handlings_001 : public handlings_base {
//   static std::string desc() {
//     return "Creating a handling with one later and adding a second handler "
//            "later";
//   }

//   bool operator()(const program::options &) {

//     handlings_t<ev1> _ehl(m_logger);
//     subscriber_1 _sub(m_logger);

//     auto _result(_ehl.add_handling<subscriber_1>(
//         "1", _sub, 1, async::handling_priority::medium, 10));
//     if (_result != async::result::OK) {
//       m_logger.err(generic::fmt("error: ", _result));
//       return false;
//     }

//     _result = _ehl.increment_handlers("1", 1);
//     if (_result != async::result::OK) {
//       m_logger.err(generic::fmt("error: ", _result));
//       return false;
//     }

//     return _ehl.get_num_handlers("1").value() == 2;
//   }
// };

// struct handlings_002 : public handlings_base {
//   static std::string desc() {
//     return "Creating 2 event handlings, one with " +
//            std::to_string(m_num_handlers_1) + " handlers the other with " +
//            std::to_string(m_num_handlers_2) + ", adding " +
//            std::to_string(m_amount_events) + " events";
//   }

//   bool operator()(const program::options &) {

//     std::mutex _mutex_wait_finish;
//     std::condition_variable _cond_wait_finish;

//     logger _log;
//     handlings_t<ev1> _handlings(_log);

//     std::mutex _mutex_counter_1;
//     uint16_t _counter_1(0);
//     std::map<std::thread::id, uint16_t> _map_1;

//     async::handling_id _handling_id_1("handling 1");

//     subscriber_a _sub_1(m_logger, _counter_1, _mutex_counter_1,
//                         _cond_wait_finish, _map_1);
//     auto _result(_handlings.add_handling<subscriber_a>(
//         _handling_id_1, _sub_1, m_num_handlers_1,
//         async::handling_priority::medium, 10));
//     if (_result != async::result::OK) {
//       m_logger.err(generic::fmt("error: ", _result));
//       return false;
//     }

//     std::mutex _mutex_counter_2;
//     uint16_t _counter_2(0);
//     std::map<std::thread::id, uint16_t> _map_2;

//     async::handling_id _handling_id_2("handling 2");
//     subscriber_b _sub_2(m_logger, _counter_2, _mutex_counter_2,
//                         _cond_wait_finish, _map_2);

//     _result = _handlings.add_handling<subscriber_b>(
//         _handling_id_2, _sub_2, m_num_handlers_2,
//         async::handling_priority::medium, 10);
//     if (_result != async::result::OK) {
//       m_logger.err(generic::fmt("error: ", _result));
//       return false;
//     }

//     using event_generator = async::sleeping_loop<logger>;
//     auto _generate = [&]() {
//       auto _result(_handlings.add_event(ev1()));
//       if (_result != async::result::OK) {
//         m_logger.err(generic::fmt(_result));
//       }
//     };
//     event_generator _event_generator(_log, _generate, 10ms);
//     _event_generator.start();

//     _log.tst(generic::fmt("waiting for all ", m_amount_events * 2,
//                           " events to be handled"));
//     {
//       std::unique_lock<std::mutex> _lock(_mutex_wait_finish);
//       _cond_wait_finish.wait(_lock, [&]() {
//         return (_counter_1 >= m_amount_events) &&
//                (_counter_2 >= m_amount_events);
//       });
//     }

//     uint16_t _total(0);
//     for (const auto &_value : _map_1) {
//       _log.tst(generic::fmt("handling ", _handling_id_1, ", thread ",
//                             _value.first, " handled ", _value.second,
//                             " events"));
//       _total += _value.second;
//     }

//     _log.tst(generic::fmt("total handled by ", _handling_id_1, " = ",
//     _total));

//     _total = 0;
//     for (const auto &_value : _map_2) {
//       _log.tst(generic::fmt("handling ", _handling_id_2, ", thread ",
//                             _value.first, " handled ", _value.second,
//                             " events"));
//       _total += _value.second;
//     }

//     _log.tst(generic::fmt("total handled by ", _handling_id_2, " = ",
//     _total));

//     return (_counter_1 == m_amount_events) && (_counter_2 ==
//     m_amount_events);
//   }

// private:
//   using logger = log::cerr;

//   struct subscriber_a {
//     using events_subscribed = std::tuple<ev1>;

//     subscriber_a(logger &p_logger, uint16_t &p_counter, std ::mutex &p_mutex,
//                  std::condition_variable &p_cond,
//                  std::map<std::thread::id, uint16_t> &p_map)
//         : m_logger(p_logger), m_counter(p_counter), m_mutex(p_mutex),
//           m_cond(p_cond), m_map(p_map) {}

//     void handle(ev1 &&) {
//       std::lock_guard<std::mutex> _lock(m_mutex);
//       m_logger.tst(generic::fmt("handler 1, thread ",
//                                 std::this_thread::get_id(),
//                                 " handling counter ", m_counter++));
//       ++m_map[std::this_thread::get_id()];
//       m_cond.notify_one();
//     }

//   private:
//     logger &m_logger;
//     uint16_t &m_counter;
//     std ::mutex &m_mutex;
//     std::condition_variable &m_cond;
//     std::map<std::thread::id, uint16_t> &m_map;
//   };

//   struct subscriber_b {
//     using events_subscribed = std::tuple<ev1>;

//     subscriber_b(logger &p_logger, uint16_t &p_counter, std ::mutex &p_mutex,
//                  std::condition_variable &p_cond,
//                  std::map<std::thread::id, uint16_t> &p_map)
//         : m_logger(p_logger), m_counter(p_counter), m_mutex(p_mutex),
//           m_cond(p_cond), m_map(p_map) {}

//     void handle(ev1 &&) {
//       std::lock_guard<std::mutex> _lock(m_mutex);
//       m_logger.tst(generic::fmt("handler 1, thread ",
//                                 std::this_thread::get_id(),
//                                 " handling counter ", m_counter++));
//       ++m_map[std::this_thread::get_id()];
//       m_cond.notify_one();
//     }

//   private:
//     logger &m_logger;
//     uint16_t &m_counter;
//     std ::mutex &m_mutex;
//     std::condition_variable &m_cond;
//     std::map<std::thread::id, uint16_t> &m_map;
//   };

// private:
//   static constexpr uint16_t m_amount_events = 100;
//   static constexpr uint16_t m_num_handlers_1 = 5;
//   static constexpr uint16_t m_num_handlers_2 = 3;
// };

// struct handlings_003 : public handlings_base {
//   static std::string desc() { return "One subscriber for each handling"; }

//   bool operator()(const program::options &) {

//     auto &_log(m_logger);
//     handlings_t<ev1> _handlings(_log);

//     subscriber_1 _sub_1(m_logger);

//     auto _result(_handlings.add_handling<subscriber_1>(
//         "1", _sub_1, 1, async::handling_priority::medium, 10));

//     if (_result != async::result::OK) {
//       m_logger.err(generic::fmt("error: ", _result));
//       return false;
//     }

//     subscriber_2 _sub_2(m_logger);

//     _result = _handlings.add_handling<subscriber_2>(
//         "2", _sub_2, 1, async::handling_priority::medium, 10);
//     if (_result != async::result::OK) {
//       m_logger.err(generic::fmt("error: ", _result));
//       return false;
//     }

//     _result = _handlings.add_event(ev1());
//     if (_result != async::result::OK) {
//       m_logger.err(generic::fmt(_result));
//       return false;
//     }

//     _log.tst("sleeping");
//     std::this_thread::sleep_for(2s);
//     _log.tst("waking");
//     return true;
//   }
// };

// struct handlings_004 : public handlings_base {
//   static std::string desc() {
//     return "One subscriber can not be used in two handlings";
//   }

//   bool operator()(const program::options &) {

//     auto &_log(m_logger);
//     handlings_t<ev1> _handlings(_log);

//     subscriber_1 _sub(m_logger);

//     auto _result(_handlings.add_handling<subscriber_1>(
//         "1", _sub, 1, async::handling_priority::medium, 10));

//     if (_result != async::result::OK) {
//       m_logger.err(generic::fmt("error: ", _result));
//       return false;
//     }

//     _result = _handlings.add_handling<subscriber_1>(
//         "2", _sub, 1, async::handling_priority::medium, 10);
//     if (_result != async::result::HANDLER_USED) {
//       m_logger.err(generic::fmt("error: ", _result));
//       return false;
//     }
//     return true;
//   }
// };

// struct handlings_005 : public handlings_base {
//   static std::string desc() { return "testing move constructor"; }

//   bool operator()(const program::options &) {

//     auto &_log(m_logger);
//     handlings_t<ev1> _handlings(_log);

//     auto _result(async::result::OK);

//     uint16_t _num_call(0);

//     subscriber _sub(_num_call, m_logger);

//     _result = _handlings.add_handling<subscriber>(
//         "handling-000", _sub, 1, async::handling_priority::high, 10);
//     if (_result != async::result::OK) {
//       m_logger.err(generic::fmt(_result));
//       return false;
//     }

//     const uint16_t _num_events(1000);

//     for (uint16_t i = 0; i < _num_events; ++i) {
//       _result = _handlings.add_event(ev1());
//       if (_result != async::result::OK) {
//         m_logger.err(generic::fmt(_result));
//         return false;
//       }
//     }

//     m_logger.tst("sleeping");
//     std::this_thread::sleep_for(300ms);
//     m_logger.tst("woke up");

//     auto _handlings_1(std::move(_handlings));

//     while (_num_call != _num_events) {
//       std::this_thread::sleep_for(200ms);
//       m_logger.tst("wating...");
//     }
//     return true;
//   }

// private:
//   struct subscriber {
//     using events_subscribed = std::tuple<ev1>;

//     subscriber(uint16_t &p_calls, logger &p_logger)
//         : calls(p_calls), m_logger(p_logger) {}

//     void handle(ev1 &&) {
//       m_logger.tst(generic::fmt("calls = ", ++calls));
//       std::this_thread::sleep_for(50ms);
//     }
//     uint16_t &calls;

//   private:
//     logger &m_logger;
//     ;
//   };
// };

#endif
