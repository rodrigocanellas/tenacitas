/// \example

#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>
#include <utility>

#include <concurrent/circular_unlimited_size_queue.h>
#include <concurrent/internal/log.h>
#include <concurrent/result.h>
#include <logger/cerr/log.h>
#include <status/result.h>
#include <tester/test.h>

using namespace tenacitas;

typedef std::pair<int16_t, double> data;

inline std::ostream &operator<<(std::ostream &p_out, const data &p_data) {
  p_out << "{" << p_data.first << ", " << p_data.second << "}";
  return p_out;
}

struct printer {

  void operator()(const data &p_value) { concurrent_debug(m_log, p_value); }

private:
  tenacitas::logger::cerr::log m_log{"printer"};
};

typedef concurrent::circular_unlimited_size_queue_t<logger::cerr::log, data>
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
      m_queue->add({m_i, m_d});
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
    _queue.add({9, -4.32});
    _queue.traverse(printer());
    std::optional<data> _maybe = _queue.get();
    if (_maybe) {
      data _value = _maybe.value();
      if (_value != data(9, -4.32)) {
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

struct queue_003 {
  static std::string desc() {
    return "Tests if the queue will expand as it becomes full";
  }

  bool operator()() {
    printer _printer;
    queue _q{4};

    if (_q.capacity() != 4) {
      concurrent_error(m_log, "capacity should be 4, but it is ",
                       _q.capacity());
      return false;
    }

    if (_q.occupied() != 0) {
      concurrent_error(m_log,
                       "there should be 0 slots occupied, but there are ",
                       _q.occupied());
      return false;
    }

    concurrent_debug(m_log, "traversing queue with 4 slots, and no occupied");
    _q.traverse(_printer);

    // addition # 1
    _q.add({10, 3.14});
    if (_q.capacity() != 4) {
      concurrent_error(m_log, "capacity should be 4, but it is ",
                       _q.capacity());
      return false;
    }

    if (_q.occupied() != 1) {
      concurrent_error(m_log, "there should be 1 slot occupied, but there are ",
                       _q.occupied());
      return false;
    }

    concurrent_debug(m_log, "traversing queue with 4 slots, and one occupied");
    _q.traverse(_printer);

    // addition # 2
    _q.add({-29, 0.93});
    if (_q.capacity() != 4) {
      concurrent_error(m_log, "capacity should be 4, but it is ",
                       _q.capacity());
      return false;
    }

    if (_q.occupied() != 2) {
      concurrent_error(m_log,
                       "there should be 2 slots occupied, but there are ",
                       _q.occupied());
      return false;
    }

    concurrent_debug(m_log, "traversing queue with 4 slots, and 2 occupied");
    _q.traverse(_printer);

    // addition # 3
    _q.add({801, -4.328});
    if (_q.capacity() != 4) {
      concurrent_error(m_log, "capacity should be 4, but it is ",
                       _q.capacity());
      return false;
    }

    if (_q.occupied() != 3) {
      concurrent_error(m_log,
                       "there should be 3 slots occupied, but there are ",
                       _q.occupied());
      return false;
    }

    concurrent_debug(m_log, "traversing queue with 4 slots, and 3 occupied");
    _q.traverse(_printer);

    // addition # 4
    _q.add({8, 1024.95});
    if (_q.capacity() != 4) {
      concurrent_error(m_log, "capacity should be 4, but it is ",
                       _q.capacity());
      return false;
    }

    if (_q.occupied() != 4) {
      concurrent_error(m_log,
                       "there should be 4 slots occupied, but there are ",
                       _q.occupied());
      return false;
    }

    concurrent_debug(m_log, "traversing queue with 4 slots, and 4 occupied");
    _q.traverse(_printer);

    // addition # 5
    _q.add({-4, -783.23});
    if (_q.capacity() != 5) {
      concurrent_error(m_log, "capacity should be 5, but it is ",
                       _q.capacity());
      return false;
    }

    if (_q.occupied() != 5) {
      concurrent_error(m_log,
                       "there should be 5 slots occupied, but there are ",
                       _q.occupied());
      return false;
    }

    concurrent_debug(m_log, "traversing queue with 5 slots, and 5 occupied");
    _q.traverse(_printer);

    // addition # 6
    _q.add({18, 333.33});
    if (_q.capacity() != 6) {
      concurrent_error(m_log, "capacity should be 6, but it is ",
                       _q.capacity());
      return false;
    }

    if (_q.occupied() != 6) {
      concurrent_error(m_log,
                       "there should be 6 slots occupied, but there are ",
                       _q.occupied());
      return false;
    }

    concurrent_debug(m_log, "traversing queue with 6 slots, and 6 occupied");
    _q.traverse(_printer);

    // addition # 7
    _q.add({455, 12.88});
    if (_q.capacity() != 7) {
      concurrent_error(m_log, "capacity should be 7, but it is ",
                       _q.capacity());
      return false;
    }

    if (_q.occupied() != 7) {
      concurrent_error(m_log,
                       "there should be 7 slots occupied, but there are ",
                       _q.occupied());
      return false;
    }

    concurrent_debug(m_log, "traversing queue with 7 slots, and 7 occupied");
    _q.traverse(_printer);

    // reading # 1
    std::optional<data> _maybe = _q.get();
    if (!_maybe) {
      concurrent_error(m_log, "no data read, when it should");
      return false;
    }

    data _data = std::move(*_maybe);
    if (_data != data{10, 3.14}) {
      concurrent_error(m_log, "data should be ", data{10, 3.14}, ", but it is ",
                       _data);
      return false;
    }

    concurrent_debug(m_log, "data = ", _data);

    if (_q.capacity() != 7) {
      concurrent_error(m_log, "capacity should be 7, but it is ",
                       _q.capacity());
      return false;
    }

    if (_q.occupied() != 6) {
      concurrent_error(m_log,
                       "there should be 6 slots occupied, but there are ",
                       _q.occupied());
      return false;
    }

    concurrent_debug(m_log, "traversing queue with 7 slots, and 6 occupied");
    _q.traverse(_printer);

    // addition # 8, with one slot empty
    _q.add({-1, -2.3});
    if (_q.capacity() != 7) {
      concurrent_error(m_log, "capacity should be 7, but it is ",
                       _q.capacity());
      return false;
    }

    if (_q.occupied() != 7) {
      concurrent_error(m_log,
                       "there should be 7 slots occupied, but there are ",
                       _q.occupied());
      return false;
    }

    concurrent_debug(m_log, "traversing queue with 7 slots, and 7 occupied");
    _q.traverse(_printer);

    // reading # 2
    _maybe = _q.get();
    if (!_maybe) {
      concurrent_error(m_log, "no data read, when it should");
      return false;
    }

    _data = std::move(*_maybe);
    if (_data != data{-29, 0.93}) {
      concurrent_error(m_log, "data should be ", data{-29, 0.93},
                       ", but it is ", _data);
      return false;
    }

    concurrent_debug(m_log, "data = ", _data);

    if (_q.capacity() != 7) {
      concurrent_error(m_log, "capacity should be 7, but it is ",
                       _q.capacity());
      return false;
    }

    if (_q.occupied() != 6) {
      concurrent_error(m_log,
                       "there should be 6 slots occupied, but there are ",
                       _q.occupied());
      return false;
    }

    concurrent_debug(m_log, "traversing queue with 7 slots, and 6 occupied");
    _q.traverse(_printer);

    // addition # 9, with one slot empty
    _q.add({23, 4.021});
    if (_q.capacity() != 7) {
      concurrent_error(m_log, "capacity should be 7, but it is ",
                       _q.capacity());
      return false;
    }

    if (_q.occupied() != 7) {
      concurrent_error(m_log,
                       "there should be 7 slots occupied, but there are ",
                       _q.occupied());
      return false;
    }

    concurrent_debug(m_log, "traversing queue with 7 slots, and 7 occupied");
    _q.traverse(_printer);

    // reading # 3
    _maybe = _q.get();
    if (!_maybe) {
      concurrent_error(m_log, "no data read, when it should");
      return false;
    }

    _data = std::move(*_maybe);
    if (_data != data{801, -4.328}) {
      concurrent_error(m_log, "data should be ", data{801, -4.328},
                       ", but it is ", _data);
      return false;
    }

    concurrent_debug(m_log, "data = ", _data);

    if (_q.capacity() != 7) {
      concurrent_error(m_log, "capacity should be 7, but it is ",
                       _q.capacity());
      return false;
    }

    if (_q.occupied() != 6) {
      concurrent_error(m_log,
                       "there should be 6 slots occupied, but there are ",
                       _q.occupied());
      return false;
    }

    concurrent_debug(m_log, "traversing queue with 7 slots, and 6 occupied");
    _q.traverse(_printer);

    // reading # 4
    _maybe = _q.get();
    if (!_maybe) {
      concurrent_error(m_log, "no data read, when it should");
      return false;
    }

    _data = std::move(*_maybe);
    if (_data != data{8, 1024.95}) {
      concurrent_error(m_log, "data should be ", data{8, 1024.95},
                       ", but it is ", _data);
      return false;
    }

    concurrent_debug(m_log, "data = ", _data);

    if (_q.capacity() != 7) {
      concurrent_error(m_log, "capacity should be 7, but it is ",
                       _q.capacity());
      return false;
    }

    if (_q.occupied() != 5) {
      concurrent_error(m_log,
                       "there should be 5 slots occupied, but there are ",
                       _q.occupied());
      return false;
    }

    concurrent_debug(m_log, "traversing queue with 7 slots, and 5 occupied");
    _q.traverse(_printer);

    // reading # 5
    _maybe = _q.get();
    if (!_maybe) {
      concurrent_error(m_log, "no data read, when it should");
      return false;
    }

    _data = std::move(*_maybe);
    if (_data != data{-4, -783.23}) {
      concurrent_error(m_log, "data should be ", data{-4, -783.23},
                       ", but it is ", _data);
      return false;
    }

    concurrent_debug(m_log, "data = ", _data);

    if (_q.capacity() != 7) {
      concurrent_error(m_log, "capacity should be 7, but it is ",
                       _q.capacity());
      return false;
    }

    if (_q.occupied() != 4) {
      concurrent_error(m_log,
                       "there should be 4 slots occupied, but there are ",
                       _q.occupied());
      return false;
    }

    concurrent_debug(m_log, "traversing queue with 7 slots, and 4 occupied");
    _q.traverse(_printer);

    // reading # 6
    _maybe = _q.get();
    if (!_maybe) {
      concurrent_error(m_log, "no data read, when it should");
      return false;
    }

    _data = std::move(*_maybe);
    if (_data != data{18, 333.33}) {
      concurrent_error(m_log, "data should be ", data{18, 333.33},
                       ", but it is ", _data);
      return false;
    }

    concurrent_debug(m_log, "data = ", _data);

    if (_q.capacity() != 7) {
      concurrent_error(m_log, "capacity should be 7, but it is ",
                       _q.capacity());
      return false;
    }

    if (_q.occupied() != 3) {
      concurrent_error(m_log,
                       "there should be 3 slots occupied, but there are ",
                       _q.occupied());
      return false;
    }

    concurrent_debug(m_log, "traversing queue with 7 slots, and 3 occupied");
    _q.traverse(_printer);

    // reading # 7
    _maybe = _q.get();
    if (!_maybe) {
      concurrent_error(m_log, "no data read, when it should");
      return false;
    }

    _data = std::move(*_maybe);
    if (_data != data{455, 12.88}) {
      concurrent_error(m_log, "data should be ", data{455, 12.88},
                       ", but it is ", _data);
      return false;
    }

    concurrent_debug(m_log, "data = ", _data);

    if (_q.capacity() != 7) {
      concurrent_error(m_log, "capacity should be 7, but it is ",
                       _q.capacity());
      return false;
    }

    if (_q.occupied() != 2) {
      concurrent_error(m_log,
                       "there should be 2 slots occupied, but there are ",
                       _q.occupied());
      return false;
    }

    concurrent_debug(m_log, "traversing queue with 7 slots, and 2 occupied");
    _q.traverse(_printer);

    // reading # 8
    _maybe = _q.get();
    if (!_maybe) {
      concurrent_error(m_log, "no data read, when it should");
      return false;
    }

    _data = std::move(*_maybe);
    if (_data != data{-1, -2.3}) {
      concurrent_error(m_log, "data should be ", data{-1, -2.3}, ", but it is ",
                       _data);
      return false;
    }

    concurrent_debug(m_log, "data = ", _data);

    if (_q.capacity() != 7) {
      concurrent_error(m_log, "capacity should be 7, but it is ",
                       _q.capacity());
      return false;
    }

    if (_q.occupied() != 1) {
      concurrent_error(m_log,
                       "there should be 1 slots occupied, but there are ",
                       _q.occupied());
      return false;
    }

    concurrent_debug(m_log, "traversing queue with 7 slots, and 1 occupied");
    _q.traverse(_printer);

    // reading # 9
    _maybe = _q.get();
    if (!_maybe) {
      concurrent_error(m_log, "no data read, when it should");
      return false;
    }

    _data = std::move(*_maybe);
    if (_data != data{23, 4.021}) {
      concurrent_error(m_log, "data should be ", data{23, 4.021},
                       ", but it is ", _data);
      return false;
    }

    concurrent_debug(m_log, "data = ", _data);

    if (_q.capacity() != 7) {
      concurrent_error(m_log, "capacity should be 7, but it is ",
                       _q.capacity());
      return false;
    }

    if (_q.occupied() != 0) {
      concurrent_error(m_log,
                       "there should be 0 slots occupied, but there are ",
                       _q.occupied());
      return false;
    }

    concurrent_debug(m_log, "traversing queue with 7 slots, and 0 occupied");
    _q.traverse(_printer);

    // reading from an empty queue
    _maybe = _q.get();

    if (_maybe) {
      concurrent_error(m_log, "read data from an empty queue");
      return false;
    }

    concurrent_debug(m_log, "no data to be read");

    return true;
  }

private:
  logger::cerr::log m_log{"queue_003"};
};

int main(int argc, char **argv) {
  logger::cerr::log::set_debug();
  tester::test _test(argc, argv);
  run_test(_test, queue_000);
  run_test(_test, queue_001);
  run_test(_test, queue_002);
  run_test(_test, queue_003);
}