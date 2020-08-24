/// \example Basic use of tenacitas::concurrent::loop_t

#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>

#include <concurrent/internal/log.h>
#include <concurrent/loop.h>
#include <concurrent/result.h>
#include <logger/cerr/log.h>
#include <status/result.h>
#include <tester/test.h>

using namespace tenacitas;

typedef tenacitas::logger::cerr::log log;
typedef tenacitas::concurrent::loop_t<void, log> loop;
typedef tenacitas::status::result result;

struct producer_consumer_000 {

  struct xpto {

    status::result breaker() {
      return (m_counter == 100 ? concurrent::stopped_by_breaker : status::ok);
    }

    status::result worker() {
      ++m_counter;
      concurrent_debug(log, "counter = ", m_counter);
      return status::ok;
    }

  private:
    int16_t m_counter{0};
  };

  bool operator()() {
    xpto _xpto;
    loop _loop([&_xpto]() -> status::result { return _xpto.worker(); },
               [&_xpto]() -> status::result { return _xpto.breaker(); },
               std::chrono::milliseconds(100));

    status::result _result = _loop.start();

    concurrent_debug(log, "result = ", _result);

    return (_result == status::ok);
  }

  static std::string desc() {
    return "Loop with work function taking no parameter. Break function stops "
           "when a counter reaches 100. No timeout.";
  }
};

struct loop_001 {

  struct xpto {

    status::result breaker() {
      return (m_counter == 20 ? concurrent::stopped_by_breaker : status::ok);
    }

    status::result worker() {
      ++m_counter;
      std::this_thread::sleep_for(std::chrono::seconds(1 * m_counter));
      concurrent_debug(log, "counter = ", m_counter);
      return status::ok;
    }

    int16_t counter() const { return m_counter; }

  private:
    int16_t m_counter{0};
  };

  bool operator()() {
    xpto _xpto;
    loop _loop([&_xpto]() -> status::result { return _xpto.worker(); },
               [&_xpto]() -> status::result { return _xpto.breaker(); },
               std::chrono::seconds(8));

    status::result _result = _loop.start();

    concurrent_debug(log, "result = ", _result,
                         ", counter = ", _xpto.counter());

    return ((_result == concurrent::stopped_by_timeout) &&
            (_xpto.counter() == 8));
  }

  static std::string desc() {
    return "Loop with work function taking no parameter. Break function stops "
           "when a counter reaches 20, but a timeout will occurr when the "
           "counter reaches 8.";
  }
};

struct loop_002 {

  static const result stop_because_so;

  struct xpto {

    status::result breaker() {
      return (m_counter == 100 ? concurrent::stopped_by_breaker : status::ok);
    }

    status::result worker() {
      ++m_counter;
      concurrent_debug(log, "counter = ", m_counter);
      if (m_counter == 22) {
        return stop_because_so;
      }
      return status::ok;
    }

  private:
    int16_t m_counter{0};
  };

  bool operator()() {
    xpto _xpto;
    loop _loop([&_xpto]() -> status::result { return _xpto.worker(); },
               [&_xpto]() -> status::result { return _xpto.breaker(); },
               std::chrono::milliseconds(100));

    status::result _result = _loop.start();

    concurrent_debug(log, "result = ", _result);

    return (_result == stop_because_so);
  }

  static std::string desc() {
    return "Loop with work function taking no parameter. Work function "
           "register a 'stop because so' status, and it is the status it "
           "returns. Loop should stop with the result the Work function "
           "created.";
  }
};

const result loop_002::stop_because_so{100, 1};

struct loop_003 {
  typedef tenacitas::concurrent::loop_t<int16_t, log> loop;

  struct xpto {

    std::pair<status::result, int16_t> provider() {
      m_counter += 4;
      return {status::ok, m_counter};
    }

    status::result breaker() {
      return (m_counter == 100 ? concurrent::stopped_by_breaker : status::ok);
    }

    status::result worker(int16_t &&p_value) {
      concurrent_debug(log, "counter = ", p_value);
      return status::ok;
    }

  private:
    int16_t m_counter{0};
  };

  bool operator()() {

    xpto _xpto;

    loop _loop(
        [&_xpto](uint16_t p_value) -> status::result {
          return _xpto.worker(std::move(p_value));
        },

        [&_xpto]() -> status::result { return _xpto.breaker(); },
        std::chrono::milliseconds(100),

        [&_xpto]() -> std::pair<status::result, int16_t> {
          return _xpto.provider();
        });

    status::result _result = _loop.start();

    concurrent_debug(log, "result = ", _result);

    return (_result == status::ok);
  }

  static std::string desc() {
    return "Loop with work function taking a int16_t. Break function stops "
           "when a counter reaches 100. No timeout.";
  }
};

int main(int argc, char **argv) {
  log::set_debug();
  tester::test _test(argc, argv);
  run_test(_test, producer_consumer_000);
  run_test(_test, loop_001);
  run_test(_test, loop_002);
  run_test(_test, loop_003);
}
