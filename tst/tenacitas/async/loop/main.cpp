/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <ctime>
#include <functional>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <tuple>

#include <tenacitas/async.h>
#include <tenacitas/logger.h>

#include <tenacitas/number.h>
#include <tenacitas/tester.h>

/// TODO test when provider and breaker functions take too long, causing
/// timeout

using namespace tenacitas;

using namespace std::chrono_literals;

struct loop_000 {

  static std::string desc() {
    std::stringstream _stream;
    _stream << "no timeout, one param";
    return _stream.str();
  }

  bool operator()() {

    int16_t _i{0};
    auto _provider = [this, &_i]() -> int16_t {
      if (_i == 8) {
        m_cond.notify_one();
      } else {
        ++_i;
        DEB(m_log, "providing ", _i);
      }
      return _i;
    };

    async::internal::loop_t<int16_t> _loop(m_id, worker(), 500ms, _provider);

    _loop.start();

    {
      std::unique_lock<std::mutex> _lock(m_mutex);
      m_cond.wait(_lock);
    }

    _loop.stop();

    if (_i != 8) {
      ERR(m_log, "i should be 8, but it is ", _i);
      return false;
    }

    INF(m_log, "i is 8, as expected");

    return true;
  }

private:
  struct worker {
    void operator()(std::shared_ptr<bool>, int16_t &&p_i) {
      DEB(m_log, "working with = ", p_i);
      std::this_thread::sleep_for(250ms);
    }

  private:
    logger::cerr<> m_log{"worker"};
  };

private:
  std::condition_variable m_cond;
  std::mutex m_mutex;
  logger::cerr<> m_log{"loop_000"};
  number::id m_id;
};

struct loop_002 {

  static std::string desc() {
    std::stringstream _stream;
    _stream << "no timeout, many params";
    return _stream.str();
  }

  bool operator()() {

    int16_t _i{0};
    float _f{0.0};
    auto _provider = [this, &_i, &_f]() -> std::tuple<int16_t, float> {
      if (_i == 8) {
        m_cond.notify_one();
      } else {
        ++_i;
        _f = _i * 2.5;
        DEB(m_log, "providing ", _i);
      }
      return {_i, _f};
    };

    async::internal::loop_t<int16_t, float> _loop(m_id, worker(), 500ms,
                                                  _provider);

    _loop.start();

    {
      std::unique_lock<std::mutex> _lock(m_mutex);
      m_cond.wait(_lock);
    }

    _loop.stop();

    if ((_i != 8) && (_f != (8 * 2.5))) {
      ERR(m_log, "i should be 8 and f should be 20.0, but they are they are ",
          _i, " and ", _f);
      return false;
    }

    INF(m_log, "i is 8 and f is 20, as expected");

    return true;
  }

private:
  struct worker {
    void operator()(std::shared_ptr<bool>, int16_t &&p_i, float &&p_f) {
      DEB(m_log, "working with = ", p_i, ", ", p_f);
      std::this_thread::sleep_for(250ms);
    }

  private:
    logger::cerr<> m_log{"worker"};
  };

private:
  std::condition_variable m_cond;
  std::mutex m_mutex;
  logger::cerr<> m_log{"loop_002"};
  number::id m_id;
};

struct loop_004 {

  static std::string desc() {
    std::stringstream _stream;
    _stream << "no timeout, no params";
    return _stream.str();
  }

  bool operator()() {

    worker _worker;

    auto _aux = [this, &_worker](std::shared_ptr<bool> p_stop) -> void {
      if (_worker.get() == 8) {
        m_cond.notify_one();
      } else {
        _worker(p_stop);
      }
    };

    async::internal::loop_t<void> _loop(m_id, _aux, 500ms);

    _loop.start();

    {
      std::unique_lock<std::mutex> _lock(m_mutex);
      m_cond.wait(_lock);
    }

    _loop.stop();

    if (_worker.get() != 8) {
      ERR(m_log, "i should be 8, but it is ", _worker.get());
      return false;
    }

    INF(m_log, "i is 8, as expected");

    return true;
  }

private:
  struct worker {

    void operator()(std::shared_ptr<bool>) {
      DEB(m_log, "working with = ", ++m_i);
      std::this_thread::sleep_for(250ms);
    }

