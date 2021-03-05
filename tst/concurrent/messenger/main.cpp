/// \example

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <mutex>
#include <sstream>
#include <string>

#include <concurrent/msg_a.h>
#include <tenacitas/concurrent.h>
#include <tenacitas/logger.h>
#include <tenacitas/tester.h>

using namespace tenacitas;
using namespace std::chrono_literals;

struct messenger_000 {
  static std::string desc() { return "compiling"; }

  bool operator()() {
    typedef concurrent::messenger_t<int16_t> messenger;

    auto _subscriber = [this](const int16_t &p_int) {
      DEB(m_log, "i = ", p_int);
    };

    messenger::pool_id _pool_id =
        messenger::add_worker_pool(messenger::priority{0}, 20, 1s);

    messenger::subscribe(_pool_id, _subscriber);

    messenger::publish(42);

    return true;
  }

private:
  logger::cerr<> m_log{"messenger_000"};
};

// struct dispatcher_005 {

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

//    const chrono::seconds _timeout_subscriber{2};
//    const data _data_to_produce{30};
//    data _data_produced{0};
//    data _data_consumed{0};

//    messenger::pool_id _pool_id =
//        messenger::add_worker_pool(messenger::priority{0}, 20, 2s);

//    function<void(const data &)> _subscriber =
//        [this, &_data_consumed,
//         _timeout_subscriber](const data &p_data) -> void {
//      typedef decltype(_timeout_subscriber) time;
//      static const time _timeout{time(_timeout_subscriber.count() * 2)};

//      static const data _half_produced{static_cast<data>(_data_to_produce /
//      2)}; DEB(m_log, "consuming ", p_data); _data_consumed = p_data; if
//      (p_data == _half_produced) {
//        this_thread::sleep_for(_timeout);
//        m_cond_consumer.notify_one();
//      }
//    };

//    messenger::subscribe(_pool_id, _subscriber);

//    function<void(void)> _slepper = [this, &_data_produced]() -> void {
//      ++_data_produced;
//      messenger::publish(_data_produced);
//      DEB(m_log, "published ", _data_produced);
//      if (_data_produced == _data_to_produce) {
//        m_cond_producer.notify_one();
//      }
//    };

//    sleeping_loop _sleeping_loop(300ms, 1s, _slepper, []() {});

//    _sleeping_loop.start();

//    DEB(m_log, "waiting for the producer to notify");
//    unique_lock<mutex> _lock_producer(m_mutex_producer);
//    m_cond_producer.wait(_lock_producer);
//    DEB(m_log, "producer notified");

//    _sleeping_loop.stop();

//    DEB(m_log, "last data produced = ", _data_produced);

//    DEB(m_log, "waiting for the consumer to notify");
//    unique_lock<mutex> _lock_consumer(m_mutex_consumer);
//    cv_status _status =
//        m_cond_consumer.wait_for(_lock_consumer, chrono::seconds(65));
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

// private:
//  log m_log{"dispatcher_005"};
//  std::mutex m_mutex_producer;
//  std::condition_variable m_cond_producer;
//  std::mutex m_mutex_consumer;
//  std::condition_variable m_cond_consumer;
//};

// struct dispatcher_006 {

//  typedef int16_t data;
//  typedef logger::cerr<>::log log;
//  typedef concurrent::sleeping_loop_t<log> sleeping_loop;
//  typedef concurrent::workers_t<log, data> dispatcher;

//  static std::string desc() {
//    std::stringstream _stream;

//    _stream << "unamed, one worker, no timeout";

//    return _stream.str();
//  }

//  bool operator()() {
//    using namespace std;

//    data _data_produced{0};
//    data _data_consumed{0};

//    dispatcher::subscribe(
//        [this, &_data_consumed, &_data_produced](data &&p_data) -> void {
//          DEB(m_log, "consuming ", p_data);
//          _data_consumed = p_data;
//          this_thread::sleep_for(chrono::seconds(2));
//          if (p_data == _data_produced) {
//            m_cond_consumer.notify_one();
//          }
//        });

//    sleeping_loop _sleeping_loop(
//        chrono::seconds(1), [this, &_data_produced]() -> void {
//          ++_data_produced;
//          dispatcher::publish(_data_produced);
//          DEB(m_log, "published ", _data_produced);
//          if (_data_produced == 30) {
//            m_cond_producer.notify_one();
//          }
//        });

//    _sleeping_loop.start();

//    DEB(m_log, "waiting for the producer to notify");
//    unique_lock<mutex> _lock_producer(m_mutex_producer);
//    m_cond_producer.wait(_lock_producer);
//    DEB(m_log, "producer notified");

//    _sleeping_loop.stop();

//    DEB(m_log, "last data produced = ", _data_produced);

//    DEB(m_log, "waiting for the consumer to notify");
//    unique_lock<mutex> _lock_consumer(m_mutex_consumer);
//    cv_status _status =
//        m_cond_consumer.wait_for(_lock_consumer, chrono::seconds(65));
//    DEB(m_log, "consumer notified");

//    if (_status == cv_status::timeout) {
//      ERR(m_log,
//                       "it took more time than allowed for the consumer");
//      return false;
//    }

//    if (_data_consumed != _data_produced) {
//      ERR(m_log, "data consumed = ", _data_consumed,
//                       ", but it should be ", _data_produced);
//      return false;
//    }

//    DEB(m_log, "data consumed = ", _data_consumed,
//                     ", and is equal to data produced = ", _data_produced);

//    dispatcher::stop();

//    return true;
//  }

// private:
//  log m_log{"dispatcher_006"};
//  std::mutex m_mutex_producer;
//  std::condition_variable m_cond_producer;
//  std::mutex m_mutex_consumer;
//  std::condition_variable m_cond_consumer;
//};

int main(int argc, char **argv) {
  logger::set_debug_level();
  tester::test _tester(argc, argv);

  run_test(_tester, messenger_000);
}
