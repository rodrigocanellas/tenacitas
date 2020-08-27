/// \example

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com
///
#include <concurrent/internal/log.h>
#include <concurrent/sleeping_loop.h>
#include <concurrent/thread_pool.h>
#include <concurrent/traits.h>
#include <logger/cerr/log.h>
#include <tester/test.h>

#include <concurrent/msg_a.h>

#include <chrono>

using namespace tenacitas;

struct thread_pool_001 {

  typedef tenacitas::concurrent::msg_a<int32_t> msg;
  typedef concurrent::thread_pool_t<msg, logger::cerr::log> thread_pool;
  typedef concurrent::sleeping_loop_t<void, logger::cerr::log> sleeping_loop;

  struct work {
    bool operator()(msg &&p_msg) {
      m_msg = p_msg;
      concurrent_debug(m_log, "handling msg ", m_msg);
      return true;
    }
    logger::cerr::log m_log{"thread_pool_001::work"};
    msg m_msg;
  };

  bool operator()() {

    work _work;

    thread_pool _pool;
    msg::data _value = 0;

    sleeping_loop _loop(
        std::chrono::milliseconds(500),
        [this, &_pool, &_value]() -> bool {
          msg _msg(++_value);
          concurrent_debug(m_log, "adding msg ", _msg);
          _pool.handle(std::move(_msg));
          return true;
        },
        std::chrono::milliseconds(300));

    _pool.add_work([&_work](msg &&p_msg) { return _work(std::move(p_msg)); },
                   std::chrono::milliseconds(100));

    _pool.start();
    _loop.start();

    concurrent_debug(m_log, "sleeping for 10 secs");
    std::this_thread::sleep_for(std::chrono::seconds(10));
    concurrent_debug(m_log, "waking up after 10 secs");

    concurrent_debug(m_log, "stopping the pool");
    _pool.stop();

    concurrent_debug(m_log, "sleeping for 5 secs");
    std::this_thread::sleep_for(std::chrono::seconds(5));
    concurrent_debug(m_log, "waking up after 5 secs");

    concurrent_debug(m_log, "runnig the pool");
    _pool.start();

    concurrent_debug(m_log, "sleeping for 4 secs");
    std::this_thread::sleep_for(std::chrono::seconds(4));
    concurrent_debug(m_log, "waking up after 4 secs");

    concurrent_debug(m_log, "consumed = ", _work.m_msg.counter(),
                     ", provided = ", _value);
    if (_work.m_msg.counter() != _value) {
      concurrent_error(m_log,
                       "Data value consumed should be equal to provided");
      return false;
    }

    return true;
  }

  static std::string desc() {
    return "\nA 'sleeping_loop' sending a message at each 500 ms, to a "
           "'thread_pool' with one consumer."

           "\nThe main thread will sleep for 10 secs, the 'thread_pool' will "
           "stop; main thread will sleep for 5 secs; 'thread_pool' will run "
           "again; main thread will sleep for 4 secs."

           "\nThe messages added while the pool was stopped are handled when "
           "the pool runs again."

           "\nThe amount of data consumed must be equal to the provided";
  }

private:
  logger::cerr::log m_log{"thread_pool_001"};
};

struct thread_pool_091 {

  typedef tenacitas::concurrent::msg_a<> msg;
  typedef concurrent::thread_pool_t<msg, logger::cerr::log> thread_pool;

  struct work {
    bool operator()(msg &&p_msg) {
      m_msg = p_msg;
      concurrent_debug(m_log, "handling msg ", m_msg);
      return true;
    }
    logger::cerr::log m_log{"thread_pool_091::work"};
    msg m_msg;
  };

  bool operator()() {
    work _work;
    thread_pool _pool;
    _pool.add_work(
        [&_work](msg &&p_msg) -> bool { return _work(std::move(p_msg)); },
        std::chrono::milliseconds(500));

    for (uint16_t _i = 0; _i < 20; ++_i) {
      msg _msg(_i);
      concurrent_debug(m_log, "adding msg ", _msg);
      _pool.handle(_msg);
    }

    _pool.start();

    std::this_thread::sleep_for(std::chrono::seconds(1));

    concurrent_debug(m_log, "consumed = ", _work.m_msg.counter());
    if (_work.m_msg.counter() != 19) {
      concurrent_error(m_log, "Data value consumed should be equal to 19");
      return false;
    }

    return true;
  }

