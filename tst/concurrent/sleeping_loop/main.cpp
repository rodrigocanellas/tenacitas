/// \example

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>

#include <concurrent/internal/log.h>
#include <concurrent/sleeping_loop.h>
#include <concurrent/traits.h>
#include <logger/cerr/log.h>
#include <tester/test.h>

using namespace tenacitas;

struct sleeping_loop_000 {

  bool operator()() {

    typedef concurrent::sleeping_loop_t<void, logger::cerr::log> loop;

    loop _loop(std::chrono::milliseconds(100),
               [] {
                 concurrent_log_debug(logger::cerr::log, "loop1");
                 return concurrent::work_status::dont_stop;
               },
               std::chrono::milliseconds(100));

    return true;
  }

  static const std::string desc() { return "'sleeping_loop' creation test"; }
};

struct sleeping_loop_001 {
  typedef concurrent::sleeping_loop_t<void, logger::cerr::log> loop;

  struct work1 {

    concurrent::work_status operator()() {

      ++counter;
      concurrent_log_debug(logger::cerr::log, "this = ", this,
                           ", counter = ", counter);
      return concurrent::work_status::dont_stop;
    }

    uint64_t counter = 0;
  };

  bool operator()() {

    work1 _work;
    concurrent_log_debug(logger::cerr::log, "work = ", this);
    loop _loop(std::chrono::milliseconds(1000), [&_work]() { return _work(); },
               std::chrono::milliseconds(500));

    _loop.run();
    std::this_thread::sleep_for(std::chrono::seconds(10));

    concurrent_log_debug(logger::cerr::log, "data = ", _work.counter);
    if (_work.counter != 10) {
      return false;
    }

    return true;
  }

  static const std::string desc() {
    return "\n'sleeping_loop' with interval of 1000 ms, work time out of 500 "
           "ms, increments a counter, and just prints using 'cerr_test', so "
           "there will be no time out."

           "\nThe main function will sleep for 10 secs, and the "
           "'sleeping_loop' will stop in the destructor."

           "\nCounter should be 11";
  }

private:
};

struct sleeping_loop_002 {
  typedef concurrent::sleeping_loop_t<void, logger::cerr::log> loop;

  struct work1 {
    concurrent::work_status operator()() {

      ++counter;
      concurrent_log_debug(logger::cerr::log, counter);
      return concurrent::work_status::dont_stop;
    }
    uint64_t counter = 0;
  };

  bool operator()() {
    using namespace tenacitas;
    work1 _work;
    loop _loop(std::chrono::milliseconds(1000), [&_work]() { return _work(); },
               std::chrono::milliseconds(500));

    _loop.run();
    std::this_thread::sleep_for(std::chrono::seconds(10));
    _loop.stop();

    concurrent_log_debug(logger::cerr::log, "data = ", _work.counter);
    if (_work.counter != 10) {
      return false;
    }

    return true;
  }

  static const std::string desc() {
    return "\n'sleeping_loop' with interval of 1000 ms, work time out of 500 "
           "ms, increments a counter, and just prints using 'cerr_test', so "
           "there will be no time out."

           "\nThe main function will sleep for 10 secs, and the "
           "'sleeping_loop' will stop by calling 'stop'."

           "\nWork counter must be 10";
  }
};

struct sleeping_loop_003 {

  typedef concurrent::sleeping_loop_t<void, logger::cerr::log> loop;

  struct work1 {
    concurrent::work_status operator()() {

      ++counter;
      concurrent_log_debug(logger::cerr::log, counter);
      return concurrent::work_status::dont_stop;
    }
    uint64_t counter = 0;
  };

  bool operator()() {
    using namespace tenacitas;
    work1 _work;
    loop _loop(std::chrono::milliseconds(1000), [&_work]() { return _work(); },
               std::chrono::milliseconds(500));

    _loop.run();
    std::this_thread::sleep_for(std::chrono::seconds(10));
    _loop.stop();

    concurrent_log_debug(logger::cerr::log, "data = ", _work.counter);
    if (_work.counter != 10) {
      return false;
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));

