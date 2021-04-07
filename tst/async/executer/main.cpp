#include <chrono>
#include <cstdint>
#include <functional>
#include <future>
#include <iostream>
#include <optional>

#include <tenacitas/concurrent.h>
#include <tenacitas/logger.h>
#include <tenacitas/tester.h>

using namespace tenacitas;

using namespace std::chrono_literals;

struct timeout_callback {
  inline void operator()(std::thread::id p_id) {
    WAR(m_log, "timeout for ", p_id);
  }

private:
  logger::cerr<> m_log{"timeout_callback"};
};

timeout_callback _timeout_callback;

// struct test000 {

//  static std::string desc() {
//    return "no timeout - two parameters - return";
//  }

//  bool operator()() {

//    typedef concurrent::executer_t<logger::cerr<>, double, int, float>
//    executer;

//    m_log.set_debug_level();

//    auto function = [this](int p_i, float p_f) -> double {
//      DEB(m_log, "i = ", p_i, ", f = ", p_f);
//      return p_i * p_f;
//    };

//    DEB(m_log, "about to create");
//    executer _executer(function, 1s, _timeout_callback);
//    DEB(m_log, "created");

//    int _i = 2;
//    float _f = 3.5;

//    DEB(m_log, "about to call");
//    std::optional<double> _maybe = _executer(_i, _f);

//    if (!_maybe) {
//      ERR(m_log, "nothing returned, but it should have");
//    }

//    double _d = std::move(*_maybe);

//    if (_d != (_i * _f)) {
//      ERR(m_log, "expected ", _i * _f, ", but got ", _d);
//      return false;
//    }

//    DEB(m_log, "got ", _d, ", as expected");

//    std::this_thread::sleep_for(1s);
//    return true;
//  }

// private:
//  logger::cerr<> m_log{"test000"};
//};

// struct test100 {

//  static std::string desc() {
//    return "executer as executer_t<logger, double, int> with timeout";
//  }

//  bool operator()() {

//    m_log.set_debug_level();

//    typedef concurrent::executer_t<logger::cerr<>, double, int> executer;

//    std::chrono::seconds _timeout(1);

//    auto _worker = [this](int p_i) -> double {
//      std::this_thread::sleep_for(2s);
//      DEB(m_log, "i = ", p_i);
//      return p_i * 2.5;
//    };

//    executer _executer(_worker, _timeout, _timeout_callback);

//    _executer.start();

//    int _i = 2;

//    std::optional<double> _maybe = _executer(_i);

//    if (_maybe) {
//      ERR(m_log, "returned, but it should have not");
//      return false;
//    }

//    DEB(m_log, "sleeping");
//    std::this_thread::sleep_for(5s);
//    DEB(m_log, "waking");
//    return true;
//  }

// private:
//  logger::cerr<> m_log{"test100"};
//};

// struct test101 {

//  static std::string desc() { return "same code as test100"; }

//  bool operator()() {

//    m_log.set_debug_level();

//    typedef concurrent::executer_t<logger::cerr<>, double, int> executer;

//    std::chrono::seconds _timeout(1);

//    auto function = [this](int p_i) -> double {
//      std::this_thread::sleep_for(2s);
//      DEB(m_log, "i = ", p_i);
//      return p_i * 2.5;
//    };

//    executer _executer(function, _timeout, timeout_callback());

//    _executer.start();

//    int _i = 2;

//    std::optional<double> _maybe = _executer(_i);

//    if (_maybe) {
//      ERR(m_log, "returned, but it should have not");
//      return false;
//    }

//    DEB(m_log, "sleeping");
//    std::this_thread::sleep_for(5s);
//    DEB(m_log, "waking");
//    return true;
//  }

// private:
//  logger::cerr<> m_log{"test101"};
//};

// struct test200 {

//  static std::string desc() {
//    return "executer as executer_t<logger, double, int>, with timeout, and "
//           "function returned before the next call, which did not timeout";
//  }

//  bool operator()() {

//    m_log.set_debug_level();

//    const std::chrono::seconds _timeout{1};
//    std::chrono::milliseconds _sleep{2000};

