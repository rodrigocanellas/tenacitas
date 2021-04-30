/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <algorithm>
#include <chrono>
#include <cmath>
#include <complex>
#include <condition_variable>
#include <cstdint>
#include <iterator>
#include <mutex>
#include <set>
#include <sstream>
#include <string>

#include <tenacitas/async.h>
#include <tenacitas/calendar.h>
#include <tenacitas/logger.h>
#include <tenacitas/number.h>
#include <tenacitas/tester.h>
#include <tenacitas/type.h>
#include <tst/tenacitas/async/msg.h>

using namespace tenacitas;
using namespace std::chrono_literals;

struct messenger_000 {

  typedef int16_t data;
  typedef logger::cerr<> log;
  typedef async::sleeping_loop sleeping_loop;

  static std::string desc() {
    std::stringstream _stream;
    _stream << "unamed, one worker, timeout";
    return _stream.str();
  }

  bool operator()() {
    using namespace std;

    //    m_log.set_debug_level();

    const data _data_to_produce{30};
    data _data_produced{0};
    data _data_consumed{0};
    const std::chrono::milliseconds _subscriber_timeout{800ms};

    number::id _id =
        async::add_handlers<data>(_subscriber_timeout, async::priority::lowest);

    auto _subscriber = [this, &_data_consumed, _subscriber_timeout](
                           std::shared_ptr<bool>, data &&p_data) -> void {
      ++_data_consumed;
      DEB(m_log, "subscriber sleeping for ", _data_consumed, ", ", p_data);
      std::this_thread::sleep_for(
          decltype(_subscriber_timeout)(_subscriber_timeout.count() / 2));
      DEB(m_log, "subscriber waking for ", _data_consumed, ", ", p_data);

      DEB(m_log, "consuming ", _data_consumed, ", ", p_data);
      if (_data_consumed == _data_to_produce) {
        m_cond_consumer.notify_one();
      }
    };

    async::add_handler<data>(_id, std::move(_subscriber));

    function<void(std::shared_ptr<bool>)> _slepper =
        [this, &_data_produced](std::shared_ptr<bool>) -> void {
      if (_data_produced == _data_to_produce) {
        m_cond_producer.notify_one();
      } else {
        ++_data_produced;
        async::send(_data_produced);
        DEB(m_log, "published ", _data_produced);
      }
    };

    sleeping_loop _sleeping_loop(
        m_id, _slepper, 300ms,
        decltype(_subscriber_timeout)(_subscriber_timeout.count() / 4));

    _sleeping_loop.start();

    DEB(m_log, "waiting for the producer to notify");
    {
      unique_lock<mutex> _lock_producer(m_mutex_producer);
      m_cond_producer.wait(_lock_producer);
    }
    _sleeping_loop.stop();

    DEB(m_log, "last data produced = ", _data_produced);

    DEB(m_log, "waiting for the consumer to notify");
    unique_lock<mutex> _lock_consumer(m_mutex_consumer);
    cv_status _status =
        m_cond_consumer.wait_for(_lock_consumer, chrono::seconds(60));
    DEB(m_log, "consumer notified");

    if (_status == cv_status::timeout) {
      ERR(m_log, "it took more time than nowed for the consumer");
      return false;
    }

    if (_data_consumed != _data_produced) {
      ERR(m_log, "data consumed = ", _data_consumed, ", but it should be ",
          _data_produced);
      return false;
    }

    DEB(m_log, "data consumed = ", _data_consumed,
        ", and is equal to data produced = ", _data_produced);

    return true;
  }

private:
  log m_log{"messenger_000"};
  std::mutex m_mutex_producer;
  std::condition_variable m_cond_producer;
  std::mutex m_mutex_consumer;
  std::condition_variable m_cond_consumer;
  number::id m_id;
};

struct messenger_001 {
  static std::string desc() { return "compiling"; }