    _loop.run();
    std::this_thread::sleep_for(std::chrono::seconds(8));

    concurrent_log_debug(logger::cerr::log, "data = ", _work.counter);
    if (_work.counter != 18) {
      return false;
    }

    return true;
  }

  static const std::string desc() {
    return "\n'sleeping_loop' with interval of 1000 ms, time out of 500 ms, "
           "increments a counter, and just prints using 'cerr_test', so there "
           "will "
           "be no time out."

           "\n'sleeping_loop' will call 'run'; main thread will sleep for 10 "
           "secs; "
           "the 'sleeping_loop' will stop by calling 'stop'; the main thread "
           "will "
           "sleep for 5 secs;, the 'sleeping_loop' will be restarted with "
           "'run'; "
           "the main thread will sleep for 8 secs; the 'sleeping_loop' will "
           "stop in "
           "the destructor."

           "\nCounter should be 18.";
  }
};

struct sleeping_loop_004 {
  typedef concurrent::sleeping_loop_t<void, logger::cerr::log> loop;

  struct work1 {
    concurrent::work_status operator()() {
      using namespace tenacitas;
      ++counter;
      concurrent_log_debug(logger::cerr::log, counter);
      return concurrent::work_status::dont_stop;
    }
    uint64_t counter = 0;
  };

  bool operator()() {

    work1 _work;
    loop _loop(std::chrono::milliseconds(1000), [&_work]() { return _work(); },
               std::chrono::milliseconds(500));

    _loop.run();
    std::this_thread::sleep_for(std::chrono::seconds(10));
    _loop.stop();

    concurrent_log_debug(logger::cerr::log, "data = ", _work.counter);
    if (_work.counter != 10) {
      return false;
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));

    _loop.run();
    std::this_thread::sleep_for(std::chrono::seconds(8));
    _loop.stop();

    concurrent_log_debug(logger::cerr::log, "data = ", _work.counter);
    if (_work.counter != 18) {
      return false;
    }
    return true;
  }

  static const std::string desc() {
    return "\n'sleeping_loop' with interval of 1000 ms, time out of 500 ms, "
           "increments a counter, and just prints using 'cerr_test', so there "
           "will "
           "be no time out."

           "\n'sleeping_loop' will call 'run'; main thread will sleep for 10 "
           "secs; "
           "the 'sleeping_loop' will stop by calling 'stop'; the main thread "
           "will "
           "sleep for 5 secs;, the 'sleeping_loop' will be restarted with "
           "'run'; "
           "the main thread will sleep for 8 secs; the 'sleeping_loop' will "
           "stop by "
           "calling 'stop'."

           "\nCounter should be 18.";
  }
};

struct sleeping_loop_005 {
  typedef concurrent::sleeping_loop_t<void, logger::cerr::log> loop;

  struct work1 {
    concurrent::work_status operator()() {
      using namespace tenacitas;
      ++counter;
      concurrent_log_debug(logger::cerr::log, counter);
      return concurrent::work_status::dont_stop;
    }
    uint64_t counter = 0;
  };

  bool operator()() {

    work1 _work;
    loop _loop_1(std::chrono::milliseconds(1000),
                 [&_work]() { return _work(); },
                 std::chrono::milliseconds(500));

    _loop_1.run();
    std::this_thread::sleep_for(std::chrono::seconds(10));

    concurrent_log_debug(logger::cerr::log, "data = ", _work.counter);
    if (_work.counter != 10) {
      return false;
    }
    _loop_1.stop();

    loop _loop_2(std::chrono::milliseconds(1000),
                 [&_work]() { return _work(); },
                 std::chrono::milliseconds(500));

    _loop_2.run();
    std::this_thread::sleep_for(std::chrono::seconds(3));

    concurrent_log_debug(logger::cerr::log, "data = ", _work.counter);
    if (_work.counter != 13) {
      return false;
    }
    return true;
  }

