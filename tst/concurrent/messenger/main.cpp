/// \example

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <cmath>
#include <complex>
#include <condition_variable>
#include <cstdint>
#include <mutex>
#include <sstream>
#include <string>

#include <concurrent/msg_a.h>
#include <tenacitas/calendar.h>
#include <tenacitas/concurrent.h>
#include <tenacitas/logger.h>
#include <tenacitas/tester.h>

using namespace tenacitas;
using namespace std::chrono_literals;

//struct messenger_000 {

//  typedef int16_t data;
//  typedef logger::cerr<> log;
//  typedef concurrent::sleeping_loop_t<void> sleeping_loop;
//  typedef concurrent::messenger_t<data> messenger;

//  static std::string desc() {
//    std::stringstream _stream;
//    _stream << "unamed, one worker, timeout";
//    return _stream.str();
//  }

//  bool operator()() {
//    using namespace std;

//    //    m_log.set_debug_level();

//    const data _data_to_produce{30};
//    data _data_produced{0};
//    data _data_consumed{0};
//    const std::chrono::milliseconds _subscriber_timeout{800ms};

//    messenger::pool_id _pool_id = messenger::add_worker_pool(
//        messenger::priority{0}, 20, _subscriber_timeout);

//    function<void(const data &)> _subscriber =
//        [this, &_data_consumed,
//         _subscriber_timeout](const data &p_data) -> void {
//      ++_data_consumed;
//      DEB(m_log, "subscriber sleeping for ", _data_consumed, ", ", p_data);
//      std::this_thread::sleep_for(
//          decltype(_subscriber_timeout)(_subscriber_timeout.count() / 2));
//      DEB(m_log, "subscriber waking for ", _data_consumed, ", ", p_data);

//      DEB(m_log, "consuming ", _data_consumed, ", ", p_data);
//      if (_data_consumed == _data_to_produce) {
//        m_cond_consumer.notify_one();
//      }
//    };

//    messenger::add_subscriber(_pool_id, _subscriber);

//    function<void(void)> _slepper = [this, &_data_produced]() -> void {
//      if (_data_produced == _data_to_produce) {
//        m_cond_producer.notify_one();
//      } else {
//        ++_data_produced;
//        messenger::publish(_data_produced);
//        DEB(m_log, "published ", _data_produced);
//      }
//    };

//    sleeping_loop _sleeping_loop(
//        300ms, decltype(_subscriber_timeout)(_subscriber_timeout.count() / 4),
//        _slepper);

//    _sleeping_loop.start();

//    DEB(m_log, "waiting for the producer to notify");
//    {
//      unique_lock<mutex> _lock_producer(m_mutex_producer);
//      m_cond_producer.wait(_lock_producer);
//    }
//    _sleeping_loop.stop();

//    DEB(m_log, "last data produced = ", _data_produced);

//    DEB(m_log, "waiting for the consumer to notify");
//    unique_lock<mutex> _lock_consumer(m_mutex_consumer);
//    cv_status _status =
//        m_cond_consumer.wait_for(_lock_consumer, chrono::seconds(60));
//    DEB(m_log, "consumer notified");

//    if (_status == cv_status::timeout) {
//      ERR(m_log, "it took more time than allowed for the consumer");
//      return false;
//    }

//    if (_data_consumed != _data_produced) {
//      ERR(m_log, "data consumed = ", _data_consumed, ", but it should be ",
//          _data_produced);
//      return false;
//    }

//    DEB(m_log, "data consumed = ", _data_consumed,
//        ", and is equal to data produced = ", _data_produced);

//    return true;
//  }

//private:
//  log m_log{"messenger_000"};
//  std::mutex m_mutex_producer;
//  std::condition_variable m_cond_producer;
//  std::mutex m_mutex_consumer;
//  std::condition_variable m_cond_consumer;
//};

//struct messenger_001 {
//  static std::string desc() { return "compiling"; }

//  bool operator()() {
//    typedef concurrent::messenger_t<int16_t> messenger;

//    DEB(m_log, "starting");

//    auto _subscriber = [this](const int16_t &p_int) {
//      DEB(m_log, "i = ", p_int);
//      m_cond.notify_one();
//    };

//    DEB(m_log, "adding worker pool");
//    messenger::pool_id _pool_id =
//        messenger::add_worker_pool(messenger::priority{0}, 20, 1s);
//    DEB(m_log, "worker pool, ", _pool_id, " added");

//    DEB(m_log, "getting priority");
//    std::optional<messenger::priority> _maybe =
//        messenger::get_priority(_pool_id);

