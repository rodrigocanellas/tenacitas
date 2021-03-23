/// \example

#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include <tenacitas/concurrent.h>
#include <tenacitas/logger.h>
#include <tenacitas/macros.h>
#include <tenacitas/tester.h>

using namespace tenacitas;

typedef std::tuple<int16_t, double> data;

typedef concurrent::internal::circular_fixed_size_queue_t<data> queue;

struct producer {
  producer(queue &p_queue) : m_queue(p_queue) {}

  void operator()() {
    DEB(m_log, "starting producer");
    while (true) {
      if (m_stop) {
        break;
      }
      m_d += 0.01;
      m_i++;
      DEB(m_log, "adding ", m_i, ",", m_d);
      m_queue.add({m_i, m_d});
      std::this_thread::sleep_for(std::chrono::milliseconds(400));
    }
  }

  void stop() { m_stop = true; }

private:
  queue &m_queue;
  int16_t m_i = 100;
  double m_d = 3.14;
  bool m_stop = false;
  logger::cerr<> m_log{"producer"};
};

struct consumer {
  consumer(queue &p_queue) : m_queue(p_queue) {}

  void operator()() {
    DEB(m_log, "starting consumer");
    while (true) {
      if ((m_stop) && (m_queue.empty())) {
        break;
      }
      std::optional<std::tuple<int16_t, double>> _maybe(m_queue.get());
      if (_maybe) {
        std::tuple<int16_t, double> _data = std::move(*_maybe);
        DEB(m_log, "getting ", _data);
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(950));
    }
  }

  void stop() { m_stop = true; }

private:
  queue &m_queue;
  bool m_stop = false;
  logger::cerr<> m_log{"consumer"};
};

struct queue_000 {

  bool operator()() {
    queue _queue(20);
    _queue.add({9, -4.32});
    _queue.traverse([this](const data &p_data) -> void { DEB(m_log, p_data); });
    std::optional<std::tuple<int16_t, double>> _maybe = _queue.get();
    if (_maybe) {
      std::tuple<int16_t, double> _value = std::move(*_maybe);
      if ((std::get<0>(_value) != 9) && (std::get<1>(_value) != -4.32)) {
        ERR(m_log, "Expected [9, -4.32], but got ", _value);
        return false;
      }
      INF(m_log, "got ", _value);
      if (!_queue.empty()) {
        ERR(m_log, "Queue should be empty, but it is not");
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
  logger::cerr<> m_log{"queue_000"};
};

struct queue_001 {

  static std::string desc() {
    return "1 consumer e 1 producer. Main thread sleeps for 5 seconds";
  }

  bool operator()() {

    queue _queue(20);
    producer _producer(_queue);
    consumer _consumer(_queue);

    std::thread _t1([&_producer]() { _producer(); });
    std::thread _t2([&_consumer]() { _consumer(); });

    DEB(m_log, "going to sleep");
    std::this_thread::sleep_for(std::chrono::seconds(5));
    DEB(m_log, "waking up");

    _producer.stop();
    _consumer.stop();

    _t1.join();
    _t2.join();

    return true;
  }

private:
  logger::cerr<> m_log{"queue_001"};
};

struct queue_002 {
  static std::string desc() {
    return "1 consumer e 1 producer."
           "\nProducer starts."
           "\nMain thread sleeps for 5 seconds."
           "\nConsumer starts."
           "\nMain thread sleeps for 5 seconds.";
  }

  bool operator()() {

    queue _queue(20);
    producer _producer(_queue);
    consumer _consumer(_queue);

    std::thread _t1([&_producer]() { _producer(); });

    DEB(m_log, "going to sleep 1");
    std::this_thread::sleep_for(std::chrono::seconds(5));
    DEB(m_log, "waking up 1");

    std::thread _t2([&_consumer]() { _consumer(); });

    DEB(m_log, "going to sleep 2");
    std::this_thread::sleep_for(std::chrono::seconds(5));
    DEB(m_log, "waking up 2");

    _producer.stop();
    _consumer.stop();

    _t1.join();
    _t2.join();

    return true;
  }

private:
  logger::cerr<> m_log{"queue_002"};
};

int main(int argc, char **argv) {
  logger::set_debug_level();

  tester::test _test(argc, argv);
  run_test(_test, queue_000);
  run_test(_test, queue_001);
  run_test(_test, queue_002);
}