  static const std::string desc() {
    return "\n'sleeping_loop' with interval of 1000 ms, time out of 500 ms, "
           "increments a counter, and just prints using 'cerr_test', so there "
           "will "
           "be no time out."

           "\nA 'sleeping_loop' will be started, the main thread will sleep "
           "for 10 secs, another 'sleeping_loop' will be created using the "
           "same "
           "parameters as the first one, the main thread will sleep for 3 "
           "secs, the "
           "new 'sleeping_loop' will stop by its destructor."

           "\nCounter should be 13.";
  }
};

struct sleeping_loop_006 {

  typedef concurrent::sleeping_loop_t<void, logger::cerr::log> loop;

  struct work1 {
    concurrent::work_status operator()() {

      ++counter;
      concurrent_log_debug(logger::cerr::log, counter);
      return concurrent::work_status::dont_stop;
    }
    uint64_t counter = 0;
  };

  bool operator()() {
    using namespace tenacitas;
    work1 _work;
    loop _loop_1(std::chrono::milliseconds(1000),
                 [&_work]() { return _work(); },
                 std::chrono::milliseconds(500));

    _loop_1.run();
    std::this_thread::sleep_for(std::chrono::seconds(10));

    concurrent_log_debug(logger::cerr::log, "data = ", _work.counter);
    if (_work.counter != 10) {
      return false;
    }

    _loop_1.stop();

    loop _loop_2(_loop_1.get_interval(), _loop_1.get_work(),
                 _loop_1.get_timeout());

    _loop_2.run();

    std::this_thread::sleep_for(std::chrono::seconds(3));

    _loop_2.stop();

    concurrent_log_debug(logger::cerr::log, "data = ", _work.counter);
    if (_work.counter != 13) {
      return false;
    }

    return true;
  }

  static const std::string desc() {
    return "\n'sleeping_loop' with interval of 1000 ms, time out of 500 ms, "
           "increments a counter, and just prints using 'cerr_test', so there "
           "will "
           "be no time out."

           "\nA 'sleeping_loop' will be started, the main thread will sleep "
           "for 10 secs, another 'sleeping_loop' will be created using the "
           "same "
           "parameters as the first one, the main thread will sleep for 3 "
           "secs, the "
           "new 'sleeping_loop' will stop by calling stop."

           "\nCounter should be 13.";
  }
};

struct sleeping_loop_007 {

  typedef concurrent::sleeping_loop_t<void, logger::cerr::log> loop;

  struct work1 {
    concurrent::work_status operator()() {
      using namespace tenacitas;
      ++counter;
      concurrent_log_debug(logger::cerr::log, counter);
      return concurrent::work_status::dont_stop;
    }
    uint64_t counter = 0;
  };

  bool operator()() {

    work1 _work;
    loop _loop_1(std::chrono::milliseconds(1000),
                 [&_work]() { return _work(); },
                 std::chrono::milliseconds(500));
    _loop_1.run();
    std::this_thread::sleep_for(std::chrono::seconds(10));
    _loop_1.stop();

    concurrent_log_debug(logger::cerr::log, "data = ", _work.counter);
    if (_work.counter != 10) {
      return false;
    }

    _loop_1.stop();

    loop _loop_2(_loop_1.get_interval(), _loop_1.get_work(),
                 _loop_1.get_timeout());

    std::this_thread::sleep_for(std::chrono::seconds(1));

    _loop_2.run();
    std::this_thread::sleep_for(std::chrono::seconds(5));

    concurrent_log_debug(logger::cerr::log, "data = ", _work.counter);
    if (_work.counter != 15) {
      return false;
    }

    return true;
  }

  static const std::string desc() {
    return "\n'sleeping_loop' with interval of 1000 ms, time out of 500 ms, "
           "increments a counter, and just prints using 'cerr_test', so there "
           "will "
           "be no time out."

           "\nA 'sleeping_loop' will be started, the main thread will sleep "
           "for 10 "
           "secs, the 'sleeping_loop' will be stopped, another 'sleeping_loop' "
           "will "
           "be created using the same parameters as the first one, the new "
           "'sleeping_loop' will be started, the main thread will sleep for 5 "
           "secs, "
           "the new 'sleeping_loop' will stop in its destructor."

           "\nCounter should be 15.";
  }
};