  static std::string desc() {
    return "\n20 messages added to a 'thread_pool' with one consumer; "
           "'thread_pool' started";
  }

private:
  logger::cerr::log m_log{"thread_pool_091"};
};

// struct thread_pool_092 {

//  typedef tenacitas::concurrent::msg_a<> msg;
//  typedef concurrent::thread_pool_t<msg, logger::cerr::log> thread_pool;

//  struct work {
//    bool operator()(msg &&p_msg) {
//      m_msg = p_msg;
//      concurrent_debug(m_log, "handling msg ", m_msg);
//      return true;
//    }
//    logger::cerr::log m_log{"thread_pool_092::work"};
//    msg m_msg;
//  };

//  bool operator()() {
//    work _work;

//    thread_pool _pool_1;
//    _pool_1.add_work([&_work](msg &&p_msg) { return _work(std::move(p_msg));
//    },
//                     std::chrono::milliseconds(500));

//    for (uint16_t _i = 0; _i < 20; ++_i) {
//      msg _msg(_i);
//      concurrent_debug(m_log, "adding msg ", _msg);
//      _pool_1.handle(_msg);
//    }

//    thread_pool _pool_2(std::move(_pool_1));

//    _pool_2.start();

//    std::this_thread::sleep_for(std::chrono::seconds(1));

//    concurrent_debug(m_log, "consumed = ", _work.m_msg.counter());
//    if (_work.m_msg.counter() != 19) {
//      concurrent_error(m_log, "Data value consumed should be equal to 19");
//      return false;
//    }

//    return true;
//  }

//  static std::string desc() {
//    return "\n20 messages added to a 'thread_pool' with one consumer; "
//           "'thread_pool' moved; new 'thread_pool' started";
//  }

// private:
//  logger::cerr::log m_log{"thread_pool_092"};
//};

// struct thread_pool_093 {

//  typedef tenacitas::concurrent::msg_a<> msg;
//  typedef concurrent::thread_pool_t<msg, logger::cerr::log> thread_pool;
//  typedef concurrent::sleeping_loop_t<void, logger::cerr::log> sleeping_loop;

//  struct work {
//    bool operator()(msg &&p_msg) {
//      std::this_thread::sleep_for(std::chrono::milliseconds(50));
//      m_msg = p_msg;
//      concurrent_debug(m_log, "handling msg ", m_msg);
//      return true;
//    }
//    logger::cerr::log m_log{"thread_pool_093::work"};
//    msg m_msg;
//  };

//  bool operator()() {
//    work _work;
//    thread_pool _pool_1;
//    _pool_1.add_work([&_work](msg &&p_msg) { return _work(std::move(p_msg));
//    },
//                     std::chrono::milliseconds(501));

//    for (uint16_t _i = 0; _i < 200; ++_i) {
//      msg _msg(_i);
//      concurrent_debug(m_log, "adding msg ", _msg);
//      _pool_1.handle(_msg);
//    }

//    concurrent_debug(m_log, "starting pool");
//    _pool_1.start();

//    concurrent_debug(m_log, "sleeping for 2 s");
//    std::this_thread::sleep_for(std::chrono::seconds(2));
//    concurrent_debug(m_log, "waking up");

//    concurrent_debug(m_log, "stopping pool");
//    _pool_1.stop();

//    concurrent_debug(m_log, "moving pool");
//    thread_pool _pool_2(std::move(_pool_1));

//    concurrent_debug(m_log, "sleeping for 3 s");
//    std::this_thread::sleep_for(std::chrono::seconds(3));

//    concurrent_debug(m_log, "starting new pool");
//    _pool_2.start();

//    std::this_thread::sleep_for(std::chrono::seconds(10));

//    concurrent_debug(m_log, "consumed = ", _work.m_msg.counter());
//    if (_work.m_msg.counter() != 199) {
//      concurrent_error(m_log, "Data value consumed should be equal to 199");
//      return false;
//    }

