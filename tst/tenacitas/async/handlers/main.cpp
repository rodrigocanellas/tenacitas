/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>

#include <tenacitas/async.h>
#include <tenacitas/logger.h>
#include <tenacitas/number.h>
#include <tenacitas/tester.h>
#include <tst/tenacitas/async/msg.h>

using namespace tenacitas;
using namespace tenacitas::async;
using namespace std::chrono_literals;

struct handlers_000 {
  typedef int16_t data;
  typedef async::internal::handlers_t<data, async::priority> handlers;

  static std::string desc() {
    return "Simple test, creating a worker, adding a single data, "
           "and starting";
  }

  bool operator()() {

    handlers _handlers(m_id, 500ms);

    _handlers.add_handler(consumer());

    DEB(m_log, "capacity = ", _handlers.capacity(),
        ", occupied = ", _handlers.occupied());

    _handlers.add_data(-8);

    DEB(m_log, "capacity = ", _handlers.capacity(),
        ", occupied = ", _handlers.occupied());

    std::this_thread::sleep_for(1s);

    return true;
  }

private:
  struct consumer {
    void operator()(std::shared_ptr<bool>, const data &p_value) {
      INF(m_log, "value = ", p_value);
    }

  private:
    logger::cerr<> m_log{"consumer"};
  };
  logger::cerr<> m_log{"handlers_000"};
  number::id m_id;
};

struct handlers_001 {
  typedef msg<'Z'> msg;

  typedef async::sleeping_loop_t<void> sleeping_loop;
  typedef async::internal::handlers_t<msg, async::priority> handlers;
  typedef std::function<void(const msg &)> on_timeout;

  static std::string desc() {
    return "\nA 'sleeping_loop' sending a message, at each 500 ms, to a "
           "message queue, with one operation, that sleeps for 1 second, and "
           "using a 'circular_fixed_size_queue', with size 10."
           "\nThe main thread will sleep for 50 secs."
           "\nThe amount of data consumed must be equal to the provided";
  }

  bool operator()() {

    const std::chrono::seconds _sleep{5s};

    consumer _consumer;
    msg _msg(0);
    {

      handlers _handlers(m_id, 2s);

      DEB(m_log, "capacity = ", _handlers.capacity(),
          ", occupied = ", _handlers.occupied());

      producer _producer(&_handlers, &_msg);

      DEB(m_log, "creating the sleeping_loop");
      sleeping_loop _loop(m_id, _producer, 500ms, 1s);

      DEB(m_log, "adding consumer to the worker");
      _handlers.add_handler(
          [&_consumer](std::shared_ptr<bool>, const msg &p_msg) -> void {
            return _consumer(p_msg);
          });

      DEB(m_log, "starting the producer loop");
      _loop.start();
      //      DEB(m_log, "starting the consumer message queue");
      //      _handlers.start();

      DEB(m_log, "sleeping for ", _sleep.count(), " secs");
      std::this_thread::sleep_for(_sleep);
      _loop.stop();
      DEB(m_log, "waking up after ", _sleep.count(), " secs, and ",
          _msg.get_value(), " was the last value produced");
    }
    DEB(m_log, "produced = ", _msg.get_value(),
        ", consumed = ", _consumer.get_msg().get_value());
    if (_consumer.get_msg().get_value() != _msg.get_value()) {
      ERR(m_log, "Data value consumed should be equal to provided");
      return false;
    }

    return true;
  }

private:
  struct producer {
    producer(handlers *p_handlers, msg *p_msg)
        : m_handlers(p_handlers), m_msg(p_msg) {}

    void operator()(std::shared_ptr<bool>) {
      if (m_msg->get_value() > m_num_msgs) {
        return;
      }

      ++(*m_msg);
      msg _msg(m_msg->get_value());
      DEB(m_log, "going to add ", _msg);

      m_handlers->add_data(_msg);
      DEB(m_log, "added msg ", _msg, "; capacity = ", m_handlers->capacity(),
          ", occupied = ", m_handlers->occupied());
    }
    const uint16_t m_num_msgs = 50;

  private:
    handlers *m_handlers;
    msg *m_msg;
    logger::cerr<> m_log{"producer"};
  };

  struct consumer {
    void operator()(const msg &p_msg) {
      m_msg = p_msg;

      DEB(m_log, "handling msg ", m_msg);

      DEB(m_log, "worker is going to sleep");
      std::this_thread::sleep_for(1s);
      DEB(m_log, "worker is waking up");
    }
    const msg &get_msg() const { return m_msg; }

  private:
    msg m_msg;
    logger::cerr<> m_log{"consumer"};
  };
  logger::cerr<> m_log{"handlers_001"};
  number::id m_id;
};

struct handlers_003 {

  typedef msg<'Z'> msg;

  typedef async::internal::handlers_t<msg, async::priority> handlers;
  typedef std::function<void(const msg &)> on_timeout;

  static std::string desc() {
    return "Produces 300 messages, and waits for all to be consumed";
  }

  bool operator()() {

    value _last_added{0};

    {
      handlers _handlers{m_id, 1s};

      _handlers.add_handler(
          [this](std::shared_ptr<bool> p_bool, const msg &p_msg) -> void {
            m_consumer(p_bool, p_msg);
          });

      for (uint16_t _i = 0; _i < 300; ++_i) {
        msg _msg(_i);
        DEB(m_log, "adding msg ", _msg);
        _handlers.add_data(_msg);
      }

      _last_added = static_cast<value>(_handlers.amount_added() - 1);

      std::this_thread::sleep_for(15s);
    }

    DEB(m_log, "amount added = ", _last_added,
        ", m_consumer.m_msg.value() = ", m_consumer.get_msg().get_value());

    return (_last_added == m_consumer.get_msg().get_value());
  }

private:
  struct consumer {
    void operator()(std::shared_ptr<bool>, const msg &p_msg) {
      m_msg = p_msg;
      DEB(m_log, "handling msg ", m_msg);
    }

