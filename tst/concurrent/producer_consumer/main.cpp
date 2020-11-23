/// \example Basic use of concurrent::loop_t

#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>

#include <concurrent/async_loop.h>
#include <concurrent/internal/log.h>
#include <concurrent/msg_a.h>
#include <concurrent/producer_consumer.h>
#include <concurrent/sleeping_loop.h>
#include <logger/cerr/log.h>
#include <tester/test.h>

using namespace tenacitas;

struct producer_consumer_000 {
  typedef logger::cerr::log log;
  typedef int16_t data;
  typedef concurrent::producer_consumer_t<log, int16_t> producer_consumer;
  typedef concurrent::timeout_callback timeout_callback;

  static std::string desc() {
    return "Simple test, creating a producer_consumer, adding a single data, "
           "and starting";
  }

  bool operator()() {
    log _log{"000"};

    producer_consumer _pc;
    consumer _consumer;

    producer_consumer::worker _worker = [&_consumer](data &&p_data) -> void {
      _consumer(std::move(p_data));
    };

    timeout_callback _timeout_callback = [&_log](std::thread::id p_id) -> void {
      concurrent_debug(_log, "timeout for ", p_id);
    };

    _pc.add(_worker, std::chrono::milliseconds(500), _timeout_callback);

    concurrent_debug(_log, "capacity = ", _pc.capacity(),
                     ", occupied = ", _pc.occupied());
    _pc.start();
    _pc.add(-8);

    concurrent_debug(_log, "capacity = ", _pc.capacity(),
                     ", occupied = ", _pc.occupied());

    std::this_thread::sleep_for(std::chrono::seconds(1));

    return true;
  }

private:
  struct consumer {
    void operator()(data &&p_value) {
      concurrent_info(m_log, "value = ", p_value);
    }

  private:
    log m_log{"000::consumer"};
  };
};

struct producer_consumer_001 {
  typedef logger::cerr::log log;
  typedef concurrent::msg_a msg;
  typedef concurrent::sleeping_loop_t<log> sleeping_loop;
  typedef concurrent::producer_consumer_t<log, msg> producer_consumer;
  typedef concurrent::timeout_callback timeout_callback;

  static std::string desc() {
    return "\nA 'sleeping_loop' sending a message at each 500 ms, to a "
           "'producer_consumer', with one consumer, that sleeps for 1 second,  "
           "and using a 'fixed_size_queue', with size 10 \nThe main thread "
           "will sleep for 50 secs"
           "\nThe amount of data consumed must be equal to the provided";
  }

  bool operator()() {
    log _log{"001"};

    producer_consumer _pc(10);

    timeout_callback _timeout_callback = [&_log](std::thread::id p_id) -> void {
      concurrent_debug(_log, "timeout for ", p_id);
    };

    concurrent_debug(_log, "capacity = ", _pc.capacity(),
                     ", occupied = ", _pc.occupied());

    msg _msg(0);

    producer _producer(&_pc, &_msg);

    consumer _consumer;

    concurrent_debug(_log, "creating the sleeping_loop");
    sleeping_loop _loop(std::chrono::milliseconds(500), std::chrono::seconds(1),
                        _producer, _timeout_callback);

    concurrent_debug(_log, "adding consumer to the producer_consumer");
    _pc.add(
        [&_consumer](msg &&p_msg) -> std::optional<bool> {
          return _consumer(std::move(p_msg));
        },
        std::chrono::milliseconds(2000), _timeout_callback);

    concurrent_debug(_log, "starting the sleeping loop");
    _loop.start();
    concurrent_debug(_log, "staring the producer consumer");
    _pc.start();

    concurrent_debug(_log, "sleeping for ", _producer.m_num_msgs, " secs");
    std::this_thread::sleep_for(std::chrono::seconds(_producer.m_num_msgs + 1));
    concurrent_debug(_log, "waking up after ", _producer.m_num_msgs, "secs");

    concurrent_debug(_log, "produced = ", _msg,
                     ", consumed = ", _consumer.m_msg.value());
    if (_consumer.m_msg.value() != _msg.value()) {
      concurrent_error(_log, "Data value consumed should be equal to provided");
      return false;
    }

    return true;
  }

  ~producer_consumer_001() {}

private:
  struct producer {
    producer(producer_consumer *p_pc, msg *p_msg) : m_pc(p_pc), m_msg(p_msg) {}

    bool operator()() {
      if (m_msg->value() > m_num_msgs) {
        return false;
      }

      m_msg->inc();
      msg _msg(m_msg->value());
      concurrent_debug(m_log, "going to add ", _msg);

      m_pc->add(_msg);
      concurrent_debug(m_log, "added msg ", _msg,
                       "; capacity = ", m_pc->capacity(),
                       ", occupied = ", m_pc->occupied());

      return true;
    }
    const uint16_t m_num_msgs = 50;

  private:
    log m_log{"001::producer"};
    producer_consumer *m_pc;
    msg *m_msg;
  };

  struct consumer {
    std::optional<bool> operator()(msg &&p_msg) {
      concurrent_debug(m_log, "handling msg ", p_msg);
      m_msg = std::move(p_msg);

      concurrent_debug(m_log, "handling msg ", m_msg);

      concurrent_debug(m_log, "worker is going to sleep");
      std::this_thread::sleep_for(std::chrono::seconds(1));
      concurrent_debug(m_log, "worker is waking up");
      return {true};
    }
    msg m_msg;

