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
      m_cond.notify_one();
    };

    messenger::pool_id _pool_id =
        messenger::add_worker_pool(messenger::priority{0}, 20, 1s);

    std::optional<messenger::priority> _maybe =
        messenger::get_priority(_pool_id);

    if (_maybe) {
      DEB(m_log, "priority = ", *_maybe);
    }

    messenger::set_priority(_pool_id, 9);

    messenger::subscribe(_pool_id, _subscriber);

    messenger::publish(42);

    {
      std::unique_lock<std::mutex> _lock(m_mutex);
      m_cond.wait(_lock);
    }

    return true;
  }

private:
  logger::cerr<> m_log{"messenger_000"};
  std::condition_variable m_cond;
  std::mutex m_mutex;
};

struct messenger_005 {

  typedef int16_t data;
  typedef logger::cerr<> log;
  typedef concurrent::sleeping_loop sleeping_loop;
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

    messenger::pool_id _pool_id = messenger::add_worker_pool(
        messenger::priority{0}, 20, _subscriber_timeout);

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

    messenger::subscribe(_pool_id, _subscriber);

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
        _slepper, []() {});

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
      ERR(m_log, "it took more time than allowed for the consumer");
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
  log m_log{"messenger_005"};
  std::mutex m_mutex_producer;
  std::condition_variable m_cond_producer;
  std::mutex m_mutex_consumer;
  std::condition_variable m_cond_consumer;
};

// struct messenger_006 {

//  typedef int16_t data;
//  typedef logger::cerr<>::log log;
//  typedef concurrent::sleeping_loop_t<log> sleeping_loop;
//  typedef concurrent::workers_t<log, data> messenger;

//  static std::string desc() {
//    std::stringstream _stream;

//    _stream << "unamed, one worker, no timeout";

//    return _stream.str();
//  }

//  bool operator()() {
//    using namespace std;

//    data _data_produced{0};
//    data _data_consumed{0};

//    messenger::subscribe(
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
//          messenger::publish(_data_produced);
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

//    messenger::stop();

//    return true;
//  }

// private:
//  log m_log{"messenger_006"};
//  std::mutex m_mutex_producer;
//  std::condition_variable m_cond_producer;
//  std::mutex m_mutex_consumer;
//  std::condition_variable m_cond_consumer;
//};

int main(int argc, char **argv) {
  logger::set_debug_level();
  tester::test _tester(argc, argv);

  run_test(_tester, messenger_000);
  run_test(_tester, messenger_005);
}
