/// \example

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <cmath>
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
/*rm -f 001_a.log 2> /dev/null; cat 001.log | while read l; do i=`echo $l | cut
 * -d '|' -f 4`; if [ "$i" == "sleeping_loop_001::work1" ]; then t=`echo "$l" |
 * cut -d '|' -f 2`; c=`echo $l | cut -d '|' -f 6`; echo "$t|$c" >> 001_a.log
 * ;fi; done*/
using namespace tenacitas;

struct sleeping_loop_000 {

  bool operator()() {

    typedef concurrent::sleeping_loop_t<logger::cerr::log,
                                        std::chrono::milliseconds,
                                        std::chrono::seconds, void>
        loop;

    auto _worker = [this]() -> std::optional<bool> {
      concurrent_debug(m_log, "loop1");
      return {true};
    };

    loop::interval _interval(1);
    loop::timeout _timeout(100);

    concurrent_debug(m_log, "timeout = ", _timeout.count(),
                     ", interval = ", _interval.count());

    loop _loop(_worker, _timeout, _interval);

    return true;
  }

  static const std::string desc() { return "'sleeping_loop' creation test"; }

private:
  logger::cerr::log m_log{"sleeping_loop_000"};
};

struct sleeping_loop_001 {
  typedef concurrent::sleeping_loop_t<logger::cerr::log,
                                      std::chrono::milliseconds,
                                      std::chrono::milliseconds, void>
      loop;

  typedef uint16_t value;

  struct work1 {

    std::optional<bool> operator()() {

      ++counter;
      concurrent_debug(m_log, "counter = ", counter);
      return {true};
    }

    value counter = 0;

  private:
    logger::cerr::log m_log{"sleeping_loop_001::work1"};
  };

  sleeping_loop_001() {
    //    if ((std::remainder(m_amount, 2)) != 0) {
    //      m_actual_amount = static_cast<value>(m_amount) + 1;
    //    } else {
    //      m_actual_amount = static_cast<value>(m_amount);
    //    }
  }

  bool operator()() {

    concurrent_debug(m_log, "interval = ", m_interval_ms,
                     ", sleep = ", m_sleep_secs, ", amount = ", m_amount,
                     ", actual amount = ", m_actual_amount);

    work1 _work;
    loop _loop([&_work]() { return _work(); }, std::chrono::milliseconds(100),
               std::chrono::milliseconds(m_interval_ms));

    _loop.start();
    std::this_thread::sleep_for(std::chrono::seconds(m_sleep_secs));

    concurrent_debug(m_log, "data = ", _work.counter);
    if (_work.counter != m_actual_amount) {
      concurrent_error(m_log, "counter should be ", m_actual_amount,
                       ", but it is ", _work.counter);
      return false;
    }

    concurrent_info(m_log, "counter should be ", m_actual_amount,
                    ", and it is ", _work.counter);

    return true;
  }

  static const std::string desc() {
    std::stringstream _stream;
    _stream << "\n'sleeping_loop' with interval of " << m_interval_ms
            << "ms, work time out of 100ms, increments a counter, and just "
               "prints using 'cerr_test', so there will be no time out."

               "\nThe main function will sleep for "
            << m_sleep_secs
            << "secs, and the 'sleeping_loop' will stop in the destructor."

               "\nCounter should be "
            << m_actual_amount;
    return _stream.str();
  }

private:
  logger::cerr::log m_log{"sleeping_loop_001"};

  static const value m_sleep_secs;
  static const value m_interval_ms;
  static const float m_amount;
  static const value m_actual_amount;
};

const sleeping_loop_001::value sleeping_loop_001::m_sleep_secs{20};
const sleeping_loop_001::value sleeping_loop_001::m_interval_ms{400};
const float sleeping_loop_001::m_amount{static_cast<float>(
    (1000 / static_cast<float>(m_interval_ms)) * m_sleep_secs)};

const sleeping_loop_001::value sleeping_loop_001::m_actual_amount{
    static_cast<value>(((m_amount - static_cast<value>(m_amount)) != 0)
                           ? static_cast<value>(m_amount) + 1
                           : static_cast<value>(m_amount))};

struct sleeping_loop_003 {