  bool operator()() {
    //    typedef async::internal::messenger_t<int16_t, async::priority>
    //    messenger;

    DEB(m_log, "starting");

    auto _subscriber = [this](type::ptr<bool>, int16_t &&p_int) {
      DEB(m_log, "i = ", p_int);
      m_cond.notify_one();
    };

    DEB(m_log, "adding worker pool");
    number::id _id = async::add_handlers<int16_t>(1s, async::priority::lowest);
    DEB(m_log, "worker pool, ", _id, " added");

    DEB(m_log, "getting priority");
    std::optional<async::priority> _maybe = async::get_priority<int16_t>(_id);

    if (_maybe) {
      DEB(m_log, "priority = ", *_maybe);
    }

    DEB(m_log, "resetting priority");
    async::set_priority<int16_t>(_id, async::priority::middle);
    DEB(m_log, "priority reset");

    DEB(m_log, "adding subscriber");
    async::add_handler<int16_t>(_id, std::move(_subscriber));
    DEB(m_log, "subscriber added");

    DEB(m_log, "publishing");
    async::send<int16_t>(42);
    DEB(m_log, "published");

    DEB(m_log, "wating notification");
    {
      std::unique_lock<std::mutex> _lock(m_mutex);
      m_cond.wait(_lock);
    }

    DEB(m_log, "notification arrived");

    return true;
  }

private:
  logger::cerr<> m_log{"messenger_001"};
  std::condition_variable m_cond;
  std::mutex m_mutex;
};

struct messenger_002 {

  typedef int16_t data;
  typedef async::sleeping_loop sleeping_loop;

  static std::string desc() {
    std::stringstream _stream;

    _stream << "unamed, one worker, no timeout";

    return _stream.str();
  }

  bool operator()() {
    //    m_log.set_debug_level();
    using namespace std;

    const data _total_to_produce{58};
    data _data_produced{0};
    data _data_consumed{0};

    number::id _id = async::add_handlers<data>(3s, async::priority::lowest);

    auto _subscriber = [this, &_data_consumed](type::ptr<bool>,
                                               data &&p_data) -> void {
      DEB(m_log, "consuming ", p_data);
      _data_consumed = p_data;
      this_thread::sleep_for(chrono::seconds(1));
      if (p_data == _total_to_produce) {
        m_cond_consumer.notify_one();
      }
    };

    async::add_handler<data>(_id, _subscriber);

    auto _sleeper = [this, &_data_produced,
                     _total_to_produce](type::ptr<bool>) -> void {
      DEB(m_log, "data produced = ", _data_produced, ", total to produce ",
          _total_to_produce);
      if (_data_produced >= _total_to_produce) {
        DEB(m_log, "notifying");
        m_cond_producer.notify_one();
      } else {
        ++_data_produced;
        async::send<data>(_data_produced);
        DEB(m_log, "published data ", _data_produced);
      }
    };

    sleeping_loop _sleeping_loop(m_id, _sleeper, 100ms, 500ms);

    _sleeping_loop.start();

    DEB(m_log, "waiting for the producer to notify");
    {
      unique_lock<mutex> _lock_producer(m_mutex_producer);
      m_cond_producer.wait(_lock_producer);
    }
    DEB(m_log, "producer notified");

    _sleeping_loop.stop();

    DEB(m_log, "last data produced = ", _data_produced);

    DEB(m_log, "waiting for the consumer to notify");
    {
      unique_lock<mutex> _lock_consumer(m_mutex_consumer);
      cv_status _status =
          m_cond_consumer.wait_for(_lock_consumer, chrono::seconds(5));

      DEB(m_log, "consumer notified");

      if (_status == cv_status::timeout) {
        INF(m_log,
            "it took more time than allowed for the consumer, as expected");
        //        messenger::wait();
        return true;
      }
    }

    ERR(m_log, "data consumed = ", _data_consumed, ", but it should be ",
        _data_produced);
    return false;
  }

private:
  logger::cerr<> m_log{"messenger_002"};
  std::mutex m_mutex_producer;
  std::condition_variable m_cond_producer;
  std::mutex m_mutex_consumer;
  std::condition_variable m_cond_consumer;
  number::id m_id;
};

