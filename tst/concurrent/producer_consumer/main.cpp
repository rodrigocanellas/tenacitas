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
    worker _worker;
    _pc.add([&_worker](data &&p_data) { return _worker(std::move(p_data)); },
            std::chrono::milliseconds(500));

    concurrent_debug(m_log, "capacity = ", _pc.capacity(),
                     ", occupied = ", _pc.occupied());
    _pc.start();
    _pc.add(-8);

    concurrent_debug(m_log, "capacity = ", _pc.capacity(),
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
      concurrent_info(m_log, "value = ", p_value);
      return status::ok;
    }

  private:
    logger::cerr::log m_log{"producer_consumer_000::worker"};
  };

private:
  logger::cerr::log m_log{"producer_consumer_000"};
};

struct producer_consumer_001 {

  typedef concurrent::fixed_size_queue_t<msg, log> queue;
  typedef concurrent::producer_consumer_t<queue, log> producer_consumer;

  bool operator()() {

    const uint16_t _num_msgs = 50;

    producer_consumer _pc({queue(10)});

    concurrent_debug(m_log, "capacity = ", _pc.capacity(),
                     ", occupied = ", _pc.occupied());

    sleeping_loop _loop(
        std::chrono::milliseconds(500),
        [&_pc, this]() {
          if (m_value > _num_msgs) {
            return concurrent::stopped_by_breaker;
          }
          msg _msg(++m_value);
          concurrent_debug(m_log, "going to add ", _msg);

          _pc.add(std::move(_msg));
          concurrent_debug(m_log, "added msg ", _msg,
                           "; capacity = ", _pc.capacity(),
                           ", occupied = ", _pc.occupied());

          return status::ok;
        },
        std::chrono::seconds(1));

    _pc.add(
        [this](msg &&p_msg) -> status::result {
          return m_work(std::move(p_msg));
        },
        std::chrono::seconds(2));

    _pc.start();
    _loop.start();

    concurrent_debug(m_log, "sleeping for ", _num_msgs, " secs");
    std::this_thread::sleep_for(std::chrono::seconds(_num_msgs + 1));
    concurrent_debug(m_log, "waking up after ", _num_msgs, "secs");

    concurrent_debug(m_log, "produced = ", m_value,
                     ", consumed = ", m_work.m_msg.counter());
    if (m_work.m_msg.counter() != m_value) {
      concurrent_error(m_log,
                       "Data value consumed should be equal to provided");
      return false;
    }

    return true;
  }

  ~producer_consumer_001() {}

  static std::string desc() {
    return "\nA 'sleeping_loop' sending a message at each 500 ms, to a "
           "'producer_consumer', with one consumer, that sleeps for 1 second, "
           "and using a 'fixed_size_queue', with size 10"

           "\nThe main thread will sleep for 50 secs"

           "\nThe amount of data consumed must be equal to the provided";
  }

private:
  struct worker {
    status::result operator()(msg &&p_msg) {
      concurrent_debug(m_log, "handling msg ", p_msg, " ", &p_msg);
      m_msg = std::move(p_msg);

      concurrent_debug(m_log, "handling msg ", p_msg, " ", &p_msg);

      concurrent_debug(m_log, "worker is going to sleep");
      std::this_thread::sleep_for(std::chrono::seconds(1));
      concurrent_debug(m_log, "worker is waking up");
      return status::ok;
    }
    msg m_msg;

  private:
    logger::cerr::log m_log{"producer_consumer_001::worker"};
  };

private:
  worker m_work;

  msg::data m_value = 0;

private:
  logger::cerr::log m_log{"producer_consumer_001"};
};

struct producer_consumer_002 {

  typedef concurrent::fixed_size_queue_t<msg, log> queue;
  typedef concurrent::producer_consumer_t<queue, log> producer_consumer;

  struct work {
    status::result operator()(msg &&p_msg) {
      m_msg = p_msg;
      concurrent_debug(m_log, "handling msg ", m_msg);
      return status::ok;
    }
    logger::cerr::log m_log{"producer_consumer_002::work"};
    msg m_msg;
  };

  struct producer {
    producer(producer_consumer *p_pc, msg::data *p_data)
        : m_pc(p_pc), m_data(p_data) {}

    status::result operator()() {
      msg _msg(++(*m_data));
      concurrent_debug(m_log, "adding msg ", _msg);
      m_pc->add(std::move(_msg));
      return status::ok;
    }

  private:
    logger::cerr::log m_log{"producer"};
    producer_consumer *m_pc;
    msg::data *m_data;
  };

  bool operator()() {

    work _work;

    producer_consumer _pc({queue(40)});

    msg::data _value = 0;

    producer _producer(&_pc, &_value);

    sleeping_loop _loop(
        std::chrono::milliseconds(500),
        [&_producer]() -> status::result { return _producer(); },
        std::chrono::milliseconds(300));

    _pc.add([&_work](msg &&p_msg) { return _work(std::move(p_msg)); },
            std::chrono::milliseconds(100));

    _pc.start();
    _loop.start();

    concurrent_debug(m_log, "sleeping for 10 secs");
    std::this_thread::sleep_for(std::chrono::seconds(10));
    concurrent_debug(m_log, "waking up after 10 secs");

    concurrent_debug(m_log, "stopping the producer_consumer");
    _pc.stop();

    concurrent_debug(m_log, "sleeping for 5 secs");
    std::this_thread::sleep_for(std::chrono::seconds(5));
    concurrent_debug(m_log, "waking up after 5 secs");

    concurrent_debug(m_log, "restarting the producer_consumer");
    _pc.start();

    concurrent_debug(m_log, "sleeping for 4 secs");
    std::this_thread::sleep_for(std::chrono::seconds(4));
    concurrent_debug(m_log, "waking up after 4 secs");

    concurrent_debug(m_log, "provided = ", _value,
                     ", consumed = ", _work.m_msg.counter());
    if (_work.m_msg.counter() != _value) {
      concurrent_error(m_log,
                       "Data value consumed should be equal to provided");
      return false;
    }

    return true;
  }

  static std::string desc() {
    return "\nA 'sleeping_loop' sending a message at each 500 ms, to a "
           "'producer_consumer' with one consumer, using a 'fixed_size_queue', "
           "with size 40"

           "\nThe main thread will sleep for 10 secs, the 'producer_consumer' "
           "will stop; main thread will sleep for 5 secs; 'producer_consumer' "
           "will run again; main thread will sleep for 4 secs."

           "\nThe messages added while the queue was stopped are handled when "
           "the pool runs again."

           "\nThe amount of data consumed must be equal to the provided";
  }

private:
  logger::cerr::log m_log{"producer_consumer_002"};
};

int main(int argc, char **argv) {
  log::set_debug();
  tester::test _test(argc, argv);
  run_test(_test, producer_consumer_000);
  run_test(_test, producer_consumer_001);
  run_test(_test, producer_consumer_002);
}