//    if (_maybe) {
//      DEB(m_log, "priority = ", *_maybe);
//    }

//    DEB(m_log, "resetting priority");
//    messenger::set_priority(_pool_id, 9);
//    DEB(m_log, "priority reset");

//    DEB(m_log, "adding subscriber");
//    messenger::add_subscriber(_pool_id, _subscriber);
//    DEB(m_log, "subscriber added");

//    DEB(m_log, "publishing");
//    messenger::publish(42);
//    DEB(m_log, "published");

//    DEB(m_log, "wating notification");
//    {
//      std::unique_lock<std::mutex> _lock(m_mutex);
//      m_cond.wait(_lock);
//    }

//    DEB(m_log, "notification arrived");

//    return true;
//  }

//private:
//  logger::cerr<> m_log{"messenger_001"};
//  std::condition_variable m_cond;
//  std::mutex m_mutex;
//};

//struct messenger_002 {

//  typedef int16_t data;
//  typedef concurrent::sleeping_loop_t<void> sleeping_loop;
//  typedef concurrent::messenger_t<data> messenger;

//  static std::string desc() {
//    std::stringstream _stream;

//    _stream << "unamed, one worker, no timeout";

//    return _stream.str();
//  }

//  bool operator()() {
//    using namespace std;

//    const data _total_to_produce{58};
//    data _data_produced{0};
//    data _data_consumed{0};

//    messenger::pool_id _pool_id =
//        messenger::add_worker_pool(messenger::priority{0}, 20, 3s);

//    auto _subscriber = [this, &_data_consumed](const data &p_data) -> void {
//      DEB(m_log, "consuming ", p_data);
//      _data_consumed = p_data;
//      this_thread::sleep_for(chrono::seconds(1));
//      if (p_data == _total_to_produce) {
//        m_cond_consumer.notify_one();
//      }
//    };

//    messenger::add_subscriber(_pool_id, _subscriber);

//    auto _sleeper = [this, &_data_produced, _total_to_produce]() -> void {
//      DEB(m_log, "data produced = ", _data_produced, ", total to produce ",
//          _total_to_produce);
//      if (_data_produced >= _total_to_produce) {
//        DEB(m_log, "notofying");
//        m_cond_producer.notify_one();
//      } else {
//        ++_data_produced;
//        messenger::publish(_data_produced);
//        DEB(m_log, "published data ", _data_produced);
//      }
//    };

//    sleeping_loop _sleeping_loop(100ms, 500ms, _sleeper);

//    _sleeping_loop.start();

//    DEB(m_log, "waiting for the producer to notify");
//    {
//      unique_lock<mutex> _lock_producer(m_mutex_producer);
//      m_cond_producer.wait(_lock_producer);
//    }
//    DEB(m_log, "producer notified");

//    _sleeping_loop.stop();

//    DEB(m_log, "last data produced = ", _data_produced);

//    DEB(m_log, "waiting for the consumer to notify");
//    {
//      unique_lock<mutex> _lock_consumer(m_mutex_consumer);
//      cv_status _status =
//          m_cond_consumer.wait_for(_lock_consumer, chrono::seconds(5));

//      DEB(m_log, "consumer notified");

//      if (_status == cv_status::timeout) {
//        ERR(m_log,
//            "it took more time than allowed for the consumer, as expected");
//        return true;
//      }
//    }

//    ERR(m_log, "data consumed = ", _data_consumed, ", but it should be ",
//        _data_produced);
//    return false;
//  }

//private:
//  logger::cerr<> m_log{"messenger_002"};
//  std::mutex m_mutex_producer;
//  std::condition_variable m_cond_producer;
//  std::mutex m_mutex_consumer;
//  std::condition_variable m_cond_consumer;
//};

//struct messenger_003 {
//  static std::string desc() { return "Testing order of worker pools"; }

//  bool operator()() {
//    typedef concurrent::messenger_t<int16_t> messenger;

//    const messenger::pool_id _p2{"hello"};
//    messenger::add_worker_pool(_p2, 2, 15, 4s);

//    const messenger::pool_id _p1{"good morning"};
//    messenger::add_worker_pool(_p1, 5, 20, 1s);

//    bool _first{true};
//    bool _result{true};
//    auto _visitor = [this, &_first, &_result, _p1,
//                     _p2](const messenger::pool_id &p_pool_id,
//                          messenger::priority p_priority,
//                          concurrent::timeout p_timeout) {
//      if (_first) {
//        if (p_pool_id != _p1) {
//          _result = false;
//        } else {
//          _first = false;
//        }
//      } else {
//        if (p_pool_id != _p2) {
//          _result = false;
//        }
//      }
//      INF(m_log, p_pool_id, ':', p_priority, ':', p_timeout.count());
//    };