struct sleeping_loop_008 {

  typedef concurrent::sleeping_loop_t<void, logger::cerr::log> loop;

  struct work1 {
    concurrent::work_status operator()() {
      using namespace tenacitas;
      ++counter;
      concurrent_log_debug(logger::cerr::log, counter);
      return concurrent::work_status::dont_stop;
    }
    uint64_t counter = 0;
  };

  bool operator()() {

    work1 _work;
    loop _loop_1(std::chrono::milliseconds(1000),
                 [&_work]() { return _work(); },
                 std::chrono::milliseconds(500));

    _loop_1.run();
    std::this_thread::sleep_for(std::chrono::seconds(10));
    _loop_1.stop();

    concurrent_log_debug(logger::cerr::log, "data = ", _work.counter);
    if (_work.counter != 10) {
      return false;
    }

    loop _loop_2(_loop_1.get_interval(), _loop_1.get_work(),
                 _loop_1.get_timeout());

    _loop_1.run();
    std::this_thread::sleep_for(std::chrono::seconds(5));
    _loop_2.stop();

    concurrent_log_debug(logger::cerr::log, "data = ", _work.counter);
    if (_work.counter != 15) {
      return false;
    }
    return true;
  }

  static const std::string desc() {
    return "\n'sleeping_loop' with interval of 1000 ms, time out of 500 ms, "
           "increments a counter, and just prints using 'cerr_test', so there "
           "will "
           "be no time out."

           "\nA 'sleeping_loop' will be started, the main thread will sleep "
           "for 10 "
           "secs, the 'sleeping_loop' will be stopped, another 'sleeping_loop' "
           "will "
           "be created using the same parameters as first one, the new "
           "'sleeping_loop' will be started, the main thread will sleep for 5 "
           "secs, "
           "the new 'sleeping_loop' will be stopped by calling 'stop'."

           "\nCounter should be 15.";
  }
};

struct sleeping_loop_009 {

  typedef concurrent::sleeping_loop_t<void, logger::cerr::log> loop;

  struct work1 {
    concurrent::work_status operator()() {
      using namespace tenacitas;
      ++counter;
      concurrent_log_debug(logger::cerr::log, counter);
      return concurrent::work_status::dont_stop;
    }
    uint64_t counter = 0;
  };

  struct work2 {
    concurrent::work_status operator()() {

      counter += 100;
      concurrent_log_debug(logger::cerr::log, counter);
      return concurrent::work_status::dont_stop;
    }
    uint64_t counter = 0;
  };

  struct work3 {
    concurrent::work_status operator()() {
      using namespace tenacitas;
      counter += 1000;
      concurrent_log_debug(logger::cerr::log, counter);
      return concurrent::work_status::dont_stop;
    }
    uint64_t counter = 0;
  };

  bool operator()() {
    using namespace tenacitas;
    work1 _work_1;
    loop _loop_1(std::chrono::milliseconds(1000),
                 [&_work_1]() { return _work_1(); },
                 std::chrono::milliseconds(100));

    work2 _work_2;
    loop _loop_2(std::chrono::milliseconds(1000),
                 [&_work_2]() { return _work_2(); },
                 std::chrono::milliseconds(100));

    work3 _work_3;
    loop _loop_3(std::chrono::milliseconds(1000),
                 [&_work_3]() { return _work_3(); },
                 std::chrono::milliseconds(100));

    _loop_1.run();
    _loop_2.run();
    _loop_3.run();

    std::this_thread::sleep_for(std::chrono::seconds(6));

    concurrent_log_debug(logger::cerr::log, "data 1 = ", _work_1.counter);
    if (_work_1.counter != 6) {
      return false;
    }

    concurrent_log_debug(logger::cerr::log, "data 2 = ", _work_2.counter);
    if (_work_2.counter != 600) {
      return false;
    }

    concurrent_log_debug(logger::cerr::log, "data 3 = ", _work_3.counter);
    if (_work_3.counter != 6000) {
      return false;
    }

    return true;
  }

