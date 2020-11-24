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

#include <concurrent/circular_fixed_size_queue.h>
#include <concurrent/dispatcher.h>
#include <concurrent/internal/log.h>
#include <concurrent/sleeping_loop.h>
#include <concurrent/timeout_callback.h>
#include <logger/cerr/log.h>
#include <tester/test.h>

using namespace tenacitas;

typedef logger::cerr::log log;

concurrent::timeout_callback _timeout_callback =
    [](std::thread::id p_id) -> void {
  log _log{"timeout_callback"};
  concurrent_warn(_log, "timeout for ", p_id);
};

struct dispatcher_100 {
  static std::string desc() {
    std::stringstream _stream;

    _stream << "one worker, no timeout";

    return _stream.str();
  }

  bool operator()() {
    typedef int16_t data;
    typedef concurrent::sleeping_loop_t<log> sleeping_loop;
    typedef concurrent::dispatcher_t<log, data> dispatcher;

    data _data_produced{0};
    data _data_consumed{0};

    dispatcher::subscribe(
        [this, &_data_consumed, &_data_produced](data &&p_data) -> void {
          concurrent_debug(m_log, "consuming ", p_data);
          _data_consumed = p_data;
          std::this_thread::sleep_for(std::chrono::seconds(2));
          if (p_data == _data_produced) {
            m_cond_consumer.notify_one();
          }
        });

    sleeping_loop _sleeping_loop(
        std::chrono::seconds(1), [this, &_data_produced]() -> void {
          ++_data_produced;
          dispatcher::publish(_data_produced);
          concurrent_debug(m_log, "published ", _data_produced);
          if (_data_produced == 30) {
            m_cond_producer.notify_one();
          }
        });

    _sleeping_loop.start();

    concurrent_debug(m_log, "waiting for the producer to notify");
    std::unique_lock<std::mutex> _lock_producer(m_mutex_producer);
    m_cond_producer.wait(_lock_producer);
    concurrent_debug(m_log, "producer notified");

    _sleeping_loop.stop();

    concurrent_debug(m_log, "        last data produced = ", _data_produced);

    concurrent_debug(m_log, "waiting for the consumer to notify");
    std::unique_lock<std::mutex> _lock_consumer(m_mutex_consumer);
    std::cv_status _status =
        m_cond_consumer.wait_for(_lock_consumer, std::chrono::seconds(65));
    concurrent_debug(m_log, "consumer notified");

    if (_status == std::cv_status::timeout) {
      concurrent_error(m_log,
                       "it took more time than allowed for the consumer");
      return false;
    }

    if (_data_consumed != _data_produced) {
      concurrent_error(m_log, "data consumed = ", _data_consumed,
                       ", but it should be ", _data_produced);
      return false;
    }

    concurrent_debug(m_log, "data consumed = ", _data_consumed,
                     ", and is equal to data produced = ", _data_produced);

    dispatcher::stop();

    return true;
  }

private:
  log m_log{"dispatcher_100"};
  std::mutex m_mutex_producer;
  std::condition_variable m_cond_producer;
  std::mutex m_mutex_consumer;
  std::condition_variable m_cond_consumer;
};

// struct dispatcher_004 {

//  static std::string desc() { return ""; }

//  // ############################## messages
//  struct msg_a {
//    friend std::ostringstream &operator<<(std::ostringstream &p_out,
//                                          const msg_a &p_msg) {
//      p_out << "|msg a|i|" << p_msg.i;
//      return p_out;
//    }

//    explicit msg_a(int16_t p_i = 0) : i(p_i) {}

//    int16_t i = {0};
//  };

//  typedef concurrent::fixed_size_queue_t<logger::cerr::log, msg_a> queue_a;

//  typedef concurrent::dispatcher_t<logger::cerr::log,
//  std::chrono::milliseconds,
//                                   queue_a>
//      dispatcher_a;

//  struct subscriber_1 {
//    void operator()(msg_a &&p_msg) { concurrent_debug(m_log, "S 1", p_msg); }
//    logger::cerr::log m_log{"dispatcher_004::subscriber_1"};
//  };

//  struct publisher_1 {
//    void operator()() {