//    typedef concurrent::executer_t<logger::cerr<>, double, int> executer;

//    auto _function = [this, &_sleep](int p_i) -> double {
//      DEB(m_log, "i = ", p_i, ", and sleep = ", _sleep.count(), "ms");
//      std::this_thread::sleep_for(_sleep);
//      return p_i * 2.5;
//    };

//    executer _executer(_function, _timeout, timeout_callback());

//    _executer.start();

//    int _i = 2;

//    std::optional<double> _maybe = _executer(_i);

//    if (_maybe) {
//      ERR(m_log, "a timeout should have occurred");
//      return false;
//    }

//    DEB(m_log,
//        "timeout in the first call, sleeping to allow the function to end");

//    std::this_thread::sleep_for(1s);

//    _sleep = 500ms;

//    DEB(m_log, "running again, and no timeout should occurr");

//    _maybe = _executer(_i);

//    if (!_maybe) {
//      ERR(m_log, "it was not possible to execute, but it should have been");
//      return false;
//    }

//    double _d = std::move(*_maybe);

//    if (_d != (_i * 2.5)) {
//      ERR(m_log, "value should be ", _i * 2.5, ", but it is ", _d);
//      return false;
//    }
//    DEB(m_log, "value is ", _d, ", as it should");

//    std::this_thread::sleep_for(3s);

//    return true;
//  }

// private:
//  logger::cerr<> m_log{"test200"};
//};

// struct test300 {

//  static std::string desc() {
//    return "executer as executer_t<logger, double, int>, with timeout, and "
//           "function did not returned before the next call, which did not "
//           "timeout";
//  }

//  bool operator()() {

//    m_log.set_debug_level();

//    const std::chrono::seconds _timeout{1};
//    std::chrono::milliseconds _sleep{4000};

//    typedef concurrent::executer_t<logger::cerr<>, double, int> executer;

//    auto _function = [this, &_sleep](int p_i) -> double {
//      DEB(m_log, "i = ", p_i, ", and sleep = ", _sleep.count(), "ms");
//      std::this_thread::sleep_for(_sleep);
//      return p_i * 2.5;
//    };

//    executer _executer(_function, _timeout, timeout_callback());

//    _executer.start();

//    int _i = 2;

//    std::optional<double> _maybe = _executer(_i);

//    if (_maybe) {
//      ERR(m_log, "a timeout should have occurred");
//      return false;
//    }

//    DEB(m_log, "timeout in the first call, sleeping not enough to "
//               "allow the function to end");

//    std::this_thread::sleep_for(500ms);

//    _sleep = 500ms;

//    DEB(m_log, "running again, and no timeout should occurr because sleep is
//    ",
//        _sleep.count(), "millisecs");

//    _maybe = _executer(_i);

//    if (!_maybe) {
//      ERR(m_log, "it was not possible to execute, but it should have been");
//      return false;
//    }

//    double _d = std::move(*_maybe);

//    if (_d != (_i * 2.5)) {
//      ERR(m_log, "value should be ", _i * 2.5, ", but it is ", _d);
//      return false;
//    }
//    DEB(m_log, "value is ", _d, ", as it should");

//    std::this_thread::sleep_for(30s);

//    return true;
//  }

// private:
//  logger::cerr<> m_log{"test300"};
//};

struct test100 {
  static std::string desc() { return "timeout - one param - return"; }

