/// \example

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <cstdint>
#include <ctime>
#include <memory>
#include <sstream>
#include <string>

#include <concurrent/async_loop.h>
#include <concurrent/internal/log.h>
#include <concurrent/result.h>
#include <concurrent/thread.h>
#include <concurrent/traits.h>
#include <logger/cerr/log.h>
#include <status/result.h>
#include <tester/test.h>

using namespace tenacitas;

struct async_loop_000 {
  typedef concurrent::async_loop_t<void, logger::cerr::log> async_loop;

  struct work {
    status::result operator()() {

      if (counter > 100) {
        return concurrent::stopped_by_work;
      }

      concurrent_log_debug(logger::cerr::log, "work = ", this,
                           ", counter = ", counter++);

      return status::ok;
    }

    uint16_t counter = 0;
  };

  bool operator()() {

    bool _result = true;
    try {

      concurrent::traits_t<void>::breaker _breaker = []() -> status::result {
        return status::ok;
      };
      work _work;
      concurrent_log_debug(logger::cerr::log, "work = ", this);
      async_loop _async_loop([&_work]() { return _work(); }, _breaker,
                             std::chrono::milliseconds(100));

      _async_loop.start();

      std::this_thread::sleep_for(std::chrono::seconds(2));

      concurrent_log_debug(logger::cerr::log,
                           "final counter = ", _work.counter);
      _result = (_work.counter == 101);

    } catch (std::exception &_ex) {
      concurrent_log_error(logger::cerr::log, "ERROR async_loop_000: '",
                           _ex.what(), "'");
      return false;
    }
    return _result;
  }

  static std::string desc() {
    return "Work in 'async_loop' will stop when a internal counter reaches "
           "100."
           "\nMain thread will sleep for 4 secs."
           "\nCounter should be 101.";
  }
};

struct async_loop_001 {
  typedef concurrent::async_loop_t<void, logger::cerr::log> async_loop;

  struct work1 {

    status::result operator()() {

      using namespace tenacitas;
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
      concurrent_log_debug(logger::cerr::log, "work = ", this,
                           ", counter = ", counter++);
      return status::ok;
    }

    uint64_t counter = 0;
  };

  bool operator()() {
    using namespace tenacitas;
    using namespace tenacitas;
    try {
      concurrent::traits_t<void>::breaker _breaker = []() -> status::result {
        return status::ok;
      };

      work1 _work;
      async_loop _async_loop([&_work]() -> status::result { return _work(); },
                             _breaker, std::chrono::milliseconds(1000));
      _async_loop.start();

      std::this_thread::sleep_for(std::chrono::seconds(1));

      concurrent_log_debug(logger::cerr::log, "counter = ", _work.counter);
      if (_work.counter != 4) {
        concurrent_log_debug(logger::cerr::log,
                             "wrong value for data, it should be 4");
        return false;
      }
    } catch (std::exception &_ex) {
      concurrent_log_error(logger::cerr::log, _ex.what());
      return false;
    }

    return true;
  }

  static std::string desc() {
    return "Work in 'async_loop' will increment an internal counter "
           "indefinetly, at each 200 ms."
           "\nMain thread will stop for 1 sec."
           "\nCounter should be 4.";
  }
};

struct async_loop_002 {
  typedef concurrent::async_loop_t<void, logger::cerr::log> async_loop;

  struct work1 {

    status::result operator()() {

      std::this_thread::sleep_for(std::chrono::milliseconds(200));
      concurrent_log_debug(logger::cerr::log, counter);
      ++counter;
      return status::ok;
    }

    uint64_t counter = 0;
  };

  bool operator()() {
    using namespace tenacitas;
    try {
      concurrent::traits_t<void>::breaker _breaker = []() -> status::result {
        return status::ok;
      };

      work1 _work;
      async_loop _async_loop([&_work]() -> status::result { return _work(); },
                             _breaker, std::chrono::milliseconds(1000));

      _async_loop.start();

      std::this_thread::sleep_for(std::chrono::seconds(1));

      concurrent_log_debug(logger::cerr::log, "counter = ", _work.counter);
      if (_work.counter != 4) {
        concurrent_log_error(logger::cerr::log,
                             "wrong value for data, it should be 4");
        return false;
      }

      _async_loop.stop();

      std::this_thread::sleep_for(std::chrono::seconds(1));

      _async_loop.start();

      std::this_thread::sleep_for(std::chrono::seconds(2));

      concurrent_log_debug(logger::cerr::log, "counter = ", _work.counter);
      if (_work.counter != 14) {
        concurrent_log_error(logger::cerr::log,
                             "wrong value for data, it should be 14");
        return false;
      }

    } catch (std::exception &_ex) {
      concurrent_log_error(logger::cerr::log, _ex.what());
      return false;
    }

    return true;
  }