//      msg_a _msg(++i);
//      concurrent_debug(m_log, "P 1", _msg);

//      dispatcher_a::publish(++i);
//    }

//  private:
//    logger::cerr::log m_log{"dispatcher_004::publisher_1"};
//    int16_t i = {10};
//  };

//  typedef concurrent::sleeping_loop_t<
//      logger::cerr::log, std::chrono::milliseconds, std::chrono::seconds>
//      sleeping_loop;

//  bool operator()() {
//    sleeping_loop _sl1(std::chrono::milliseconds(200),
//    std::chrono::seconds(1),
//                       publisher_1(), _timeout_callback);

//    bool _rc = true;
//    std::chrono::milliseconds _work_timeout(500);

//    dispatcher_a::create("a1", 50);

//    dispatcher_a::subscribe("a1", subscriber_1(), _work_timeout);

//    _sl1.start();

//    concurrent_debug(m_log, "------> starting to sleep");
//    std::this_thread::sleep_for(std::chrono::seconds(10));
//    concurrent_debug(m_log, "------> waking up");
//    return _rc;

//    return _rc;
//  }

// private:
//  logger::cerr::log m_log{"dispatcher_004"};
//};

//// ############################## subscribers
// struct dispatcher_000 {
//  static const std::string desc() {
//    return "\n3 types of messages, 4 subscribers, 3 publishers."

//           "\n\nMessage 'a' has an int16_t attribute; message 'b' has an "
//           "int32_t attribute; message 'c' has a double attribute."

//           "\n\nSubscriber 1 for message 'a', with two instances; subscriber 2
//           " "for messages 'a' and 'b'; subscriber 3 for message 'b';
//           subscriber " "4 for messages 'a', 'b' and 'c'."

//           "\n\nPublisher 1 publishes 'a', starting at 10, increasing 1 at "
//           "each publishing; publisher 2 publishes 'a' stating at -100, and "
//           "incrementing 10, and 'c', starting at 1.0 and multiplying times "
//           "2.5; publisher 3 publishes 'b', stating at 5000, and incrementing
//           " "300."

//           "\n\nA 'sleeping_loop' will call Publisher 1 to send messages at "
//           "each 1000ms; another 'sleeping_loop' will call Publisher 2 to send
//           " "messages at each 2000ms; and a third 'sleeping_loop' will call "
//           "Publisher 3 to send messages at each 1500ms."

//           "\n\nMain thread will sleep for 10 seconds, while publishers send "
//           "messages.";
//  }

//  // ############################## messages
//  struct msg_a {
//    friend std::ostringstream &operator<<(std::ostringstream &p_out,
//                                          const msg_a &p_msg) {
//      p_out << "|msg a|i|" << p_msg.i;
//      return p_out;
//    }

//    explicit msg_a(int16_t p_i = 0) : i(p_i) {}

//    int16_t i = {0};
//  };

//  struct msg_b {
//    friend std::ostringstream &operator<<(std::ostringstream &p_out,
//                                          const msg_b &p_msg) {
//      p_out << "|msg b|i|" << p_msg.i;
//      return p_out;
//    }

//    explicit msg_b(int32_t p_i = 0) : i(p_i) {}
//    int32_t i = {0};
//  };

//  struct msg_c {
//    friend std::ostringstream &operator<<(std::ostringstream &p_out,
//                                          const msg_c &p_msg) {
//      p_out << "|msg c|d|" << p_msg.d;
//      return p_out;
//    }

//    explicit msg_c(double p_d = 0.0)
//        : d(p_d)

//    {}

//    double d = {0.0};
//  };

//  typedef concurrent::fixed_size_queue_t<logger::cerr::log, msg_a> queue_a;
//  typedef concurrent::fixed_size_queue_t<logger::cerr::log, msg_b> queue_b;
//  typedef concurrent::fixed_size_queue_t<logger::cerr::log, msg_c> queue_c;

//  typedef concurrent::dispatcher_t<logger::cerr::log,
//  std::chrono::milliseconds,
//                                   queue_a>
//      dispatcher_a;

//  typedef concurrent::dispatcher_t<logger::cerr::log,
//  std::chrono::milliseconds,
//                                   queue_b>
//      dispatcher_b;