  bool operator()() {
    try {
      typedef concurrent::executer_t<logger::cerr<>, double, int16_t> executer;

      constexpr std::chrono::milliseconds _timeout{50};
      constexpr uint16_t _max{100};
      constexpr uint16_t _missed{static_cast<uint16_t>(_max / 3)};

      uint16_t _expected = 0;
      for (int16_t _i{0}; _i < _max; ++_i) {
        if (_i != _missed) {
          _expected += _i;
        }
      }

      work _work(_timeout, _missed);
      executer _exec([&_work](int16_t p_val) -> double { return _work(p_val); },
                     _timeout,
                     [this](std::thread::id p_id) -> void {
                       WAR(m_log, "timeout for ", p_id);
                     });

      for (int16_t _i{0}; _i < _max; ++_i) {
        _exec(_i);
      }

      // template <class Rep, class Period, class Predicate>
      // bool wait_for (unique_lock<mutex>& lck, const
      // chrono::duration<Rep,Period>& rel_time, Predicate pred);
      //
      // false if the predicate pred still evaluates to false after the rel_time
      // timeout expired, otherwise true.
      //
      // or...
      //
      // The predicate version (2) returns pred(), regardless of whether the
      // timeout was triggered (although it can only be false if triggered).
      constexpr auto _rel_time{
          std::chrono::milliseconds(_timeout.count() * _max + 3000)};
      auto _pred = [&_work, _expected]() -> bool {
        return _work.get() == _expected;
      };
      std::unique_lock<std::mutex> _lock{m_mutex};
      if (!m_cond.wait_for(_lock, _rel_time, _pred)) {
        ERR(m_log, _rel_time.count(), "ms elapsed, and the result is ",
            _work.get(), ", when it should be ", _expected);
        return false;
      }
      INF(m_log, "result is ", _work.get(),
          ", and it must be correct, as it should ", _expected);
      return true;
    } catch (std::exception &_ex) {
      FAT(m_log, _ex.what());
    }
    return false;
  }

private:
  struct work {

    work() = delete;
    work(std::chrono::milliseconds p_timeout, uint16_t p_missed)
        : m_timeout(p_timeout), m_missed(p_missed) {
      DEB(m_log, "max = ", m_missed, ", timeout = ", m_timeout.count());
    }

    double operator()(int16_t p_val) {

      if (p_val == m_missed) {
        INF(m_log, "causing timeout with i = ", p_val);
        std::this_thread::sleep_for(
            std::chrono::milliseconds(m_timeout.count() * 2));
        DEB(m_log, "woke up");

        return -3.14;
      }
      m_val += p_val;
      DEB(m_log, "p_val = ", p_val, ", m_val = ", m_val);
      return m_val;
    }

    uint16_t get() const { return m_val; }

  private:
    std::chrono::milliseconds m_timeout;

    uint16_t m_missed;
    uint16_t m_val{0};

    logger::cerr<> m_log{"work100"};
  };

private:
  logger::cerr<> m_log{"test100"};
  std::mutex m_mutex;
  std::condition_variable m_cond;
};

struct test101 {
  static std::string desc() { return "timeout - one param - return"; }

  bool operator()() {
    try {

      constexpr std::chrono::milliseconds _timeout{50};
      constexpr uint16_t _max{100};
      constexpr uint16_t _missed{static_cast<uint16_t>(_max / 3)};

      uint16_t _expected = 0;
      for (int16_t _i{0}; _i < _max; ++_i) {
        if (_i != _missed) {
          _expected += _i;
        }
      }

      work _work(_timeout, _missed);

      auto _exec = [&_work](int16_t p_i) -> void { _work(p_i); };

      for (int16_t _i{0}; _i < _max; ++_i) {
        std::future<void> _future = std::async(std::launch::async, _exec, _i);
      }

      // template <class Rep, class Period, class Predicate>
      // bool wait_for (unique_lock<mutex>& lck, const
      // chrono::duration<Rep,Period>& rel_time, Predicate pred);
      //
      // false if the predicate pred still evaluates to false after the rel_time
      // timeout expired, otherwise true.
      //
      // or...
      //
      // The predicate version (2) returns pred(), regardless of whether the
      // timeout was triggered (although it can only be false if triggered).
      constexpr auto _rel_time{
          std::chrono::milliseconds(_timeout.count() * _max + 3000)};
      auto _pred = [&_work, _expected]() -> bool {
        return _work.get() == _expected;
      };
      std::unique_lock<std::mutex> _lock{m_mutex};
      if (!m_cond.wait_for(_lock, _rel_time, _pred)) {
        ERR(m_log, _rel_time.count(), "ms elapsed, and the result is ",
            _work.get(), ", when it should be ", _expected);
        return false;
      }
      INF(m_log, "result is ", _work.get(),
          ", and it must be correct, as it should ", _expected);
      return true;
    } catch (std::exception &_ex) {
      FAT(m_log, _ex.what());
    }
    return false;
  }

private:
  struct work {

