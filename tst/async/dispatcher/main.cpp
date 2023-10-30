/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <algorithm>
#include <chrono>
#include <cmath>
#include <condition_variable>
#include <cstdint>
#include <fstream>
#include <iterator>
#include <map>
#include <mutex>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>

#include "bus.h"
#include "cfg.h"
#include "sto.h"
#include "typ.h"
#include "uix.h"
#include <tenacitas.lib/src/async/alg/dispatcher.h>
#include <tenacitas.lib/src/log/alg/logger.h>
#include <tenacitas.lib/src/program/alg/options.h>
#include <tenacitas.lib/src/test/alg/tester.h>

using namespace tenacitas::lib;
using namespace std::chrono_literals;
// using namespace tenacitas::lib::async::test;

namespace evt {
struct ev_0 {

  ev_0(uint16_t p_value = 0) : value(p_value) {}

  friend std::ostream &operator<<(std::ostream &p_out, const ev_0 &p_ev) {
    p_out << "ev_0 = " << p_ev.value;
    return p_out;
  }

  uint16_t value{0};
};

struct ev_1 {

  ev_1(char p_value = 'a') : value(p_value) {}

  friend std::ostream &operator<<(std::ostream &p_out, const ev_1 &p_ev) {
    p_out << "ev_1 = " << p_ev.value;
    return p_out;
  }

  char value{0};
};
} // namespace evt

struct test_000 {
  test_000() { TNCT_LOG_TST("creating test_000"); }
  ~test_000() { TNCT_LOG_TST("destroying test_000"); }
  static std::string desc() { return "Declaring a 'Dispatcher' "; }

  using dispatcher = async::alg::dispatcher<evt::ev_0, evt::ev_1>;

  bool operator()(const program::alg::options &) {
    log::alg::set_info_level();
    TNCT_LOG_TST("starting test_000");
    dispatcher::ptr _dispatcher = dispatcher::create();
    TNCT_LOG_TST("finishing test_000");
    return true;
  }
};

struct test_001 {
  test_001() { TNCT_LOG_TST("creating test_001"); }

  ~test_001() { TNCT_LOG_TST("destroying test_001"); }

  using events_subscribed = std::tuple<evt::ev_0>;
  using events_published = std::tuple<evt::ev_0>;

  static std::string desc() {
    return "'Dispatcher' as a local object, publishing 1 event, but not "
           "waiting all to be handled";
  }

  using dispatcher = async::alg::dispatcher<evt::ev_0, evt::ev_1>;
  bool operator()(const program::alg::options &) {
    log::alg::set_info_level();
    TNCT_LOG_TST("starting test_001");
    dispatcher::ptr _dispatcher = dispatcher::create();
    _dispatcher->subscribe<test_001, evt::ev_0>([](auto p_event) {
      std::this_thread::sleep_for(50ms);
      TNCT_LOG_TST("event = ", p_event);
    });

    try {

      _dispatcher->publish<test_001, evt::ev_0>(static_cast<uint16_t>(22));

    } catch (std::exception &_ex) {
      TNCT_LOG_TST("finishing test_001 with false");
      TNCT_LOG_ERR(_ex.what());
      return false;
    }
    TNCT_LOG_TST("finishing test_001 with true");
    return true;
  }
};

struct test_002 {
  test_002() { TNCT_LOG_TST("creating test_002"); }
  ~test_002() { TNCT_LOG_TST("destroying test_002"); }

  using dispatcher = async::alg::dispatcher<evt::ev_0, evt::ev_1>;

  using events_subscribed = std::tuple<evt::ev_0>;
  using events_published = std::tuple<evt::ev_0>;

  static std::string desc() {
    return "'Dispatcher' as a local object, publishing 1 event, and waiting "
           "for it to be handled";
  }