//    return true;
//  }

//  static std::string desc() {
//    return "\n200 messages added to a 'thread_pool' with one consumer, each "
//           "sleeping for 50 ms; 'thread_pool' started; main thread sleeps for
//           " "2 s; "
//           "'thread_pool' stopped; 'thread_pool' moved; main thread sleeps for
//           " "2 s; " "new 'thread_pool' started";
//  }

// private:
//  logger::cerr::log m_log{"thread_pool_093"};
//};

struct thread_pool_094 {
  typedef tenacitas::concurrent::msg_a<> msg;
  typedef concurrent::thread_pool_t<msg, logger::cerr::log> thread_pool;
  typedef concurrent::sleeping_loop_t<void, logger::cerr::log> sleeping_loop;

  struct work {
    bool operator()(msg &&p_msg) {
      m_msg = p_msg;
      concurrent_debug(m_log, "handling msg ", m_msg);
      return true;
    }
    logger::cerr::log m_log{"thread_pool_094::work"};
    msg m_msg;
  };

  bool operator()() {
    work _work;
    thread_pool _pool;

    _pool.add_work([&_work](msg &&p_msg) { return _work(std::move(p_msg)); },
                   std::chrono::milliseconds(1000));

    for (uint16_t _i = 0; _i < 200; ++_i) {
      msg _msg(_i);
      concurrent_debug(m_log, "adding msg ", _msg);
      _pool.handle(_msg);
    }

    concurrent_debug(m_log, "first start");
    _pool.start();

    concurrent_debug(m_log, "sleeping for 2 s");
    std::this_thread::sleep_for(std::chrono::seconds(2));
    concurrent_debug(m_log, "waking up");

    concurrent_debug(m_log, "second start");
    _pool.start();

    std::this_thread::sleep_for(std::chrono::seconds(3));

    concurrent_debug(m_log, "consumed = ", _work.m_msg.counter());
    if (_work.m_msg.counter() != 199) {
      concurrent_error(m_log, "Data value consumed should be equal to 199");
      return false;
    }

    return true;
  }

  static std::string desc() { return "\nStarting a 'thread_pool' twice"; }

private:
  logger::cerr::log m_log{"thread_pool_094"};
};

struct thread_pool_095 {

  typedef tenacitas::concurrent::msg_a<> msg;
  typedef concurrent::thread_pool_t<msg, logger::cerr::log> thread_pool;

  struct work {
    bool operator()(msg &&p_msg) {
      m_msg = p_msg;
      concurrent_debug(m_log, "handling msg ", m_msg);
      return true;
    }
    logger::cerr::log m_log{"thread_pool_095::work"};
    msg m_msg;
  };

  bool operator()() {
    work _work;

    thread_pool _pool;

    _pool.add_work([&_work](msg &&p_msg) { return _work(std::move(p_msg)); },
                   std::chrono::milliseconds(200));

    msg _msg(18);
    concurrent_debug(m_log, "adding msg ", _msg);
    _pool.handle(_msg);

    concurrent_debug(m_log, "start pool");
    _pool.start();

    concurrent_debug(m_log, "sleeping for 1 s");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    concurrent_debug(m_log, "waking up");

    return true;
  }

  static std::string desc() { return "\nWork function taking too long"; }

private:
  logger::cerr::log m_log{"thread_pool_095"};
};

struct thread_pool_096 {

  typedef tenacitas::concurrent::msg_a<> msg;
  typedef concurrent::thread_pool_t<msg, logger::cerr::log> thread_pool;

  struct work {
    bool operator()(msg &&p_msg) {

      concurrent_debug(m_log, "handling msg ", p_msg);
      if ((p_msg.counter() % 2) == 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        ++m_timeout;
      } else {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
      }
      return true;
    }
    uint16_t m_timeout = 0;
    logger::cerr::log m_log{"thread_pool_096::work"};
  };