  typedef concurrent::sleeping_loop_t<logger::cerr::log,
                                      std::chrono::milliseconds,
                                      std::chrono::milliseconds, void>
      loop;

  struct work1 {
    std::optional<bool> operator()() {

      ++counter;
      concurrent_debug(m_log, counter);
      return {true};
    }
    uint64_t counter = 0;
    logger::cerr::log m_log{"sleeping_loop_003::work1"};
  };

  bool operator()() {
    using namespace tenacitas;
    work1 _work;
    loop _loop([&_work]() { return _work(); }, std::chrono::milliseconds(500),
               std::chrono::milliseconds(1000));

    _loop.start();
    std::this_thread::sleep_for(std::chrono::seconds(10));
    _loop.stop();

    concurrent_debug(m_log, "data = ", _work.counter);
    if (_work.counter != 10) {
      return false;
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));

    _loop.start();
    std::this_thread::sleep_for(std::chrono::seconds(8));

    concurrent_debug(m_log, "data = ", _work.counter);
    if (_work.counter != 18) {
      return false;
    }

    return true;
  }

  static const std::string desc() {
    return "\n'sleeping_loop' with interval of 1000 ms, time out of 500 ms, "
           "increments a counter, and just prints using 'cerr_test', so there "
           "will be no time out."

           "\n'sleeping_loop' will call 'start'; main thread will sleep for 10 "
           "secs; the 'sleeping_loop' will stop by calling 'stop'; the main "
           "thread will sleep for 5 secs;, the 'sleeping_loop' will be "
           "restarted with 'start'; the main thread will sleep for 8 secs; the "
           "'sleeping_loop' will stop in the destructor."

           "\nCounter should be 18.";
  }
  logger::cerr::log m_log{"sleeping_loop_003"};
};

struct sleeping_loop_004 {
  typedef concurrent::sleeping_loop_t<logger::cerr::log,
                                      std::chrono::milliseconds,
                                      std::chrono::milliseconds, void>
      loop;

  struct work1 {
    std::optional<bool> operator()() {
      using namespace tenacitas;
      ++counter;
      concurrent_debug(m_log, counter);
      return {true};
    }
    uint64_t counter = 0;
    logger::cerr::log m_log{"sleeping_loop_004::work1"};
  };

  bool operator()() {

    work1 _work;
    loop _loop([&_work]() { return _work(); }, std::chrono::milliseconds(500),
               std::chrono::milliseconds(1000));

    _loop.start();
    std::this_thread::sleep_for(std::chrono::seconds(10));
    _loop.stop();

    concurrent_debug(m_log, "data = ", _work.counter);
    if (_work.counter != 10) {
      return false;
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));

    _loop.start();
    std::this_thread::sleep_for(std::chrono::seconds(8));
    _loop.stop();

    concurrent_debug(m_log, "data = ", _work.counter);
    if (_work.counter != 18) {
      return false;
    }
    return true;
  }

  static const std::string desc() {
    return "\n'sleeping_loop' with interval of 1000 ms, time out of 500 ms, "
           "increments a counter, and just prints using 'cerr_test', so there "
           "will be no time out."

           "\n'sleeping_loop' will call 'start'; main thread will sleep for 10 "
           "secs; the 'sleeping_loop' will stop by calling 'stop'; the main "
           "thread will sleep for 5 secs;, the 'sleeping_loop' will be "
           "restarted with 'start'; the main thread will sleep for 8 secs; the "
           "'sleeping_loop' will stop by calling 'stop'."

           "\nCounter should be 18.";
  }

private:
  logger::cerr::log m_log{"sleeping_loop_004"};
};

struct sleeping_loop_005 {
  typedef concurrent::sleeping_loop_t<logger::cerr::log,
                                      std::chrono::milliseconds,
                                      std::chrono::milliseconds, void>
      loop;

  struct work1 {
    std::optional<bool> operator()() {
      using namespace tenacitas;
      ++counter;
      concurrent_debug(m_log, counter);
      return {true};
    }
    uint64_t counter = 0;
    logger::cerr::log m_log{"sleeping_loop_005::work1"};
  };

