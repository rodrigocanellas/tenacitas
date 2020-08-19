/// \example Basic use of tenacitas::concurrent::loop_t

#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>

#include <concurrent/fixed_size_queue.h>
#include <concurrent/internal/log.h>
#include <concurrent/loop.h>
#include <concurrent/msg_a.h>
#include <concurrent/producer_consumer.h>
#include <concurrent/result.h>
#include <concurrent/sleeping_loop.h>
#include <logger/cerr/log.h>
#include <status/result.h>
#include <tester/test.h>

using namespace tenacitas;

typedef int16_t data;
typedef logger::cerr::log log;
typedef concurrent::msg_a<data> msg;
typedef concurrent::sleeping_loop_t<void, log> sleeping_loop;

struct producer_consumer_000 {
  typedef concurrent::fixed_size_queue_t<data, log> queue;
  typedef concurrent::producer_consumer_t<queue, log> producer_consumer;

  bool operator()() {
    producer_consumer _pc({queue(10)});
    _pc.add(worker(), std::chrono::milliseconds(500));

    concurrent_log_debug(logger::cerr::log, "capacity = ", _pc.capacity(),
                         ", occupied = ", _pc.occupied());
    _pc.start();
    _pc.add(-8);

    concurrent_log_debug(logger::cerr::log, "capacity = ", _pc.capacity(),
                         ", occupied = ", _pc.occupied());

    std::this_thread::sleep_for(std::chrono::seconds(1));

    return true;
  }

  static std::string desc() {
    return "Simple test, creating a producer_consumer, adding a single data, "
           "and starting";
  }

private:
  struct worker {
    status::result operator()(data &&p_value) {
      concurrent_log_info(log, "value = ", p_value);
      return status::ok;
    }
  };
};

struct producer_consumer_001 {

  typedef concurrent::fixed_size_queue_t<msg, log> queue;
  typedef concurrent::producer_consumer_t<queue, log> producer_consumer;

  bool operator()() {

    producer_consumer _pc({queue(10)});

    concurrent_log_debug(logger::cerr::log, "capacity = ", _pc.capacity(),
                         ", occupied = ", _pc.occupied());

    worker _work;

    msg::data _value = 0;

    sleeping_loop _loop(
        std::chrono::milliseconds(500),
        [&_pc, &_value]() {
          msg _msg(++_value);
          concurrent_log_debug(log, "going to add ", _msg);

          _pc.add(std::move(_msg));
          concurrent_log_debug(logger::cerr::log, "added msg ", _msg,
                               "; capacity = ", _pc.capacity(),
                               ", occupied = ", _pc.occupied());
          return status::ok;
        },
        std::chrono::milliseconds(300));

    _pc.add(
        [&_work](msg &&p_msg) -> status::result {
          return _work(std::move(p_msg));
        },
        std::chrono::minutes(1));

    _pc.start();
    _loop.start();

    concurrent_log_debug(logger::cerr::log, "sleeping for 10 secs");
    std::this_thread::sleep_for(std::chrono::seconds(10));
    concurrent_log_debug(logger::cerr::log, "waking up after 10 secs");

    //    concurrent_log_debug(logger::cerr::log, "stopping the pool");
    //    _pc.stop();

    //    concurrent_log_debug(logger::cerr::log, "sleeping for 5 secs");
    //    std::this_thread::sleep_for(std::chrono::seconds(5));
    //    concurrent_log_debug(logger::cerr::log, "waking up after 5 secs");

    //    concurrent_log_debug(logger::cerr::log, "runnig the pool");
    //    _pc.start();

    //    concurrent_log_debug(logger::cerr::log, "sleeping for 4 secs");
    //    std::this_thread::sleep_for(std::chrono::seconds(4));
    //    concurrent_log_debug(logger::cerr::log, "waking up after 4 secs");

    //    concurrent_log_debug(logger::cerr::log, "provided = ", _value,
    //                         ", consumed = ", _work.m_msg.counter());
    //    if (_work.m_msg.counter() != _value) {
    //      concurrent_log_error(logger::cerr::log,
    //                           "Data value consumed should be equal to
    //                           provided");
    //      return false;
    //    }

    return true;
  }

  static std::string desc() {
    return "\nA 'sleeping_loop' sending a message at each 500 ms, to a "
           "'producer_consumer', with one consumer, and using a "
           "'fixed_size_queue', with size 10"

           "\nThe main thread will sleep for 10 secs, the 'procuder_consumer' "
           "will stop; main thread will sleep for 5 secs; 'producer_consumer' "
           "will run again; main thread will sleep for 4 secs."

           "\nThe messages added while the 'producer_consumer' was stopped are "
           "handled when it runs again."

           "\nThe amount of data consumed must be equal to the provided";
  }

private:
  struct worker {
    status::result operator()(msg &&p_msg) {
      concurrent_log_debug(logger::cerr::log, "handling msg ", p_msg, " ",
                           &p_msg);
      m_msg = std::move(p_msg);

      concurrent_log_debug(logger::cerr::log, "handling msg ", p_msg, " ",
                           &p_msg);

      concurrent_log_debug(log, "worker is going to sleep");
      std::this_thread::sleep_for(std::chrono::seconds(5));
      concurrent_log_debug(log, "worker is waking up");
      return status::ok;
    }
    msg m_msg;
  };
};

int main(int argc, char **argv) {
  log::set_debug();
  tester::test _test(argc, argv);
  run_test(_test, producer_consumer_000);
  run_test(_test, producer_consumer_001);
}