  bool operator()(const program::alg::options &) {
    log::alg::set_info_level();
    TNCT_LOG_TST("starting test_002");
    dispatcher::ptr _dispatcher = dispatcher::create();
    _dispatcher->subscribe<test_002, evt::ev_0>([](auto p_event) {
      std::this_thread::sleep_for(50ms);
      TNCT_LOG_TST("event = ", p_event);
    });

    try {
      _dispatcher->publish<test_002, evt::ev_0>(static_cast<uint16_t>(22));

      std::this_thread::sleep_for(3s);

    } catch (std::exception &_ex) {
      TNCT_LOG_TST("finishing test_001 with false");
      TNCT_LOG_ERR(_ex.what());
      return false;
    }
    TNCT_LOG_TST("finishing test_001 with true");
    return true;
  }
};

struct test_003 {
  test_003() { TNCT_LOG_TST("creating test_003"); }
  ~test_003() { TNCT_LOG_TST("destroying test_003"); }

  using dispatcher = async::alg::dispatcher<evt::ev_0, evt::ev_1>;

  using events_subscribed = std::tuple<evt::ev_0, evt::ev_1>;
  using events_published = std::tuple<evt::ev_0, evt::ev_1>;

  static std::string desc() {
    return "'Dispatcher' as a local object, publishing 1 event evt::ev_0, 1 "
           "event "
           "evt::ev_1 and waiting for them to be handled";
  }

  bool operator()(const program::alg::options &) {
    log::alg::set_info_level();
    TNCT_LOG_TST("starting test_002");
    dispatcher::ptr _dispatcher = dispatcher::create();

    _dispatcher->subscribe<test_003, evt::ev_0>([](auto p_event) {
      std::this_thread::sleep_for(50ms);
      TNCT_LOG_TST("event = ", p_event);
    });

    _dispatcher->subscribe<test_003, evt::ev_1>([](auto p_event) {
      std::this_thread::sleep_for(50ms);
      TNCT_LOG_TST("event = ", p_event);
    });

    try {
      _dispatcher->publish<test_003, evt::ev_0>(static_cast<uint16_t>(22));
      _dispatcher->publish<test_003, evt::ev_1>('z');

      std::this_thread::sleep_for(3s);

    } catch (std::exception &_ex) {
      TNCT_LOG_TST("finishing test_001 with false");
      TNCT_LOG_ERR(_ex.what());
      return false;
    }
    TNCT_LOG_TST("finishing test_001 with true");
    return true;
  }
};

struct test_004 {
  test_004() { TNCT_LOG_TST("creating test_004"); }
  ~test_004() { TNCT_LOG_TST("destroying test_004"); }

  using dispatcher = async::alg::dispatcher<evt::ev_0, evt::ev_1>;

  using events_subscribed = std::tuple<evt::ev_0>;
  using events_published = std::tuple<evt::ev_0>;

  static std::string desc() {
    return "'Dispatcher' as a local object, creating a publishing for "
           "evt::ev_0, "
           "publishing 1 event, and waiting for it to be handled";
  }

  bool operator()(const program::alg::options &) {
    log::alg::set_info_level();
    TNCT_LOG_TST("starting test_002");
    dispatcher::ptr _dispatcher = dispatcher::create();

    auto _publishing_id{_dispatcher->add_queue<evt::ev_0>()};

    _dispatcher->subscribe<test_004, evt::ev_0>(
        _publishing_id, [](auto p_event) {
          std::this_thread::sleep_for(50ms);
          TNCT_LOG_TST("event = ", p_event);
        });

    try {
      _dispatcher->publish<test_004, evt::ev_0>(static_cast<uint16_t>(22));

      std::this_thread::sleep_for(3s);

    } catch (std::exception &_ex) {
      TNCT_LOG_TST("finishing test_001 with false");
      TNCT_LOG_ERR(_ex.what());
      return false;
    }
    TNCT_LOG_TST("finishing test_001 with true");
    return true;
  }
};