  bool operator()() {
    work _work;

    thread_pool _pool;
    _pool.add_work([&_work](msg &&p_msg) { return _work(std::move(p_msg)); },
                   std::chrono::milliseconds(300));

    for (uint16_t _i = 0; _i < 20; ++_i) {
      msg _msg(_i);
      concurrent_debug(m_log, "adding msg ", _msg);
      _pool.handle(_msg);
    }

    concurrent_debug(m_log, "start pool");
    _pool.start();

    concurrent_debug(m_log, "sleeping for 10 s");
    std::this_thread::sleep_for(std::chrono::seconds(10));
    concurrent_debug(m_log, "waking up");

    concurrent_debug(m_log, "timeout = ", _work.m_timeout);
    if (_work.m_timeout != 10) {
      concurrent_error(m_log, "# of timeout should be equal to 10");
      return false;
    }

    return true;
  }

  static std::string desc() {
    return "\n20 messages sent to the pool, and work function taking too "
           "long on messages which counter is even, and not too long for "
           "messages which counter is odd";
  }

private:
  logger::cerr::log m_log{"thread_pool_096"};
};

struct thread_pool_097 {

  typedef concurrent::thread_pool_t<int32_t, logger::cerr::log> thread_pool;
  typedef concurrent::sleeping_loop_t<void, logger::cerr::log> sleeping_loop;

  struct work_1 {
    bool operator()(int32_t &&p_value) {
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
      ++counter;
      concurrent_debug(m_log, "work 1 handling msg ", p_value);
      return true;
    }
    int16_t counter = 0;
    logger::cerr::log m_log{"thread_pool_097::work_1"};
  };

  struct work_2 {
    bool operator()(int32_t &&p_value) {
      std::this_thread::sleep_for(std::chrono::milliseconds(60));
      ++counter;
      concurrent_debug(m_log, "work 2 handling msg ", p_value);
      return true;
    }
    int16_t counter = 0;
    logger::cerr::log m_log{"thread_pool_097::work_2"};
  };

  bool operator()() {
    work_1 _work_1;
    work_2 _work_2;

    {
      thread_pool _pool;
      int32_t _value = 0;
      int16_t _counter = 0;
      sleeping_loop _loop(
          std::chrono::milliseconds(100),
          [&_pool, &_value, &_counter]() -> bool {
            ++_value;
            ++_counter;
            _pool.handle(_value);
            return true;
          },
          std::chrono::milliseconds(300));

      _pool.add_work(
          [&_work_1](int32_t &&p_val) -> bool {
            return _work_1(std::move(p_val));
          },
          std::chrono::milliseconds(200));
      _pool.add_work(
          [&_work_2](int32_t &&p_val) -> bool {
            return _work_2(std::move(p_val));
          },
          std::chrono::milliseconds(200));

      _loop.start();

      std::this_thread::sleep_for(std::chrono::seconds(6));

      _loop.stop();

      concurrent_debug(m_log, "counter = ", _counter);

      _pool.start();
    }
    concurrent_debug(m_log, "work 1 counter = ", _work_1.counter);
    concurrent_debug(m_log, "work 2 counter = ", _work_2.counter);
    return ((_work_1.counter + _work_2.counter) == 60);
  }

  static std::string desc() {
    return "\nA 'sleeping_loop' with interval of 100 ms, generating int32_t "
           "numbers to a 'thread_pool'."
           "\nThe 'thread_pool' has two workers, which just print number."
           "\nThe first work sleeps for 50 ms before printing, and the second "
           "one "
           "sleeps for 60 ms."
           "\nThe main thread runs the 'sleeping_loop', sleeps for 6 secs, "
           "stops "
           "the "
           "'sleeping_loop', and then starts the 'thread_pool'."
           "\nThe amount of values work 1 and work 2 handle must be 60";
  }

private:
  logger::cerr::log m_log{"thread_pool_097"};
};

struct thread_pool_098 {
  typedef concurrent::thread_pool_t<int32_t, logger::cerr::log> thread_pool;
  typedef concurrent::sleeping_loop_t<void, logger::cerr::log> sleeping_loop;

