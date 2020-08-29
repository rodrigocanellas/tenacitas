/// \example Basic use of tenacitas::concurrent::loop_t

#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>

#include <concurrent/internal/log.h>
#include <concurrent/loop.h>
#include <logger/cerr/log.h>
#include <tester/test.h>

using namespace tenacitas;

typedef tenacitas::logger::cerr::log log;
typedef tenacitas::concurrent::loop_t<void, log> loop;
typedef tenacitas::status::result result;

struct loop_000 {

  struct xpto {

    bool breaker() { return (m_counter == 100 ? true : false); };

    bool worker() {
      ++m_counter;
      concurrent_debug(m_log, "counter = ", m_counter);
      return true;
    }
    int16_t m_counter{0};

  private:
    log m_log{"loop_000::work1"};
  };

  bool operator()() {
    xpto _xpto;
    auto _worker = [&_xpto]() -> bool { return _xpto.worker(); };
    auto _breaker = [&_xpto]() -> bool { return _xpto.breaker(); };
    loop _loop(_worker, _breaker, std::chrono::milliseconds(100));

    _loop.start();

    bool _result = (_xpto.m_counter == 100);

    concurrent_debug(m_log, "counter == ", _xpto.m_counter,
                     ", result = ", _result);

    return _result;
  }

  static std::string desc() {
    return "Loop with work function taking no parameter. Break function stops "
           "when a counter reaches 100. No timeout.";
  }
  logger::cerr::log m_log{"loop_000"};
};

// struct loop_001 {

//  struct xpto {

//    bool breaker() { return (m_counter == 20 ? true : false); }

//    bool worker() {
//      ++m_counter;
//      std::this_thread::sleep_for(std::chrono::seconds(1 * m_counter));
//      concurrent_debug(m_log, "counter = ", m_counter);
//      return true;
//    }

//    int16_t counter() const { return m_counter; }

//  private:
//    int16_t m_counter{0};
//    logger::cerr::log m_log{"loop_001::xpto"};
//  };

//  bool operator()() {
//    xpto _xpto;
//    loop _loop([&_xpto]() -> bool { return _xpto.worker(); },
//               [&_xpto]() -> bool { return _xpto.breaker(); },
//               std::chrono::seconds(8));

//    status::result _result = _loop.start();

//    concurrent_debug(m_log, "result = ", _result,
//                     ", counter = ", _xpto.counter());

//    return ((_result == concurrent::stopped_by_timeout) &&
//            (_xpto.counter() == 8));
//  }

//  static std::string desc() {
//    return "Loop with work function taking no parameter. Break function stops
//    "
//           "when a counter reaches 20, but a timeout will occurr when the "
//           "counter reaches 8.";
//  }
//  logger::cerr::log m_log{"loop_001"};
//};

// struct loop_002 {

//  static const result stop_because_so;

//  struct xpto {

//    bool breaker() { return (m_counter == 100 ? true : false); }

//    bool worker() {
//      ++m_counter;
//      concurrent_debug(m_log, "counter = ", m_counter);
//      if (m_counter == 22) {
//        return false;
//      }
//      return true;
//    }

//  private:
//    logger::cerr::log m_log{"loop_002::xpto"};
//    int16_t m_counter{0};
//  };

//  bool operator()() {
//    xpto _xpto;
//    loop _loop([&_xpto]() -> bool { return _xpto.worker(); },
//               [&_xpto]() -> bool { return _xpto.breaker(); },
//               std::chrono::milliseconds(100));

//    status::result _result = _loop.start();

//    concurrent_debug(m_log, "result = ", _result);

//    return (_result == stop_because_so);
//  }

//  static std::string desc() {
//    return "Loop with work function taking no parameter. Work function "
//           "register a 'stop because so' status, and it is the status it "
//           "returns. Loop should stop with the result the Work function "
//           "created.";
//  }
//  logger::cerr::log m_log{"loop_002"};
//};

// const result loop_002::stop_because_so{100, 1};

struct loop_003 {
  typedef tenacitas::concurrent::loop_t<int16_t, log> loop;

  struct xpto {

    std::pair<bool, int16_t> provider() {
      m_counter += 4;
      return {true, m_counter};
    }

    bool breaker() { return (m_counter == 100 ? true : false); }

    bool worker(int16_t &&p_value) {
      concurrent_debug(m_log, "counter = ", p_value);
      return true;
    }
    int16_t m_counter{0};

  private:
    logger::cerr::log m_log{"loop_003::xpto"};
  };

  bool operator()() {

    xpto _xpto;

    loop _loop(
        [&_xpto](uint16_t p_value) -> bool {
          return _xpto.worker(std::move(p_value));
        },

        [&_xpto]() -> bool { return _xpto.breaker(); },
        std::chrono::milliseconds(100),

        [&_xpto]() -> std::pair<bool, int16_t> { return _xpto.provider(); });

    _loop.start();

    bool _result = (_xpto.m_counter == 100);

    concurrent_debug(m_log, "counter == ", _xpto.m_counter,
                     ", result = ", _result);

    concurrent_debug(m_log, "result = ", _result);

    return _result;
  }

  static std::string desc() {
    return "Loop with work function taking a int16_t. Break function stops "
           "when a counter reaches 100. No timeout.";
  }
  logger::cerr::log m_log{"loop_003"};
};

int main(int argc, char **argv) {
  log::set_debug();
  tester::test _test(argc, argv);
  run_test(_test, loop_000);
  //  run_test(_test, loop_001);
  //  run_test(_test, loop_002);
  run_test(_test, loop_003);
}