  static std::string desc() {
    return "Work in 'async_loop' will increment an internal counter "
           "indefinetly, "
           "with a 200ms sleep."

           "\nMain thread will stop for 1 secs; counter should be 4; "
           "'async_loop' "
           "will stop; main thread will sleep for 2 secs; 'async_loop' will "
           "run "
           "again; counter should be 14";
  }
};

struct async_loop_003 {

  typedef concurrent::async_loop_t<uint32_t, logger::cerr::log> async_loop;

  struct work1 {

    status::result operator()(uint32_t &&p_data) {

      data = p_data;
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
      concurrent_log_debug(logger::cerr::log, p_data);
      return status::ok;
    }

    uint32_t data = 0;
  };

  struct provide {
    std::pair<status::result, uint32_t> operator()() {

      return {status::ok, m_data++};
    }

  private:
    uint32_t m_data = 0;
  };

  bool operator()() {
    using namespace tenacitas;

    try {
      concurrent::traits_t<uint32_t>::breaker _breaker =
          []() -> status::result { return status::ok; };

      work1 _work;

      concurrent::traits_t<uint32_t>::worker _worker =
          [&_work](uint32_t &&p_data) -> status::result {
        return _work(std::move(p_data));
      };

      async_loop _async_loop(_worker, _breaker, std::chrono::milliseconds(1000),
                             provide());

      concurrent_log_debug(logger::cerr::log, "starting");
      _async_loop.start();

      concurrent_log_debug(logger::cerr::log, "sleeping");
      std::this_thread::sleep_for(std::chrono::seconds(4));
      concurrent_log_debug(logger::cerr::log, "waking");

      concurrent_log_debug(logger::cerr::log, "stoping");
      _async_loop.stop();

      concurrent_log_debug(logger::cerr::log, "sleeping");
      std::this_thread::sleep_for(std::chrono::seconds(2));
      concurrent_log_debug(logger::cerr::log, "waking");

      concurrent_log_debug(logger::cerr::log, "starting");
      _async_loop.start();

      concurrent_log_debug(logger::cerr::log, "sleeping");
      std::this_thread::sleep_for(std::chrono::seconds(3));
      concurrent_log_debug(logger::cerr::log, "waking");

      concurrent_log_debug(logger::cerr::log, "data = ", _work.data);
      if (_work.data != 34) {
        concurrent_log_error(logger::cerr::log,
                             "wrong value for data, it should be 34");
        return false;
      }

    } catch (std::exception &_ex) {
      concurrent_log_error(logger::cerr::log, _ex.what());
      return false;
    }

    return true;
  }

  static std::string desc() {
    return "Work in 'async_loop' will print a provided uint32_t value at "
           "each 200 ms."

           "\nProvider will provide uint32_t values, starting in 0, "
           "incremented by 1."

           "\nMain thread will stop for 4 secs; 'async_loop' will stop; main "
           "thread will sleep for 2 sec; 'async_loop' will run again; main "
           "thread will sleep for 3 secs."

           "\nData in work should be 34";
  }
};

struct async_loop_004 {

  typedef concurrent::async_loop_t<uint32_t, logger::cerr::log> async_loop;

  struct work1 {
    status::result operator()(uint32_t &&p_data) {

      data = p_data;
      concurrent_log_debug(logger::cerr::log, p_data);
      return status::ok;
    }
    uint32_t data = 0;
  };

  struct provide {
    std::pair<status::result, uint32_t> operator()() {
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
      if (m_data > 1000) {
        return {concurrent::stopped_by_provider, 0};
      }
      return {status::ok, ++m_data};
    }