struct messenger_004 {

  typedef int16_t data;
  typedef async::sleeping_loop sleeping_loop;

  static std::string desc() {
    std::stringstream _stream;

    _stream << "unamed, one worker, no timeout, using wrapper functions";

    return _stream.str();
  }

  bool operator()() {
    //    m_log.set_debug_level();
    using namespace std;

    const data _total_to_produce{58};
    data _data_produced{0};
    data _data_consumed{0};

    number::id _id = async::add_handlers<data>(3s, async::priority::lowest);

    auto _subscriber = [this, &_data_consumed](type::ptr<bool>,
                                               data &&p_data) -> void {
      DEB(m_log, "consuming ", p_data);
      _data_consumed = p_data;
      this_thread::sleep_for(chrono::seconds(1));
      if (p_data == _total_to_produce) {
        m_cond_consumer.notify_one();
      }
    };

    async::add_handler<data>(_id, _subscriber);

    auto _sleeper = [this, &_data_produced,
                     _total_to_produce](type::ptr<bool>) -> void {
      DEB(m_log, "data produced = ", _data_produced, ", total to produce ",
          _total_to_produce);
      if (_data_produced >= _total_to_produce) {
        DEB(m_log, "notifying");
        m_cond_producer.notify_one();
      } else {
        ++_data_produced;
        async::send<data>(_data_produced);
        DEB(m_log, "published data ", _data_produced);
      }
    };

    sleeping_loop _sleeping_loop(m_id, _sleeper, 100ms, 500ms);

    _sleeping_loop.start();

    DEB(m_log, "waiting for the producer to notify");
    {
      unique_lock<mutex> _lock_producer(m_mutex_producer);
      m_cond_producer.wait(_lock_producer);
    }
    DEB(m_log, "producer notified");

    _sleeping_loop.stop();

    DEB(m_log, "last data produced = ", _data_produced);

    DEB(m_log, "waiting for the consumer to notify");
    {
      unique_lock<mutex> _lock_consumer(m_mutex_consumer);
      cv_status _status =
          m_cond_consumer.wait_for(_lock_consumer, chrono::seconds(5));

      DEB(m_log, "consumer notified");

      if (_status == cv_status::timeout) {
        INF(m_log,
            "it took more time than allowed for the consumer, as expected");
        //        messenger::wait();
        return true;
      }
    }

    ERR(m_log, "data consumed = ", _data_consumed, ", but it should be ",
        _data_produced);
    return false;
  }

private:
  logger::cerr<> m_log{"messenger_004"};
  std::mutex m_mutex_producer;
  std::condition_variable m_cond_producer;
  std::mutex m_mutex_consumer;
  std::condition_variable m_cond_consumer;
  number::id m_id;
};

struct messenger_003 {
  static std::string desc() { return "Testing order of worker pools"; }

  bool operator()() {
    typedef int32_t data;

    const number::id _p2 = async::add_handlers<data>(4s, async::priority::low);

    const number::id _p1 =
        async::add_handlers<data>(1s, async::priority::middle);

    bool _first{true};
    bool _result{true};
    auto _visitor = [this, &_first, &_result, _p1,
                     _p2](const number::id &p_id,
                          const async::priority &p_priority,
                          const std::chrono::milliseconds &p_timeout) {
      if (_first) {
        if (p_id != _p2) {
          _result = false;
        } else {
          _first = false;
        }
      } else {
        if (p_id != _p1) {
          _result = false;
        }
      }
      INF(m_log, p_id, ':', p_priority, ':', p_timeout.count());
    };

    async::traverse<data>(_visitor);
    return _result;
  }

private:
  logger::cerr<> m_log{"messenger_003"};
};

