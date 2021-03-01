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
  typedef int16_t data;
  typedef concurrent::message_queue_t<
      logger::cerr<>, concurrent::queue_type::CIRCULAR_UNLIMITED_SIZE, data>
      message_queue;
  typedef std::function<void(data &&)> on_timeout;

  static std::string desc() {
    return "Simple test, creating a worker, adding a single data, "
           "and starting";
  }

  bool operator()() {

    message_queue _msg_queue(10);

    on_timeout _on_timeout = [this](data &&p_data) -> void {
      WAR(m_log, "timeout handlind ", p_data);
    };

    _msg_queue.add(consumer(), 500ms, _on_timeout);

    DEB(m_log, "capacity = ", _msg_queue.capacity(),
        ", occupied = ", _msg_queue.occupied());
    _msg_queue.start();
    _msg_queue.add(-8);

    DEB(m_log, "capacity = ", _msg_queue.capacity(),
        ", occupied = ", _msg_queue.occupied());

    std::this_thread::sleep_for(1s);

    return true;
  }

private:
  struct consumer {
    void operator()(data &&p_value) { INF(m_log, "value = ", p_value); }

  private:
    logger::cerr<> m_log{"consumer"};
  };
  logger::cerr<> m_log{"message_queue_000"};
};

struct message_queue_001 {
  typedef logger::cerr<> log;
  typedef concurrent::msg_a msg;

  typedef concurrent::sleeping_loop_t<log, void> sleeping_loop;
  typedef concurrent::message_queue_t<
      log, concurrent::queue_type::CIRCULAR_FIXED_SIZE, msg>
      message_queue;
  typedef std::function<void(msg &&)> on_timeout;

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

      on_timeout _on_timeout = [this](msg &&p_msg) -> void {
        WAR(m_log, "timeout handling ", p_msg);
      };

      DEB(m_log, "capacity = ", _msg_queue.capacity(),
          ", occupied = ", _msg_queue.occupied());

      producer _producer(&_msg_queue, &_msg);

      DEB(m_log, "creating the sleeping_loop");
      sleeping_loop _loop(500ms, 1s, _producer, []() -> void {});

      DEB(m_log, "adding consumer to the worker");
      _msg_queue.add(
          [&_consumer](msg &&p_msg) -> void {
            return _consumer(std::move(p_msg));
          },
          2s, _on_timeout);

      DEB(m_log, "starting the producer loop");
      _loop.start();
      DEB(m_log, "starting the consumer message queue");
      _msg_queue.start();

      DEB(m_log, "sleeping for ", _sleep.count(), " secs");
      std::this_thread::sleep_for(_sleep);
      _loop.stop();
      DEB(m_log, "waking up after ", _sleep.count(), " secs, and ",
          _msg.value(), " was the last value produced");
    }
    DEB(m_log, "produced = ", _msg, ", consumed = ", _consumer.get_msg());
    if (_consumer.get_msg().value() != _msg.value()) {
      ERR(m_log, "Data value consumed should be equal to provided");
      return false;
    }

    return true;
  }

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
      DEB(m_log, "going to add ", _msg);

      m_msg_queue->add(_msg);
      DEB(m_log, "added msg ", _msg, "; capacity = ", m_msg_queue->capacity(),
          ", occupied = ", m_msg_queue->occupied());
    }
    const uint16_t m_num_msgs = 50;

  private:
    message_queue *m_msg_queue;
    msg *m_msg;
    log m_log{"producer"};
  };

  struct consumer {
    void operator()(msg &&p_msg) {
      DEB(m_log, "handling msg ", p_msg);
      m_msg = std::move(p_msg);

      DEB(m_log, "handling msg ", m_msg);

      DEB(m_log, "worker is going to sleep");
      std::this_thread::sleep_for(1s);
      DEB(m_log, "worker is waking up");
    }
    const msg &get_msg() const { return m_msg; }

  private:
    msg m_msg;
    log m_log{"consumer"};
  };
  log m_log{"message_queue_001"};
};

