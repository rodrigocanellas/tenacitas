/// \example Basic use of concurrent::loop_t

#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>

#include <concurrent/msg_a.h>
#include <tenacitas/concurrent.h>
#include <tenacitas/logger.h>
#include <tenacitas/tester.h>

using namespace tenacitas;
using namespace std::chrono_literals;

struct message_queue_000 {
  typedef logger::log log;
  typedef int16_t data;
  typedef concurrent::message_queue_t<
      log, concurrent::queue_type::CIRCULAR_UNLIMITED_SIZE, data>
      message_queue;
  typedef concurrent::on_timeout on_timeout;

  static std::string desc() {
    return "Simple test, creating a worker, adding a single data, "
           "and starting";
  }

  bool operator()() {

    message_queue _msg_queue(10);

    on_timeout _on_timeout = []() -> void { WAR(logger::log, "timeout!!"); };

    _msg_queue.add(consumer(), 500ms, _on_timeout);

    DEB(logger::log, "capacity = ", _msg_queue.capacity(),
        ", occupied = ", _msg_queue.occupied());
    _msg_queue.start();
    _msg_queue.add(-8);

    DEB(logger::log, "capacity = ", _msg_queue.capacity(),
        ", occupied = ", _msg_queue.occupied());

    std::this_thread::sleep_for(1s);

    return true;
  }

private:
  struct consumer {
    void operator()(data &&p_value) { INF(logger::log, "value = ", p_value); }
  };
};

struct message_queue_001 {
  typedef logger::log log;
  typedef concurrent::msg_a msg;

  typedef concurrent::sleeping_loop_t<log, void> sleeping_loop;
  typedef concurrent::message_queue_t<
      log, concurrent::queue_type::CIRCULAR_FIXED_SIZE, msg>
      message_queue;
  typedef concurrent::on_timeout on_timeout;

  static std::string desc() {
    return "\nA 'sleeping_loop' sending a message, at each 500 ms, to a "
           "message queue, with one operation, that sleeps for 1 second, and "
           "using a 'circular_fixed_size_queue', with size 10."
           "\nThe main thread will sleep for 50 secs."
           "\nThe amount of data consumed must be equal to the provided";
  }

  bool operator()() {

    const std::chrono::seconds _sleep{10s};

    consumer _consumer;
    msg _msg(0);
    {

      message_queue _msg_queue(10);

      on_timeout _on_timeout = []() -> void { WAR(logger::log, "timeout!!"); };

      DEB(logger::log, "capacity = ", _msg_queue.capacity(),
          ", occupied = ", _msg_queue.occupied());

      producer _producer(&_msg_queue, &_msg);

      DEB(logger::log, "creating the sleeping_loop");
      sleeping_loop _loop(500ms, 1s, _producer, _on_timeout);

      DEB(logger::log, "adding consumer to the worker");
      _msg_queue.add(
          [&_consumer](msg &&p_msg) -> void {
            return _consumer(std::move(p_msg));
          },
          2s, _on_timeout);

      DEB(logger::log, "starting the producer loop");
      _loop.start();
      DEB(logger::log, "starting the consumer message queue");
      _msg_queue.start();

      DEB(logger::log, "sleeping for ", _sleep.count(), " secs");
      std::this_thread::sleep_for(_sleep);
      _loop.stop();
      DEB(logger::log, "waking up after ", _sleep.count(), " secs, and ",
          _msg.value(), " was the last value produced");
    }
    DEB(logger::log, "produced = ", _msg, ", consumed = ", _consumer.get_msg());
    if (_consumer.get_msg().value() != _msg.value()) {
      ERR(logger::log, "Data value consumed should be equal to provided");
      return false;
    }

    return true;
  }

  ~message_queue_001() {}

private:
  struct producer {
    producer(message_queue *p_msg_queue, msg *p_msg)
        : m_msg_queue(p_msg_queue), m_msg(p_msg) {}

    void operator()() {
      if (m_msg->value() > m_num_msgs) {
        return;
      }

      m_msg->inc();
      msg _msg(m_msg->value());
      DEB(logger::log, "going to add ", _msg);

      m_msg_queue->add(_msg);
      DEB(logger::log, "added msg ", _msg,
          "; capacity = ", m_msg_queue->capacity(),
          ", occupied = ", m_msg_queue->occupied());
    }
    const uint16_t m_num_msgs = 50;

  private:
    message_queue *m_msg_queue;
    msg *m_msg;
  };

  struct consumer {
    void operator()(msg &&p_msg) {
      DEB(logger::log, "handling msg ", p_msg);
      m_msg = std::move(p_msg);

      DEB(logger::log, "handling msg ", m_msg);

      DEB(logger::log, "worker is going to sleep");
      std::this_thread::sleep_for(1s);
      DEB(logger::log, "worker is waking up");
    }
    const msg &get_msg() const { return m_msg; }

  private:
    msg m_msg;
  };
};

struct message_queue_002 {
  typedef logger::log log;
  typedef concurrent::msg_a msg;

  typedef concurrent::sleeping_loop_t<log, void> sleeping_loop;
  typedef concurrent::message_queue_t<
      log, concurrent::queue_type::CIRCULAR_FIXED_SIZE, msg>
      message_queue;
  typedef concurrent::on_timeout on_timeout;