  static const std::string desc() {
    return "\n'3 sleeping_loop' objects with interval of 1000 ms, time out of "
           "500 "
           "ms."

           "\nThe first one increments a counter by 1; the second by 100, and "
           "the "
           "third by 1000."

           "\nAll will be started, the main thread will sleep for 6 secs, and "
           "the "
           "all 'sleeping_loop' will stopped by theirs destructors."

           "\nCounter 1 should be 6, counter 2 should be 600, and counter 3 "
           "should "
           "be 6000.";
  }
};

struct sleeping_loop_010 {

  typedef concurrent::sleeping_loop_t<void, logger::cerr::log> loop;

  struct work1 {
    concurrent::work_status operator()() {
      using namespace tenacitas;
      ++counter;
      concurrent_log_debug(logger::cerr::log, counter);
      return concurrent::work_status::dont_stop;
    }
    uint64_t counter = 0;
  };

  struct work2 {
    concurrent::work_status operator()() {
      counter += 100;
      concurrent_log_debug(logger::cerr::log, counter);
      return concurrent::work_status::dont_stop;
    }
    uint64_t counter = 0;
  };

  struct work3 {
    concurrent::work_status operator()() {
      using namespace tenacitas;
      counter += 1000;
      concurrent_log_debug(logger::cerr::log, counter);
      return concurrent::work_status::dont_stop;
    }
    uint64_t counter = 0;
  };

  bool operator()() {
    using namespace tenacitas;
    work1 _work_1;
    loop _loop_1(std::chrono::milliseconds(1000),
                 [&_work_1]() { return _work_1(); },
                 std::chrono::milliseconds(100));

    work2 _work_2;
    loop _loop_2(std::chrono::milliseconds(1000),
                 [&_work_2]() { return _work_2(); },
                 std::chrono::milliseconds(100));

    work3 _work_3;
    loop _loop_3(std::chrono::milliseconds(1000),
                 [&_work_3]() { return _work_3(); },
                 std::chrono::milliseconds(100));

    _loop_1.run();
    _loop_2.run();
    _loop_3.run();

    std::this_thread::sleep_for(std::chrono::seconds(6));

    concurrent_log_debug(logger::cerr::log, "stopping 2");
    _loop_2.stop();

    std::this_thread::sleep_for(std::chrono::seconds(3));

    concurrent_log_debug(logger::cerr::log, "data 1 = ", _work_1.counter);
    if (_work_1.counter != 9) {
      return false;
    }

    concurrent_log_debug(logger::cerr::log, "data 2 = ", _work_2.counter);
    if (_work_2.counter != 600) {
      return false;
    }

    concurrent_log_debug(logger::cerr::log, "data 3 = ", _work_3.counter);
    if (_work_3.counter != 9000) {
      return false;
    }

    return true;
  }

  static const std::string desc() {
    return "\n'3 sleeping_loop' objects with interval of 1000 ms, time out of "
           "500 "
           "ms."

           "\nThe first one increments a counter by 1; the second by 100, and "
           "the "
           "third by 1000."

           "\nAll will be started, the main thread will sleep for 6 secs, the "
           "second 'sleeping_loop' will be stopped, the main thread will sleep "
           "for "
           "seconds all 'sleeping_loop' will stopped by theirs destructors."

           "\nCounter 1 should be 6, counter 2 should be 900, and counter 3 "
           "should "
           "be 6000.";
  }
};

struct sleeping_loop_011 {

  typedef concurrent::sleeping_loop_t<void, logger::cerr::log> loop;

  struct work1 {
    concurrent::work_status operator()() {
      ++counter;
      concurrent_log_debug(logger::cerr::log, counter);
      return concurrent::work_status::dont_stop;
    }
    uint64_t counter = 0;
  };

  struct work2 {
    concurrent::work_status operator()() {
      counter += 100;
      concurrent_log_debug(logger::cerr::log, counter);
      return concurrent::work_status::dont_stop;
    }
    uint64_t counter = 0;
  };

  struct work3 {
    concurrent::work_status operator()() {
      counter += 1000;
      concurrent_log_debug(logger::cerr::log, counter);
      return concurrent::work_status::dont_stop;
    }
    uint64_t counter = 0;
  };