  private:
    log m_log{"001::consumer"};
  };
};

struct producer_consumer_002 {
  typedef logger::cerr::log log;
  typedef concurrent::msg_a msg;
  typedef concurrent::sleeping_loop_t<log> sleeping_loop;
  typedef concurrent::producer_consumer_t<log, msg> producer_consumer;
  typedef concurrent::timeout_callback timeout_callback;

  static std::string desc() {
    std::stringstream _stream;
    _stream
        << "\nA 'sleeping_loop' sending a message at each 500 ms, to "
        << "a 'producer_consumer' with one consumer, using a "
        << "'fixed_size_queue', with size 40"

        << "\nThe main thread will sleep for 10 secs, the 'producer_consumer' "
        << "will stop; main thread will sleep for 5 secs; 'producer_consumer' "
        << "will run again; main thread will sleep for 4 secs."

        << "\nThe messages added while the queue was stopped are handled when "
        << "the pool runs again ."

        << "\nThe amount of data consumed must be equal to the provided";
    return _stream.str();
  }

  bool operator()() {

    log _log{"002"};

    producer_consumer _pc(40);

    msg _msg(0);

    producer _producer(&_pc, &_msg);

    consumer _consumer;

    timeout_callback _timeout_callback = [&_log](std::thread::id p_id) -> void {
      concurrent_debug(_log, "timeout for ", p_id);
    };

    concurrent_debug(_log, "creating the sleeping_loop");

    sleeping_loop _loop(
        std::chrono::milliseconds(300), std::chrono::milliseconds(500),
        [&_producer]() -> void { _producer(); }, _timeout_callback);

    concurrent_debug(_log, "adding consumer to the producer_consumer");
    _pc.add([&_consumer](msg &&p_msg) { return _consumer(std::move(p_msg)); },
            std::chrono::milliseconds(100), _timeout_callback);

    _loop.start();
    _pc.start();

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

    concurrent_info(_log, "provided = ", _msg,
                    ", consumed = ", _consumer.m_msg.value());
    if (_consumer.m_msg.value() != _msg.value()) {
      concurrent_error(_log, "Data value consumed should be equal to provided");
      return false;
    }

    return true;
  }

  struct consumer {
    bool operator()(msg &&p_msg) {
      m_msg = p_msg;
      concurrent_debug(m_log, "handling msg ", m_msg);
      return true;
    }
    log m_log{"002::consumer"};
    msg m_msg;
  };

  struct producer {
    producer(producer_consumer *p_pc, msg *p_data)
        : m_pc(p_pc), m_msg(p_data) {}

    bool operator()() {
      m_msg->inc();
      msg _msg(m_msg->value());
      concurrent_debug(m_log, "adding msg ", _msg);
      m_pc->add(std::move(_msg));
      return true;
    }

  private:
    log m_log{"002::producer"};
    producer_consumer *m_pc;
    msg *m_msg;
  };
};

struct producer_consumer_003 {
  typedef logger::cerr::log log;
  typedef concurrent::msg_a msg;
  typedef concurrent::producer_consumer_t<log, msg> producer_consumer;
  typedef concurrent::timeout_callback timeout_callback;

  static std::string desc() {
    return "\nProduces 300 messages, and waits for all to be consumed";
  }

  bool operator()() {
    timeout_callback _timeout_callback = [this](std::thread::id p_id) -> void {
      concurrent_debug(m_log, "timeout for ", p_id);
    };

    producer_consumer _pc{40};

    _pc.add(
        [this](msg &&p_msg) -> std::optional<bool> {
          return m_consumer(std::move(p_msg));
        },
        std::chrono::milliseconds(1000), _timeout_callback);

    _pc.start();

    for (uint16_t _i = 0; _i < 300; ++_i) {
      msg _msg(_i);
      concurrent_debug(m_log, "adding msg ", _msg);
      _pc.add(std::move(_msg));
    }

    msg::number _amount_added = static_cast<msg::number>(_pc.amount_added());

    while ((m_consumer.m_msg.value() + 1) != _amount_added) {
      concurrent_debug(
          m_log, "amount added = ", _amount_added,
          ", m_consumer.m_msg.value() = ", m_consumer.m_msg.value());
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
      concurrent_debug(m_log, "still waiting...");
    }

    return true;
  }

private:
  struct consumer {
    std::optional<bool> operator()(msg &&p_msg) {
      m_msg = p_msg;
      concurrent_debug(m_log, "handling msg ", m_msg);
      return {true};
    }
    log m_log{"003::consumer"};
    msg m_msg;
  };

private:
  log m_log{"producer_consumer_003"};
  consumer m_consumer;
};

int main(int argc, char **argv) {
  typedef logger::cerr::log log;
  typedef tester::test test;
  log::set_debug();
  test _test(argc, argv);
  run_test(_test, producer_consumer_000);
  run_test(_test, producer_consumer_001);
  run_test(_test, producer_consumer_002);
  run_test(_test, producer_consumer_003);
}
