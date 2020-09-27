/// \example

#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>

#include <concurrent/fixed_size_queue.h>
#include <concurrent/internal/log.h>
#include <concurrent/result.h>
#include <logger/cerr/log.h>
#include <status/result.h>
#include <tester/test.h>

using namespace tenacitas;

void printer(const std::tuple<int16_t, double> &p_value) {
  std::cout << "[" << std::get<0>(p_value) << "," << std::get<1>(p_value)
            << "] ";
}

typedef concurrent::fixed_size_queue_t<logger::cerr::log, int16_t, double>
    queue;

struct producer {
  producer(queue *p_queue) : m_queue(p_queue) {}

  void operator()() {
    concurrent_debug(m_log, "starting producer");
    while (true) {
      if (m_stop) {
        break;
      }
      m_d += 0.01;
      m_i++;
      concurrent_debug(m_log, "adding ", m_i, ",", m_d);
      m_queue->add(m_i, m_d);
      std::this_thread::sleep_for(std::chrono::milliseconds(400));
    }
  }

  void stop() { m_stop = true; }

private:
  queue *m_queue = nullptr;
  int16_t m_i = 100;
  double m_d = 3.14;
  bool m_stop = false;
  logger::cerr::log m_log{"producer"};
};

struct consumer {
  consumer(queue *p_queue) : m_queue(p_queue) {}

  void operator()() {
    concurrent_debug(m_log, "starting consumer");
    while (true) {
      if ((m_stop) && (m_queue->empty())) {
        break;
      }
      std::optional<std::tuple<int16_t, double>> _maybe(m_queue->get());
      if (_maybe) {
        std::tuple<int16_t, double> _data = _maybe.value();
        concurrent_debug(m_log, "getting ", _data);
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(950));
    }
  }

  void stop() { m_stop = true; }

private:
  queue *m_queue = nullptr;
  bool m_stop = false;
  logger::cerr::log m_log{"consumer"};
};

struct queue_000 {

  bool operator()() {
    queue _queue(20);
    _queue.add(9, -4.32);
    _queue.traverse(printer);
    std::optional<std::tuple<int16_t, double>> _maybe = _queue.get();
    if (_maybe) {
      std::tuple<int16_t, double> _value = _maybe.value();
      if ((std::get<0>(_value) != 9) && (std::get<1>(_value) != -4.32)) {
        concurrent_error(m_log, "Expected [9, -4.32], but got ", _value);
        return false;
      }
      concurrent_info(m_log, "got ", _value);
      if (!_queue.empty()) {
        concurrent_error(m_log, "Queue should be empty, but it is not");
        return false;
      }
    }
    return true;
  }

  static std::string desc() {
    return "Simple test, inserting and getting a element, and testing if the "
           "queue becomes empty";
  }

private:
  logger::cerr::log m_log{"queue_000"};
};

struct queue_001 {

  bool operator()() {

    queue _queue(20);
    producer _producer(&_queue);
    consumer _consumer(&_queue);

    std::thread _t1([&_producer]() { _producer(); });
    std::thread _t2([&_consumer]() { _consumer(); });

    concurrent_debug(m_log, "going to sleep");
    std::this_thread::sleep_for(std::chrono::seconds(5));
    concurrent_debug(m_log, "waking up");

    _producer.stop();
    _consumer.stop();

    _t1.join();
    _t2.join();

    return true;
  }

  static std::string desc() {
    return "1 consumer e 1 producer. Main thread sleeps for 5 seconds";
  }

private:
  logger::cerr::log m_log{"queue_001"};
};

struct queue_002 {

  bool operator()() {

    queue _queue(20);
    producer _producer(&_queue);
    consumer _consumer(&_queue);

    std::thread _t1([&_producer]() { _producer(); });

    concurrent_debug(m_log, "going to sleep 1");
    std::this_thread::sleep_for(std::chrono::seconds(5));
    concurrent_debug(m_log, "waking up 1");

    std::thread _t2([&_consumer]() { _consumer(); });

    concurrent_debug(m_log, "going to sleep 2");
    std::this_thread::sleep_for(std::chrono::seconds(5));
    concurrent_debug(m_log, "waking up 2");

    _producer.stop();
    _consumer.stop();

    _t1.join();
    _t2.join();

    return true;
  }

  static std::string desc() {
    return "1 consumer e 1 producer."

           "\nProducer starts."

           "\nMain thread sleeps for 5 seconds."

           "\nConsumer starts."

           "\nMain thread sleeps for 5 seconds.";
  }

private:
  logger::cerr::log m_log{"queue_002"};
};

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();
  tester::test _test(argc, argv);
  run_test(_test, queue_000);
  run_test(_test, queue_001);
  run_test(_test, queue_002);
}