  private:
    uint32_t m_data = 0;
  };

  bool operator()() {
    using namespace tenacitas;
    try {
      work1 _work;
      async_loop _async_loop(
          [&_work](uint32_t &&p_data) { return _work(std::move(p_data)); },
          []() { return status::ok; }, std::chrono::milliseconds(1000),
          provide());

      _async_loop.start();

      std::this_thread::sleep_for(std::chrono::seconds(20));

      _async_loop.stop();

      concurrent_log_debug(logger::cerr::log, "data 1 ", _work.data);
      if (_work.data != 99) {
        concurrent_log_error(logger::cerr::log,
                             "wrong value for data, it should be 99");
        return false;
      }

      std::this_thread::sleep_for(std::chrono::seconds(4));

      _async_loop.start();

      std::this_thread::sleep_for(std::chrono::seconds(20));

      _async_loop.stop();

      concurrent_log_debug(logger::cerr::log, "data 2 ", _work.data);
      if (_work.data != 199) {
        concurrent_log_error(logger::cerr::log,
                             "wrong value for data, it should be 199");
        return false;
      }

    } catch (std::exception &_ex) {
      concurrent_log_error(logger::cerr::log, _ex.what());
      return false;
    }

    return true;
  }

  static std::string desc() {
    return "Work in 'async_loop' will print a provided uint32_t value."

           "\nProvider will provide uint32_t values, until 1000, with a 200 ms "
           "sleep."

           "\nMain thread will stop for 20 secs; 'async_loop' will stop; main "
           "thread will sleep for 4 secs; 'async_loop' will run again; main "
           "thread will sleep for 20 secs; 'async_loop' stops."

           "\nData in work should be 199";
  }
};

struct async_loop_005 {
  typedef concurrent::async_loop_t<uint32_t, logger::cerr::log> async_loop;
  struct work1 {
    status::result operator()(uint32_t &&p_data) {

      std::this_thread::sleep_for(std::chrono::milliseconds(10));
      concurrent_log_debug(logger::cerr::log, p_data);
      return status::ok;
    }
  };

  struct provide {
    std::pair<status::result, uint32_t> operator()() {
      std::this_thread::sleep_for(std::chrono::milliseconds(20));

      if (++m_data > 830) {
        return std::make_pair(concurrent::stopped_by_provider, 0);
      }
      return {status::ok, m_data};
    }
    uint32_t m_data = 0;
  };

