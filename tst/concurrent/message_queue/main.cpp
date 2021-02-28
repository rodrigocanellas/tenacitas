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

struct message_queue_003 {
  typedef logger::log log;
  typedef concurrent::msg_a msg;

  typedef concurrent::message_queue_t<
      log, concurrent::queue_type::CIRCULAR_FIXED_SIZE, msg>
      message_queue;
  typedef concurrent::on_timeout on_timeout;

  static std::string desc() {
    return "Produces 300 messages, and waits for all to be consumed";
  }

  bool operator()() {
    on_timeout _on_timeout = []() -> void { WAR(logger::log, "timeout!!"); };

    msg::number _last_added{0};

    {
      message_queue _msg_queue{40};

      _msg_queue.add(
          [this](msg &&p_msg) -> void { m_consumer(std::move(p_msg)); }, 1s,
          _on_timeout);

      _msg_queue.start();

      for (uint16_t _i = 0; _i < 300; ++_i) {
        msg _msg(_i);
        DEB(logger::log, "adding msg ", _msg);
        _msg_queue.add(std::move(_msg));
      }

      _last_added = static_cast<msg::number>(_msg_queue.amount_added() - 1);

      std::this_thread::sleep_for(15s);
    }

    DEB(logger::log, "amount added = ", _last_added,
        ", m_consumer.m_msg.value() = ", m_consumer.get_msg().value());

    return (_last_added == m_consumer.get_msg().value());
  }

private:
  struct consumer {
    void operator()(msg &&p_msg) {
      m_msg = p_msg;
      DEB(logger::log, "handling msg ", m_msg);
    }

    inline const msg &get_msg() const { return m_msg; }

  private:
    msg m_msg;
  };

private:
  consumer m_consumer;
};

struct message_queue_004 {
  typedef logger::log log;
  typedef concurrent::msg_a msg;

  typedef concurrent::message_queue_t<
      log, concurrent::queue_type::CIRCULAR_UNLIMITED_SIZE, msg>
      message_queue;
  typedef concurrent::on_timeout on_timeout;

  static std::string desc() {
    return "Produces 3000 messages, and waits for all to be consumed by 4 "
           "consumers";
  }

  bool operator()() {
    on_timeout _on_timeout_c1 = []() -> void {
      WAR(logger::log, "timeout c2!!");
    };
    on_timeout _on_timeout_c2 = []() -> void {
      WAR(logger::log, "timeout c2!!");
    };
    on_timeout _on_timeout_c3 = []() -> void {
      WAR(logger::log, "timeout c3!!");
    };
    on_timeout _on_timeout_c4 = []() -> void {
      WAR(logger::log, "timeout c4!!");
    };

    msg::number _last_added{0};

    consumer _c1{"c1"};
    consumer _c2{"c2"};
    consumer _c3{"c3"};
    consumer _c4{"c4"};

    {
      message_queue _msg_queue{40};

      _msg_queue.add([&_c1](msg &&p_msg) -> void { _c1(std::move(p_msg)); }, 2s,
                     _on_timeout_c1);

      _msg_queue.add([&_c2](msg &&p_msg) -> void { _c2(std::move(p_msg)); }, 2s,
                     _on_timeout_c2);

      _msg_queue.add([&_c3](msg &&p_msg) -> void { _c3(std::move(p_msg)); }, 2s,
                     _on_timeout_c3);

      _msg_queue.add([&_c4](msg &&p_msg) -> void { _c4(std::move(p_msg)); }, 2s,
                     _on_timeout_c4);

      _msg_queue.start();

      for (uint16_t _i = 0; _i < 3000; ++_i) {
        msg _msg(_i);
        DEB(logger::log, "adding msg ", _msg);
        _msg_queue.add(std::move(_msg));
      }

      _last_added = static_cast<msg::number>(_msg_queue.amount_added() - 1);

      std::this_thread::sleep_for(10s);
    }

    DEB(logger::log, "amount added = ", _last_added, ", _c1 = (",
        _c1.get_msg().value(), ",", _c1.get_num(), "), _c2 = (",
        _c2.get_msg().value(), ",", _c2.get_num(),
        "), _c3 = ", _c3.get_msg().value(), ",", _c3.get_num(), "), _c4 = (",
        _c4.get_msg().value(), ",", _c4.get_num(), ")");

    return (_last_added ==
            (_c1.get_num() + _c2.get_num()) + _c3.get_num() + _c4.get_num());
  }

private:
  struct consumer {
    consumer(const char *p_id) : m_id(p_id) {}
    void operator()(msg &&p_msg) {
      m_msg = p_msg;
      ++m_num;
      DEB(logger::log, m_id, " handling msg ", m_msg, ", total = ", m_num);
    }

    inline const msg &get_msg() const { return m_msg; }
    inline msg::number get_num() const { return m_num; }

  private:
    std::string m_id;
    msg m_msg;
    msg::number m_num{0};
  };
};

int main(int argc, char **argv) {
  logger::log::set_debug_level();
  logger::log::use_cerr();
  tester::test<> _test(argc, argv);
  run_test(_test, message_queue_000);
  run_test(_test, message_queue_001);
  run_test(_test, message_queue_002);
  run_test(_test, message_queue_003);
  run_test(_test, message_queue_004);
}