  static std::string desc() {
    std::stringstream _stream;
    _stream
        << "\nA 'sleeping_loop' sending a message, at each 500 ms, to "
        << "a 'worker' with one operation, using a "
        << "'circular_fixed_size_queue', with size 40"

        << "\nThe main thread will sleep for 10 secs, the 'worker' "
        << "will stop; main thread will sleep for 5 secs; 'worker' "
        << "will run again; main thread will sleep for 4 secs."

        << "\nThe messages added while the queue was stopped are handled when "
        << "the pool runs again."

        << "\nThe amount of data consumed must be equal to the provided";
    return _stream.str();
  }

  bool operator()() {

    msg _msg(0);

    consumer _consumer;

    {
      message_queue _msg_queue(40);

      on_timeout _on_timeout = []() -> void { WAR(logger::log, "timeout!!"); };

      DEB(logger::log, "creating the sleeping_loop");

      sleeping_loop _loop(300ms, 500ms, producer{&_msg_queue, &_msg},
                          _on_timeout);

      DEB(logger::log, "adding consumer to the worker");
      _msg_queue.add(
          [&_consumer](msg &&p_msg) { return _consumer(std::move(p_msg)); },
          100ms, _on_timeout);

      _loop.start();
      _msg_queue.start();

      DEB(logger::log, "sleeping for 10 secs");
      std::this_thread::sleep_for(10s);
      DEB(logger::log, "waking up after 10 secs");

      DEB(logger::log, "stopping the message queue");
      _msg_queue.stop();

      DEB(logger::log, "sleeping for 5 secs");
      std::this_thread::sleep_for(5s);
      DEB(logger::log, "waking up after 5 secs");

      DEB(logger::log, "restarting the worker");
      _msg_queue.start();

      DEB(logger::log, "sleeping for 4 secs");
      std::this_thread::sleep_for(4s);
      DEB(logger::log, "waking up after 4 secs");

      INF(logger::log, _msg.value(), " was the last value produced");
    }

    INF(logger::log, "provided = ", _msg,
        ", consumed = ", _consumer.get_msg().value());
    if (_consumer.get_msg().value() != _msg.value()) {
      ERR(logger::log, "Data value consumed should be equal to provided");
      return false;
    }

    return true;
  }

  struct consumer {
    void operator()(msg &&p_msg) {
      m_msg = p_msg;
      DEB(logger::log, "handling msg ", m_msg);
    }

    inline const msg &get_msg() const { return m_msg; }

  private:
    msg m_msg;
  };

  struct producer {
    producer(message_queue *p_msg_queue, msg *p_data)
        : m_msg_queue(p_msg_queue), m_msg(p_data) {}

    void operator()() {
      m_msg->inc();
      msg _msg(m_msg->value());
      DEB(logger::log, "adding msg ", _msg);
      m_msg_queue->add(std::move(_msg));
    }

  private:
    message_queue *m_msg_queue;
    msg *m_msg;
  };
};

// struct message_queue_003 {
//  typedef logger::log log;
//  typedef concurrent::msg_a msg;

//  typedef concurrent::message_queue_t<
//      log, concurrent::queue_type::CIRCULAR_FIXED_SIZE, msg>
//      message_queue;
//  typedef concurrent::on_timeout on_timeout;

//  static std::string desc() {
//    return "\nProduces 300 messages, and waits for all to be consumed";
//  }

//  bool operator()() {
//    on_timeout _on_timeout = [this]() -> void { WAR(logger::log, "timeout!!");
//    };

//    message_queue _msg_queue{40};

//    _msg_queue.add(
//        [this](msg &&p_msg) -> std::optional<bool> {
//          return m_consumer(std::move(p_msg));
//        },
//        1s, _on_timeout);

//    _msg_queue.start();

//    for (uint16_t _i = 0; _i < 300; ++_i) {
//      msg _msg(_i);
//      DEB(logger::log, "adding msg ", _msg);
//      _msg_queue.add(std::move(_msg));
//    }

//    msg::number _amount_added =
//        static_cast<msg::number>(_msg_queue.amount_added());

//    while ((m_consumer.m_msg.value() + 1) != _amount_added) {
//      DEB(logger::log, "amount added = ", _amount_added,
//          ", m_consumer.m_msg.value() = ", m_consumer.m_msg.value());
//      std::this_thread::sleep_for(50ms);
//      DEB(logger::log, "still waiting...");
//    }

//    return true;
//  }

// private:
//  struct consumer {
//    std::optional<bool> operator()(msg &&p_msg) {
//      m_msg = p_msg;
//      DEB(logger::log, "handling msg ", m_msg);
//      return {true};
//    }
//    log logger::log{"003::consumer"};
//    msg m_msg;
//  };

// private:
//  log logger::log{"message_queue_003"};
//  consumer m_consumer;
//};

int main(int argc, char **argv) {
  logger::log::set_debug_level();
  logger::log::use_cerr();
  tester::test<> _test(argc, argv);
  run_test(_test, message_queue_000);
  run_test(_test, message_queue_001);
  run_test(_test, message_queue_002);
  //  run_test(_test, message_queue_003);
}