template <message_id msg_id> struct publish_t {

  template <typename t_time>
  explicit publish_t(const number::id &p_owner, value p_max,
                     std::condition_variable *p_cond, t_time p_interval)
      : m_owner(p_owner), m_max(p_max), m_cond(p_cond),
        m_log("publish_" + std::string(1, msg_id)),
        m_loop(
            m_owner,
            [this](type::ptr<bool>) -> void {
              if (m_current >= m_max) {
                DEB(m_log, m_current, " == ", m_max, " for publisher ", msg_id);
                m_cond->notify_one();
                return;
              }
              msg_t<msg_id> _msg{++m_current};
              DEB(m_log, "publishing ", _msg);
              async::send<msg_t<msg_id>>(_msg);
            },
            2s, p_interval) {
    m_log.set_debug_level();
  }

  inline void start() { m_loop.start(); }
  inline void stop() { m_loop.stop(); }
  inline bool over() const {
    return (m_loop.is_stopped() ? true : m_current == m_max);
  }

private:
  number::id m_owner;
  const value m_max;
  std::condition_variable *m_cond;
  logger::cerr<> m_log;

  async::sleeping_loop m_loop;
  value m_current{0};
};

template <message_id msg_id> struct subscriber_t {

  subscriber_t(const number::id &p_handlers, uint16_t p_number)
      : m_handlers(p_handlers), m_number(p_number),
        m_log("subscriber_" + std::string(1, msg_id) + "_" + p_handlers.str() +
              "_" + std::to_string(p_number)) {

    async::add_handler<msg_t<msg_id>>(
        m_handlers, [this](type::ptr<bool>, msg_t<msg_id> &&p_msg) -> void {
          DEB(m_log, "handling ", p_msg);
          m_current = p_msg.get_value();
        });
  }

  inline value get_last() const { return m_current; }

private:
private:
  const number::id m_handlers;
  const uint16_t m_number;
  logger::cerr<> m_log;
  value m_current;
};

struct messenger_005 {
  static std::string desc() { return ""; }

  bool operator()() {
    m_log.set_debug_level();

    number::id _e_1 = async::add_handlers<msg_t<'E'>>(1s);
    //    number::id _f_1 = async::add_handlers<msg_t<'F'>>(1s);

    msg_t<'E'> _current;

    auto _sub_e_1_1 = [&](type::ptr<bool>, msg_t<'E'> &&p_msg) -> void {
      DEB(m_log, "handling ", p_msg);
      _current = p_msg;
    };
    async::add_handler<msg_t<'E'>>(_e_1, _sub_e_1_1);

    //    subscriber_t<'E'> _sub_e_1_1(_e_1, 1);
    //    subscriber_t<'F'> _sub_f_1_1(_f_1, 1);

    publish();

    while (true) {
      // uint16_t _amount = _sub_e_1_1.get_last() + _sub_f_1_1.get_last();
      uint16_t _amount = _current.get_value();
      if (_amount == 8) {
        DEB(m_log, "all ", _amount, " messages where handled");
        break;
      }
      DEB(m_log, _amount, " messages were handled so far");
      std::this_thread::sleep_for(1s);
    }

    return true;
  }

private:
  void publish() {
    publish_t<'E'> _pub_e(m_id, value{5}, &m_cond_publish, 1s);
    publish_t<'F'> _pub_f(m_id, value{3}, &m_cond_publish, 2s);

    _pub_e.start();
    //    _pub_f.start();

    {
      std::unique_lock<std::mutex> _lock(m_mutex_publish);
      m_cond_publish.wait(_lock, [&]() -> bool {
        bool _e = _pub_e.over();
        bool _f = _pub_f.over();

        if (_e) {
          _pub_e.stop();
        }

        if (_f) {
          _pub_f.stop();
        }

        if (_e && _f) {
          DEB(m_log, "publisher E and F are over");
          return true;
        }
        DEB(m_log, "waiting for all the publihsers to finish");
        return false;
      });
    }
  }

private:
  number::id m_id;
  std::condition_variable m_cond_publish;
  std::mutex m_mutex_publish;
  logger::cerr<> m_log{"messenger_005"};
};

struct messenger_006 {

  typedef msg_t<'H'> data;
  typedef logger::cerr<> log;
  typedef async::sleeping_loop sleeping_loop;