struct message_queue_002 {
  typedef logger::cerr<> log;
  typedef concurrent::msg_a msg;

  typedef concurrent::sleeping_loop_t<log, void> sleeping_loop;
  typedef concurrent::message_queue_t<
      log, concurrent::queue_type::CIRCULAR_FIXED_SIZE, msg>
      message_queue;
  typedef std::function<void(msg &&)> on_timeout;

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

      on_timeout _on_timeout = [this](msg &&p_msg) -> void {
        WAR(m_log, "timeout handling ", p_msg);
      };

      DEB(m_log, "creating the sleeping_loop");

      sleeping_loop _loop(300ms, 500ms, producer{&_msg_queue, &_msg},
                          []() -> void {});

      DEB(m_log, "adding consumer to the worker");
      _msg_queue.add(
          [&_consumer](msg &&p_msg) { return _consumer(std::move(p_msg)); },
          100ms, _on_timeout);

      _loop.start();
      _msg_queue.start();

      DEB(m_log, "sleeping for 10 secs");
      std::this_thread::sleep_for(10s);
      DEB(m_log, "waking up after 10 secs");

      DEB(m_log, "stopping the message queue");
      _msg_queue.stop();

      DEB(m_log, "sleeping for 5 secs");
      std::this_thread::sleep_for(5s);
      DEB(m_log, "waking up after 5 secs");

      DEB(m_log, "restarting the worker");
      _msg_queue.start();

      DEB(m_log, "sleeping for 4 secs");
      std::this_thread::sleep_for(4s);
      DEB(m_log, "waking up after 4 secs");

      INF(m_log, _msg.value(), " was the last value produced");
    }

    INF(m_log, "provided = ", _msg,
        ", consumed = ", _consumer.get_msg().value());
    if (_consumer.get_msg().value() != _msg.value()) {
      ERR(m_log, "Data value consumed should be equal to provided");
      return false;
    }

    return true;
  }

  struct consumer {
    void operator()(msg &&p_msg) {
      m_msg = p_msg;
      DEB(m_log, "handling msg ", m_msg);
    }

    inline const msg &get_msg() const { return m_msg; }

  private:
    msg m_msg;
    log m_log{"consumer"};
  };

  struct producer {
    producer(message_queue *p_msg_queue, msg *p_data)
        : m_msg_queue(p_msg_queue), m_msg(p_data) {}

    void operator()() {
      m_msg->inc();
      msg _msg(m_msg->value());
      DEB(m_log, "adding msg ", _msg);
      m_msg_queue->add(std::move(_msg));
    }

  private:
    message_queue *m_msg_queue;
    msg *m_msg;
    log m_log{"producer"};
  };
  log m_log{"message_queue_001"};
};

struct message_queue_003 {
  typedef logger::cerr<> log;
  typedef concurrent::msg_a msg;

  typedef concurrent::message_queue_t<
      log, concurrent::queue_type::CIRCULAR_FIXED_SIZE, msg>
      message_queue;
  typedef std::function<void(msg &&)> on_timeout;

  static std::string desc() {
    return "Produces 300 messages, and waits for all to be consumed";
  }

  bool operator()() {
    on_timeout _on_timeout = [this](msg &&p_msg) -> void {
      WAR(m_log, "timeout handling ", p_msg);
    };

    msg::number _last_added{0};

    {
      message_queue _msg_queue{40};

      _msg_queue.add(
          [this](msg &&p_msg) -> void { m_consumer(std::move(p_msg)); }, 1s,
          _on_timeout);

      _msg_queue.start();

      for (uint16_t _i = 0; _i < 300; ++_i) {
        msg _msg(_i);
        DEB(m_log, "adding msg ", _msg);
        _msg_queue.add(std::move(_msg));
      }

      _last_added = static_cast<msg::number>(_msg_queue.amount_added() - 1);

      std::this_thread::sleep_for(15s);
    }

    DEB(m_log, "amount added = ", _last_added,
        ", m_consumer.m_msg.value() = ", m_consumer.get_msg().value());

    return (_last_added == m_consumer.get_msg().value());
  }

private:
  struct consumer {
    void operator()(msg &&p_msg) {
      m_msg = p_msg;
      DEB(m_log, "handling msg ", m_msg);
    }