    inline int16_t get() const { return m_i; }

  private:
    int16_t m_i{0};
    logger::cerr<> m_log{"worker"};
  };

private:
  std::condition_variable m_cond;
  std::mutex m_mutex;
  logger::cerr<> m_log{"loop_003"};
  number::id m_id;
};

struct loop_006 {

  static std::string desc() {
    std::stringstream _stream;
    _stream << "timeout, one param";
    return _stream.str();
  }

  loop_006() { INF(m_log, "006 starts"); }
  ~loop_006() { INF(m_log, "006 ends"); }

  bool operator()() {
    try {

      const int16_t _max{9};
      const std::chrono::milliseconds _work_timeout{500};
      const std::chrono::milliseconds _work_normal_sleep{250ms};
      const std::chrono::milliseconds _work_timeout_sleep{
          _work_timeout.count() * 2};
      const std::chrono::milliseconds _wait{_max * _work_normal_sleep.count() +
                                            3000};
      int16_t _i{0};
      auto _provider = [this, &_i,
                        _max]() -> std::optional<std::tuple<int16_t>> {
        if (_i == _max) {
          DEB(m_log, "causing timeout to not provide data when ", _i,
              " == ", _max);
          std::this_thread::sleep_for(2s);
          return {};
        }
        ++_i;
        DEB(m_log, "providing ", _i);
        return {{_i}};
      };

      worker _worker(_max, _work_normal_sleep, _work_timeout_sleep);

      auto _aux = [this, &_worker](std::shared_ptr<bool> p_stop,
                                   int16_t &&p_i) -> void {
        if (p_i == _max) {
          m_cond.notify_one();
        }
        _worker(p_stop, std::move(p_i));
      };

      async::internal::loop_t<int16_t> _loop(m_id, _aux, _work_timeout,
                                             _provider);

      _loop.start();

      {
        std::unique_lock<std::mutex> _lock{m_mutex};
        m_cond.wait_for(_lock, _wait);
        _loop.stop();
      }

      if (_i != _max) {
        ERR(m_log, "i should be ", _max, ", but it is ", _i);
        return false;
      }

      INF(m_log, "i is ", _max, " as expected");

      return true;
    } catch (std::exception &_ex) {
      FAT(m_log, _ex.what());
    }
    return false;
  }

private:
  struct worker {
    worker(int16_t p_max, std::chrono::milliseconds p_work_normal_sleep,
           std::chrono::milliseconds p_work_timeout_sleep)
        : m_max(p_max), m_work_normal_sleep(p_work_normal_sleep),
          m_work_timeout_sleep(p_work_timeout_sleep) {}

    void operator()(std::shared_ptr<bool>, int16_t &&p_i) {
      DEB(m_log, "working with = ", p_i);
      if (p_i == m_max) {
        DEB(m_log, "causing timeout sleeping for ",
            m_work_timeout_sleep.count());
        std::this_thread::sleep_for(m_work_timeout_sleep);
        return;
      }

      std::this_thread::sleep_for(m_work_normal_sleep);
    }

  private:
    int16_t m_max;
    std::chrono::milliseconds m_work_normal_sleep;
    std::chrono::milliseconds m_work_timeout_sleep;
    logger::cerr<> m_log{"worker"};
  };

private:
  std::condition_variable m_cond;
  std::mutex m_mutex;
  logger::cerr<> m_log{"loop_006"};
  number::id m_id;
};

struct loop_007 {

  static std::string desc() {
    std::stringstream _stream;
    _stream << "timeout, 2 params";
    return _stream.str();
  }

  loop_007() { INF(m_log, "007 starts"); }
  ~loop_007() { INF(m_log, "007 ends"); }

