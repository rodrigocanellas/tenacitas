/// \example

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

#include <concurrent/msg.h>
#include <tenacitas/calendar.h>
#include <tenacitas/concurrent.h>
#include <tenacitas/logger.h>
#include <tenacitas/tester.h>

using namespace tenacitas;
using namespace tenacitas::concurrent::test;
using namespace std::chrono_literals;

// static const concurrent::id g_pool_b1{"pool b1"};
// static const concurrent::id g_pool_b2{"pool b2"};
// static const concurrent::id g_pool_b3{"pool b3"};
// static const concurrent::id g_pool_c{"pool c"};
// static const concurrent::id g_pool_d1{"pool d1"};
// static const concurrent::id g_pool_d2{"pool d2"};

struct messenger_000 {

  typedef int16_t data;
  typedef logger::cerr<> log;
  typedef concurrent::sleeping_loop_t<void> sleeping_loop;
  typedef concurrent::messenger_t<data> messenger;

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

    concurrent::id _id = messenger::add_worker_pool(concurrent::priority{0},
                                                    _subscriber_timeout);

    function<void(const data &)> _subscriber =
        [this, &_data_consumed,
         _subscriber_timeout](const data &p_data) -> void {
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

    messenger::add_subscriber(_id, _subscriber);

    function<void(void)> _slepper = [this, &_data_produced]() -> void {
      if (_data_produced == _data_to_produce) {
        m_cond_producer.notify_one();
      } else {
        ++_data_produced;
        messenger::publish(_data_produced);
        DEB(m_log, "published ", _data_produced);
      }
    };

    sleeping_loop _sleeping_loop(
        300ms, decltype(_subscriber_timeout)(_subscriber_timeout.count() / 4),
        _slepper);

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
};

struct messenger_001 {
  static std::string desc() { return "compiling"; }

  bool operator()() {
    typedef concurrent::messenger_t<int16_t> messenger;

    DEB(m_log, "starting");

    auto _subscriber = [this](const int16_t &p_int) {
      DEB(m_log, "i = ", p_int);
      m_cond.notify_one();
    };

    DEB(m_log, "adding worker pool");
    concurrent::id _id =
        messenger::add_worker_pool(concurrent::priority{0}, 1s);
    DEB(m_log, "worker pool, ", _id, " added");

    DEB(m_log, "getting priority");
    std::optional<concurrent::priority> _maybe = messenger::get_priority(_id);

    if (_maybe) {
      DEB(m_log, "priority = ", *_maybe);
    }

    DEB(m_log, "resetting priority");
    messenger::set_priority(_id, concurrent::priority{9});
    DEB(m_log, "priority reset");

    DEB(m_log, "adding subscriber");
    messenger::add_subscriber(_id, _subscriber);
    DEB(m_log, "subscriber added");

    DEB(m_log, "publishing");
    messenger::publish(42);
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
  typedef concurrent::sleeping_loop_t<void> sleeping_loop;
  typedef concurrent::messenger_t<data> messenger;

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

    concurrent::id _id =
        messenger::add_worker_pool(concurrent::priority{0}, 3s);

    auto _subscriber = [this, &_data_consumed](const data &p_data) -> void {
      DEB(m_log, "consuming ", p_data);
      _data_consumed = p_data;
      this_thread::sleep_for(chrono::seconds(1));
      if (p_data == _total_to_produce) {
        m_cond_consumer.notify_one();
      }
    };

    messenger::add_subscriber(_id, _subscriber);

    auto _sleeper = [this, &_data_produced, _total_to_produce]() -> void {
      DEB(m_log, "data produced = ", _data_produced, ", total to produce ",
          _total_to_produce);
      if (_data_produced >= _total_to_produce) {
        DEB(m_log, "notifying");
        m_cond_producer.notify_one();
      } else {
        ++_data_produced;
        messenger::publish(_data_produced);
        DEB(m_log, "published data ", _data_produced);
      }
    };

    sleeping_loop _sleeping_loop(100ms, 500ms, _sleeper);

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
};

struct messenger_003 {
  static std::string desc() { return "Testing order of worker pools"; }