  bool operator()() {
    using namespace tenacitas;
    try {
      async_loop sl1(work1(), []() { return status::ok; },
                     std::chrono::milliseconds(1000), provide());

      // uncomment the line below for a compiler error "error:
      // ‘tenacitas::concurrent::async_loop<t_timeout,
      // t_data>::async_loop(tenacitas::concurrent::async_loop<t_timeout,
      // t_data>::work_t&&, t_timeout&&,
      // tenacitas::concurrent::async_loop<t_timeout,
      // t_data>::control_t&&,
      // tenacitas::concurrent::async_loop<t_timeout,
      // t_data>::provide_t&&) [with t_timeout =
      // std::chrono::duration<long int, std::ratio<1, 1000> >; t_data =
      // unsigned int;
      // tenacitas::concurrent::async_loop<t_timeout,
      // t_data>::work_t = std::function<bool(unsigned int&&)>;
      // tenacitas::concurrent::async_loop<t_timeout,
      // t_data>::control_t = std::function<bool()>;
      // tenacitas::concurrent::async_loop<t_timeout,
      // t_data>::provide_t = std::function<std::pair<bool, unsigned
      // int>()>]’ is private within this context"
      //            async_loop_t sl0(work1(),
      //                             std::chrono::milliseconds(1000),
      //                             []() { return true; },
      //                             provide());

      // uncomment the line below for a compiler error "error: use of
      // deleted function
      // ‘tenacitas::concurrent::async_loop<t_work_ptr, t_time,
      // t_provide_ptr>::async_loop(const
      // tenacitas::concurrent::async_loop<t_work_ptr, t_time,
      // t_provide_ptr>&) [with t_work_ptr = std::shared_ptr<work1>;
      // t_time = std::chrono::duration<long int, std::ratio<1, 1000> >;
      // t_provide_ptr = std::shared_ptr<provide>]’"
      //            async_loop_t sl2(sl1);

      // *****************************************************************

      // uncomment the line below for a compiler error "error: use of
      // deleted function
      // ‘tenacitas::concurrent::async_loop<t_work_ptr, t_time,
      // t_provide_ptr>::async_loop() [with t_work_ptr =
      // std::shared_ptr<work1>; t_time = std::chrono::duration<long int,
      // std::ratio<1, 1000> >; t_provide_ptr =
      // std::shared_ptr<provide>]’"
      //            async_loop_t sl3;

      // *****************************************************************

      // uncomment the two lines below for a compiler error "error: use of
      // deleted function
      // ‘tenacitas::concurrent::async_loop<t_work_ptr, t_time,
      // t_provide_ptr>&
      // tenacitas::concurrent::async_loop<t_work_ptr, t_time,
      // t_provide_ptr>::operator=(const
      // tenacitas::concurrent::async_loop<t_work_ptr, t_time,
      // t_provide_ptr>&) [with t_work_ptr = std::shared_ptr<work1>;
      // t_time = std::chrono::duration<long int, std::ratio<1, 1000> >;
      // t_provide_ptr = std::shared_ptr<provide>]’"
      //            async_loop_t sl3(std::make_shared<work1>(),
      //                             std::chrono::milliseconds(1000),
      //                             []() { return true; },
      //                             std::make_shared<provide>(provide()));
      //            sl3 = sl1;

      // *****************************************************************

      // uncomment the line below for a compiler error "error: use of
      // deleted function
      // ‘tenacitas::concurrent::async_loop<t_work_ptr, t_time,
      // t_provide_ptr>::async_loop(tenacitas::concurrent::async_loop<t_work_ptr,
      // t_time, t_provide_ptr>&&) [with t_work_ptr =
      // std::shared_ptr<work1>; t_time = std::chrono::duration<long int,
      // std::ratio<1, 1000> >; t_provide_ptr =
      // std::shared_ptr<provide>]’"
      //            async_loop_t sl4(std::move(sl1));

      // *****************************************************************

      // uncomment the two lines below for a compiler error "error: use of
      // deleted function
      // ‘tenacitas::concurrent::async_loop<t_work_ptr, t_time,
      // t_provide_ptr>&
      // tenacitas::concurrent::async_loop<t_work_ptr, t_time,
      // t_provide_ptr>::operator=(tenacitas::concurrent::async_loop<t_work_ptr,
      // t_time, t_provide_ptr>&&) [with t_work_ptr =
      // std::shared_ptr<work1>; t_time = std::chrono::duration<long int,
      // std::ratio<1, 1000> >; t_provide_ptr =
      // std::shared_ptr<provide>]’"
      //            async_loop_t sl5(std::make_shared<work1>(),
      //                             std::chrono::milliseconds(1000),
      //                             []() { return true; },
      //                             std::make_shared<provide>(provide()));
      //            sl5 = std::move(sl1);

    } catch (std::exception &_ex) {
      concurrent_log_error(logger::cerr::log, _ex.what());
      return false;
    }

    return true;
  }

  static std::string desc() {
    return "Testing compile failing for private constructor, default "
           "constructor, copy constructor, move constructor, copy assignment "
           "and move assignment of 'async_loop'";
  }
};

struct async_loop_006 {

  typedef concurrent::async_loop_t<uint32_t, logger::cerr::log> async_loop;

  struct work1 {

    status::result operator()(uint32_t &&p_data) {

      std::this_thread::sleep_for(std::chrono::milliseconds(1001));
      concurrent_log_debug(logger::cerr::log, p_data);
      return status::ok;
    }
  };

  struct provide {
    std::pair<status::result, uint32_t> operator()() {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));