    inline const msg &get_msg() const { return m_msg; }

  private:
    msg m_msg;
    logger::cerr<> m_log{"consumer"};
  };

private:
  consumer m_consumer;
  logger::cerr<> m_log{"handlers_003"};
  number::id m_id;
};

struct handlers_004 {

  typedef msg<'Z'> msg;

  typedef async::internal::handlers_t<msg, async::priority> handlers;
  typedef std::function<void(const msg &)> on_timeout;

  static std::string desc() {
    return "Produces 3000 messages, and waits for all to be consumed by 5 "
           "consumers";
  }

  bool operator()() {

    value _amount_added{0};
    const value _amount_to_add{3000};

    std::vector<consumer> _consumers{{"c1"}, {"c2"}, {"c3"}, {"c4"}, {"c5"}};

    {
      handlers _handlers{m_id, 2s};

      for (consumer &_consumer : _consumers) {
        _handlers.add_handler(
            [&_consumer](std::shared_ptr<bool>, const msg &p_msg) -> void {
              _consumer(p_msg);
            });
      }

      for (uint16_t _i = 0; _i < _amount_to_add; ++_i) {
        msg _msg(_i);
        DEB(m_log, "adding msg ", _msg);
        _handlers.add_data(_msg);
      }

      _amount_added = static_cast<value>(_handlers.amount_added());

      while (_handlers.occupied() != 0) {
        DEB(m_log, "msg queue still not empty");
        std::this_thread::sleep_for(50ms);
      }
    }

    value _consumed{0};
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
    void operator()(const msg &p_msg) {
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
    inline value get_num() const { return m_num; }

  private:
    std::string m_id;
    msg m_msg;
    value m_num{0};
    logger::cerr<> m_log{"consumer"};
  };
  logger::cerr<> m_log{"handlers_004"};
  number::id m_id;
};

struct handlers_005 {

  typedef msg<'Z'> msg;

  typedef async::internal::handlers_t<msg, async::priority> handlers;
  typedef std::function<void(const msg &)> on_timeout;

  static std::string desc() {
    std::stringstream _stream;
    _stream << "Produces 50 messages, and waits for all to be consumed by "
            << "5 consumers.\n"
            << "From 8 to 8 messages, every consumer will cause a timeout, "
            << "the message will be added again to the pool, and will "
            << "eventually be handled, may be by another worker.\n"
            << "As a consequence, they will appear in the log in different a "
            << "position from the sequencial.";

    return _stream.str();
  }

  bool operator()() {

    m_log.set_debug_level();

    value _amount_added{0};
    const value _amount_to_add{50};

    std::vector<consumer> _consumers;
    _consumers.push_back({"c1", 200ms});
    _consumers.push_back({"c2", 200ms});
    _consumers.push_back({"c3", 200ms});
    _consumers.push_back({"c4", 200ms});
    _consumers.push_back({"c5", 200ms});

    uint16_t _num_timeouts{0};
    {
      handlers _handlers{m_id, 200ms};

      for (consumer &_consumer : _consumers) {
        _handlers.add_handler(
            [&_consumer](std::shared_ptr<bool>, const msg &p_msg) -> void {
              _consumer(p_msg);
            });
      }

      for (uint16_t _i = 0; _i < _amount_to_add; ++_i) {
        msg _msg(_i);
        DEB(m_log, "adding msg ", _msg);
        _handlers.add_data(_msg);
      }

      _amount_added =
          static_cast<value>(_handlers.amount_added()) - _num_timeouts;

      std::this_thread::sleep_for(3s);

      while (_handlers.occupied() != 0) {
        DEB(m_log, "msg queue still not empty");
        std::this_thread::sleep_for(50ms);
      }
    }

    value _consumed{0};
    {
      std::stringstream _stream;
      for (const consumer &_consumer : _consumers) {
        _stream << _consumer << " ";
        _consumed += _consumer.get_num();
        _num_timeouts += _consumer.get_num_timeouts();
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

    void operator()(const msg &p_msg) {
      m_log.set_debug_level();
      if (((m_count % 8) == 0) && (m_timeout_counter < 5)) {
        m_count = 1;
        ++m_timeout_counter;
        WAR(m_log, "causing timeout for ", m_id, " when msg = ", p_msg,
            ", count = ", m_count, ", timeout counter = ", m_timeout_counter);
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
    inline value get_num() const { return m_num; }
    std::chrono::milliseconds get_timeout() const { return m_timeout; }
    uint16_t get_num_timeouts() const { return m_timeout_counter; }

  private:
    std::string m_id;
    std::chrono::milliseconds m_timeout;
    std::chrono::milliseconds m_over_timeout;
    msg m_msg;

    value m_num{0};
    value m_count{1};
    uint16_t m_timeout_counter{0};
    logger::cerr<> m_log{"consumer"};
  };

  struct timeout {
    timeout(const char *p_id) : m_id(p_id) {}

    void operator()() { WAR(m_log, "timeout for ", m_id); }

  private:
    std::string m_id;
    logger::cerr<> m_log{"timeout"};
  };
  logger::cerr<> m_log{"handlers_005"};
  number::id m_id;
};

int main(int argc, char **argv) {
  logger::set_debug_level();
  tester::test<> _test(argc, argv);
  run_test(_test, handlers_000);
  run_test(_test, handlers_001);
  run_test(_test, handlers_003);
  run_test(_test, handlers_004);
  run_test(_test, handlers_005);
}
