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

// using namespace tenacitas;

struct producer_consumer_000 {
  typedef tenacitas::logger::cerr::log log;
  typedef tenacitas::status::result result;
  typedef int16_t data;
  typedef tenacitas::concurrent::msg_a<data> msg;
  typedef tenacitas::concurrent::sleeping_loop_t<void, log> sleeping_loop;
  typedef tenacitas::concurrent::fixed_size_queue_t<data, log> queue;
  typedef tenacitas::concurrent::producer_consumer_t<queue, log>
      producer_consumer;

  bool operator()() {
    log _log{"000"};

    producer_consumer _pc({queue(10)});
    consumer _consumer;
    _pc.add(
        [&_consumer](data &&p_data) -> result {
          return _consumer(std::move(p_data));
        },
        std::chrono::milliseconds(500));

    concurrent_debug(_log, "capacity = ", _pc.capacity(),
                     ", occupied = ", _pc.occupied());
    _pc.start();
    _pc.add(-8);

    concurrent_debug(_log, "capacity = ", _pc.capacity(),
                     ", occupied = ", _pc.occupied());

    std::this_thread::sleep_for(std::chrono::seconds(1));

    return true;
  }

  static std::string desc() {
    return "Simple test, creating a producer_consumer, adding a single data, "
           "and starting";
  }

private:
  struct consumer {
    result operator()(data &&p_value) {
      concurrent_info(m_log, "value = ", p_value);
      return tenacitas::status::ok;
    }

  private:
    log m_log{"000::consumer"};
  };
};

struct producer_consumer_001 {
  typedef tenacitas::logger::cerr::log log;
  typedef tenacitas::status::result result;
  typedef int16_t data;
  typedef tenacitas::concurrent::msg_a<data> msg;
  typedef tenacitas::concurrent::sleeping_loop_t<void, log> sleeping_loop;
  typedef tenacitas::concurrent::fixed_size_queue_t<msg, log> queue;
  typedef tenacitas::concurrent::producer_consumer_t<queue, log>
      producer_consumer;