      if (m_data > 200) {
        return {concurrent::stopped_by_provider, 0};
      }
      return {status::ok, ++m_data};
    }
    uint32_t m_data = 0;
  };

  bool operator()() {
    using namespace tenacitas;
    try {

      async_loop _async_loop(work1(), []() { return status::ok; },
                             std::chrono::milliseconds(1000), provide());

      _async_loop.start();

      std::this_thread::sleep_for(std::chrono::seconds(4));

      _async_loop.stop();

    } catch (std::exception &_ex) {
      concurrent_log_error(logger::cerr::log, _ex.what());
      return false;
    }

    return true;
  }

  static std::string desc() {
    return "Work in 'async_loop' will print a provided uint32_t value, with "
           "a 1001 ms sleep, which will cause a timeout. "
           "\nProvider will provide uint32_t values, until 1000, with a 200 ms "
           "sleep. "
           "\nMain thread will stop for 4 secs; 'async_loop' stops.\n4 "
           "timeouts warnings will be printed";
  }
};

struct async_loop_007 {

  typedef concurrent::async_loop_t<uint32_t, logger::cerr::log> async_loop;
  struct work1 {
    status::result operator()(uint32_t &&p_data) {

      data = p_data;
      concurrent_log_debug(logger::cerr::log, p_data);
      return status::ok;
    }
    uint32_t data = 0;
  };

  struct provide {
    std::pair<status::result, uint32_t> operator()() {
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
      if (m_data > 1000) {
        return {concurrent::stopped_by_provider, 0};
      }
      return {status::ok, m_data++};
    }

    uint32_t get_data() const { return m_data; }

  private:
    uint32_t m_data = 1;
  };

  bool operator()() {
    using namespace tenacitas;
    try {
      work1 _work;
      provide _provide;
      async_loop _async_loop_1(
          [&_work](uint32_t &&p_data) { return _work(std::move(p_data)); },
          []() { return status::ok; }, std::chrono::milliseconds(1000),

          [&_provide]() -> std::pair<status::result, uint32_t> {
            return _provide();
          });

      _async_loop_1.start();

      std::this_thread::sleep_for(std::chrono::seconds(4));

      _async_loop_1.stop();

      concurrent_log_debug(logger::cerr::log, "work data = ", _work.data,
                           ", provider data = ", _provide.get_data());

      if (_work.data != 19) {
        concurrent_log_error(logger::cerr::log,
                             "wrong value for data, it should be 19");
        return false;
      }

      concurrent_log_debug(logger::cerr::log, "work data = ", _work.data,
                           ", provider data = ", _provide.get_data());

      async_loop _async_loop_2(
          [&_work](uint32_t &&p_data) { return _work(std::move(p_data)); },
          _async_loop_1.get_breaker(), _async_loop_1.get_timeout(),
          [&_provide]() -> std::pair<status::result, uint32_t> {
            return _provide();
          });

      concurrent_log_debug(logger::cerr::log, "work data = ", _work.data,
                           ", provider data = ", _provide.get_data());

      std::this_thread::sleep_for(std::chrono::seconds(2));

      concurrent_log_debug(logger::cerr::log, "work data = ", _work.data,
                           ", provider data = ", _provide.get_data());

      _async_loop_2.start();

      std::this_thread::sleep_for(std::chrono::seconds(3));

      _async_loop_2.stop();

      concurrent_log_debug(logger::cerr::log, "work data = ", _work.data,
                           ", provider data = ", _provide.get_data());

      if (_work.data != 34) {
        concurrent_log_error(logger::cerr::log,
                             "wrong value for data, it should be 35");
        return false;
      }

    } catch (std::exception &_ex) {
      concurrent_log_error(logger::cerr::log, _ex.what());
      return false;
    }

    return true;
  }

  static std::string desc() {
    return "Work in 'async_loop' will print a provided uint32_t value."

           "\nProvider will provide uint32_t values, until 1000, with a 200 ms "
           "sleep."

           "\nFirst 'async_loop' will run, main thread will sleep for 4 secs, "
           "then 'async_loop' will stop."

           "\nA second 'async_loop' is created, based on first one. "

           "\nMain thread sleeps for 2 secs, second 'async_loop' will run. "

           "\nMain thread sleeps for 3 secs, and second 'async_loop' stops. "

           "\nData in work should be 34.";
  }
};

struct async_loop_008 {
  typedef concurrent::async_loop_t<uint32_t, logger::cerr::log> async_loop;
  struct work1 {
    status::result operator()(uint32_t &&p_data) {
      data += p_data;
      concurrent_log_debug(logger::cerr::log, "1 -> ", data);
      return status::ok;
    }
    uint32_t data = 0;
  };