  bool operator()() {

    const int16_t _max{6};
    const std::chrono::milliseconds _work_timeout{500};
    const std::chrono::milliseconds _work_normal_sleep{250ms};
    const std::chrono::milliseconds _work_timeout_sleep{_work_timeout.count() *
                                                        2};
    const std::chrono::milliseconds _wait{_max * _work_normal_sleep.count() +
                                          3000};

    int16_t _i{0};
    auto _provider = [this, &_i, _work_timeout,
                      _max]() -> std::optional<std::tuple<int16_t, float>> {
      if (_i == _max) {
        DEB(m_log, "causing timeout to not provide data when ", _i,
            " == ", _max);
        std::this_thread::sleep_for(
            std::chrono::milliseconds(_work_timeout.count() * 2));
        return {};
      }
      ++_i;
      float _f(_i * 2.5);
      DEB(m_log, "providing ", _i);
      return {{_i, _f}};
    };

    worker _worker(_max, _work_normal_sleep, _work_timeout_sleep);

    auto _aux = [&_worker](std::shared_ptr<bool> p_stop, int16_t &&p_i,
                           float &&p_f) -> void {
      _worker(p_stop, std::move(p_i), std::move(p_f));
    };

    async::internal::loop_t<int16_t, float> _loop(m_id, _aux, _work_timeout,
                                                  _provider);

    _loop.start();

    {
      std::unique_lock<std::mutex> _lock{m_mutex};
      m_cond.wait_for(_lock, _wait);
      _loop.stop();
    }

    if (_i != _max) {
      ERR(m_log, "i should be ", _max, ", but it is ", _i);
      return false;
    }

    INF(m_log, "i is ", _max, " as expected");

    return true;
  }

private:
  struct worker {

    worker(int16_t p_max, std::chrono::milliseconds p_work_normal_sleep,
           std::chrono::milliseconds p_work_timeout_sleep)
        : m_max(p_max), m_work_normal_sleep(p_work_normal_sleep),
          m_work_timeout_sleep(p_work_timeout_sleep) {}

    void operator()(std::shared_ptr<bool>, int16_t &&p_i, float &&p_f) {
      DEB(m_log, "working with = ", p_i, ", ", p_f);
      if (p_i == m_max) {
        DEB(m_log, "causing timeout");
        std::this_thread::sleep_for(m_work_timeout_sleep);
        return;
      }
      std::this_thread::sleep_for(m_work_normal_sleep);
    }

  private:
    int16_t m_max;
    std::chrono::milliseconds m_work_normal_sleep;
    std::chrono::milliseconds m_work_timeout_sleep;
    logger::cerr<> m_log{"worker"};
  };

private:
  std::condition_variable m_cond;
  std::mutex m_mutex;
  logger::cerr<> m_log{"loop_007"};
  number::id m_id;
};

struct loop_008 {

  static std::string desc() {
    std::stringstream _stream;
    _stream << "timeout, no params";
    return _stream.str();
  }

  bool operator()() {

    const int16_t _max{23};
    const std::chrono::milliseconds _work_timeout{500ms};
    const std::chrono::milliseconds _work_normal_sleep{250ms};
    const std::chrono::milliseconds _work_timeout_sleep{_work_timeout.count() *
                                                        2};
    const std::chrono::milliseconds _wait{_max * _work_normal_sleep.count() +
                                          3000};

    worker _worker(_max, _work_normal_sleep, _work_timeout_sleep);

    async::internal::loop_t<void> _loop(
        m_id,
        [&_worker](std::shared_ptr<bool> p_stop) -> void { _worker(p_stop); },
        _work_timeout);

    _loop.start();

    {
      std::unique_lock<std::mutex> _lock{m_mutex};
      m_cond.wait_for(_lock, _wait);
      _loop.stop();
    }

    if (_worker.get() != _max) {
      ERR(m_log, "i should be ", _max, ", but it is ", _worker.get());
      return false;
    }

    INF(m_log, "i is ", _max, " as expected");

    return true;
  }

private:
  struct worker {

    worker(int16_t p_max, std::chrono::milliseconds p_work_normal_sleep,
           std::chrono::milliseconds p_work_timeout_sleep)
        : m_max(p_max), m_work_normal_sleep(p_work_normal_sleep),
          m_work_timeout_sleep(p_work_timeout_sleep) {}

    void operator()(std::shared_ptr<bool>) {
      if (m_i > m_max) {
        return;
      }

      if (m_i == m_max) {
        DEB(m_log, "causing timeout");
        std::this_thread::sleep_for(m_work_timeout_sleep);
        return;
      }
      DEB(m_log, "working with = ", m_i);
      ++m_i;
      std::this_thread::sleep_for(m_work_normal_sleep);
    }