  static std::string desc() {
    std::stringstream _stream;
    _stream << "unamed, one worker, timeout";
    return _stream.str();
  }

  bool operator()() {
    using namespace std;

    //    m_log.set_debug_level();

    const data _data_to_produce{30};
    data _data_produced{0};
    data _data_consumed{0};
    const std::chrono::milliseconds _subscriber_timeout{800ms};

    number::id _id =
        async::add_handlers<data>(_subscriber_timeout, async::priority::lowest);

    auto _subscriber = [this, &_data_consumed, &_data_to_produce,
                        _subscriber_timeout](std::shared_ptr<bool>,
                                             data &&p_data) -> void {
      _data_consumed = p_data;
      DEB(m_log, "subscriber sleeping for ", p_data);
      std::this_thread::sleep_for(
          decltype(_subscriber_timeout)(_subscriber_timeout.count() / 2));
      DEB(m_log, "subscriber waking for ", p_data);

      DEB(m_log, "consuming ", p_data);
      if (_data_consumed == _data_to_produce) {
        m_cond_consumer.notify_one();
      }
    };

    async::add_handler<data>(_id, std::move(_subscriber));

    function<void(std::shared_ptr<bool>)> _slepper =
        [this, &_data_produced,
         &_data_to_produce](std::shared_ptr<bool>) -> void {
      if (_data_produced == _data_to_produce) {
        m_cond_producer.notify_one();
      } else {
        ++_data_produced;
        async::send(_data_produced);
        DEB(m_log, "published ", _data_produced);
      }
    };

    sleeping_loop _sleeping_loop(
        m_id, _slepper, 300ms,
        decltype(_subscriber_timeout)(_subscriber_timeout.count() / 4));

    _sleeping_loop.start();

    DEB(m_log, "waiting for the producer to notify");
    {
      unique_lock<mutex> _lock_producer(m_mutex_producer);
      m_cond_producer.wait(_lock_producer);
    }
    _sleeping_loop.stop();

    DEB(m_log, "last data produced = ", _data_produced);

    DEB(m_log, "waiting for the consumer to notify");
    unique_lock<mutex> _lock_consumer(m_mutex_consumer);
    cv_status _status =
        m_cond_consumer.wait_for(_lock_consumer, chrono::seconds(60));
    DEB(m_log, "consumer notified");

    if (_status == cv_status::timeout) {
      ERR(m_log, "it took more time than nowed for the consumer");
      return false;
    }

    if (_data_consumed != _data_produced) {
      ERR(m_log, "data consumed = ", _data_consumed, ", but it should be ",
          _data_produced);
      return false;
    }

    DEB(m_log, "data consumed = ", _data_consumed,
        ", and is equal to data produced = ", _data_produced);

    return true;
  }

private:
  log m_log{"messenger_006"};
  std::mutex m_mutex_producer;
  std::condition_variable m_cond_producer;
  std::mutex m_mutex_consumer;
  std::condition_variable m_cond_consumer;
  number::id m_id;
};

struct messenger_007 {
  typedef msg_t<'R'> msg;
  typedef async::sleeping_loop sleeping_loop;
  typedef async::internal::handlers_t<msg> handlers;

  static std::string desc() {
    std::stringstream _stream;

    _stream << "A 'sleeping_loop' sends " << m_max << " messages, at each "
            << m_interval.count() << "ms, to a 'handlers' with timeout of "
            << m_handlers_timeout.count()
            << "s, with one handler that sleeps for " << m_handler_sleep.count()
            << "ms.\n"
            << "The amount of messages consumed must be equal to the produced";

    return _stream.str();
  }