  bool operator()() {

    work1 _work;
    loop _loop_1([&_work]() { return _work(); },

                 std::chrono::milliseconds(500),
                 std::chrono::milliseconds(1000));

    _loop_1.start();
    std::this_thread::sleep_for(std::chrono::seconds(10));

    if (_work.counter != 10) {
      concurrent_error(m_log, "counter should be 10, but it is ",
                       _work.counter);
      return false;
    }
    concurrent_debug(m_log, "data = ", _work.counter);

    _loop_1.stop();

    loop _loop_2([&_work]() { return _work(); }, std::chrono::milliseconds(500),
                 std::chrono::milliseconds(1000));

    _loop_2.start();
    std::this_thread::sleep_for(std::chrono::seconds(3));

    if (_work.counter != 13) {
      concurrent_error(m_log, "counter should be 10, but it is ",
                       _work.counter);
      return false;
    }
    concurrent_debug(m_log, "data = ", _work.counter);
    return true;
  }

  static const std::string desc() {
    return "\n'sleeping_loop' with interval of 1000 ms, time out of 500 ms, "
           "increments a counter, and just prints using 'cerr_test', so there "
           "will be no time out."

           "\nA 'sleeping_loop' will be started, the main thread will sleep "
           "for 10 secs, another 'sleeping_loop' will be created using the "
           "same parameters as the first one, the main thread will sleep for 3 "
           "secs, the new 'sleeping_loop' will stop by its destructor."

           "\nCounter should be 13.";
  }
  logger::cerr::log m_log{"sleeping_loop_005"};
};

struct sleeping_loop_006 {

  typedef concurrent::sleeping_loop_t<logger::cerr::log,
                                      std::chrono::milliseconds,
                                      std::chrono::milliseconds, void>
      loop;

  struct work1 {
    std::optional<bool> operator()() {

      ++counter;
      concurrent_debug(m_log, counter);
      return {true};
    }
    uint64_t counter = 0;
    logger::cerr::log m_log{"sleeping_loop_006::work1"};
  };

  bool operator()() {
    using namespace tenacitas;
    work1 _work;
    loop _loop_1([&_work]() { return _work(); }, std::chrono::milliseconds(500),
                 std::chrono::milliseconds(1000));

    _loop_1.start();
    std::this_thread::sleep_for(std::chrono::seconds(10));

    concurrent_debug(m_log, "data = ", _work.counter);
    if (_work.counter != 10) {
      concurrent_error(m_log, "counter should be 10, but it is ",
                       _work.counter);
      return false;
    }

    _loop_1.stop();

    loop _loop_2(_loop_1.get_worker(), _loop_1.get_timeout(),
                 _loop_1.get_interval());

    _loop_2.start();

    std::this_thread::sleep_for(std::chrono::seconds(3));

    _loop_2.stop();

    concurrent_debug(m_log, "data = ", _work.counter);
    if (_work.counter != 13) {
      concurrent_error(m_log, "counter should be 13, but it is ",
                       _work.counter);
      return false;
    }

    return true;
  }

  static const std::string desc() {
    return "\n'sleeping_loop' with interval of 1000 ms, time out of 500 ms, "
           "increments a counter, and just prints using 'cerr_test', so there "
           "will be no time out."

           "\nA 'sleeping_loop' will be started, the main thread will sleep "
           "for 10 secs, another 'sleeping_loop' will be created using the "
           "same parameters as the first one, the main thread will sleep for 3 "
           "secs, the new 'sleeping_loop' will stop by calling stop."

           "\nCounter should be 13.";
  }
  logger::cerr::log m_log{"sleeping_loop_006"};
};

struct sleeping_loop_007 {

  typedef concurrent::sleeping_loop_t<logger::cerr::log,
                                      std::chrono::milliseconds,
                                      std::chrono::milliseconds, void>
      loop;

  struct work1 {
    std::optional<bool> operator()() {
      using namespace tenacitas;
      ++counter;
      concurrent_debug(m_log, counter);
      return {true};
    }
    uint64_t counter = 0;
    logger::cerr::log m_log{"sleeping_loop_007::work1"};
  };