  bool operator()() {
    work1 _work_1;
    loop _loop_1(std::chrono::milliseconds(1000),
                 [&_work_1]() { return _work_1(); },
                 std::chrono::milliseconds(100));

    work2 _work_2;
    loop _loop_2(std::chrono::milliseconds(1000),
                 [&_work_2]() { return _work_2(); },
                 std::chrono::milliseconds(100));

    work3 _work_3;
    loop _loop_3(std::chrono::milliseconds(1000),
                 [&_work_3]() { return _work_3(); },
                 std::chrono::milliseconds(100));

    _loop_1.run();
    _loop_2.run();
    _loop_3.run();

    std::this_thread::sleep_for(std::chrono::seconds(12));

    concurrent_log_debug(logger::cerr::log, "stopping");
    _loop_1.stop();
    _loop_2.stop();
    _loop_3.stop();

    concurrent_log_debug(logger::cerr::log, "data 1 = ", _work_1.counter);
    if (_work_1.counter != 12) {
      return false;
    }

    concurrent_log_debug(logger::cerr::log, "data 2 = ", _work_2.counter);
    if (_work_2.counter != 1200) {
      return false;
    }

    concurrent_log_debug(logger::cerr::log, "data 3 = ", _work_3.counter);
    if (_work_3.counter != 12000) {
      return false;
    }

    std::this_thread::sleep_for(std::chrono::seconds(3));

    return true;
  }

  static const std::string desc() {
    return "\n'3 sleeping_loop' objects with interval of 1000 ms, time out of "
           "500 "
           "ms."

           "\nThe first one increments a counter by 1; the second by 100, and "
           "the "
           "third by 1000."

           "\nAll will be started, the main thread will sleep for 12 secs, and "
           "the "
           "all 'sleeping_loop' will stopped by calling stop."

           "\nCounter 1 should be 12, counter 2 should be 1200, and counter 3 "
           "should "
           "be 12000.";
  }
};

struct sleeping_loop_012 {

  struct msg {

    friend std::ostream &operator<<(std::ostream &p_out, const msg &p_msg) {
      p_out << p_msg.m_i << "-" << p_msg.m_f << "'";
      return p_out;
    }

    msg() = default;

    msg(int32_t p_i, double p_f) : m_i(p_i), m_f(p_f) {}

    msg(msg &&) noexcept = default;
    msg(const msg &) = default;

    msg &operator=(msg &&p_msg) noexcept {
      if (this != &p_msg) {
        m_i = std::move(p_msg.m_i);
        m_f = std::move(p_msg.m_f);
      }
      return *this;
    }

    msg &operator=(const msg &) = delete;

    ~msg() = default;

    int32_t m_i = 0;
    double m_f = 0.0;
  };

  typedef concurrent::sleeping_loop_t<msg, logger::cerr::log> loop;

  struct provide_1 {
    std::pair<bool, msg> operator()() {
      ++m_i;
      m_f *= 2;
      std::pair<bool, msg> result(true, msg(m_i, m_f));
      return result;
    }

  private:
    int32_t m_i = 0;
    double m_f = 1.0;
  };

  struct provide_2 {
    std::pair<bool, msg> operator()() {
      m_i += 100;
      m_f *= 3;
      std::pair<bool, msg> result(true, msg(m_i, m_f));
      return result;
    }

  private:
    int32_t m_i = 0;
    double m_f = 1.0;
  };

  struct provide_3 {
    std::pair<bool, msg> operator()() {
      m_i += 1000;
      m_f *= 4;
      std::pair<bool, msg> result(true, msg(m_i, m_f));
      return result;
    }

  private:
    int32_t m_i = 0;
    double m_f = 1.0;
  };

  struct work_1 {
    concurrent::work_status operator()(msg &&p_msg) {
      m_msg = std::move(p_msg);
      concurrent_log_debug(logger::cerr::log, "1: ", m_msg);
      return concurrent::work_status::dont_stop;
    }
    msg m_msg;
  };

