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
  typedef async::internal::handlers_t<data> handlers;

  static std::string desc() {
    return "Simple test, creating a worker, adding a single data, "
           "and starting";
  }

  bool operator()() {
    logger::set_debug_level();

    DEB(m_log, "starting");

    handlers _handlers(m_id, 500ms);

    DEB(m_log, "adding handler");

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
    void operator()(std::shared_ptr<bool>, data &&p_value) {
      INF(m_log, "value = ", p_value);
    }

  private:
    logger::cerr<> m_log{"consumer"};
  };

  logger::cerr<> m_log{"handlers_000"};
  number::id m_id;
};

struct handlers_001 {
  typedef msg_t<'A'> msg;

  typedef async::sleeping_loop sleeping_loop;
  typedef async::internal::handlers_t<msg> handlers;
  typedef std::function<void(const msg &)> on_timeout;

  static std::string desc() {
    return "\nA 'sleeping_loop' sends 51 messages, at each 500 ms, to a "
           "'handlers', with timeout of 2s, with one handler that sleeps for 1 "
           "second."
           "\nThe amount of messages consumed must be equal to the produced";
  }

  bool operator()() {

    logger::set_debug_level();

    consumer _consumer;
    msg _msg(0);
    {

      handlers _handlers(m_id, 2s);

      DEB(m_log, "capacity = ", _handlers.capacity(),
          ", occupied = ", _handlers.occupied());

      producer _producer(&_handlers, &m_cond, &_msg);

      DEB(m_log, "creating the sleeping_loop");
      sleeping_loop _loop(m_id, _producer, 500ms, 1s);

      DEB(m_log, "adding consumer to the worker");
      _handlers.add_handler(
          [&_consumer](std::shared_ptr<bool> p_bool, msg &&p_msg) -> void {
            _consumer(p_bool, std::move(p_msg));
          });

      DEB(m_log, "starting the producer loop");
      _loop.start();

      {
        std::unique_lock<std::mutex> _lock(m_mutex);
        m_cond.wait(_lock);
      }

      DEB(m_log, "stoping the producer loop");
      _loop.stop();
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
    producer(handlers *p_handlers, std::condition_variable *p_cond, msg *p_msg)
        : m_handlers(p_handlers), m_cond(p_cond), m_msg(p_msg) {}

    void operator()(std::shared_ptr<bool>) {
      if (m_msg->get_value() > m_num_msgs) {
        m_cond->notify_one();
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
    std::condition_variable *m_cond;
    msg *m_msg;

    logger::cerr<> m_log{"producer"};
  };

  struct consumer {
    void operator()(std::shared_ptr<bool>, msg &&p_msg) {
      m_msg = std::move(p_msg);

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
  std::condition_variable m_cond;
  std::mutex m_mutex;
};

struct handlers_002 {

  typedef msg_t<'B'> msg;
  typedef async::internal::handlers_t<msg> handlers;
  typedef async::sleeping_loop sleeping_loop;

  static std::string desc() {
    return "Produces 300 messages, and waits for all to be consumed";
  }

  bool operator()() {

    logger::cerr<> _log{"handlers_002"};
    //    _log.set_debug_level();
    logger::set_debug_level();

    number::id _id;
    std::condition_variable _cond_producer;
    std::condition_variable _cond_consumer;

    const value _max{300};

    handlers _handlers{_id, 2s};

    consumer _consumer(_max, &_cond_consumer);
    producer _producer(_max, &_cond_producer, &_handlers);

    _handlers.add_handler(
        [&_consumer](std::shared_ptr<bool> p_bool, msg &&p_msg) -> void {
          _consumer(p_bool, std::move(p_msg));
        });

    sleeping_loop _sleeping_loop(_id, _producer, 1s, 100ms);

    _sleeping_loop.start();

    {
      std::mutex _mutex_producer;
      std::unique_lock<std::mutex> _lock(_mutex_producer);
      _cond_producer.wait(_lock);
    }

    _sleeping_loop.stop();

    INF(_log, "queue size = ", _handlers.amount_added());

    {
      std::mutex _mutex_consumer;
      std::unique_lock<std::mutex> _lock(_mutex_consumer);
      _cond_consumer.wait(_lock);
    }

    INF(_log, "amount added = ", _producer.last(),
        ", m_consumer.m_msg.value() = ", _consumer.last());

    return (_producer.last() == _consumer.last());
  }

private:
  struct consumer {
    consumer(uint16_t p_max, std::condition_variable *p_cond)
        : m_max(p_max), m_cond(p_cond) {
      m_log.set_debug_level();
    }

    void operator()(std::shared_ptr<bool>, msg &&p_msg) {
      std::this_thread::sleep_for(500ms);
      m_value = p_msg.get_value();
      DEB(m_log, "handling msg ", p_msg);
      if (m_value == m_max) {
        DEB(m_log, "stoping consuming");
        m_cond->notify_one();
        return;
      }
    }

    inline const value &last() const { return m_value; }

  private:
    uint16_t m_max;
    std::condition_variable *m_cond;
    value m_value;

    logger::cerr<> m_log{"consumer"};
  };

  struct producer {

    producer(uint16_t p_max, std::condition_variable *p_cond,
             handlers *p_handlers)
        : m_max(p_max), m_cond(p_cond), m_handlers(p_handlers) {
      m_log.set_debug_level();
    }

    void operator()(std::shared_ptr<bool>) {
      if (m_value == m_max) {
        DEB(m_log, "stoping production");
        m_cond->notify_one();
        return;
      }
      msg _msg(++m_value);
      DEB(m_log, "producing msg ", _msg);
      m_handlers->add_data(_msg);
    }

    value last() const { return m_value; }

  private:
    uint16_t m_max;
    std::condition_variable *m_cond;
    handlers *m_handlers;
    value m_value{0};

    logger::cerr<> m_log{"producer"};
  };
};

struct handlers_003 {

  typedef msg_t<'C'> msg;

  typedef async::internal::handlers_t<msg> handlers;
  typedef async::sleeping_loop sleeping_loop;

  static std::string desc() {
    return "Produces 3000 messages, and waits for all to be consumed by 5 "
           "consumers";
  }

  bool operator()() {
    logger::set_debug_level();
    logger::cerr<> _log{"handlers_003"};
    _log.set_debug_level();
    number::id _id;

    DEB(_log, "id = ", _id);

    const value _max{100};

    std::condition_variable _cond_producer;

    //    //    std::vector<consumer> _consumers{{"c1"}, {"c2"}, {"c3"}, {"c4"},
    //    //    {"c5"}};
    //    std::vector<consumer> _consumers{{"c1"}, {"c2"}};

    handlers _handlers{_id, 2s};

    producer _producer(_max, &_cond_producer, &_handlers);

    //    for (consumer &_consumer : _consumers) {
    //      DEB(_log, "adding consumer ", _consumer.get_id());
    //      _handlers.add_handler(
    //          [&_consumer](std::shared_ptr<bool> p_bool, msg &&p_msg) -> void
    //          {
    //            _consumer(p_bool, std::move(p_msg));
    //          });
    //    }

    consumer _c1{"c1"};
    _handlers.add_handler(
        [&_c1](std::shared_ptr<bool> p_bool, msg &&p_msg) -> void {
          _c1(p_bool, std::move(p_msg));
        });

    consumer _c2{"c2"};
    _handlers.add_handler(
        [&_c2](std::shared_ptr<bool> p_bool, msg &&p_msg) -> void {
          _c2(p_bool, std::move(p_msg));
        });

    sleeping_loop _sleeping_loop(
        _id,
        [&_producer](std::shared_ptr<bool> p_bool) -> void {
          _producer(p_bool);
        },
        1s, 100ms);

    _sleeping_loop.start();

    {
      std::mutex _mutex_producer;
      std::unique_lock<std::mutex> _lock(_mutex_producer);
      _cond_producer.wait(_lock);
    }

    _sleeping_loop.stop();

    while (_handlers.occupied() != 0) {
      DEB(_log, "msg queue still not empty");
      std::this_thread::sleep_for(50ms);
    }

    value _consumed{0};
    //    {
    //      std::stringstream _stream;
    //      for (const consumer &_consumer : _consumers) {
    //        _stream << _consumer << " ";
    //        _consumed += _consumer.get_num();
    //      }

    //      DEB(_log, "amount added = ", _producer.last(),
    //          ", consumed: ", _stream.str(), " - total = ", _consumed);
    //    }

    {
      std::stringstream _stream;

      _stream << _c1 << " ";
      _consumed += _c1.get_num();
      _stream << _c2 << " ";
      _consumed += _c2.get_num();

      DEB(_log, "amount added = ", _producer.last(),
          ", consumed: ", _stream.str(), " - total = ", _consumed);
    }

    return (_producer.last() == _consumed);
  }

private:
  struct consumer {
    consumer(const char *p_id) : m_id(p_id) { m_log.set_debug_level(); }
    void operator()(std::shared_ptr<bool>, msg &&p_msg) {
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

  struct producer {

    producer(uint16_t p_max, std::condition_variable *p_cond,
             handlers *p_handlers)
        : m_max(p_max), m_cond(p_cond), m_handlers(p_handlers) {
      m_log.set_debug_level();
    }

    void operator()(std::shared_ptr<bool>) {
      if (m_value == m_max) {
        DEB(m_log, "stoping production");
        m_cond->notify_one();
        return;
      }
      msg _msg(++m_value);
      DEB(m_log, "producing msg ", _msg);
      m_handlers->add_data(_msg);
    }

    value last() const { return m_value; }

  private:
    uint16_t m_max;
    std::condition_variable *m_cond;
    handlers *m_handlers;
    value m_value{0};

    logger::cerr<> m_log{"producer"};
  };
};

struct handlers_004 {

  typedef msg_t<'D'> msg;

  typedef async::internal::handlers_t<msg> handlers;
  typedef async::sleeping_loop sleeping_loop;

  static std::string desc() {
    return "Produces 3000 messages, and waits for all to be consumed by 5 "
           "consumers";
  }

  bool operator()() {
    logger::set_debug_level();
    logger::cerr<> _log{"handlers_004"};
    _log.set_debug_level();
    number::id _id;

    DEB(_log, "id = ", _id);

    const value _max{300};

    std::condition_variable _cond_producer;

    std::vector<consumer> _consumers{{"c1"}, {"c2"}, {"c3"}, {"c4"}, {"c5"}};
    //    std::vector<consumer> _consumers{{"c1"}, {"c2"}};

    handlers _handlers{_id, 2s};

    producer _producer(_max, &_cond_producer, &_handlers);

    //    for (std::vector<consumer>::size_type _i = 0; _i < _consumers.size();
    //         ++_i) {
    //      DEB(_log, "adding consumer ", _consumers[_i].get_id());
    //      consumer &_consumer = _consumers[_i];
    //      _handlers.add_handler(
    //          [&_consumer](std::shared_ptr<bool> p_bool, msg &&p_msg) -> void
    //          {
    //            _consumer(p_bool, std::move(p_msg));
    //          });
    //    }

    //    for (consumer &_consumer : _consumers) {
    //      DEB(_log, "adding consumer ", _consumer.get_id());
    //      _handlers.add_handler(
    //          [&_consumer](std::shared_ptr<bool> p_bool, msg &&p_msg) -> void
    //          {
    //            _consumer(p_bool, std::move(p_msg));
    //          });
    //    }

    _handlers.add_handler(
        [&_consumers](std::shared_ptr<bool> p_bool, msg &&p_msg) -> void {
          _consumers[0](p_bool, std::move(p_msg));
        });

    _handlers.add_handler(
        [&_consumers](std::shared_ptr<bool> p_bool, msg &&p_msg) -> void {
          _consumers[1](p_bool, std::move(p_msg));
        });

    _handlers.add_handler(
        [&_consumers](std::shared_ptr<bool> p_bool, msg &&p_msg) -> void {
          _consumers[2](p_bool, std::move(p_msg));
        });

    _handlers.add_handler(
        [&_consumers](std::shared_ptr<bool> p_bool, msg &&p_msg) -> void {
          _consumers[3](p_bool, std::move(p_msg));
        });

    _handlers.add_handler(
        [&_consumers](std::shared_ptr<bool> p_bool, msg &&p_msg) -> void {
          _consumers[4](p_bool, std::move(p_msg));
        });

    sleeping_loop _sleeping_loop(
        _id,
        [&_producer](std::shared_ptr<bool> p_bool) -> void {
          _producer(p_bool);
        },
        1s, 100ms);

    _sleeping_loop.start();

    {
      std::mutex _mutex_producer;
      std::unique_lock<std::mutex> _lock(_mutex_producer);
      _cond_producer.wait(_lock);
    }

    _sleeping_loop.stop();

    while (_handlers.occupied() != 0) {
      DEB(_log, "msg queue still not empty");
      std::this_thread::sleep_for(50ms);
    }

    value _consumed{0};
    {
      std::stringstream _stream;
      for (const consumer &_consumer : _consumers) {
        _stream << _consumer << " ";
        _consumed += _consumer.get_num();
      }

      DEB(_log, "amount added = ", _producer.last(),
          ", consumed: ", _stream.str(), " - total = ", _consumed);
    }

    return (_producer.last() == _consumed);
  }

private:
  struct consumer {
    consumer(const char *p_id) : m_id(p_id) { m_log.set_debug_level(); }
    void operator()(std::shared_ptr<bool>, msg &&p_msg) {
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

  struct producer {

    producer(uint16_t p_max, std::condition_variable *p_cond,
             handlers *p_handlers)
        : m_max(p_max), m_cond(p_cond), m_handlers(p_handlers) {
      m_log.set_debug_level();
    }

    void operator()(std::shared_ptr<bool>) {
      if (m_value == m_max) {
        DEB(m_log, "stoping production");
        m_cond->notify_one();
        return;
      }
      msg _msg(++m_value);
      DEB(m_log, "producing msg ", _msg);
      m_handlers->add_data(_msg);
    }

    value last() const { return m_value; }

  private:
    uint16_t m_max;
    std::condition_variable *m_cond;
    handlers *m_handlers;
    value m_value{0};

    logger::cerr<> m_log{"producer"};
  };
};

struct handlers_005 {

  typedef msg_t<'E'> msg;

  typedef async::internal::handlers_t<msg> handlers;
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

    handlers _handlers{m_id, 200ms};

    std::vector<consumer> _consumers;
    _consumers.push_back({"c1", 200ms, &_handlers});
    _consumers.push_back({"c2", 200ms, &_handlers});
    _consumers.push_back({"c3", 200ms, &_handlers});
    _consumers.push_back({"c4", 200ms, &_handlers});
    _consumers.push_back({"c5", 200ms, &_handlers});

    uint16_t _num_timeouts{0};
    {

      _handlers.add_handler(
          [&_consumers](std::shared_ptr<bool> p_bool, msg &&p_msg) -> void {
            _consumers[0](p_bool, std::move(p_msg));
          });

      _handlers.add_handler(
          [&_consumers](std::shared_ptr<bool> p_bool, msg &&p_msg) -> void {
            _consumers[1](p_bool, std::move(p_msg));
          });

      _handlers.add_handler(
          [&_consumers](std::shared_ptr<bool> p_bool, msg &&p_msg) -> void {
            _consumers[2](p_bool, std::move(p_msg));
          });

      _handlers.add_handler(
          [&_consumers](std::shared_ptr<bool> p_bool, msg &&p_msg) -> void {
            _consumers[3](p_bool, std::move(p_msg));
          });

      _handlers.add_handler(
          [&_consumers](std::shared_ptr<bool> p_bool, msg &&p_msg) -> void {
            _consumers[4](p_bool, std::move(p_msg));
          });

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
    consumer(const char *p_id, std::chrono::milliseconds p_timeout,
             handlers *p_handlers)
        : m_id(p_id), m_timeout(p_timeout), m_handlers(p_handlers),
          m_over_timeout(p_timeout.count() * 2) {}

    void operator()(std::shared_ptr<bool> p_bool, msg &&p_msg) {
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
      if (*p_bool) {
        m_handlers->add_data(p_msg);
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
    handlers *m_handlers;
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
  //  logger::set_debug_level();
  tester::test<> _test(argc, argv);
  run_test(_test, handlers_000);
  run_test(_test, handlers_001);
  run_test(_test, handlers_002);
  run_test(_test, handlers_003);
  run_test(_test, handlers_004);
  run_test(_test, handlers_005);
}