  template <typename t_notifier> struct work_1 {
    work_1(t_notifier *p_notifier) : m_notifier(p_notifier) {}

    bool operator()(int32_t &&p_value) {
      std::this_thread::sleep_for(std::chrono::milliseconds(170));
      concurrent_debug(m_log, "work 1 handling msg ", p_value);
      if (p_value > 200) {
        m_notifier->stop("work1");
        return false;
      }
      return true;
    }

  private:
    t_notifier *m_notifier = nullptr;
    logger::cerr::log m_log{"thread_pool_098::work_1"};
  };

  thread_pool_098() {
    //    m_pool.add_work(work_1(this), std::chrono::milliseconds(200));
    //    m_pool.add_work(work_1(this), std::chrono::milliseconds(200));
    //    m_pool.add_work(work_1(this), std::chrono::milliseconds(200));
    //    m_pool.add_work(work_1(this), std::chrono::milliseconds(200));
    //    m_pool.add_work(work_1(this), std::chrono::milliseconds(200));

    m_pool.add_work(
        4, [this]() { return work_1<thread_pool_098>(this); },
        std::chrono::milliseconds(200));
  }

  bool operator()() {
    //    m_pool.add_work(
    //      4, [this]() { return work_1(this); },
    //      std::chrono::milliseconds(200));

    int32_t _value = 0;

    m_pool.start();

    m_pool.handle(_value);
    while (true) {
      std::unique_lock<std::mutex> _lock(m_mutex_stop);
      if (m_cond_stop.wait_for(_lock, std::chrono::milliseconds(50),
                               [this]() -> bool { return m_stop; })) {
        concurrent_debug(m_log, "notified");
        m_pool.stop();
        break;
      }
      //      if (m_stop) {
      //        m_pool.stop();
      //        break;
      //      }
      m_pool.handle(++_value);
    }
    concurrent_debug(m_log, "sleeping");
    std::this_thread::sleep_for(std::chrono::seconds(3));
    concurrent_debug(m_log, "woke up");
    return true;
  }

  void stop(const std::string &p_str) {
    concurrent_debug(m_log, "stop called with ", p_str);
    m_stop = true;
    std::unique_lock<std::mutex> _lock(m_mutex_stop);
    m_cond_stop.notify_all();
  }

  static std::string desc() { return "\nStopping when a max value is hit"; }

private:
  bool m_stop = false;
  thread_pool m_pool;
  std::condition_variable m_cond_stop;
  std::mutex m_mutex_stop;

private:
  logger::cerr::log m_log{"thread_pool_098"};
};

struct thread_pool_099 {

  typedef concurrent::thread_pool_t<int32_t, logger::cerr::log> thread_pool;
  typedef concurrent::sleeping_loop_t<void, logger::cerr::log> sleeping_loop;

  struct work_1 {
    bool operator()(int32_t &&p_value) {
      std::this_thread::sleep_for(std::chrono::milliseconds(170));
      concurrent_debug(m_log, "work 1 handling msg ", p_value);
      if (p_value > 200) {
        concurrent_debug(m_log, "stopping this worker");
        return false;
      }
      return true;
    }
    logger::cerr::log m_log{"thread_pool_099::work_1"};
  };

  thread_pool_099() {
    m_pool.add_work(
        1, []() { return work_1(); }, std::chrono::milliseconds(200));
    m_pool.start();
  }

  bool operator()() {

    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    int32_t _value = 199;
    m_pool.handle(_value);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    _value = 201;

    m_pool.handle(_value);

    concurrent_debug(m_log, "sleeping");
    std::this_thread::sleep_for(std::chrono::seconds(5));
    concurrent_debug(m_log, "woke up");
    return true;
  }

  static std::string desc() {
    return "\nTesting thread 'waiting' implementation";
  }

private:
  thread_pool m_pool;
  logger::cerr::log m_log{"thread_pool_099"};
};

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();

  tester::test _tester(argc, argv);

  run_test(_tester, thread_pool_001);
  run_test(_tester, thread_pool_091);
  //  run_test(_tester, thread_pool_092);
  //  run_test(_tester, thread_pool_093);
  run_test(_tester, thread_pool_094);
  run_test(_tester, thread_pool_095);
  run_test(_tester, thread_pool_096);
  run_test(_tester, thread_pool_097);
  run_test(_tester, thread_pool_098);
  run_test(_tester, thread_pool_099);
}
