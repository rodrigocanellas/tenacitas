/// \example Basic use of tenacitas::concurrent::loop_t

#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>

#include <concurrent/circular_queue.h>
#include <concurrent/internal/log.h>
#include <concurrent/loop.h>
#include <concurrent/result.h>
#include <logger/cerr/log.h>
#include <status/result.h>
#include <tester/test.h>

using namespace tenacitas;
typedef tenacitas::logger::cerr::log log;

void printer(const int16_t &p_i) { std::cout << p_i << " "; }

typedef concurrent::circular_queue_t<int16_t, 5, log> queue;

struct producer {
  producer(queue *p_queue) : m_queue(p_queue) {}

  void operator()() {
    concurrent_log_debug(log, "starting producer");
    while (true) {
      if (m_stop) {
        break;
      }
      m_queue->add(m_i++);
      std::this_thread::sleep_for(std::chrono::milliseconds(400));
    }
  }

  void stop() { m_stop = true; }

private:
  queue *m_queue = nullptr;
  int16_t m_i = 100;
  bool m_stop = false;
};

struct consumer {
  consumer(queue *p_queue) : m_queue(p_queue) {}

  void operator()() {
    concurrent_log_debug(log, "starting consumer");
    while (true) {
      if (m_stop) {
        break;
      }
      concurrent_log_debug(log, "got ", m_queue->get());
      std::this_thread::sleep_for(std::chrono::milliseconds(950));
    }
  }

  void stop() { m_stop = true; }

private:
  queue *m_queue = nullptr;
  bool m_stop = false;
};

struct producer_consumer_000 {

  bool operator()() {
    queue _queue;
    _queue.add(9);
    _queue.traverse(printer);
    int16_t _i = _queue.get();
    if (_i != 9) {
      concurrent_log_error(log, "Expected 9, but got ", _i);
      return false;
    }
    concurrent_log_info(log, "Got ", _i);
    if (!_queue.empty()) {
      concurrent_log_error(log, "Queue should be empty, but it is not");
      return false;
    }
    return true;
  }

  static std::string desc() {
    return "Simple test, inserting and getting a element, and testing if the "
           "queue becomes empty";
  }
};

struct queue_001 {
  typedef concurrent::circular_queue_t<int16_t, 5, log> queue;

  bool operator()() {

    queue _queue;
    producer _producer(&_queue);
    consumer _consumer(&_queue);

    std::thread _t1([&_producer]() { _producer(); });
    std::thread _t2([&_consumer]() { _consumer(); });

    concurrent_log_debug(log, "Going to sleep");
    std::this_thread::sleep_for(std::chrono::seconds(5));
    concurrent_log_debug(log, "Waking up");

    _producer.stop();
    _consumer.stop();

    _t1.join();
    _t2.join();

    return true;
  }

  static std::string desc() { return "1 Consumer e 1 producer"; }
};

struct queue_002 {
  typedef concurrent::circular_queue_t<int16_t, 5, log> queue;

  bool operator()() {

    queue _queue;
    producer _producer(&_queue);
    consumer _consumer(&_queue);

    std::thread _t1([&_producer]() { _producer(); });

    concurrent_log_debug(log, "Going to sleep 1");
    std::this_thread::sleep_for(std::chrono::seconds(5));
    concurrent_log_debug(log, "Waking up 1");

    std::thread _t2([&_consumer]() { _consumer(); });

    concurrent_log_debug(log, "Going to sleep 2");
    std::this_thread::sleep_for(std::chrono::seconds(5));
    concurrent_log_debug(log, "Waking up 2");

    _producer.stop();
    _consumer.stop();

    _t1.join();
    _t2.join();

    return true;
  }

  static std::string desc() {
    return "1 Consumer e 1 producer, consumer started 5 seconds after producer";
  }
};

int main(int argc, char **argv) {
  log::set_debug();
  tester::test _test(argc, argv);
  run_test(_test, producer_consumer_000);
  run_test(_test, queue_001);
  run_test(_test, queue_002);
}