  bool operator()() {

    work1 _work;
    loop _loop_1([&_work]() { return _work(); },

                 std::chrono::milliseconds(500),
                 std::chrono::milliseconds(1000));
    _loop_1.start();
    std::this_thread::sleep_for(std::chrono::seconds(10));
    _loop_1.stop();

    concurrent_debug(m_log, "data = ", _work.counter);
    if (_work.counter != 10) {
      return false;
    }

    _loop_1.stop();

    loop _loop_2(_loop_1.get_worker(), _loop_1.get_timeout(),
                 _loop_1.get_interval());

    std::this_thread::sleep_for(std::chrono::seconds(1));

    _loop_2.start();
    std::this_thread::sleep_for(std::chrono::seconds(5));

    concurrent_debug(m_log, "data = ", _work.counter);
    if (_work.counter != 15) {
      return false;
    }

    return true;
  }

  static const std::string desc() {
    return "\n'sleeping_loop' with interval of 1000 ms, time out of 500 ms, "
           "increments a counter, and just prints using 'cerr_test', so there "
           "will be no time out."

           "\nA 'sleeping_loop' will be started, the main thread will sleep "
           "for 10 secs, the 'sleeping_loop' will be stopped, another "
           "'sleeping_loop' will be created using the same parameters as the "
           "first one, the new 'sleeping_loop' will be started, the main "
           "thread will sleep for 5 secs, the new 'sleeping_loop' will stop in "
           "its destructor."

           "\nCounter should be 15.";
  }
  logger::cerr::log m_log{"sleeping_loop_007"};
};

struct sleeping_loop_008 {

  typedef concurrent::sleeping_loop_t<logger::cerr::log,
                                      std::chrono::milliseconds,
                                      std::chrono::milliseconds, void>
      loop;

  struct work1 {
    std::optional<bool> operator()() {
      using namespace tenacitas;
      ++counter;
      concurrent_debug(m_log, counter);
      return {true};
    }
    uint64_t counter = 0;
    logger::cerr::log m_log{"sleeping_loop_008::work1"};
  };

  bool operator()() {

    work1 _work;
    loop _loop_1([&_work]() { return _work(); },

                 std::chrono::milliseconds(500),
                 std::chrono::milliseconds(1000));

    _loop_1.start();

    std::this_thread::sleep_for(std::chrono::seconds(10));
    concurrent_debug(m_log, "stopping loop 1");
    _loop_1.stop();

    concurrent_debug(m_log, "data = ", _work.counter);
    if (_work.counter != 10) {
      return false;
    }

    loop _loop_2(_loop_1.get_worker(), _loop_1.get_timeout(),
                 _loop_1.get_interval());

    _loop_2.start();
    std::this_thread::sleep_for(std::chrono::seconds(5));
    _loop_2.stop();

    if (_work.counter != 15) {
      concurrent_error(m_log, "data is ", _work.counter,
                       ", but it should be 15");
      return false;
    }
    concurrent_debug(m_log, "data = ", _work.counter);
    return true;
  }

  static const std::string desc() {
    return "\n'sleeping_loop' with interval of 1000 ms, time out of 500 ms, "
           "increments a counter, and just prints using 'cerr_test', so there "
           "will be no time out."

           "\nA 'sleeping_loop' will be started, the main thread will sleep "
           "for 10 secs, the 'sleeping_loop' will be stopped, another "
           "'sleeping_loop' will be created using the same parameters as first "
           "one, the new 'sleeping_loop' will be started, the main thread will "
           "sleep for 5 secs, the new 'sleeping_loop' will be stopped by "
           "calling 'stop'."

           "\nCounter should be 15.";
  }
  logger::cerr::log m_log{"sleeping_loop_008"};
};

struct sleeping_loop_009 {

  typedef concurrent::sleeping_loop_t<logger::cerr::log,
                                      std::chrono::milliseconds,
                                      std::chrono::milliseconds, void>
      loop;

  struct work1 {
    std::optional<bool> operator()() {
      using namespace tenacitas;
      ++counter;
      concurrent_debug(m_log, counter);
      return true;
    }
    uint64_t counter = 0;
    logger::cerr::log m_log{"sleeping_loop_009::work1"};
  };