//    messenger::traverse(_visitor);
//    return _result;
//  }

//private:
//  logger::cerr<> m_log{"messenger_003"};
//};

//struct messenger_004 {
//  static std::string desc() {
//    std::stringstream stream;

//    stream << "messengers for 'msb_b', 'msg_c' and 'msg_d'.\n"
//           << "3 types of subscribers for 'msg_b', 1 for 'msg_c', and 2 for "
//              "'msg_d'.\n"
//           << "4 instances of subscribers for 'msg_c'.\n"
//           << m_max_b << " 'msg_b' will be published, " << m_max_c
//           << " 'msg_b' will be published, and " << m_max_d
//           << " 'msg_d' will be published.\n"
//           << "None of the subscribers will timeout.";

//    return stream.str();
//  }

//  bool operator()() {
//    logger::set_debug_level();

//    typedef concurrent::sleeping_loop_t<void> publisher;

//    typedef concurrent::messenger_t<msg_b> messenger_b;
//    typedef concurrent::messenger_t<msg_c> messenger_c;
//    typedef concurrent::messenger_t<msg_d> messenger_d;

//    // creation of worker pools
//    const messenger_b::pool_id _pool_b1{"pool b1"};
//    messenger_b::add_worker_pool(_pool_b1, 100, 2s);

//    const messenger_b::pool_id _pool_b2{"pool b2"};
//    messenger_b::add_worker_pool(_pool_b2, 50, 2s);

//    const messenger_b::pool_id _pool_b3{"pool b3"};
//    messenger_b::add_worker_pool(_pool_b3, 10, 2s);

//    const messenger_c::pool_id _pool_c{"pool c"};
//    messenger_c::add_worker_pool(_pool_c, 1, 4s);

//    const messenger_d::pool_id _pool_d1{"pool d1"};
//    messenger_d::add_worker_pool(_pool_d1, 10, 2s);

//    const messenger_d::pool_id _pool_d2{"pool d2"};
//    messenger_d::add_worker_pool(_pool_d2, 1, 2s);

//    // adding subscribers to work pools
//    subscriber_b1 _sub_b1(this);
//    messenger_b::add_subscriber(
//        _pool_b1, [&_sub_b1](const msg_b &p_msg) -> void { _sub_b1(p_msg); });

//    subscriber_b2 _sub_b2(this);
//    messenger_b::add_subscriber(
//        _pool_b2, [&_sub_b2](const msg_b &p_msg) -> void { _sub_b2(p_msg); });

//    subscriber_b3 _sub_b31(this, '1');
//    messenger_b::add_subscriber(
//        _pool_b3, [&_sub_b31](const msg_b &p_msg) -> void { _sub_b31(p_msg); });

//    subscriber_b3 _sub_b32(this, '2');
//    messenger_b::add_subscriber(
//        _pool_b3, [&_sub_b32](const msg_b &p_msg) -> void { _sub_b32(p_msg); });

//    std::vector<subscriber_c> _subs_c{
//        {this, '1'}, {this, '2'}, {this, '3'}, {this, '4'}};

//    for (subscriber_c &_sub_c : _subs_c) {
//      DEB(m_log, "adding sub c ", _sub_c.get_id());
//      messenger_c::add_subscriber(
//          _pool_c, [&_sub_c](const msg_c &p_msg) -> void { _sub_c(p_msg); });
//    }

//    subscriber_d1 _sub_d1(this);
//    messenger_d::add_subscriber(
//        _pool_d1, [&_sub_d1](const msg_d &p_msg) -> void { _sub_d1(p_msg); });

//    subscriber_d2 _sub_d2(this);
//    messenger_d::add_subscriber(
//        _pool_d2, [&_sub_d2](const msg_d &p_msg) -> void { _sub_d2(p_msg); });

//    // creating publishers
//    uint16_t _count_b{0};
//    msg_b _msg_b;

//    uint16_t _count_c{0};
//    msg_c _msg_c;

//    uint16_t _count_d{0};
//    msg_d _msg_d;

//    publisher _publisher_b(1s, 500ms, [this, &_count_b, &_msg_b]() -> void {
//      if (_count_b >= m_max_b) {
//        m_cond_b.notify_one();
//        return;
//      }
//      std::this_thread::sleep_for(350ms);
//      ++_count_b;
//      ++_msg_b;
//      DEB(m_log, "publishing B: ", _count_b, ",", _msg_b);
//      messenger_b::publish(_msg_b);
//    });