  struct work_2 {
    concurrent::work_status operator()(msg &&p_msg) {
      m_msg = std::move(p_msg);
      concurrent_log_debug(logger::cerr::log, "2: ", m_msg);
      return concurrent::work_status::dont_stop;
    }
    msg m_msg;
  };

  struct work_3 {
    concurrent::work_status operator()(msg &&p_msg) {
      m_msg = std::move(p_msg);
      concurrent_log_debug(logger::cerr::log, "3: ", m_msg);
      return concurrent::work_status::dont_stop;
    }
    msg m_msg;
  };

  bool operator()() {
    provide_1 _provide_1;
    provide_2 _provide_2;
    provide_3 _provide_3;
    work_1 _work_1;
    work_2 _work_2;
    work_3 _work_3;

    loop _loop_1(
        std::chrono::milliseconds(1000),
        [&_work_1](msg &&p_msg) -> concurrent::work_status {
          return _work_1(std::move(p_msg));
        },
        std::chrono::milliseconds(100),
        [&_provide_1]() -> std::pair<bool, msg> { return _provide_1(); });

    loop _loop_2(
        std::chrono::milliseconds(500),
        [&_work_2](msg &&p_msg) -> concurrent::work_status {
          return _work_2(std::move(p_msg));
        },
        std::chrono::milliseconds(100),
        [&_provide_2]() -> std::pair<bool, msg> { return _provide_2(); });

    loop _loop_3(
        std::chrono::milliseconds(2000),
        [&_work_3](msg &&p_msg) -> concurrent::work_status {
          return _work_3(std::move(p_msg));
        },
        std::chrono::milliseconds(100),
        [&_provide_3]() -> std::pair<bool, msg> { return _provide_3(); });

    _loop_1.run();
    _loop_2.run();
    _loop_3.run();

    std::this_thread::sleep_for(std::chrono::seconds(10));

    concurrent_log_debug(logger::cerr::log, "stopping");
    _loop_1.stop();
    _loop_2.stop();
    _loop_3.stop();

    concurrent_log_debug(logger::cerr::log, "data 1 = ", _work_1.m_msg);
    if ((_work_1.m_msg.m_i != 10) && (_work_1.m_msg.m_f != 1024.0)) {
      return false;
    }

    concurrent_log_debug(logger::cerr::log, "data 2 = ", _work_2.m_msg);
    if ((_work_2.m_msg.m_i != 2000) && (_work_2.m_msg.m_f != 3486784401.0)) {
      return false;
    }

    concurrent_log_debug(logger::cerr::log, "data 3 = ", _work_3.m_msg);
    if ((_work_3.m_msg.m_i != 5000) && (_work_3.m_msg.m_f != 1024.0)) {
      return false;
    }

    return true;
  }

  static const std::string desc() {
    return "3 'sleeping_loops':\n'1' with 1000 ms of interval, work time out "
           "of 50 "
           "ms, and work function too simple to cause time out.\n'2' with "
           "interval "
           "of 500 ms, work time out of 100 ms, and work won't cause time "
           "out.\n'3' "
           "with interval of 2000 ms, work time out of 75 ms, and work also "
           "too "
           "simple.\nAll 3 use the same provider. \nAll will be started, the "
           "main "
           "thread will sleep for 12 secs, all the loops will be stopped by "
           "calling "
           "'stop', the main thread will "
           "sleep for 3 secs.";
  }
};

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();
  tester::test _tester(argc, argv);

  run_test(_tester, sleeping_loop_000);
  run_test(_tester, sleeping_loop_001);
  run_test(_tester, sleeping_loop_002);
  run_test(_tester, sleeping_loop_003);
  run_test(_tester, sleeping_loop_004);
  run_test(_tester, sleeping_loop_005);
  run_test(_tester, sleeping_loop_006);
  run_test(_tester, sleeping_loop_007);
  run_test(_tester, sleeping_loop_008);
  run_test(_tester, sleeping_loop_009);
  run_test(_tester, sleeping_loop_010);
  run_test(_tester, sleeping_loop_011);
  run_test(_tester, sleeping_loop_012);
}