  bool operator()() {
    typedef concurrent::messenger_t<int16_t> messenger;

    const concurrent::id _p2{"hello"};
    messenger::add_worker_pool(_p2, concurrent::priority{2}, 4s);

    const concurrent::id _p1{"good morning"};
    messenger::add_worker_pool(_p1, concurrent::priority{5}, 1s);

    bool _first{true};
    bool _result{true};
    auto _visitor = [this, &_first, &_result, _p1,
                     _p2](const concurrent::id &p_id,
                          concurrent::priority p_priority,
                          concurrent::timeout p_timeout) {
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

    messenger::traverse(_visitor);
    return _result;
  }

private:
  logger::cerr<> m_log{"messenger_003"};
};

// struct messenger_004 {
//  static std::string desc() {
//    std::stringstream stream;

//    stream << "messengers for 'msb_b', 'xpto_c' and 'xpto_d'.\n"
//           << "3 types of subscribers for 'xpto_b', 1 for 'xpto_c', and 2 for
//           "
//              "'xpto_d'.\n"
//           << "4 instances of subscribers for 'xpto_c'.\n"
//           << p_max_b << " 'xpto_b' will be published, " << p_max_c
//           << " 'xpto_b' will be published, and " << p_max_d
//           << " 'xpto_d' will be published.\n"
//           << "None of the subscribers will timeout.";

//    return stream.str();
//  }

//  bool operator()() {
//    logger::set_info_level();

//    // creation of worker pools
//    messenger_b::add_worker_pool(g_pool_b1, concurrent::priority{100}, 2s);
//    messenger_b::add_worker_pool(g_pool_b2, concurrent::priority{50}, 2s);
//    messenger_b::add_worker_pool(g_pool_b3, concurrent::priority{10}, 2s);
//    messenger_c::add_worker_pool(g_pool_c, concurrent::priority{1}, 4s);
//    messenger_d::add_worker_pool(g_pool_d1, concurrent::priority{10}, 2s);
//    messenger_d::add_worker_pool(g_pool_d2, concurrent::priority{1}, 2s);

//    // adding subscribers to work pools
//    subscriber<'B'> _sub_b1(1);
//    messenger_b::add_subscriber(
//        g_pool_b1, [&_sub_b1](const xpto_b &p_msg) -> void { _sub_b1(p_msg);
//        });

//    subscriber<'B'> _sub_b2(2);
//    messenger_b::add_subscriber(
//        g_pool_b2, [&_sub_b2](const xpto_b &p_msg) -> void { _sub_b2(p_msg);
//        });

//    subscriber<'B'> _sub_b31(3, 1);
//    messenger_b::add_subscriber(
//        g_pool_b3,
//        [&_sub_b31](const xpto_b &p_msg) -> void { _sub_b31(p_msg); });

//    subscriber<'B'> _sub_b32(3, 2);
//    messenger_b::add_subscriber(
//        g_pool_b3,
//        [&_sub_b32](const xpto_b &p_msg) -> void { _sub_b32(p_msg); });

//    std::vector<subscriber<'C'>> _subs_c{{1, 1}, {1, 2}, {1, 3}, {1, 4}};

//    for (subscriber<'C'> &_sub_c : _subs_c) {
//      DEB(m_log, "adding sub c ", _sub_c.get_id());
//      messenger_c::add_subscriber(
//          g_pool_c, [&_sub_c](const xpto_c &p_msg) -> void { _sub_c(p_msg);
//          });
//    }

//    subscriber<'D'> _sub_d1(1);
//    messenger_d::add_subscriber(
//        g_pool_d1, [&_sub_d1](const xpto_d &p_msg) -> void { _sub_d1(p_msg);
//        });

//    subscriber<'D'> _sub_d2(1);
//    messenger_d::add_subscriber(
//        g_pool_d2, [&_sub_d2](const xpto_d &p_msg) -> void { _sub_d2(p_msg);
//        });

//    // creating publishers
//    uint16_t _count_b{0};
//    xpto_b _xpto_b;

//    uint16_t _count_c{0};
//    xpto_c _xpto_c;

//    uint16_t _count_d{0};
//    xpto_d _xpto_d;

//    publisher _publisher_b(1s, 500ms,
//                           publish<'B'>(_xpto_b, m_cond_b, _count_b,
//                           p_max_b));

//    publisher _publisher_c(1s, 300ms,
//                           publish<'C'>(_xpto_c, m_cond_c, _count_c,
//                           p_max_c));

//    publisher _publisher_d(1s, 750ms,
//                           publish<'D'>(_xpto_d, m_cond_d, _count_d,
//                           p_max_d));

//    DEB(m_log, "starting publishers");

//    _publisher_b.start();
//    _publisher_c.start();
//    _publisher_d.start();

//    {
//      DEB(m_log, "waiting for 'b'");
//      std::unique_lock<std::mutex> _lock_b(m_mutex_b);
//      m_cond_b.wait(_lock_b,
//                    [&_count_b]() -> bool { return _count_b == p_max_b; });
//    }

//    {
//      DEB(m_log, "waiting for 'c'");
//      std::unique_lock<std::mutex> _lock_c(m_mutex_c);
//      m_cond_c.wait(_lock_c,
//                    [&_count_c]() -> bool { return _count_c == p_max_c; });
//    }

//    {
//      DEB(m_log, "waiting for 'd'");
//      std::unique_lock<std::mutex> _lock_d(m_mutex_d);
//      m_cond_d.wait(_lock_d,
//                    [&_count_d]() -> bool { return _count_d == p_max_d; });
//    }

//    while (true) {
//      if ((messenger_b::occupied(g_pool_b1) == 0) &&
//          (messenger_b::occupied(g_pool_b2) == 0) &&
//          (messenger_b::occupied(g_pool_b3) == 0) &&
//          (messenger_c::occupied(g_pool_c) == 0) &&
//          (messenger_d::occupied(g_pool_d1) == 0) &&
//          (messenger_d::occupied(g_pool_d2) == 0)) {
//        break;
//      }
//      std::this_thread::sleep_for(1s);
//      DEB(m_log, "pool b1 = ", messenger_b::occupied(g_pool_b1),
//          " pool b2 = ", messenger_b::occupied(g_pool_b2),
//          " pool b3 = ", messenger_b::occupied(g_pool_b3),
//          " pool c = ", messenger_c::occupied(g_pool_c),
//          " pool d1 = ", messenger_d::occupied(g_pool_d1),
//          " pool d2 = ", messenger_b::occupied(g_pool_d2));
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

//    return (_total == (3 * p_max_b + p_max_c + 2 * p_max_d));
//  }

// private:
//  // ###################################
//  //                     messages

// private:
//  logger::cerr<> m_log{"messenger_004"};
//  std::mutex m_mutex_b;
//  std::condition_variable m_cond_b;

//  std::mutex m_mutex_c;
//  std::condition_variable m_cond_c;

//  std::mutex m_mutex_d;
//  std::condition_variable m_cond_d;

//  static const uint16_t p_max_b{5};
//  static const uint16_t p_max_c{20};
//  static const uint16_t p_max_d{2};
//};

// struct messenger_005 {
//  static std::string desc() {
//    std::stringstream stream;

//    stream << "messengers for 'xpto_c'.\n"
//           << "4 instances of subscribers for 'xpto_c'.\n"
//           << p_max_c << " 'xpto_c' will be published.\n"
//           << "None of the subscribers will timeout.";

//    return stream.str();
//  }

//  bool operator()() {
//    logger::set_debug_level();

//    typedef concurrent::sleeping_loop_t<void> publisher;

//    typedef concurrent::messenger_t<xpto_c> messenger_c;

//    const concurrent::id _pool_c{"pool c"};
//    messenger_c::add_worker_pool(_pool_c, concurrent::priority{1}, 4s);

//    std::vector<subscriber_c> _subs_c{
//        {this, '1'}, {this, '2'}, {this, '3'}, {this, '4'}};

//    for (subscriber_c &_sub_c : _subs_c) {
//      DEB(m_log, "adding sub c ", _sub_c.get_id());
//      messenger_c::add_subscriber(
//          _pool_c, [&_sub_c](const xpto_c &p_msg) -> void { _sub_c(p_msg); });
//    }

//    uint16_t _count_c{0};
//    xpto_c _xpto_c;

//    publisher _publisher_c(1s, 300ms, [this, &_count_c, &_xpto_c]() -> void {
//      if (_count_c >= p_max_c) {
//        m_cond_c.notify_one();
//        return;
//      }
//      std::this_thread::sleep_for(100ms);
//      ++_count_c;
//      ++_xpto_c;
//      DEB(m_log, "publishing C: ", _count_c, ",", _xpto_c);
//      messenger_c::publish(_xpto_c);
//    });

//    DEB(m_log, "starting publishers");

//    _publisher_c.start();

//    {
//      DEB(m_log, "waiting for 'c'");
//      std::unique_lock<std::mutex> _lock_c(m_mutex_c);
//      m_cond_c.wait(_lock_c,
//                    [&_count_c]() -> bool { return _count_c == p_max_c; });
//    }

//    while (true) {
//      if (messenger_c::occupied(_pool_c) == 0) {
//        break;
//      }
//      std::this_thread::sleep_for(1s);
//      DEB(m_log, " pool c = ", messenger_c::occupied(_pool_c));
//    }

//    uint16_t _total{0};
//    {
//      std::stringstream stream;

//      for (uint16_t _i = 0; _i < _subs_c.size(); ++_i) {
//        _total += _subs_c[_i].total();
//        stream << "sub c[" << _i << "]: " << _subs_c[_i].total() << ", ";
//      }

//      stream << "total: " << _total;
//      INF(m_log, stream.str());
//    }

//    return (_total == p_max_c);
//  }

// private:
//  // ###################################
//  //                     messages

//  struct subscriber {

//    inline uint16_t total() const { return m_counter; }
//    inline uint16_t inc() {
//      ++m_counter;
//      return m_counter;
//    };

//  private:
//    uint16_t m_counter{0};
//  };

//  struct subscriber_c : public subscriber {
//    subscriber_c(messenger_005 *p_test, char p_id)
//        : m_test(p_test), m_id(p_id) {}
//    void operator()(const xpto_c &p_xpto_c) {
//      INF(m_test->m_log, "C", m_id, ": ", p_xpto_c, " - ", inc());
//      std::this_thread::sleep_for(2s);
//    }

//    inline char get_id() const { return m_id; }

//  private:
//    messenger_005 *m_test;
//    char m_id;
//  };

// private:
//  logger::cerr<> m_log{"messenger_005"};

//  std::mutex m_mutex_c;
//  std::condition_variable m_cond_c;

//  static const uint16_t p_max_c{20};
//};

struct messenger_006 {
  static std::string desc() {
    std::stringstream _stream;
    _stream
        << "Messages 'B', 'C' and 'E.\n"
        << "Publisher #2 will publish 15 'B' messages, at each 500ms.\n"
        << "Publisher #1 will publish 12 'C' messages, at each 2ms.\n"
        << "Publisher #732 will publish 14 'E' messages, at each 100ms.\n"
        << "Message 'B' has one pool, #1, with priority 5, and timeout of 1s.\n"
        << "Message 'C' has two pools: #1 with priority 10, and timeout of 1s, "
        << "#2 with priority 2, and timeout of 1s.\n"
        << "Message 'E' has two pools: #1 with priority 10, and timeout of 1s, "
        << "#2 with priority 2, and timeout of 1s.\n"
        << "Message 'B' has two subscribers for pool #1.\n"
        << "Message 'C' has one subscribers for pool #1, and two subscribres "
        << "for pool #2.\n"
        << "Message 'E' has one subscriber for pool #1, and one subscriber "
        << "for pool #2\n"
        << "subscriber B-1-1 and B-1-2 will handle 15 messages among them\n"
        << "Subscriber C-1-1 will handle 12 messages\n"
        << "Subscriber C-2-1 and will C-2-2 12 messages among them\n"
        << "Subscriber E-2-1 will handle 14 messages\n"
        << "Subscriber E-1-2 will handle 14 messages\n";
    return _stream.str();
  }