struct test_005 {
  test_005() { TNCT_LOG_TST("creating test_005"); }
  ~test_005() { TNCT_LOG_TST("destroying test_005"); }

  using dispatcher = async::alg::dispatcher<evt::ev_0, evt::ev_1>;
  using events_subscribed = std::tuple<evt::ev_0, evt::ev_1>;
  using events_published = std::tuple<evt::ev_0, evt::ev_1>;

  static std::string desc() {
    return "'Dispatcher' as a local object, creating publishing for evt::ev_0, "
           "publishing for evt::ev_1, publishing 1 event evt::ev_0, 1 event "
           "evt::ev_1 "
           "and "
           "waiting for them to be handled";
  }

  bool operator()(const program::alg::options &) {
    log::alg::set_info_level();
    TNCT_LOG_TST("starting test_002");
    dispatcher::ptr _dispatcher = dispatcher::create();

    auto _publishing_id_0{_dispatcher->add_queue<evt::ev_0>()};
    auto _publishing_id_1{_dispatcher->add_queue<evt::ev_1>()};

    _dispatcher->subscribe<test_005, evt::ev_0>(
        _publishing_id_0, [](auto p_event) {
          std::this_thread::sleep_for(50ms);
          TNCT_LOG_TST("event = ", p_event);
        });

    _dispatcher->subscribe<test_005, evt::ev_1>(
        _publishing_id_1, [](auto p_event) {
          std::this_thread::sleep_for(50ms);
          TNCT_LOG_TST("event = ", p_event);
        });

    try {
      _dispatcher->publish<test_005, evt::ev_0>(static_cast<uint16_t>(22));
      _dispatcher->publish<test_005, evt::ev_1>('z');

      std::this_thread::sleep_for(3s);

    } catch (std::exception &_ex) {
      TNCT_LOG_TST("finishing test_001 with false");
      TNCT_LOG_ERR(_ex.what());
      return false;
    }
    TNCT_LOG_TST("finishing test_001 with true");
    return true;
  }
};

struct test_006 {
  static std::string desc() {
    return "Tests if the creation of event queue is correct";
  }

  struct e {};
  using events_published = std::tuple<e>;
  using events_subscribed = std::tuple<e>;

  bool operator()(const program::alg::options &) {
    log::alg::set_info_level();
    size_t _counter{0};

    m_dispatcher->subscribe<test_006, e>(
        [&](auto) { TNCT_LOG_TST("handling # ", _counter++); });

    TNCT_LOG_TST("publishing");

    m_dispatcher->publish<test_006, e>();

    TNCT_LOG_TST("# amount event queues: ",
                 m_dispatcher->amount_of_queues<e>());

    TNCT_LOG_TST("sleeping");
    std::this_thread::sleep_for(2s);
    TNCT_LOG_TST("waking up");
    return true;
  }

private:
  using dispatcher = async::alg::dispatcher<e>;

private:
  dispatcher::ptr m_dispatcher{dispatcher::create()};
};

struct test_corner_000 {

  using events_published = std::tuple<evt::ev_0>;
  using events_subscribed = std::tuple<evt::ev_0>;
  using dispatcher = async::alg::dispatcher<evt::ev_0, evt::ev_1>;

  static std::string desc() {
    return "'Dispatcher' as a local object, publishing 200 events, but not "
           "waiting all to be handled";
  }

  bool operator()(const program::alg::options &) {
    log::alg::set_info_level();
    dispatcher::ptr _dispatcher = dispatcher::create();
    _dispatcher->subscribe<test_corner_000, evt::ev_0>([](auto p_event) {
      std::this_thread::sleep_for(50ms);
      TNCT_LOG_TST("event = ", p_event);
    });

    try {
      for (uint16_t _i = 0; _i < 3; ++_i) {
        _dispatcher->publish<test_corner_000, evt::ev_0>(_i);
      }

      //      std::this_thread::sleep_for(100ms);

    } catch (std::exception &_ex) {
      TNCT_LOG_ERR(_ex.what());
      return false;
    }

    return true;
  }
};