//  typedef concurrent::dispatcher_t<logger::cerr::log,
//  std::chrono::milliseconds,
//                                   queue_c>
//      dispatcher_c;

//  // ############################## publishers
//  struct publisher_1 {

//  public:
//    void operator()() {

//      msg_a _msg(++i);
//      concurrent_debug(m_log, "P 1", _msg);

//      dispatcher_a::publish(++i);
//    }
//    logger::cerr::log m_log{"dispatcher_000::publisher_1"};
//    int16_t i = {10};
//  };

//  struct publisher_2 {
//  public:
//    void operator()() {

//      i += 10;
//      msg_a _msg_a(i);
//      concurrent_debug(m_log, "P 2", _msg_a);
//      dispatcher_a::publish(_msg_a);

//      d *= 2.5;
//      msg_c _msg_c(d);
//      concurrent_debug(m_log, "P 2", _msg_c);
//      dispatcher_c::publish(_msg_c);
//    }
//    logger::cerr::log m_log{"dispatcher_000::publisher_2"};
//    int16_t i = {-100};
//    double d = {1.0};
//  };

//  struct publisher_3 {
//  public:
//    void operator()() {

//      i += 300;
//      msg_b _msg_b(i);

//      concurrent_debug(m_log, "P 3", _msg_b);
//      dispatcher_b::publish(_msg_b);
//    }
//    logger::cerr::log m_log{"dispatcher_000::publisher_3"};
//    int32_t i = {5000};
//  };

//  // ############################## subscribers
//  struct subscriber_1 {
//    void operator()(msg_a &&p_msg) { concurrent_debug(m_log, "S 1", p_msg); }
//    logger::cerr::log m_log{"dispatcher_000::subscriber_1"};
//  };

//  struct subscriber_2 {
//    void operator()(msg_a &&p_msg) { concurrent_debug(m_log, "S 2", p_msg); }

//    void operator()(msg_b &&p_msg) { concurrent_debug(m_log, "S 2", p_msg); }
//    logger::cerr::log m_log{"dispatcher_000::subscriber_2"};
//  };

//  struct subscriber_3 {
//    void operator()(msg_b &&p_msg) { concurrent_debug(m_log, "S 3", p_msg); }
//    logger::cerr::log m_log{"dispatcher_000::subscriber_3"};
//  };

//  struct subscriber_4 {
//    void operator()(msg_a &&p_msg) { concurrent_debug(m_log, "S 4", p_msg); }

//    void operator()(msg_b &&p_msg) { concurrent_debug(m_log, "S 4", p_msg); }

//    void operator()(msg_c &&p_msg) { concurrent_debug(m_log, "S 4", p_msg); }

//    logger::cerr::log m_log{"dispatcher_000::subscriber_4"};
//  };

//  typedef concurrent::sleeping_loop_t<
//      logger::cerr::log, std::chrono::milliseconds, std::chrono::seconds>
//      sleeping_loop;

//  bool operator()() {

//    sleeping_loop _sl1(std::chrono::milliseconds(200),
//    std::chrono::seconds(1),
//                       publisher_1(), _timeout_callback);
//    sleeping_loop _sl2(std::chrono::milliseconds(200),
//    std::chrono::seconds(1),
//                       publisher_2(), _timeout_callback);
//    sleeping_loop _sl3(std::chrono::milliseconds(200),
//    std::chrono::seconds(1),
//                       publisher_3(), _timeout_callback);

//    bool _rc = true;
//    std::chrono::milliseconds _work_timeout(500);

//    dispatcher_a::create("a1", queue_a(50));
//    dispatcher_a::create("a2", queue_a(90));
//    dispatcher_a::create("a3", queue_a(25));

//    dispatcher_b::create("b1", queue_b(100));
//    dispatcher_b::create("b2", queue_b(40));
//    dispatcher_b::create("b3", queue_b(75));

//    dispatcher_c::create("c1", queue_c(65));

//    dispatcher_a::subscribe(
//        "a1", []() { return subscriber_1(); }, 2, _work_timeout);

