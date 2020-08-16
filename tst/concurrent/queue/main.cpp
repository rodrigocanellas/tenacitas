/// \example Basic use of tenacitas::concurrent::loop_t

#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>

#include <concurrent/internal/log.h>
#include <concurrent/loop.h>
#include <concurrent/queue.h>
#include <concurrent/result.h>
#include <logger/cerr/log.h>
#include <status/result.h>
#include <tester/test.h>

using namespace tenacitas;
typedef tenacitas::logger::cerr::log log;

void printer(const int16_t &p_i) { std::cout << p_i << " "; }

struct queue_000 {
  typedef concurrent::queue<int16_t, 5, log> queue;
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
  typedef concurrent::queue<int16_t, 5, log> queue;

  bool operator()() {

    queue _queue;
    producer _producer(&_queue);
    consumer _consumer(&_queue);

    std::thread _t1(_producer);
    std::thread _t2(_consumer);

    concurrent_log_debug(log, "Going to sleep");
    std::this_thread::sleep_for(std::chrono::seconds(5));
    concurrent_log_debug(log, "Waking up");

    _t1.join();
    _t2.join();

    return true;
  }

  static std::string desc() { return "1 Consumer e 1 producer"; }

private:
  struct producer {
    producer(queue *p_queue) : m_queue(p_queue) {}

    void operator()() {
      concurrent_log_debug(log, "starting producer");
      while (true) {
        m_queue->add(m_i++);
        std::this_thread::sleep_for(std::chrono::milliseconds(400));
      }
    }

  private:
    queue *m_queue = nullptr;
    int16_t m_i = 100;
  };

  struct consumer {
    consumer(queue *p_queue) : m_queue(p_queue) {}

    void operator()() {
      concurrent_log_debug(log, "starting consumer");
      while (true) {
        std::cerr << "got " << m_queue->get() << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(950));
      }
    }

  private:
    queue *m_queue = nullptr;
  };
};

int main(int argc, char **argv) {
  log::set_debug();
  tester::test _test(argc, argv);
  run_test(_test, queue_000);
  run_test(_test, queue_001);
}