struct test_corner_001 {
  using dispatcher = async::alg::dispatcher<evt::ev_0>;
  using events_published = std::tuple<evt::ev_0>;
  using events_subscribed = std::tuple<evt::ev_0>;

  static std::string desc() {
    return "'Dispatcher' as an attribute, publishing 200 events, but not "
           "waiting all to be handled";
  }

  bool operator()(const program::alg::options &) {
    log::alg::set_info_level();
    m_dispatcher->subscribe<test_corner_001, evt::ev_0>([](auto p_event) {
      std::this_thread::sleep_for(50ms);
      TNCT_LOG_TST("event = ", p_event);
    });

    try {
      for (uint16_t _i = 0; _i < 200; ++_i) {
        m_dispatcher->publish<test_corner_001, evt::ev_0>(_i);
      }

      std::this_thread::sleep_for(100ms);

    } catch (std::exception &_ex) {
      TNCT_LOG_ERR(_ex.what());
      return false;
    }

    return true;
  }

private:
  dispatcher::ptr m_dispatcher{dispatcher::create()};
};

async::alg::dispatcher<evt::ev_0, evt::ev_1>::ptr s_dispatcher_test_corner_002{
    async::alg::dispatcher<evt::ev_0, evt::ev_1>::create()};

struct test_corner_002 {
  using events_published = std::tuple<evt::ev_0>;
  using events_subscribed = std::tuple<evt::ev_0>;

  static std::string desc() {
    return "'Dispatcher' as a global object, publishing 200 events, but not "
           "waiting all to be handled";
  }

  bool operator()(const program::alg::options &) {
    log::alg::set_info_level();
    s_dispatcher_test_corner_002->subscribe<test_corner_002, evt::ev_0>(
        [](auto p_event) {
          std::this_thread::sleep_for(50ms);
          TNCT_LOG_TST("event = ", p_event);
        });

    try {
      for (uint16_t _i = 0; _i < 200; ++_i) {
        s_dispatcher_test_corner_002->publish<test_corner_002, evt::ev_0>(_i);
      }

      std::this_thread::sleep_for(100ms);

    } catch (std::exception &_ex) {
      TNCT_LOG_ERR(_ex.what());
      return false;
    }

    return true;
  }
};

struct test_dispatcher {

  static void setup(int p_argc, char **p_argv) {
    m_argc = p_argc;
    m_argv = p_argv;
  }

  static std::string desc() {
    std::stringstream _stream;

    cfg::help(m_argv[0], _stream);
    return _stream.str();
  }

  bool operator()(const program::alg::options &) {
    log::alg::set_trace_level();
    try {
      bus::dispatcher::ptr _dispatcher{bus::dispatcher::create()};

      cfg::options _options{m_argc, m_argv};

      sto::saver _save(_options);

      bus::processor _processor(std::move(_save));
      return uix::start(_options, _dispatcher, std::move(_processor));

    } catch (std::exception &_ex) {
      TNCT_LOG_ERR(_ex.what());
    }
    return false;
  }

private:
  static int m_argc;
  static char **m_argv;
};

int test_dispatcher::m_argc;
char **test_dispatcher::m_argv;

int main(int argc, char **argv) {

  using namespace tenacitas::lib;

  log::alg::set_max_file_name_lenght(13);

  program::alg::options _options;

  test_dispatcher::setup(argc, argv);

  test::alg::tester _tester(argc, argv);

  run_test(_tester, test_dispatcher);
  run_test(_tester, test_000);
  run_test(_tester, test_001);
  run_test(_tester, test_002);
  run_test(_tester, test_003);
  run_test(_tester, test_004);
  run_test(_tester, test_005);
  run_test(_tester, test_006);
  run_test(_tester, test_corner_000);
  run_test(_tester, test_corner_001);
  run_test(_tester, test_corner_002);
}