  bool operator()() {
    logger::set_info_level();
    using namespace concurrent;
    using namespace concurrent::test;

    test_base _test("messenger_006");

    // publishers
    _test.add_publisher<'B'>(500ms, publish_id{2}, value{15});
    _test.add_publisher<'C'>(2s, publish_id{1}, value{12});
    _test.add_publisher<'E'>(100ms, publish_id{732}, value{14});

    // pools
    _test.add_pool<'B'>(pool_num{1}, priority{5}, 1s);
    _test.add_pool<'C'>(pool_num{1}, priority{10}, 1s);
    _test.add_pool<'C'>(pool_num{2}, priority{2}, 1s);
    _test.add_pool<'E'>(pool_num{1}, priority{10}, 1s);
    _test.add_pool<'E'>(pool_num{2}, priority{2}, 1s);

    // subscribers
    _test.add_subscriber<'B'>(pool_num{1}, sub_id{1});
    _test.add_subscriber<'B'>(pool_num{1}, sub_id{2});

    _test.add_subscriber<'C'>(pool_num{1}, sub_id{1});
    _test.add_subscriber<'C'>(pool_num{2}, sub_id{1});
    _test.add_subscriber<'C'>(pool_num{2}, sub_id{2});

    _test.add_subscriber<'E'>(pool_num{1}, sub_id{1});
    _test.add_subscriber<'E'>(pool_num{2}, sub_id{1});

    // start test
    return _test(1min);
  }
};

int main(int argc, char **argv) {
  logger::set_debug_level();
  tester::test _tester(argc, argv);

  run_test(_tester, messenger_000);
  run_test(_tester, messenger_001);
  run_test(_tester, messenger_002);
  run_test(_tester, messenger_003);
  //  run_test(_tester, messenger_004);
  //  run_test(_tester, messenger_005);
  run_test(_tester, messenger_006);
}