//    dispatcher_a::subscribe("a2", subscriber_2(), _work_timeout);
//    dispatcher_a::subscribe("a3", subscriber_4(), _work_timeout);
//    dispatcher_b::subscribe("b1", subscriber_2(), _work_timeout);
//    dispatcher_b::subscribe("b2", subscriber_3(), _work_timeout);
//    dispatcher_b::subscribe("b3", subscriber_4(), _work_timeout);
//    dispatcher_c::subscribe("c1", subscriber_4(), _work_timeout);

//    _sl1.start();
//    _sl2.start();
//    _sl3.start();

//    concurrent_debug(m_log, "------> starting to sleep");
//    std::this_thread::sleep_for(std::chrono::seconds(10));
//    concurrent_debug(m_log, "------> waking up");
//    return _rc;
//  }

// private:
//  logger::cerr::log m_log{"dispatcher_000"};
//};

//// ############################## subscribers
// struct dispatcher_001 {

//  static const std::string desc() {
//    return "\nTesting asynchronous work collaboration.";
//  }

//  // ############################## messages
//  struct request {
//    friend std::ostringstream &operator<<(std::ostringstream &p_out,
//                                          const request &p_msg) {
//      p_out << "|msg a|i|" << p_msg.i;
//      return p_out;
//    }

//    explicit request(int16_t p_i = 0) : i(p_i) {}

//    int32_t i = {0};
//  };

//  struct reply {
//    friend std::ostringstream &operator<<(std::ostringstream &p_out,
//                                          const reply &p_msg) {
//      p_out << "|msg a|i|" << p_msg.i;
//      return p_out;
//    }

//    explicit reply(int16_t p_i = 0) : i(p_i) {}

//    int32_t i = {0};
//  };

//  typedef concurrent::fixed_size_queue_t<logger::cerr::log, request>
//      request_queue;

//  typedef concurrent::fixed_size_queue_t<logger::cerr::log, reply>
//  reply_queue;

//  typedef concurrent::dispatcher_t<logger::cerr::log,
//  std::chrono::milliseconds,
//                                   request_queue>
//      request_dispatcher;

//  typedef concurrent::dispatcher_t<logger::cerr::log,
//  std::chrono::milliseconds,
//                                   reply_queue>
//      reply_dispatcher;

//  // ############################## processors
//  struct requester {
//    void start() { request_dispatcher::publish(request(time(nullptr))); }

//    void operator()(reply &&p_reply) {

//      concurrent_debug(m_log, "reply ", p_reply);
//      if (m_counter++ > 100) {
//        concurrent_info(m_log, "counter = ", m_counter, ", stopping");
//        request_dispatcher::publish(request(0));
//        return;
//      }
//      if ((p_reply.i % 2) == 0) {
//        request_dispatcher::publish(request(5 * time(nullptr)));
//      } else {
//        request_dispatcher::publish(request(3 * time(nullptr)));
//      }
//    }

//  private:
//    logger::cerr::log m_log{"dispatcher_001::requester"};
//    int16_t m_counter = {0};
//  };

//  struct replier {
//    void operator()(request &&p_request) {

//      concurrent_debug(m_log, "request ", p_request);
//      if (p_request.i == 0) {
//        concurrent_info(m_log, "stopping");
//        return;
//      }
//      if ((p_request.i % 2) == 0) {
//        reply_dispatcher::publish(reply(4 * time(nullptr)));
//      } else {
//        reply_dispatcher::publish(reply(4 * time(nullptr)));
//      }
//    }
//    logger::cerr::log m_log{"dispatcher_001::replier"};
//  };

//  bool operator()() {

//    std::chrono::milliseconds _work_timeout(500);
//    reply_dispatcher::subscribe("req", requester(), reply_queue(40),
//                                _work_timeout);
//    request_dispatcher::subscribe("rep", replier(), request_queue(40),
//                                  _work_timeout);

//    requester().start();

//    concurrent_debug(m_log, "------> starting to sleep");
//    std::this_thread::sleep_for(std::chrono::seconds(10));
//    concurrent_debug(m_log, "------> waking up");

//    return true;
//  }

//  logger::cerr::log m_log{"dispatcher_001"};
//};

// struct dispatcher_002 {