    inline const msg &get_msg() const { return m_msg; }

  private:
    msg m_msg;
    log m_log{"consumer"};
  };

private:
  consumer m_consumer;
  log m_log{"message_queue_003"};
};

struct message_queue_004 {
  typedef logger::cerr<> log;
  typedef concurrent::msg_a msg;

  typedef concurrent::message_queue_t<
      log, concurrent::queue_type::CIRCULAR_UNLIMITED_SIZE, msg>
      message_queue;
  typedef std::function<void(msg &&)> on_timeout;

  static std::string desc() {
    return "Produces 3000 messages, and waits for all to be consumed by 5 "
           "consumers";
  }

  bool operator()() {

    msg::number _amount_added{0};

    std::vector<consumer> _consumers;
    _consumers.push_back({"c1"});
    _consumers.push_back({"c2"});
    _consumers.push_back({"c3"});
    _consumers.push_back({"c4"});
    _consumers.push_back({"c5"});

    {
      message_queue _msg_queue{40};

      on_timeout _on_timeout = [this, &_msg_queue](msg &&p_msg) -> void {
        WAR(m_log, "timeout hadling ", p_msg);
        _msg_queue.add(std::move(p_msg));
      };

      for (consumer &_consumer : _consumers) {
        _msg_queue.add(
            [&_consumer](msg &&p_msg) -> void { _consumer(std::move(p_msg)); },
            2s, _on_timeout);
      }

      _msg_queue.start();

      for (uint16_t _i = 0; _i < 3000; ++_i) {
        msg _msg(_i);
        DEB(m_log, "adding msg ", _msg);
        _msg_queue.add(std::move(_msg));
      }

      _amount_added = static_cast<msg::number>(_msg_queue.amount_added());

      while (_msg_queue.occupied() != 0) {
        DEB(m_log, "msg queue still not empty");
        std::this_thread::sleep_for(50ms);
      }
    }

    msg::number _consumed{0};
    {
      std::stringstream _stream;
      for (const consumer &_consumer : _consumers) {
        _stream << _consumer << " ";
        _consumed += _consumer.get_num();
      }

      DEB(m_log, "consumed: ", _stream.str(), " - total = ", _consumed);
    }

    return (_amount_added == _consumed);
  }

private:
  struct consumer {
    consumer(const char *p_id) : m_id(p_id) {}
    void operator()(msg &&p_msg) {
      m_msg = p_msg;
      ++m_num;
      DEB(m_log, "handling msg|", m_id, "|", m_msg, "|", m_num);
    }

    friend std::ostream &operator<<(std::ostream &p_out,
                                    const consumer &p_consumer) {
      p_out << "(" << p_consumer.get_id() << "," << p_consumer.get_msg() << ","
            << p_consumer.get_num() << ")";
      return p_out;
    }

    const std::string &get_id() const { return m_id; }
    inline const msg &get_msg() const { return m_msg; }
    inline msg::number get_num() const { return m_num; }

  private:
    std::string m_id;
    msg m_msg;
    msg::number m_num{0};
    log m_log{"consumer"};
  };
  log m_log{"message_queue_004"};
};

struct message_queue_005 {
  typedef logger::cerr<> log;
  typedef concurrent::msg_a msg;

  typedef concurrent::message_queue_t<
      log, concurrent::queue_type::CIRCULAR_UNLIMITED_SIZE, msg>
      message_queue;
  typedef std::function<void(msg &&)> on_timeout;

  static std::string desc() {
    std::stringstream _stream;
    _stream << "Produces 3000 messages, and waits for all to be consumed by 5 "
            << "consumers.\n"
            << "From 10 to 10 messages, every consumer will cause a timeout";

    return _stream.str();
  }