  struct work2 {
    std::optional<bool> operator()() {

      counter += 100;
      concurrent_debug(m_log, counter);
      return true;
    }
    uint64_t counter = 0;
    logger::cerr::log m_log{"sleeping_loop_009::work2"};
  };

  struct work3 {
    std::optional<bool> operator()() {
      using namespace tenacitas;
      counter += 1000;
      concurrent_debug(m_log, counter);
      return true;
    }
    uint64_t counter = 0;
    logger::cerr::log m_log{"sleeping_loop_009::work3"};
  };

  bool operator()() {
    using namespace tenacitas;
    work1 _work_1;
    loop _loop_1([&_work_1]() { return _work_1(); },
                 std::chrono::milliseconds(100),
                 std::chrono::milliseconds(1000));

    work2 _work_2;
    loop _loop_2([&_work_2]() { return _work_2(); },
                 std::chrono::milliseconds(100),
                 std::chrono::milliseconds(1000));

    work3 _work_3;
    loop _loop_3([&_work_3]() { return _work_3(); },
                 std::chrono::milliseconds(100),
                 std::chrono::milliseconds(1000));

    _loop_1.start();
    _loop_2.start();
    _loop_3.start();

    std::this_thread::sleep_for(std::chrono::seconds(6));

    concurrent_debug(m_log, "data 1 = ", _work_1.counter);
    if (_work_1.counter != 6) {
      return false;
    }

    concurrent_debug(m_log, "data 2 = ", _work_2.counter);
    if (_work_2.counter != 600) {
      return false;
    }

    concurrent_debug(m_log, "data 3 = ", _work_3.counter);
    if (_work_3.counter != 6000) {
      return false;
    }

    return true;
  }

  static const std::string desc() {
    return "\n'3 sleeping_loop' objects with interval of 1000 ms, time out of "
           "500 ms."

           "\nThe first one increments a counter by 1; the second by 100, and "
           "the third by 1000."

           "\nAll will be started, the main thread will sleep for 6 secs, and "
           "the all 'sleeping_loop' will stopped by theirs destructors."

           "\nCounter 1 should be 6, counter 2 should be 600, and counter 3 "
           "should be 6000.";
  }
  logger::cerr::log m_log{"sleeping_loop_009"};
};

struct sleeping_loop_010 {

  typedef concurrent::sleeping_loop_t<logger::cerr::log,
                                      std::chrono::milliseconds,
                                      std::chrono::milliseconds, void>
      loop;

  struct work1 {
    std::optional<bool> operator()() {
      using namespace tenacitas;
      ++counter;
      concurrent_debug(m_log, counter);
      return {true};
    }
    uint64_t counter = 0;
    logger::cerr::log m_log{"sleeping_loop_010::work1"};
  };

  struct work2 {
    std::optional<bool> operator()() {
      counter += 100;
      concurrent_debug(m_log, counter);
      return {true};
    }
    uint64_t counter = 0;
    logger::cerr::log m_log{"sleeping_loop_010::work2"};
  };

  struct work3 {
    std::optional<bool> operator()() {
      using namespace tenacitas;
      counter += 1000;
      concurrent_debug(m_log, counter);
      return {true};
    }
    uint64_t counter = 0;
    logger::cerr::log m_log{"sleeping_loop_010::work3"};
  };

  bool operator()() {
    using namespace tenacitas;
    work1 _work_1;
    loop _loop_1([&_work_1]() { return _work_1(); },
                 std::chrono::milliseconds(100),
                 std::chrono::milliseconds(1000));

    work2 _work_2;
    loop _loop_2([&_work_2]() { return _work_2(); },
                 std::chrono::milliseconds(100),
                 std::chrono::milliseconds(1000));

    work3 _work_3;
    loop _loop_3([&_work_3]() { return _work_3(); },
                 std::chrono::milliseconds(100),
                 std::chrono::milliseconds(1000));

    _loop_1.start();
    _loop_2.start();
    _loop_3.start();

    std::this_thread::sleep_for(std::chrono::seconds(6));

    concurrent_debug(m_log, "stopping 2");
    _loop_2.stop();

    std::this_thread::sleep_for(std::chrono::seconds(3));

    concurrent_debug(m_log, "data 1 = ", _work_1.counter);
    if (_work_1.counter != 9) {
      return false;
    }

    concurrent_debug(m_log, "data 2 = ", _work_2.counter);
    if (_work_2.counter != 600) {
      return false;
    }

    concurrent_debug(m_log, "data 3 = ", _work_3.counter);
    if (_work_3.counter != 9000) {
      return false;
    }

    return true;
  }