//  // ############################## messages
//  struct request {
//    friend std::ostringstream &operator<<(std::ostringstream &p_out,
//                                          const request &p_msg) {
//      p_out << "|msg a|i|" << p_msg.i;
//      return p_out;
//    }

//    explicit request(uint32_t p_i = 0) : i(p_i) {}

//    uint32_t i = {0};
//  };

//  struct reply {
//    friend std::ostringstream &operator<<(std::ostringstream &p_out,
//                                          const reply &p_msg) {
//      p_out << "|msg a|i|" << p_msg.i;
//      return p_out;
//    }

//    explicit reply(uint32_t p_i = 0) : i(p_i) {}

//    uint32_t i = {0};
//  };

//  typedef concurrent::fixed_size_queue_t<logger::cerr::log, request>
//      request_queue;

//  typedef concurrent::fixed_size_queue_t<logger::cerr::log, reply>
//  reply_queue;

//  typedef concurrent::dispatcher_t<logger::cerr::log,
//  std::chrono::milliseconds,
//                                   request_queue>
//      request_dispatcher;

//  typedef concurrent::dispatcher_t<logger::cerr::log,
//  std::chrono::milliseconds,
//                                   reply_queue>
//      reply_dispatcher;

//  // ############################## processors
//  struct requester {
//    requester(std::condition_variable *p_cond, std::mutex *p_mutex)
//        : m_cond(p_cond), m_mutex(p_mutex) {}

//    bool operator()(reply &&p_reply) {

//      concurrent_info(m_log, "reply ", p_reply);
//      if (m_counter++ > 100) {
//        concurrent_info(m_log, "counter = ", m_counter, ", stopping");
//        request_dispatcher::publish(request(0));
//        concurrent_info(m_log, "sleeping...");
//        std::this_thread::sleep_for(std::chrono::seconds(8));
//        concurrent_info(m_log, "woke up!");
//        std::unique_lock<std::mutex> _lock(*m_mutex);
//        m_cond->notify_all();
//        return false;
//      }
//      if ((p_reply.i % 2) == 0) {
//        request_dispatcher::publish(
//            request(5 * static_cast<uint32_t>(time(nullptr))));
//      } else {
//        request_dispatcher::publish(
//            request(3 * static_cast<uint32_t>(time(nullptr))));
//      }
//      return true;
//    }

//  private:
//    logger::cerr::log m_log{"dispatcher_002::requester"};
//    int16_t m_counter = {0};
//    std::condition_variable *m_cond;
//    std::mutex *m_mutex;
//  };

//  struct replier {
//    bool operator()(request &&p_request) {

//      concurrent_info(m_log, "request ", p_request);
//      if (p_request.i == 0) {
//        concurrent_info(m_log, "stopping");
//        return false;
//      }
//      if ((p_request.i % 2) == 0) {
//        reply_dispatcher::publish(
//            reply(4 * static_cast<uint32_t>(time(nullptr))));
//      } else {
//        reply_dispatcher::publish(
//            reply(4 * static_cast<uint32_t>(time(nullptr))));
//      }
//      return true;
//    }
//    logger::cerr::log m_log{"dispatcher_002::replier"};
//  };

//  bool operator()() {

//    std::chrono::milliseconds _work_timeout(15000);
//    reply_dispatcher::subscribe("req", requester(&m_cond, &m_mutex),
//                                reply_queue(30), _work_timeout);
//    request_dispatcher::subscribe("rep", replier(), request_queue(30),
//                                  _work_timeout);

//    start();

//    concurrent_info(m_log, "------> waiting...");
//    std::unique_lock<std::mutex> _lock(m_mutex);
//    m_cond.wait(_lock);
//    concurrent_info(m_log, "------> done!");

//    return true;
//  }

//  static const std::string desc() {
//    return "\nTesting asynchronous work collaboration, and ending with "
//           "message.";
//  }

// private:
//  logger::cerr::log m_log{"dispatcher_002"};
//  bool start() {
//    request_dispatcher::publish(request(static_cast<uint32_t>(time(nullptr))));
//    return true;
//  }

// private:
//  std::condition_variable m_cond;
//  std::mutex m_mutex;
//};

