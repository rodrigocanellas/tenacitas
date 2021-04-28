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

struct messenger_006 {
  static std::string desc() {
    std::stringstream _stream;
    _stream
        << "Messages 'B', 'C' and 'E.\n"
        << "Publisher #2 will publish 15 'B' messages, at each 500ms.\n"
        << "Publisher #1 will publish 12 'C' messages, at each 2ms.\n"
        << "Publisher #732 will publish 14 'E' messages, at each 100ms.\n"
        << "Message 'B' has one pool, #1, with priority 5, and timeout of "
        << "1s.\n "
        << "Message 'C' has two pools: #1 with priority 10, and timeout of "
        << "1s, #2 with priority 2, and timeout of 1s.\n"
        << "Message 'E' has two pools: #1 with priority 10, and timeout of "
        << "1s, #2 with priority 2, and timeout of 1s.\n"
        << "Message 'B' has two subscribers for pool #1.\n"
        << "Message 'C' has one subscribers for pool #1, and two subscribres "
        << "for pool #2.\n"
        << "Message 'E' has one subscriber for pool #1, and one subscriber "
        << "for pool #2\n"
        << "Subscriber B-1-1 and B-1-2 will handle 15 messages among them.\n"
        << "Subscriber C-1-1 will handle 12 messages.\n"
        << "Subscriber C-2-1 and will C-2-2 12 messages among them.\n"
        << "Subscriber E-2-1 will handle 14 messages.\n"
        << "Subscriber E-1-2 will handle 14 messages.\n";
    return _stream.str();
  }

  bool operator()() {
    //    logger::set_info_level();
    using namespace async;

    test_base _test("messenger_006");

    // publishers
    _test.add_publisher<'B'>(m_id, 500ms, publish_id{2}, value{15});
    _test.add_publisher<'C'>(m_id, 2s, publish_id{1}, value{12});
    _test.add_publisher<'E'>(m_id, 100ms, publish_id{732}, value{14});

    // pools
    _test.add_pool<'B'>(1s, priority{priority::low});
    _test.add_pool<'C'>(1s, priority{priority::middle});
    _test.add_pool<'C'>(1s, priority{priority::low});
    _test.add_pool<'E'>(1s, priority{priority::middle});
    _test.add_pool<'E'>(1s, priority{priority::low});

    // subscribers
    _test.add_subscriber<'B'>(pool_num{1}, sub_id{1});
    _test.add_subscriber<'B'>(pool_num{1}, sub_id{2});

    _test.add_subscriber<'C'>(pool_num{1}, sub_id{1});
    _test.add_subscriber<'C'>(pool_num{2}, sub_id{1});
    _test.add_subscriber<'C'>(pool_num{2}, sub_id{2});

    _test.add_subscriber<'E'>(pool_num{1}, sub_id{1});
    _test.add_subscriber<'E'>(pool_num{2}, sub_id{1});

    bool _res = _test(1min);

    // start test
    if (_res) {
      const updates &_updates = _test.get_updates();
      for (const update &_update : _updates) {
        INF(m_log, _update);
      }
    }
    return _res;
  }

private:
  logger::cerr<> m_log{"messenger_006"};
  number::id m_id;
};

// struct messenger_007 {
//  static std::string desc() {
//    std::stringstream _stream;
//    _stream
//        << "Messages 'B', 'C' and 'E.\n"
//        << "Publisher #2 will publish 15 'B' messages, at each 500ms.\n"
//        << "Publisher #1 will publish 12 'C' messages, at each 2ms.\n"
//        << "Publisher #732 will publish 14 'E' messages, at each 100ms.\n"
//        << "Message 'B' has one pool, #1, with priority 5, and timeout of
//        1s.\n"
//        << "Message 'C' has two pools: #1 with priority 10, and timeout of 1s,
//        "
//        << "#2 with priority 2, and timeout of 1s.\n"
//        << "Message 'E' has two pools: #1 with priority 10, and timeout of 1s,
//        "
//           "#2 with priority 2, and timeout of 1s.\n"
//        << "Message 'B' has two subscribers for pool #1.\n"
//        << "Message 'C' has one subscribers for pool #1, and two subscribres "
//           "for pool #2.\n"
//        << "Message 'E' has one subscriber for pool #1, and one subscriber for
//        "
//           "pool #2.\n"
//        << "Subscriber B-1-1 and B-1-2 will handle 15 messages among them.\n"
//        << "Subscriber C-1-1 will handle 12 messages.\n"
//        << "Subscriber C-2-1 will sleep for 2s, causing timeout for all the "
//           "messages. Those messages will be inserted again in the pool C-2, "
//           "so C-2-2 will handle all 12 messages.\n"
//        << "Subscriber C-2-1 will handle 12 messages among them.\n"
//        << "Subscriber E-2-1 will handle 14 messages.\n"
//        << "Subscriber E-1-2 will handle 14 messages.\n";

//    return _stream.str();
//  }

//  bool operator()() {
//    logger::set_info_level();
//    using namespace async;

//    test_base _test("messenger_006");

//    // publishers
//    _test.add_publisher<'B'>(m_id, 500ms, publish_id{2}, value{15});
//    _test.add_publisher<'C'>(m_id, 2s, publish_id{1}, value{12});
//    _test.add_publisher<'E'>(m_id, 100ms, publish_id{732}, value{14});

//    // pools
//    _test.add_pool<'B'>(priority{priority::low}, 1s);
//    _test.add_pool<'C'>(priority{priority::low_middle}, 1s);
//    _test.add_pool<'C'>(priority{priority::low}, 1s);
//    _test.add_pool<'E'>(priority{priority::low_middle}, 1s);
//    _test.add_pool<'E'>(priority{priority::low}, 1s);

//    // subscribers
//    _test.add_subscriber<'B'>(pool_num{1}, sub_id{1});
//    _test.add_subscriber<'B'>(pool_num{1}, sub_id{2});

//    _test.add_subscriber<'C'>(pool_num{1}, sub_id{1});
//    _test.add_subscriber<'C'>(pool_num{2}, sub_id{1}, 2s);
//    _test.add_subscriber<'C'>(pool_num{2}, sub_id{2});

//    _test.add_subscriber<'E'>(pool_num{1}, sub_id{1});
//    _test.add_subscriber<'E'>(pool_num{2}, sub_id{1});

//    bool _res = _test(1min);

//    // start test
//    if (_res) {
//      const updates &_updates = _test.get_updates();
//      for (const update &_update : _updates) {
//        INF(m_log, _update);
//      }
//    }
//    return _res;
//  }

// private:
//  logger::cerr<> m_log{"messenger_007"};
//  number::id m_id;
//};

int main(int argc, char **argv) {
  logger::set_debug_level();
  tester::test _tester(argc, argv);

  run_test(_tester, messenger_000);
  run_test(_tester, messenger_001);
  run_test(_tester, messenger_002);
  run_test(_tester, messenger_003);
  run_test(_tester, messenger_004);
  run_test(_tester, messenger_006);
  //  run_test(_tester, messenger_007);
}