    work() = delete;
    work(std::chrono::milliseconds p_timeout, uint16_t p_missed)
        : m_timeout(p_timeout), m_missed(p_missed) {
      DEB(m_log, "max = ", m_missed, ", timeout = ", m_timeout.count());
    }

    double operator()(int16_t p_val) {

      if (p_val == m_missed) {
        INF(m_log, "causing timeout with i = ", p_val);
        std::this_thread::sleep_for(
            std::chrono::milliseconds(m_timeout.count() * 2));
        DEB(m_log, "woke up");

        return -3.14;
      }
      m_val += p_val;
      DEB(m_log, "p_val = ", p_val, ", m_val = ", m_val);
      return m_val;
    }

    uint16_t get() const { return m_val; }

  private:
    std::chrono::milliseconds m_timeout;

    uint16_t m_missed;
    uint16_t m_val{0};

    logger::cerr<> m_log{"work101"};
  };

private:
  logger::cerr<> m_log{"test101"};
  std::mutex m_mutex;
  std::condition_variable m_cond;
};

struct test200 {
  static std::string desc() { return "timeout - no param - return"; }

  bool operator()() {
    try {
      typedef concurrent::executer_t<logger::cerr<>, int16_t, void> executer;

      const std::chrono::milliseconds _timeout{1000};
      const uint16_t _max{9};

      work _work(_timeout, static_cast<uint16_t>(_max / 3));
      executer _exec([&_work](void) -> uint16_t { return _work(); }, _timeout,
                     [this](std::thread::id p_id) -> void {
                       WAR(m_log, "timeout for ", p_id);
                     });

      for (int16_t _i{0}; _i < _max; ++_i) {
        _exec();
      }
      INF(m_log, "sleeping...");
      std::this_thread::sleep_for(
          std::chrono::milliseconds(_timeout.count() * _max + 3000));
      uint16_t _res = _work.get();
      uint16_t _expected{_max};
      if (_res != _expected) {
        ERR(m_log, "res = ", _res, ", but it should be ", _expected);
        return false;
      }
      INF(m_log, "res = ", _res, ", and it is right, as ", _expected,
          " was expected");
      return true;

    } catch (std::exception &_ex) {
      FAT(m_log, _ex.what());
    }
    return false;
  }

private:
  struct work {

    work() = delete;
    work(std::chrono::milliseconds p_timeout, uint16_t p_missed)
        : m_timeout(p_timeout), m_max(p_missed) {
      DEB(m_log, "max = ", m_max, ", timeout = ", m_timeout.count());
    }

    int16_t operator()() {
      ++m_val;
      if (m_val == m_max) {
        INF(m_log, "causing timeout with i = ", m_val);
        std::this_thread::sleep_for(
            std::chrono::milliseconds(m_timeout.count() * 2));
        DEB(m_log, "woke up");

        return std::numeric_limits<uint16_t>::max();
      }

      DEB(m_log, "val = ", m_val);
      return m_val;
    }

    uint16_t get() const { return m_val; }

  private:
    std::chrono::milliseconds m_timeout;
    uint16_t m_max;
    uint16_t m_val{0};
    logger::cerr<> m_log{"work200"};
  };

private:
  logger::cerr<> m_log{"test200"};
};

struct test300 {
  static std::string desc() { return "timeout - one param - no return"; }