  bool operator()() {

    m_log.set_debug_level();

    msg::number _amount_added{0};

    std::vector<consumer> _consumers;
    _consumers.push_back({"c1", 100ms});
    _consumers.push_back({"c2", 200ms});
    _consumers.push_back({"c3", 150ms});
    _consumers.push_back({"c4", 2000ms});
    _consumers.push_back({"c5", 100ms});

    uint16_t _num_timeouts{0};
    {
      message_queue _msg_queue{40};

      on_timeout _on_timeout = [this, &_msg_queue,
                                &_num_timeouts](msg &&p_msg) -> void {
        WAR(m_log, "timeout handling ", p_msg);
        ++_num_timeouts;
        _msg_queue.add(p_msg);
      };

      for (consumer &_consumer : _consumers) {
        _msg_queue.add(
            [&_consumer](msg &&p_msg) -> void { _consumer(std::move(p_msg)); },
            _consumer.get_timeout(), _on_timeout);
      }

      _msg_queue.start();

      for (uint16_t _i = 0; _i < 5000; ++_i) {
        msg _msg(_i);
        DEB(m_log, "adding msg ", _msg);
        _msg_queue.add(_msg);
      }

      _amount_added =
          static_cast<msg::number>(_msg_queue.amount_added()) - _num_timeouts;

      std::this_thread::sleep_for(3s);

      while (_msg_queue.occupied() != 0) {
        DEB(m_log, "msg queue still not empty");
        std::this_thread::sleep_for(50ms);
      }
    }

    msg::number _consumed{0};
    {
      std::stringstream _stream;
      for (const consumer &_consumer : _consumers) {
        _stream << _consumer << " ";
        _consumed += _consumer.get_num();
      }

      DEB(m_log, "consumed: ", _stream.str(), " - total = ", _consumed);
    }

    DEB(m_log, "amount added = ", _amount_added,
        ", num timeouts = ", _num_timeouts);

    return (_amount_added == _consumed);
  }

private:
  struct consumer {
    consumer(const char *p_id, std::chrono::milliseconds p_timeout)
        : m_id(p_id), m_timeout(p_timeout),
          m_over_timeout(p_timeout.count() * 2) {}

    void operator()(msg &&p_msg) {
      m_log.set_debug_level();
      if (((m_count % 8) == 0) && (m_timeout_counter < 5)) {
        m_count = 1;
        ++m_timeout_counter;
        WAR(m_log, "causing timeout for ", m_id, " when msg = ", p_msg,
            ", count = ", m_count, ", timeour counter = ", m_timeout_counter);
        std::this_thread::sleep_for(m_over_timeout);
      } else {
        m_msg = p_msg;
        ++m_num;
        ++m_count;
        DEB(m_log, "handling msg|", m_id, "|", m_msg, "|", m_num);
      }
    }

    friend std::ostream &operator<<(std::ostream &p_out,
                                    const consumer &p_consumer) {
      p_out << "(" << p_consumer.get_id() << "," << p_consumer.get_msg() << ","
            << p_consumer.get_num() << ")";
      return p_out;
    }

    const std::string &get_id() const { return m_id; }
    inline const msg &get_msg() const { return m_msg; }
    inline msg::number get_num() const { return m_num; }
    std::chrono::milliseconds get_timeout() const { return m_timeout; }

  private:
    std::string m_id;
    std::chrono::milliseconds m_timeout;
    std::chrono::milliseconds m_over_timeout;
    msg m_msg;

    msg::number m_num{0};
    msg::number m_count{1};
    uint16_t m_timeout_counter{0};
    log m_log{"consumer"};
  };

  struct timeout {
    timeout(const char *p_id) : m_id(p_id) {}

    void operator()() { WAR(m_log, "timeout for ", m_id); }

  private:
    std::string m_id;
    logger::cerr<> m_log{"timeout"};
  };
  log m_log{"message_queue_005"};
};

int main(int argc, char **argv) {
  logger::set_debug_level();
  tester::test<> _test(argc, argv);
  run_test(_test, message_queue_000);
  run_test(_test, message_queue_001);
  run_test(_test, message_queue_002);
  run_test(_test, message_queue_003);
  run_test(_test, message_queue_004);
  run_test(_test, message_queue_005);
}