//// ############################## main
// struct dispatcher_003 {

//  // ############################## loops
//  typedef concurrent::sleeping_loop_t<
//      logger::cerr::log, std::chrono::milliseconds, std::chrono::milliseconds>
//      sync_loop;
//  typedef concurrent::sleeping_loop_t<
//      logger::cerr::log, std::chrono::milliseconds, std::chrono::milliseconds>
//      rx_loop;

//  // ############################## messages

//  struct end {};

//  struct msg_tpdo {
//    msg_tpdo() { ++counter; }

//    friend std::ostream &operator<<(std::ostream &p_out,
//                                    const msg_tpdo & /*p_msg*/) {
//      p_out << static_cast<uint64_t>(
//          std::chrono::duration_cast<std::chrono::microseconds>(
//              std::chrono::high_resolution_clock::now().time_since_epoch())
//              .count());
//      return p_out;
//    }
//    static uint16_t counter;
//  };

//  struct sync {
//    void operator()() { concurrent_debug(m_log, "sync"); }
//    logger::cerr::log m_log{"dispatcher_003::sync"};
//  };

//  typedef concurrent::fixed_size_queue_t<logger::cerr::log, msg_tpdo>
//      tpdo_queue;

//  typedef concurrent::dispatcher_t<logger::cerr::log,
//  std::chrono::milliseconds,
//                                   tpdo_queue>
//      tpdo_dispatcher;

//  // ############################## publishers
//  struct rx {
//    void operator()() {
//      concurrent_debug(m_log, "rx");
//      tpdo_dispatcher::publish(msg_tpdo());
//      tpdo_dispatcher::publish(msg_tpdo());
//      tpdo_dispatcher::publish(msg_tpdo());
//      tpdo_dispatcher::publish(msg_tpdo());
//      tpdo_dispatcher::publish(msg_tpdo());
//      tpdo_dispatcher::publish(msg_tpdo());
//      tpdo_dispatcher::publish(msg_tpdo());
//      tpdo_dispatcher::publish(msg_tpdo());
//      tpdo_dispatcher::publish(msg_tpdo());
//      tpdo_dispatcher::publish(msg_tpdo());
//      tpdo_dispatcher::publish(msg_tpdo());
//      tpdo_dispatcher::publish(msg_tpdo());
//      tpdo_dispatcher::publish(msg_tpdo());
//    }
//    logger::cerr::log m_log{"dispatcher_003::rx"};
//  };

//  // ############################## handler
//  struct tx {
//    tx() : m_id(++m_counter) {}

//    void operator()(msg_tpdo &&p_msg) {
//      concurrent_debug(m_log, m_id, "|", p_msg);
//    }

//  private:
//    logger::cerr::log m_log{"dispatcher_003::tx"};
//    uint16_t m_id = 0;
//    static uint16_t m_counter;
//  };

//  dispatcher_003() {}

//  bool operator()() {
//    //    sync_loop _sync(
//    //      std::chrono::milliseconds(100), sync(),
//    //      std::chrono::milliseconds(2));

//    auto _timeout_callback = [](std::thread::id) -> void {};

//    rx_loop _rx(std::chrono::milliseconds(40), std::chrono::milliseconds(15),
//                rx(), _timeout_callback);

//    tpdo_dispatcher::subscribe("tx", tx(), tpdo_queue(200),
//                               std::chrono::milliseconds(10));

//    //    _sync.start();
//    _rx.start();

//    std::this_thread::sleep_for(std::chrono::milliseconds(122));

//    return true;
//  }

//  static const std::string desc() {
//    return "\nTest inspired in a case brought by Fabio Costa";
//  }

//  static const std::string name() { return "dispatcher_003"; }
//};

// uint16_t dispatcher_003::msg_tpdo::counter = 0;
// uint16_t dispatcher_003::tx::m_counter = 0;

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();

  tester::test _tester(argc, argv);

  run_test(_tester, dispatcher_100);

  //  run_test(_tester, dispatcher_000);
  //  run_test(_tester, dispatcher_001);
  //  run_test(_tester, dispatcher_002);
  //  run_test(_tester, dispatcher_003);
  //  run_test(_tester, dispatcher_004);
}