  bool operator()() {
    try {
      typedef concurrent::executer_t<logger::cerr<>, void, int16_t> executer;

      const std::chrono::milliseconds _timeout{1000};
      const uint16_t _max{9};

      work _work(_timeout, static_cast<uint16_t>(_max / 3));
      executer _exec([&_work](int16_t p_val) -> void { _work(p_val); },
                     _timeout,
                     [this](std::thread::id p_id) -> void {
                       WAR(m_log, "timeout for ", p_id);
                     });

      for (int16_t _i{0}; _i < _max; ++_i) {
        _exec(_i);
      }
      INF(m_log, "sleeping...");
      std::this_thread::sleep_for(
          std::chrono::milliseconds(_timeout.count() * _max + 3000));
      uint16_t _res = _work.get();
      uint16_t _expected{_max - 1};
      if (_res != _expected) {
        ERR(m_log, "res = ", _res, ", but it should be ", _expected);
        return false;
      }
      INF(m_log, "res = ", _res, ", and it is right, as ", _expected,
          " was expected");
      return true;

    } catch (std::exception &_ex) {
      FAT(m_log, _ex.what());
    }
    return false;
  }

private:
  struct work {

    work() = delete;
    work(std::chrono::milliseconds p_timeout, uint16_t p_missed)
        : m_timeout(p_timeout),

          m_max(p_missed) {
      DEB(m_log, "max = ", m_max, ", timeout = ", m_timeout.count());
    }

    void operator()(int16_t p_val) {
      m_val = p_val;
      if (m_val == m_max) {
        INF(m_log, "causing timeout with i = ", p_val);
        std::this_thread::sleep_for(
            std::chrono::milliseconds(m_timeout.count() * 2));
        DEB(m_log, "woke up");

        return;
      }

      DEB(m_log, "val = ", m_val);
    }

    uint16_t get() const { return m_val; }

  private:
    std::chrono::milliseconds m_timeout;
    uint16_t m_max;
    uint16_t m_val{0};
    logger::cerr<> m_log{"work300"};
  };

private:
  logger::cerr<> m_log{"test300"};
};

struct test400 {
  static std::string desc() { return "timeout - no params - no return"; }

  bool operator()() {
    try {
      typedef concurrent::executer_t<logger::cerr<>, void, void> executer;

      const std::chrono::milliseconds _timeout{1000};
      const uint16_t _max{9};

      work _work(_timeout, static_cast<uint16_t>(_max / 3));
      executer _exec([&_work]() -> void { _work(); }, _timeout,
                     [this](std::thread::id p_id) -> void {
                       WAR(m_log, "timeout for ", p_id);
                     });

      for (uint16_t _i{0}; _i < _max; ++_i) {
        _exec();
      }
      INF(m_log, "sleeping...");
      std::this_thread::sleep_for(
          std::chrono::milliseconds(_timeout.count() * _max + 3000));
      uint16_t _res = _work.get();
      if (_res != _max) {
        ERR(m_log, "res = ", _res, ", but it should be ", _max);
        return false;
      }
      INF(m_log, "res = ", _res, ", and it is right, as ", _max,
          " was expected");
      return true;

    } catch (std::exception &_ex) {
      FAT(m_log, _ex.what());
    }
    return false;
  }

private:
  struct work {

    work() = delete;
    work(std::chrono::milliseconds p_timeout, uint16_t p_missed)
        : m_timeout(p_timeout), m_sleep(p_timeout.count() / 2),
          m_max(p_missed) {
      DEB(m_log, "max = ", m_max, ", timeout = ", m_timeout.count());
    }

    void operator()() {
      if (++m_i == m_max) {
        DEB(m_log, "causing timeout with i = ", m_i);
        std::this_thread::sleep_for(
            std::chrono::milliseconds(m_timeout.count() * 2));
        DEB(m_log, "woke up");
        return;
      }

      DEB(m_log, "sleeping");
      std::this_thread::sleep_for(m_sleep);
      DEB(m_log, "i = ", m_i);
    }

    uint16_t get() const { return m_i; }

  private:
    std::chrono::milliseconds m_timeout;
    std::chrono::milliseconds m_sleep;
    uint16_t m_max;
    uint16_t m_i{0};
    logger::cerr<> m_log{"work400"};
  };

private:
  logger::cerr<> m_log{"test400"};
};

int main(int argc, char **argv) {

  logger::set_info_level();
  tester::test _test(argc, argv);
  run_test(_test, test100);
  run_test(_test, test101);
  run_test(_test, test200);
  run_test(_test, test300);
  run_test(_test, test400);

  return 0;
}