  struct work2 {
    status::result operator()(uint32_t &&p_data) {
      using namespace tenacitas;
      data = p_data;
      concurrent_log_debug(logger::cerr::log, "2 -> ", data);
      return status::ok;
    }
    uint32_t data = 0;
  };

  struct provide {
    std::pair<status::result, uint32_t> operator()() {
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
      if (m_data > 1000) {
        return {concurrent::stopped_by_provider, 0};
      }
      return {status::ok, ++m_data};
    }

  private:
    uint32_t m_data = 0;
  };

  bool operator()() {
    using namespace tenacitas;
    try {
      provide _provide;
      work1 _work_1;
      work2 _work_2;

      async_loop _al_1(
          [&_work_1](uint32_t &&p_data) { return _work_1(std::move(p_data)); },
          []() { return status::ok; }, std::chrono::milliseconds(1000),
          [&_provide]() -> std::pair<status::result, uint32_t> {
            return _provide();
          });

      async_loop _al_2(
          [&_work_2](uint32_t &&p_data) { return _work_2(std::move(p_data)); },
          []() { return status::ok; }, std::chrono::milliseconds(1000),
          [&_provide]() -> std::pair<status::result, uint32_t> {
            return _provide();
          });

      _al_1.start();
      _al_2.start();

      std::this_thread::sleep_for(std::chrono::seconds(10));

      _al_1.stop();
      _al_2.stop();

      concurrent_log_debug(logger::cerr::log, "work 1 data = ", _work_1.data);
      concurrent_log_debug(logger::cerr::log, "work 2 data = ", _work_2.data);

    } catch (std::exception &_ex) {
      concurrent_log_error(logger::cerr::log, _ex.what());
      return false;
    }

    return true;
  }

  static std::string desc() {
    return "Work 1 in 'async_loop' will print a provided uint32_t value, as "
           "well as Work 2."

           "\nProvider will provide incremente uint32_t values, to a Work 1 "
           "and Work 2 objects, until 1000, with a 200 ms sleep."

           "\nMain thread will run a 'sleeping_loop'; stop for 10 secs."

           "\nThe final data for Work 1 and Work 2 is unpredictable.";
  }
};

struct async_loop_009 {
  typedef concurrent::async_loop_t<void, logger::cerr::log> async_loop;

  struct work1 {

    status::result operator()() {

      using namespace tenacitas;
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
      concurrent_log_debug(logger::cerr::log, "work = ", this,
                           ", counter = ", counter++);
      return status::ok;
    }

    uint64_t counter = 0;
  };

  bool operator()() {
    using namespace tenacitas;
    using namespace tenacitas;
    try {
      work1 _work;
      async_loop _async_loop(_work,
                             []() -> status::result { return status::ok; },
                             std::chrono::milliseconds(1000));

      _async_loop.start();

      std::this_thread::sleep_for(std::chrono::seconds(1));

      concurrent_log_debug(logger::cerr::log, "counter = ", _work.counter);
      if (_work.counter != 0) {
        concurrent_log_debug(logger::cerr::log,
                             "wrong value for data, it should be 0, but it is ",
                             _work.counter);
        return false;
      }
    } catch (std::exception &_ex) {
      concurrent_log_error(logger::cerr::log, _ex.what());
      return false;
    }

    return true;
  }

  static std::string desc() {
    return "Work in 'async_loop' will increment an internal counter "
           "indefinetly, at each 200 ms."
           "\nMain thread will stop for 1 sec."
           "\nThe work functor will not have its internal counter modified, as "
           "it was passed by copy to 'async_loop'";
  }
};

int main(int argc, char **argv) {
  tenacitas::logger::cerr::log::set_debug();

  tester::test _tester(argc, argv);

  run_test(_tester, async_loop_000);
  run_test(_tester, async_loop_001);
  run_test(_tester, async_loop_002);
  run_test(_tester, async_loop_003);
  run_test(_tester, async_loop_004);
  run_test(_tester, async_loop_005);
  run_test(_tester, async_loop_006);
  run_test(_tester, async_loop_007);
  run_test(_tester, async_loop_008);
  run_test(_tester, async_loop_009);
}