    inline int16_t get() const { return m_i; }

  private:
    int16_t m_max;
    std::chrono::milliseconds m_work_normal_sleep;
    std::chrono::milliseconds m_work_timeout_sleep;
    int16_t m_i{0};
    logger::cerr<> m_log{"worker"};
  };

private:
  std::condition_variable m_cond;
  std::mutex m_mutex;
  logger::cerr<> m_log{"loop_008"};
  number::id m_id;
};

struct loop_009 {

  static std::string desc() {
    std::stringstream _stream;
    _stream << "timeout, no params";
    return _stream.str();
  }

  bool operator()() {

    const int16_t _max{249};
    const std::chrono::milliseconds _work_timeout{500};
    const std::chrono::milliseconds _work_normal_sleep{250ms};
    const std::chrono::milliseconds _work_timeout_sleep{_work_timeout.count() *
                                                        2};
    const std::chrono::milliseconds _wait{_max * _work_normal_sleep.count() +
                                          10000};

    worker _worker(_max, _work_normal_sleep, _work_timeout_sleep);

    auto _aux = [&_worker](std::shared_ptr<bool> p_stop) -> void {
      _worker(p_stop);
    };

    async::internal::loop_t<void> _loop(m_id, _aux, _work_timeout);

    _loop.start();

    {
      std::unique_lock<std::mutex> _lock{m_mutex};
      m_cond.wait_for(_lock, _wait);
      _loop.stop();
    }

    if (_worker.get() != _max) {
      ERR(m_log, "i should be ", _max, ", but it is ", _worker.get());
      return false;
    }

    INF(m_log, "i is ", _max, " as expected");

    return true;
  }

private:
  struct worker {

    worker(int16_t p_max, std::chrono::milliseconds p_work_normal_sleep,
           std::chrono::milliseconds p_work_timeout_sleep)
        : m_max(p_max), m_work_normal_sleep(p_work_normal_sleep),
          m_work_timeout_sleep(p_work_timeout_sleep) {}

    void operator()(std::shared_ptr<bool>) {
      DEB(m_log, "working with = ", m_i);
      if (m_i == m_max) {
        DEB(m_log, "causing timeout");
        std::this_thread::sleep_for(m_work_timeout_sleep);
        return;
      }
      if (m_i > m_max) {
        return;
      }
      ++m_i;
      std::this_thread::sleep_for(m_work_normal_sleep);
    }

    inline int16_t get() const { return m_i; }

  private:
    int16_t m_max;
    std::chrono::milliseconds m_work_normal_sleep;
    std::chrono::milliseconds m_work_timeout_sleep;
    int16_t m_i{0};
    logger::cerr<> m_log{"worker"};
  };

private:
  std::condition_variable m_cond;
  std::mutex m_mutex;
  logger::cerr<> m_log{"loop_009"};
  number::id m_id;
};

struct loop_010 {

  static std::string desc() {
    std::stringstream _stream;
    _stream << "timeout, no params";
    return _stream.str();
  }

  bool operator()() {

    const int16_t _max{40};
    const std::chrono::milliseconds _work_timeout{500};
    const std::chrono::milliseconds _work_normal_sleep{250ms};
    const std::chrono::milliseconds _work_timeout_sleep{_work_timeout.count() *
                                                        2};
    const std::chrono::milliseconds _wait{_max * _work_normal_sleep.count() +
                                          10000};

    worker _worker(_max, _work_normal_sleep, _work_timeout_sleep);

    auto _aux = [&_worker](std::shared_ptr<bool> p_stop) -> void {
      _worker(p_stop);
    };

    async::internal::loop_t<void> _loop(m_id, _aux, _work_timeout);

    DEB(m_log, "moving loop to loop1");
    async::internal::loop_t<void> _loop1(std::move(_loop));

    _loop1.start();

    std::this_thread::sleep_for(500ms);

    DEB(m_log, "moving loop1 to loop");
    _loop = std::move(_loop1);

    {
      std::unique_lock<std::mutex> _lock{m_mutex};
      m_cond.wait_for(_lock, _wait);
      _loop.stop();
    }

    if (_worker.get() != _max) {
      ERR(m_log, "i should be ", _max, ", but it is ", _worker.get());
      return false;
    }

    INF(m_log, "i is ", _max, " as expected");

    return true;
  }

private:
  struct worker {