//    publisher _publisher_c(1s, 300ms, [this, &_count_c, &_msg_c]() -> void {
//      if (_count_c >= m_max_c) {
//        m_cond_c.notify_one();
//        return;
//      }
//      std::this_thread::sleep_for(100ms);
//      ++_count_c;
//      ++_msg_c;
//      DEB(m_log, "publishing C: ", _count_c, ",", _msg_c);
//      messenger_c::publish(_msg_c);
//    });

//    publisher _publisher_d(1s, 750ms, [this, &_count_d, &_msg_d]() -> void {
//      if (_count_d >= m_max_d) {
//        m_cond_d.notify_one();
//        return;
//      }
//      std::this_thread::sleep_for(300ms);
//      ++_count_d;
//      ++_msg_d;
//      DEB(m_log, "publishing D: ", _count_d, ",", _msg_d);
//      messenger_d::publish(_msg_d);
//    });

//    DEB(m_log, "starting publishers");

//    _publisher_b.start();
//    _publisher_c.start();
//    _publisher_d.start();

//    {
//      DEB(m_log, "waiting for 'b'");
//      std::unique_lock<std::mutex> _lock_b(m_mutex_b);
//      m_cond_b.wait(_lock_b,
//                    [&_count_b]() -> bool { return _count_b == m_max_b; });
//    }

//    {
//      DEB(m_log, "waiting for 'c'");
//      std::unique_lock<std::mutex> _lock_c(m_mutex_c);
//      m_cond_c.wait(_lock_c,
//                    [&_count_c]() -> bool { return _count_c == m_max_c; });
//    }

//    {
//      DEB(m_log, "waiting for 'd'");
//      std::unique_lock<std::mutex> _lock_d(m_mutex_d);
//      m_cond_d.wait(_lock_d,
//                    [&_count_d]() -> bool { return _count_d == m_max_d; });
//    }

//    while (true) {
//      if ((messenger_b::size(_pool_b1) == 0) &&
//          (messenger_b::size(_pool_b2) == 0) &&
//          (messenger_b::size(_pool_b3) == 0) &&
//          (messenger_c::size(_pool_c) == 0) &&
//          (messenger_d::size(_pool_d1) == 0) &&
//          (messenger_d::size(_pool_d2) == 0)) {
//        break;
//      }
//      std::this_thread::sleep_for(1s);
//      DEB(m_log, "pool b1 = ", messenger_b::size(_pool_b1),
//          " pool b2 = ", messenger_b::size(_pool_b2),
//          " pool b3 = ", messenger_b::size(_pool_b3),
//          " pool c = ", messenger_c::size(_pool_c),
//          " pool d1 = ", messenger_d::size(_pool_d1),
//          " pool d2 = ", messenger_b::size(_pool_d2));
//    }

//    uint16_t _total{0};
//    {
//      std::stringstream stream;
//      stream << "sub b1: " << _sub_b1.total() << ", "
//             << "sub b2: " << _sub_b2.total() << ", "
//             << "sub b31: " << _sub_b31.total() << ", "
//             << "sub b32: " << _sub_b32.total() << ", ";
//      _total += _sub_b1.total();
//      _total += _sub_b2.total();
//      _total += _sub_b31.total();
//      _total += _sub_b32.total();

//      for (uint16_t _i = 0; _i < _subs_c.size(); ++_i) {
//        _total += _subs_c[_i].total();
//        stream << "sub c[" << _i << "]: " << _subs_c[_i].total() << ", ";
//      }
//      _total += _sub_d1.total();
//      _total += _sub_d2.total();
//      stream << "sub d1: " << _sub_d1.total() << ", "
//             << "sub d2: " << _sub_d2.total() << ", "
//             << "total: " << _total;
//      INF(m_log, stream.str());
//    }

//    return (_total == (3 * m_max_b + m_max_c + 2 * m_max_d));
//  }

//private:
//  // ###################################
//  //                     messages

//  template <char id> struct msg {
//    typedef uint64_t number;
//    explicit msg(number p_value = 1)
//        : m_counter(p_value),
//          m_up(tenacitas::number::format(std::numeric_limits<number>::min() +
//                                         p_value)),
//          m_down(tenacitas::number::format(tenacitas::number::format(
//              std::numeric_limits<number>::max() - p_value)))

//    {
//      m_d = 2.5 * p_value;
//    }