  bool operator()() {

    logger::set_debug_level();

    DEB(m_log, "capacity = ", m_handlers.capacity(),
        ", occupied = ", m_handlers.occupied());

    DEB(m_log, "adding consumer to the worker");
    number::id _handlers = async::add_handlers<msg>(m_handlers_timeout);
    async::add_handler<msg>(
        _handlers, [this](std::shared_ptr<bool> p_bool, msg &&p_msg) -> void {
          m_consumer(p_bool, std::move(p_msg));
        });

    producer _producer{_handlers, &m_cond_produced, &m_msg};
    sleeping_loop _loop{m_id, _producer, m_sleeping_loop_timeout, m_interval};

    DEB(m_log, "starting the producer loop");
    _loop.start();

    {
      std::unique_lock<std::mutex> _lock(m_mutex_produced);
      m_cond_produced.wait(_lock);
    }

    DEB(m_log, "stoping the producer loop");
    _loop.stop();

    {
      std::unique_lock<std::mutex> _lock(m_mutex_handled);
      m_cond_handled.wait(_lock);
    }

    DEB(m_log, "produced = ", m_msg.get_value(),
        ", consumed = ", m_consumer.get_msg().get_value());
    if (m_consumer.get_msg().get_value() != m_msg.get_value()) {
      ERR(m_log, "Data value consumed should be equal to provided");
      return false;
    }

    return true;
  }

private:
  struct producer {
    producer(const number::id &p_handlers, std::condition_variable *p_cond,
             msg *p_msg)
        : m_handlers(p_handlers), m_cond(p_cond), m_msg(p_msg) {}

    void operator()(std::shared_ptr<bool>) {
      if (m_msg->get_value() == m_max) {
        m_cond->notify_one();
        return;
      }

      ++(*m_msg);
      msg _msg(m_msg->get_value());
      DEB(m_log, "going to add ", _msg);

      async::send<msg>(_msg);
      //      DEB(m_log, "added msg ", _msg, "; capacity = ",
      //      m_handlers->capacity(),
      //          ", occupied = ", m_handlers->occupied());
    }

  private:
    const number::id m_handlers;
    std::condition_variable *m_cond;
    msg *m_msg;

    logger::cerr<> m_log{"producer"};
  };

  struct consumer {
    consumer(std::condition_variable *p_cond) : m_cond(p_cond) {}
    void operator()(std::shared_ptr<bool>, msg &&p_msg) {

      m_msg = std::move(p_msg);

      DEB(m_log, "handling msg ", m_msg);

      if (m_msg.get_value() == m_max) {
        DEB(m_log, "Last msg handled = ", m_msg);
        m_cond->notify_one();
        return;
      }

      DEB(m_log, "worker is going to sleep");
      std::this_thread::sleep_for(m_handler_sleep);
      DEB(m_log, "worker is waking up");
    }
    const msg &get_msg() const { return m_msg; }

  private:
    msg m_msg;
    std::condition_variable *m_cond;
    logger::cerr<> m_log{"consumer"};
  };

  logger::cerr<> m_log{"messenger_007"};
  number::id m_id;
  std::condition_variable m_cond_produced;
  std::mutex m_mutex_produced;
  std::condition_variable m_cond_handled;
  std::mutex m_mutex_handled;
  consumer m_consumer{&m_cond_handled};
  msg m_msg{0};
  handlers m_handlers{m_id, m_handlers_timeout};
  static const value m_max{10};
  static const std::chrono::milliseconds m_interval;
  static const std::chrono::seconds m_handlers_timeout;
  static const std::chrono::milliseconds m_handler_sleep;
  static const std::chrono::seconds m_sleeping_loop_timeout;
};

const std::chrono::milliseconds messenger_007::m_interval{150};
const std::chrono::seconds messenger_007::m_handlers_timeout{2};
const std::chrono::milliseconds messenger_007::m_handler_sleep{300};
const std::chrono::seconds messenger_007::m_sleeping_loop_timeout{1};

int main(int argc, char **argv) {
  logger::set_debug_level();
  tester::test _tester(argc, argv);

  run_test(_tester, messenger_000);
  run_test(_tester, messenger_001);
  run_test(_tester, messenger_002);
  run_test(_tester, messenger_003);
  run_test(_tester, messenger_004);
  run_test(_tester, messenger_005);
  run_test(_tester, messenger_006);
  run_test(_tester, messenger_007);
}