    worker(int16_t p_max, std::chrono::milliseconds p_work_normal_sleep,
           std::chrono::milliseconds p_work_timeout_sleep)
        : m_max(p_max), m_work_normal_sleep(p_work_normal_sleep),
          m_work_timeout_sleep(p_work_timeout_sleep) {}

    void operator()(std::shared_ptr<bool>) {
      DEB(m_log, "working with = ", m_i);
      if (m_i == m_max) {
        DEB(m_log, "causing timeout");
        std::this_thread::sleep_for(m_work_timeout_sleep);
        return;
      }
      if (m_i > m_max) {
        return;
      }
      ++m_i;
      std::this_thread::sleep_for(m_work_normal_sleep);
    }

    inline int16_t get() const { return m_i; }

  private:
    int16_t m_max;
    std::chrono::milliseconds m_work_normal_sleep;
    std::chrono::milliseconds m_work_timeout_sleep;
    int16_t m_i{0};
    logger::cerr<> m_log{"worker"};
  };

private:
  std::condition_variable m_cond;
  std::mutex m_mutex;
  logger::cerr<> m_log{"loop_010"};
  number::id m_id;
};

struct loop_011 {
  static std::string desc() {
    return "stopping and starting the same async_loop";
  }

  bool operator()() {
    typedef async::internal::loop_t<void> async_loop;

    const uint16_t _max{10};
    uint16_t _counter{1};

    auto _worker = [this, &_counter](std::shared_ptr<bool>) -> void {
      if (_counter == _max) {
        DEB(m_log, "notifying");
        m_cond.notify_one();
        return;
      }
      DEB(m_log, "counter = ", _counter++);
      std::this_thread::sleep_for(500ms);
    };

    async_loop _a1(m_id, _worker, 1s);

    DEB(m_log, "starting");
    _a1.start();

    DEB(m_log, "sleeping");
    std::this_thread::sleep_for(2s);
    DEB(m_log, "waking up");

    DEB(m_log, "stopping");
    _a1.stop();

    DEB(m_log, "starting again");
    _a1.start();

    {
      DEB(m_log, "waiting");
      std::unique_lock<std::mutex> _lock(m_mutex);
      m_cond.wait(_lock, [&_counter]() -> bool { return _counter == _max; });
    }

    DEB(m_log, "done waiting");

    _a1.stop();
    DEB(m_log, "stopped");

    if (_counter != _max) {
      ERR(m_log, "counter should be ", _max, ", but it is ", _counter);
      return false;
    }

    INF(m_log, "counter should be ", _max, ", and it is ", _counter);
    return true;
  }

private:
  logger::cerr<> m_log{"loop_011"};
  std::condition_variable m_cond;
  std::mutex m_mutex;
  number::id m_id;
};

struct loop_012 {
  static std::string desc() {
    return "starting, sleeping, stopping, moving, starting the new one, "
           "stopping";
  }

  bool operator()() {
    typedef async::internal::loop_t<void> async_loop;

    const uint16_t _max{10};
    uint16_t _counter{1};

    auto _worker = [this, &_counter](std::shared_ptr<bool>) -> void {
      if (_counter == _max) {
        DEB(m_log, "notifying");
        m_cond.notify_one();
        return;
      }
      DEB(m_log, " counter = ", _counter++);
      std::this_thread::sleep_for(500ms);
    };

    async_loop _a1(m_id, _worker, 1s);

    DEB(m_log, "starting");
    _a1.start();

    DEB(m_log, "sleeping");
    std::this_thread::sleep_for(2s);

    DEB(m_log, "stopping");
    _a1.stop();

    DEB(m_log, "moving");
    async_loop _a2(std::move(_a1));

    DEB(m_log, "starting");
    _a2.start();

    {
      std::unique_lock<std::mutex> _lock(m_mutex);
      m_cond.wait(_lock, [&_counter]() -> bool { return _counter == _max; });
    }

    DEB(m_log, "stopping");
    _a2.stop();

    if (_counter != _max) {
      ERR(m_log, "counter should be ", _max, ", but it is ", _counter);
      return false;
    }

    INF(m_log, "counter should be ", _max, ", and it is ", _counter);
    return true;
  }

private:
  logger::cerr<> m_log{"loop_012"};
  std::condition_variable m_cond;
  std::mutex m_mutex;
  number::id m_id;
};