  static const std::string desc() {
    return "\n'3 sleeping_loop' objects with interval of 1000 ms, time out of "
           "500 ms."

           "\nThe first one increments a counter by 1; the second by 100, and "
           "the third by 1000."

           "\nAll will be started, the main thread will sleep for 6 secs, the "
           "second 'sleeping_loop' will be stopped, the main thread will sleep "
           "for seconds all 'sleeping_loop' will stopped by theirs destructors."

           "\nCounter 1 should be 6, counter 2 should be 900, and counter 3 "
           "should be 6000.";
  }
  logger::cerr::log m_log{"sleeping_loop_010"};
};

struct sleeping_loop_011 {

  typedef concurrent::sleeping_loop_t<logger::cerr::log,
                                      std::chrono::milliseconds,
                                      std::chrono::milliseconds, void>
      loop;

  struct work1 {
    std::optional<bool> operator()() {
      ++counter;
      concurrent_debug(m_log, counter);
      return {true};
    }
    uint64_t counter = 0;
    logger::cerr::log m_log{"sleeping_loop_011::work1"};
  };

  struct work2 {
    std::optional<bool> operator()() {
      counter += 100;
      concurrent_debug(m_log, counter);
      return {true};
    }
    uint64_t counter = 0;
    logger::cerr::log m_log{"sleeping_loop_011::work2"};
  };

  struct work3 {
    std::optional<bool> operator()() {
      counter += 1000;
      concurrent_debug(m_log, counter);
      return {true};
    }
    logger::cerr::log m_log{"sleeping_loop_011::work3"};
    uint64_t counter = 0;
  };

  bool operator()() {
    work1 _work_1;
    loop _loop_1([&_work_1]() { return _work_1(); },
                 std::chrono::milliseconds(100),
                 std::chrono::milliseconds(1000));

    work2 _work_2;
    loop _loop_2([&_work_2]() { return _work_2(); },
                 std::chrono::milliseconds(100),
                 std::chrono::milliseconds(1000));

    work3 _work_3;
    loop _loop_3([&_work_3]() { return _work_3(); },
                 std::chrono::milliseconds(100),
                 std::chrono::milliseconds(1000));

    _loop_1.start();
    _loop_2.start();
    _loop_3.start();

    std::this_thread::sleep_for(std::chrono::seconds(12));

    concurrent_debug(m_log, "stopping");
    _loop_1.stop();
    _loop_2.stop();
    _loop_3.stop();

    concurrent_debug(m_log, "data 1 = ", _work_1.counter);
    if (_work_1.counter != 12) {
      return false;
    }

    concurrent_debug(m_log, "data 2 = ", _work_2.counter);
    if (_work_2.counter != 1200) {
      return false;
    }

    concurrent_debug(m_log, "data 3 = ", _work_3.counter);
    if (_work_3.counter != 12000) {
      return false;
    }

    std::this_thread::sleep_for(std::chrono::seconds(3));

    return true;
  }

  static const std::string desc() {
    return "\n3 'sleeping_loop' objects with interval of 1000 ms, time out of "
           "500 ms."

           "\nThe first one increments a counter by 1; the second by 100, and "
           "the third by 1000."

           "\nAll will be started, the main thread will sleep for 12 secs, and "
           "the all 'sleeping_loop' will stopped by calling stop."

           "\nCounter 1 should be 12, counter 2 should be 1200, and counter 3 "
           "should be 12000.";
  }
  logger::cerr::log m_log{"sleeping_loop_010:"};
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

  typedef concurrent::sleeping_loop_t<logger::cerr::log,
                                      std::chrono::milliseconds,
                                      std::chrono::milliseconds, msg>
      loop;

  struct provide_1 {
    std::optional<msg> operator()() {
      ++m_i;
      m_f *= 2;
      return {msg(m_i, m_f)};
    }