//    friend std::ostream &operator<<(std::ostream &p_out, const msg &p_msg) {
//      p_out << "(" << id << "," << p_msg.m_counter << "," << p_msg.m_up << ","
//            << p_msg.m_down << "," << p_msg.m_d << ")";
//      return p_out;
//    }
//    inline number value() const { return m_counter; }
//    inline void inc() { ++m_counter; }
//    inline msg &operator++() {
//      ++m_counter;
//      m_up = tenacitas::number::format(m_counter);
//      m_down = tenacitas::number::format(std::numeric_limits<number>::min() -
//                                         m_counter);
//      m_d = 2.5 * m_counter;
//      return *this;
//    }

//  private:
//    number m_counter{std::numeric_limits<number>::min()};
//    std::string m_up;
//    std::string m_down;
//    double m_d;
//  };

//  typedef msg<'B'> msg_b;
//  typedef msg<'C'> msg_c;
//  typedef msg<'D'> msg_d;

//  struct subscriber {

//    inline uint16_t total() const { return m_counter; }
//    inline uint16_t inc() {
//      ++m_counter;
//      return m_counter;
//    };

//  private:
//    uint16_t m_counter{0};
//  };

//  struct subscriber_b1 : public subscriber {
//    subscriber_b1(messenger_004 *p_test) : m_test(p_test) {}
//    void operator()(const msg_b &p_msg_b) {
//      INF(m_test->m_log, "B1: ", p_msg_b, " - ", inc());
//      std::this_thread::sleep_for(250ms);
//    };

//  private:
//    messenger_004 *m_test;
//  };

//  struct subscriber_b2 : public subscriber {
//    subscriber_b2(messenger_004 *p_test) : m_test(p_test) {}
//    void operator()(const msg_b &p_msg_b) {
//      INF(m_test->m_log, "B2: ", p_msg_b, " - ", inc());
//      std::this_thread::sleep_for(250ms);
//    };

//  private:
//    messenger_004 *m_test;
//  };

//  struct subscriber_b3 : public subscriber {
//    subscriber_b3(messenger_004 *p_test, char p_id)
//        : m_test(p_test), m_id(p_id) {}
//    void operator()(const msg_b &p_msg_b) {
//      INF(m_test->m_log, "B3", m_id, ": ", p_msg_b, " - ", inc());
//      std::this_thread::sleep_for(250ms);
//    };

//  private:
//    messenger_004 *m_test;
//    char m_id;
//  };

//  struct subscriber_c : public subscriber {
//    subscriber_c(messenger_004 *p_test, char p_id)
//        : m_test(p_test), m_id(p_id) {}
//    void operator()(const msg_c &p_msg_c) {
//      INF(m_test->m_log, "C", m_id, ": ", p_msg_c, " - ", inc());
//      std::this_thread::sleep_for(2s);
//    }

//    inline char get_id() const { return m_id; }

//  private:
//    messenger_004 *m_test;
//    char m_id;
//  };

//  struct subscriber_d1 : public subscriber {
//    subscriber_d1(messenger_004 *p_test) : m_test(p_test) {}

//    void operator()(const msg_d &p_msg_d) {
//      INF(m_test->m_log, "D1: ", p_msg_d, " - ", inc());
//      std::this_thread::sleep_for(500ms);
//    }

//  private:
//    messenger_004 *m_test;
//  };

//  struct subscriber_d2 : public subscriber {
//    subscriber_d2(messenger_004 *p_test) : m_test(p_test) {}

//    void operator()(const msg_d &p_msg_d) {
//      INF(m_test->m_log, "D2: ", p_msg_d, " - ", inc());
//      std::this_thread::sleep_for(1s);
//    }

//  private:
//    messenger_004 *m_test;
//  };

//private:
//  logger::cerr<> m_log{"messenger_004"};
//  std::mutex m_mutex_b;
//  std::condition_variable m_cond_b;

//  std::mutex m_mutex_c;
//  std::condition_variable m_cond_c;

//  std::mutex m_mutex_d;
//  std::condition_variable m_cond_d;

//  static const uint16_t m_max_b{5};
//  static const uint16_t m_max_c{20};
//  static const uint16_t m_max_d{2};
//};

//int main(int argc, char **argv) {
//  //  logger::set_debug_level();
//  tester::test _tester(argc, argv);

//  run_test(_tester, messenger_000);
//  run_test(_tester, messenger_001);
//  run_test(_tester, messenger_002);
//  run_test(_tester, messenger_003);
//  run_test(_tester, messenger_004);
//}