  bool operator()() {
    log _log{"001"};

    producer_consumer _pc({queue(10)});

    concurrent_debug(_log, "capacity = ", _pc.capacity(),
                     ", occupied = ", _pc.occupied());

    msg::data _data = 0;

    producer _producer(&_pc, &_data);

    consumer _consumer;

    sleeping_loop _loop(
        std::chrono::milliseconds(500),
        [&_producer]() -> result { return _producer(); },
        std::chrono::seconds(1));

    _pc.add([&_consumer](
                msg &&p_msg) -> result { return _consumer(std::move(p_msg)); },
            std::chrono::seconds(2));

    _pc.start();
    _loop.start();

    concurrent_debug(_log, "sleeping for ", _producer.m_num_msgs, " secs");
    std::this_thread::sleep_for(std::chrono::seconds(_producer.m_num_msgs + 1));
    concurrent_debug(_log, "waking up after ", _producer.m_num_msgs, "secs");

    concurrent_debug(_log, "produced = ", _data,
                     ", consumed = ", _consumer.m_msg.counter());
    if (_consumer.m_msg.counter() != _data) {
      concurrent_error(_log, "Data value consumed should be equal to provided");
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
  struct producer {
    producer(producer_consumer *p_pc, msg::data *p_data)
        : m_pc(p_pc), m_data(p_data) {}

    result operator()() {
      if (*(m_data) > m_num_msgs) {
        return tenacitas::concurrent::stopped_by_worker;
      }
      msg _msg(++(*m_data));
      concurrent_debug(m_log, "going to add ", _msg);

      m_pc->add(std::move(_msg));
      concurrent_debug(m_log, "added msg ", _msg,
                       "; capacity = ", m_pc->capacity(),
                       ", occupied = ", m_pc->occupied());

      return tenacitas::status::ok;
    }
    const uint16_t m_num_msgs = 50;

  private:
    log m_log{"001::producer"};
    producer_consumer *m_pc;
    msg::data *m_data;
  };

  struct consumer {
    result operator()(msg &&p_msg) {
      concurrent_debug(m_log, "handling msg ", p_msg, " ", &p_msg);
      m_msg = std::move(p_msg);

      concurrent_debug(m_log, "handling msg ", p_msg, " ", &p_msg);

      concurrent_debug(m_log, "worker is going to sleep");
      std::this_thread::sleep_for(std::chrono::seconds(1));
      concurrent_debug(m_log, "worker is waking up");
      return tenacitas::status::ok;
    }
    msg m_msg;

  private:
    log m_log{"001::consumer"};
  };
};

struct producer_consumer_002 {
  typedef tenacitas::logger::cerr::log log;
  typedef tenacitas::status::result result;
  typedef int16_t data;
  typedef tenacitas::concurrent::msg_a<data> msg;
  typedef tenacitas::concurrent::sleeping_loop_t<void, log> sleeping_loop;
  typedef tenacitas::concurrent::fixed_size_queue_t<msg, log> queue;
  typedef tenacitas::concurrent::producer_consumer_t<queue, log>
      producer_consumer;

  bool operator()() {

    log _log{"002"};

    producer_consumer _pc({queue(40)});

    msg::data _value = 0;

    producer _producer(&_pc, &_value);

    consumer _consumer;

    sleeping_loop _loop(
        std::chrono::milliseconds(500),
        [&_producer]() -> result { return _producer(); },
        std::chrono::milliseconds(300));

    _pc.add([&_consumer](msg &&p_msg) { return _consumer(std::move(p_msg)); },
            std::chrono::milliseconds(100));

    _pc.start();
    _loop.start();

    concurrent_debug(_log, "sleeping for 10 secs");
    std::this_thread::sleep_for(std::chrono::seconds(10));
    concurrent_debug(_log, "waking up after 10 secs");

    concurrent_debug(_log, "stopping the producer_consumer");
    _pc.stop();

    concurrent_debug(_log, "sleeping for 5 secs");
    std::this_thread::sleep_for(std::chrono::seconds(5));
    concurrent_debug(_log, "waking up after 5 secs");

    concurrent_debug(_log, "restarting the producer_consumer");
    _pc.start();

    concurrent_debug(_log, "sleeping for 4 secs");
    std::this_thread::sleep_for(std::chrono::seconds(4));
    concurrent_debug(_log, "waking up after 4 secs");

    concurrent_info(_log, "provided = ", _value,
                    ", consumed = ", _consumer.m_msg.counter());
    if (_consumer.m_msg.counter() != _value) {
      concurrent_error(_log, "Data value consumed should be equal to provided");
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

  struct consumer {
    result operator()(msg &&p_msg) {
      m_msg = p_msg;
      concurrent_debug(m_log, "handling msg ", m_msg);
      return tenacitas::status::ok;
    }
    log m_log{"002::consumer"};
    msg m_msg;
  };

  struct producer {
    producer(producer_consumer *p_pc, msg::data *p_data)
        : m_pc(p_pc), m_data(p_data) {}

    result operator()() {
      msg _msg(++(*m_data));
      concurrent_debug(m_log, "adding msg ", _msg);
      m_pc->add(std::move(_msg));
      return tenacitas::status::ok;
    }

  private:
    log m_log{"002::producer"};
    producer_consumer *m_pc;
    msg::data *m_data;
  };
};

struct producer_consumer_003 {
  typedef tenacitas::logger::cerr::log log;
  typedef tenacitas::status::result result;
  typedef uint16_t data;
  typedef tenacitas::concurrent::msg_a<data> msg;
  typedef tenacitas::concurrent::sleeping_loop_t<void, log> sleeping_loop;
  typedef tenacitas::concurrent::fixed_size_queue_t<msg, log> queue;
  typedef tenacitas::concurrent::producer_consumer_t<queue, log>
      producer_consumer;

  bool operator()() {

    producer_consumer _pc{queue(40)};

    _pc.add([this](msg &&p_msg) { return m_consumer(std::move(p_msg)); },
            std::chrono::milliseconds(1000));

    _pc.start();

    for (uint16_t _i = 0; _i < 300; ++_i) {
      msg _msg(_i);
      concurrent_debug(m_log, "adding msg ", _msg);
      _pc.add(std::move(_msg));
    }

    uint64_t _amount_added = _pc.amount_added();

    while (m_consumer.m_msg.counter() != _amount_added) {
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
      concurrent_debug(m_log, "still waiting...");
    }

    return true;
  }

  static std::string desc() {
    return "\nProduces 300 messages, and waits for all to be consumed";
  }

private:
  struct consumer {
    result operator()(msg &&p_msg) {
      m_msg = p_msg;
      concurrent_debug(m_log, "handling msg ", m_msg);
      //      std::this_thread::sleep_for(std::chrono::milliseconds(150));
      return tenacitas::status::ok;
    }
    log m_log{"003::work"};
    msg m_msg;
  };

private:
  log m_log{"producer_consumer_003"};
  consumer m_consumer;
};

int main(int argc, char **argv) {
  typedef tenacitas::logger::cerr::log log;
  typedef tenacitas::tester::test test;
  log::set_debug();
  test _test(argc, argv);
  run_test(_test, producer_consumer_000);
  run_test(_test, producer_consumer_001);
  run_test(_test, producer_consumer_002);
  run_test(_test, producer_consumer_003);
  //  run_test(_test, producer_consumer_003);
}