  private:
    int32_t m_i = 0;
    double m_f = 1.0;
  };

  struct provide_2 {
    std::optional<msg> operator()() {
      m_i += 100;
      m_f *= 3;
      return {msg(m_i, m_f)};
    }

  private:
    int32_t m_i = 0;
    double m_f = 1.0;
  };

  struct provide_3 {
    std::optional<msg> operator()() {
      m_i += 1000;
      m_f *= 4;
      return {msg(m_i, m_f)};
    }

  private:
    int32_t m_i = 0;
    double m_f = 1.0;
  };

  struct work_1 {
    std::optional<bool> operator()(msg &&p_msg) {
      m_msg = std::move(p_msg);
      concurrent_debug(m_log, "1: ", m_msg);
      return {true};
    }
    msg m_msg;
    logger::cerr::log m_log{"sleeping_loop_012::work1"};
  };

  struct work_2 {
    std::optional<bool> operator()(msg &&p_msg) {
      m_msg = std::move(p_msg);
      concurrent_debug(m_log, "2: ", m_msg);
      return {true};
    }
    logger::cerr::log m_log{"sleeping_loop_012::work2"};
    msg m_msg;
  };

  struct work_3 {
    std::optional<bool> operator()(msg &&p_msg) {
      m_msg = std::move(p_msg);
      concurrent_debug(m_log, "3: ", m_msg);
      return {true};
    }
    logger::cerr::log m_log{"sleeping_loop_012::work3"};
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

        [&_work_1](msg &&p_msg) -> std::optional<bool> {
          return _work_1(std::move(p_msg));
        },
        std::chrono::milliseconds(50), std::chrono::milliseconds(1000),
        [&_provide_1]() -> std::optional<msg> { return _provide_1(); });

    loop _loop_2(

        [&_work_2](msg &&p_msg) -> std::optional<bool> {
          return _work_2(std::move(p_msg));
        },
        std::chrono::milliseconds(100), std::chrono::milliseconds(500),
        [&_provide_2]() -> std::optional<msg> { return _provide_2(); });

    loop _loop_3(

        [&_work_3](msg &&p_msg) -> std::optional<bool> {
          return _work_3(std::move(p_msg));
        },
        std::chrono::milliseconds(75), std::chrono::milliseconds(2000),
        [&_provide_3]() -> std::optional<msg> { return _provide_3(); });

    _loop_1.start();
    _loop_2.start();
    _loop_3.start();

    std::this_thread::sleep_for(std::chrono::seconds(10));

    concurrent_debug(m_log, "stopping");
    _loop_1.stop();
    _loop_2.stop();
    _loop_3.stop();

    concurrent_debug(m_log, "data 1 = ", _work_1.m_msg);
    if ((_work_1.m_msg.m_i != 10) && (_work_1.m_msg.m_f != 1024.0)) {
      return false;
    }

    concurrent_debug(m_log, "data 2 = ", _work_2.m_msg);
    if ((_work_2.m_msg.m_i != 2000) && (_work_2.m_msg.m_f != 3486784401.0)) {
      return false;
    }

    concurrent_debug(m_log, "data 3 = ", _work_3.m_msg);
    if ((_work_3.m_msg.m_i != 5000) && (_work_3.m_msg.m_f != 1024.0)) {
      return false;
    }

    return true;
  }

  static const std::string desc() {
    return "3 'sleeping_loops':"

           "\n'1' with 1000 ms of interval, work time out of 50 ms, and work "
           "function too simple to cause time out."

           "\n'2' with interval of 500 ms, work time out of 100 ms, and work "
           "won't cause timeout."

           "\n'3' with interval of 2000 ms, work time out of 75 ms, and work "
           "also too simple."

           "\nAll 3 use the same provider."

           "\nAll will be started, the main thread will sleep for 12 secs, all "
           "the loops will be stopped by calling 'stop', the main thread will "
           "sleep for 3 secs.";
  }
  logger::cerr::log m_log{"sleeping_loop_012"};
};

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();
  tester::test _tester(argc, argv);

  run_test(_tester, sleeping_loop_000);
  run_test(_tester, sleeping_loop_001);
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