struct loop_013 {
  static std::string desc() { return "starting, sleeping, moving, stopping"; }

  bool operator()() {
    typedef async::internal::loop_t<void> async_loop;

    const uint16_t _max{10};
    uint16_t _counter{1};

    auto _worker = [this, &_counter](std::shared_ptr<bool>) -> void {
      if (_counter == _max) {
        DEB(m_log, "notifying");
        m_cond.notify_one();
        return;
      }
      DEB(m_log, " counter = ", _counter++);
      std::this_thread::sleep_for(500ms);
    };

    async_loop _a1(m_id, _worker, 1s);

    DEB(m_log, "starting");
    _a1.start();

    DEB(m_log, "sleeping");
    std::this_thread::sleep_for(2s);

    DEB(m_log, "moving");
    async_loop _a2(std::move(_a1));

    DEB(m_log, "starting");
    _a2.start();

    {
      std::unique_lock<std::mutex> _lock(m_mutex);
      m_cond.wait(_lock, [&_counter]() -> bool { return _counter == _max; });
    }

    DEB(m_log, "stopping");
    _a2.stop();

    if (_counter != _max) {
      ERR(m_log, "counter should be ", _max, ", but it is ", _counter);
      return false;
    }

    INF(m_log, "counter should be ", _max, ", and it is ", _counter);
    return true;
  }

private:
  logger::cerr<> m_log{"loop_013"};
  std::condition_variable m_cond;
  std::mutex m_mutex;
  number::id m_id;
};

struct loop_014 {
  static std::string desc() {
    return "starting, sleeping, moving, destructing";
  }

  bool operator()() {
    typedef async::internal::loop_t<void> async_loop;

    const uint16_t _max{10};
    uint16_t _counter{1};

    auto _worker = [this, &_counter](std::shared_ptr<bool>) -> void {
      if (_counter == _max) {
        DEB(m_log, "notifying");
        m_cond.notify_one();
        return;
      }
      DEB(m_log, " counter = ", _counter++);
      std::this_thread::sleep_for(500ms);
    };

    async_loop _a1(m_id, _worker, 1s);

    DEB(m_log, "starting");
    _a1.start();

    DEB(m_log, "sleeping");
    std::this_thread::sleep_for(2s);

    DEB(m_log, "moving");
    async_loop _a2(std::move(_a1));

    DEB(m_log, "starting");
    _a2.start();

    {
      std::unique_lock<std::mutex> _lock(m_mutex);
      m_cond.wait(_lock, [&_counter]() -> bool { return _counter == _max; });
    }

    if (_counter != _max) {
      ERR(m_log, "counter should be ", _max, ", but it is ", _counter);
      return false;
    }

    INF(m_log, "counter should be ", _max, ", and it is ", _counter);
    return true;
  }

private:
  logger::cerr<> m_log{"loop_014"};
  std::condition_variable m_cond;
  std::mutex m_mutex;
  number::id m_id;
};

int main(int argc, char **argv) {
  logger::set_debug_level();

  logger::cerr<> m_log{"main"};

  try {
    tenacitas::tester::test _tester(argc, argv);
    run_test(_tester, loop_000);

    run_test(_tester, loop_002);
    run_test(_tester, loop_004);
    run_test(_tester, loop_006);
    run_test(_tester, loop_007);
    run_test(_tester, loop_008);
    run_test(_tester, loop_009);
    run_test(_tester, loop_010);
    run_test(_tester, loop_011);
    run_test(_tester, loop_012);
    run_test(_tester, loop_013);
    run_test(_tester, loop_014);

  } catch (std::exception &_ex) {
    FAT(m_log, "ERROR: '", _ex.what(), '\'');
  }
}