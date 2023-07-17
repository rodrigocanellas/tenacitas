/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <cstdint>
#include <iostream>
#include <optional>
#include <string>
#include <utility>

#include <tenacitas.lib/src/container/typ/circular_queue.h>
#include <tenacitas.lib/src/log/alg/logger.h>
#include <tenacitas.lib/src/test/alg/tester.h>

using namespace tenacitas::lib;

using queue = container::typ::circular_queue<int16_t>;

// struct producer {
//   producer(typ::queue &p_queue) : m_queue(p_queue) {}

//  void operator()() {
//    TNCT_LOG_TST("starting producer");
//    while (true) {
//      if (m_stop) {
//        break;
//      }
//      m_d += 0.01;
//      m_i++;
//      TNCT_LOG_TST("adding ", m_i, ",", m_d);
//      m_queue.add(std::make_shared<queue::data>(queue::data{m_i, m_d}));
//      TNCT_LOG_TST(m_i, ",", m_d, " added");
//      std::this_thread::sleep_for(std::chrono::milliseconds(400));
//    }
//  }

//  void stop() { m_stop = true; }

// private:
//   typ::queue &m_queue;
//   int16_t m_i = 100;
//   double m_d = 3.14;
//   bool m_stop = false;
// };

// struct consumer {
//   consumer(typ::queue &p_queue) : m_queue(p_queue) {}

//  void operator()() {
//    TNCT_LOG_TST("starting consumer");
//    while (true) {
//      if ((m_stop) && (m_queue.empty())) {
//        break;
//      }
//      std::optional<queue::data> _maybe{m_queue.get()};

//      if (_maybe.has_value()) {

//        std::shared_ptr<const queue::data> _data(std::move(*_maybe));

//        TNCT_LOG_TST("getting ", *_data);
//      }
//      std::this_thread::sleep_for(std::chrono::milliseconds(950));
//    }
//  }

//  void stop() { m_stop = true; }

// private:
//   typ::queue &m_queue;
//   bool m_stop = false;
// };

// struct queue_000 {

//  bool operator()(const program::alg::options &) {
//    typ::queue _queue(20);
//    _queue.add(std::make_shared<queue::data>(queue::data{9, -4.32}));
//    _queue.traverse([](std::shared_ptr<const queue::data> p_data) -> void {
//      TNCT_LOG_TRA(*p_data);
//    });

//    std::optional<queue::data> _maybe{_queue.get()};

//    if (!_maybe.has_value()) {
//      std::shared_ptr<const queue::data> _data(std::move(*_maybe));

//      if (*_data != queue::data(9, -4.32)) {
//        TNCT_LOG_ERR("Expected [9, -4.32], but got ", *_data);
//        return false;
//      }
//      TNCT_LOG_INF("got ", *_data);
//      if (!_queue.empty()) {
//        TNCT_LOG_ERR("Queue should be empty, but it is not");
//        return false;
//      }
//    }
//    return true;
//  }

//  static std::string desc() {
//    return "Simple test, inserting and getting a element, and testing if "
//           "the queue becomes empty";
//  }
//};

// struct queue_001 {

//  bool operator()(const program::alg::options &) {

//    typ::queue _queue(20);
//    producer _producer(_queue);
//    consumer _consumer(_queue);

//    std::thread _t1([&_producer]() { _producer(); });
//    std::thread _t2([&_consumer]() { _consumer(); });

//    TNCT_LOG_TST("going to sleep");
//    std::this_thread::sleep_for(std::chrono::seconds(5));
//    TNCT_LOG_TST("waking up");

//    _producer.stop();
//    _consumer.stop();

//    _t1.join();
//    _t2.join();

//    return true;
//  }

//  static std::string desc() {
//    return "1 consumer e 1 producer. Main thread sleeps for 5 seconds";
//  }
//};

// struct queue_002 {

//  bool operator()(const program::alg::options &) {

//    typ::queue _queue(20);
//    producer _producer(_queue);
//    consumer _consumer(_queue);

//    std::thread _t1([&_producer]() { _producer(); });

//    TNCT_LOG_TST("going to sleep 1");
//    std::this_thread::sleep_for(std::chrono::seconds(5));
//    TNCT_LOG_TST("waking up 1");

//    std::thread _t2([&_consumer]() { _consumer(); });

//    TNCT_LOG_TST("going to sleep 2");
//    std::this_thread::sleep_for(std::chrono::seconds(5));
//    TNCT_LOG_TST("waking up 2");

//    _producer.stop();
//    _consumer.stop();

//    _t1.join();
//    _t2.join();

//    return true;
//  }

//  static std::string desc() {
//    return "1 consumer e 1 producer."
//           "\nProducer starts."
//           "\nMain thread sleeps for 5 seconds."
//           "\nConsumer starts."
//           "\nMain thread sleeps for 5 seconds.";
//  }
//};

struct queue_003 {
  static std::string desc() {
    return "Tests if the queue will expand as it becomes full";
  }

  bool operator()(const program::alg::options &) {
    auto _printer = [](const queue::data &p_data) -> void {
      TNCT_LOG_TST(p_data);
    };
    queue _queue;

    if (_queue.capacity() != 0) {
      TNCT_LOG_ERR("capacity should be 0, but it is ", _queue.capacity());
      return false;
    }

    if (_queue.occupied() != 0) {
      TNCT_LOG_ERR("there should be 0 slots occupied, but there are ",
                   _queue.occupied());
      return false;
    }

    TNCT_LOG_TST("traversing queue with 0 slots, and no occupied");
    _queue.traverse(_printer);

    // addition # 1
    _queue.add(10);
    if (_queue.capacity() != 1) {
      TNCT_LOG_ERR("capacity should be 1, but it is ", _queue.capacity());
      return false;
    }

    if (_queue.occupied() != 1) {
      TNCT_LOG_ERR("there should be 1 slot occupied, but there are ",
                   _queue.occupied());
      return false;
    }

    TNCT_LOG_TST("traversing queue with 1 slots, and one occupied");
    _queue.traverse(_printer);

    // addition # 2
    _queue.add(-29);
    if (_queue.capacity() != 2) {
      TNCT_LOG_ERR("capacity should be 2, but it is ", _queue.capacity());
      return false;
    }

    if (_queue.occupied() != 2) {
      TNCT_LOG_ERR("there should be 2 slots occupied, but there are ",
                   _queue.occupied());
      return false;
    }

    TNCT_LOG_TST("traversing queue with 2 slots, and 2 occupied");
    _queue.traverse(_printer);

    // addition # 3
    _queue.add(801);
    if (_queue.capacity() != 3) {
      TNCT_LOG_ERR("capacity should be 3, but it is ", _queue.capacity());
      return false;
    }

    if (_queue.occupied() != 3) {
      TNCT_LOG_ERR("there should be 3 slots occupied, but there are ",
                   _queue.occupied());
      return false;
    }

    TNCT_LOG_TST("traversing queue with 3 slots, and 3 occupied");
    _queue.traverse(_printer);

    // addition # 4
    _queue.add(8);
    if (_queue.capacity() != 4) {
      TNCT_LOG_ERR("capacity should be 4, but it is ", _queue.capacity());
      return false;
    }

    if (_queue.occupied() != 4) {
      TNCT_LOG_ERR("there should be 4 slots occupied, but there are ",
                   _queue.occupied());
      return false;
    }

    TNCT_LOG_TST("traversing queue with 4 slots, and 4 occupied");
    _queue.traverse(_printer);

    // addition # 5
    _queue.add(-4);
    if (_queue.capacity() != 5) {
      TNCT_LOG_ERR("capacity should be 5, but it is ", _queue.capacity());
      return false;
    }

    if (_queue.occupied() != 5) {
      TNCT_LOG_ERR("there should be 5 slots occupied, but there are ",
                   _queue.occupied());
      return false;
    }

    TNCT_LOG_TST("traversing queue with 5 slots, and 5 occupied");
    _queue.traverse(_printer);

    // addition # 6
    _queue.add(18);
    if (_queue.capacity() != 6) {
      TNCT_LOG_ERR("capacity should be 6, but it is ", _queue.capacity());
      return false;
    }

    if (_queue.occupied() != 6) {
      TNCT_LOG_ERR("there should be 6 slots occupied, but there are ",
                   _queue.occupied());
      return false;
    }

    TNCT_LOG_TST("traversing queue with 6 slots, and 6 occupied");
    _queue.traverse(_printer);

    // addition # 7
    _queue.add(455);
    if (_queue.capacity() != 7) {
      TNCT_LOG_ERR("capacity should be 7, but it is ", _queue.capacity());
      return false;
    }

    if (_queue.occupied() != 7) {
      TNCT_LOG_ERR("there should be 7 slots occupied, but there are ",
                   _queue.occupied());
      return false;
    }

    TNCT_LOG_TST("traversing queue with 7 slots, and 7 occupied");
    _queue.traverse(_printer);

    // reading # 1
    queue::data _data;
    {
      std::optional<queue::data> _maybe{_queue.get()};
      if (!_maybe.has_value()) {
        TNCT_LOG_ERR("no data read, when it should");
        return false;
      }
      _data = std::move(*_maybe);
    }
    if (_data != queue::data{10}) {
      TNCT_LOG_ERR("data should be ", queue::data{10}, ", but it is ", _data);
      return false;
    }

    TNCT_LOG_TST("data = ", _data);

    if (_queue.capacity() != 7) {
      TNCT_LOG_ERR("capacity should be 7, but it is ", _queue.capacity());
      return false;
    }

    if (_queue.occupied() != 6) {
      TNCT_LOG_ERR("there should be 6 slots occupied, but there are ",
                   _queue.occupied());
      return false;
    }

    TNCT_LOG_TST("traversing queue with 7 slots, and 6 occupied");
    _queue.traverse(_printer);

    // addition # 8, with one slot empty
    _queue.add(-1);
    if (_queue.capacity() != 7) {
      TNCT_LOG_ERR("capacity should be 7, but it is ", _queue.capacity());
      return false;
    }

    if (_queue.occupied() != 7) {
      TNCT_LOG_ERR("there should be 7 slots occupied, but there are ",
                   _queue.occupied());
      return false;
    }

    TNCT_LOG_TST("traversing queue with 7 slots, and 7 occupied");
    _queue.traverse(_printer);

    // reading # 2
    {
      std::optional<queue::data> _maybe{_queue.get()};
      if (!_maybe.has_value()) {
        TNCT_LOG_ERR("no queue::data read, when it should");
        return false;
      }
      _data = std::move(*_maybe);
    }

    if (_data != queue::data{-29}) {
      TNCT_LOG_ERR("queue::data should be ", queue::data{-29}, ", but it is ",
                   _data);
      return false;
    }

    TNCT_LOG_TST("queue::data = ", _data);

    if (_queue.capacity() != 7) {
      TNCT_LOG_ERR("capacity should be 7, but it is ", _queue.capacity());
      return false;
    }

    if (_queue.occupied() != 6) {
      TNCT_LOG_ERR("there should be 6 slots occupied, but there are ",
                   _queue.occupied());
      return false;
    }

    TNCT_LOG_TST("traversing queue with 7 slots, and 6 occupied");
    _queue.traverse(_printer);

    // addition # 9, with one slot empty
    _queue.add(queue::data{23});
    if (_queue.capacity() != 7) {
      TNCT_LOG_ERR("capacity should be 7, but it is ", _queue.capacity());
      return false;
    }

    if (_queue.occupied() != 7) {
      TNCT_LOG_ERR("there should be 7 slots occupied, but there are ",
                   _queue.occupied());
      return false;
    }

    TNCT_LOG_TST("traversing queue with 7 slots, and 7 occupied");
    _queue.traverse(_printer);

    // reading # 3
    {
      std::optional<queue::data> _maybe{_queue.get()};
      if (!_maybe.has_value()) {
        TNCT_LOG_ERR("no queue::data read, when it should");
        return false;
      }
      _data = std::move(*_maybe);
    }

    if (_data != queue::data{801}) {
      TNCT_LOG_ERR("queue::data should be ", queue::data{801}, ", but it is ",
                   _data);
      return false;
    }

    TNCT_LOG_TST("queue::data = ", _data);

    if (_queue.capacity() != 7) {
      TNCT_LOG_ERR("capacity should be 7, but it is ", _queue.capacity());
      return false;
    }

    if (_queue.occupied() != 6) {
      TNCT_LOG_ERR("there should be 6 slots occupied, but there are ",
                   _queue.occupied());
      return false;
    }

    TNCT_LOG_TST("traversing queue with 7 slots, and 6 occupied");
    _queue.traverse(_printer);

    // reading # 4
    {
      std::optional<queue::data> _maybe{_queue.get()};
      if (!_maybe.has_value()) {
        TNCT_LOG_ERR("no queue::data read, when it should");
        return false;
      }
      _data = std::move(*_maybe);
    }

    if (_data != queue::data{8}) {
      TNCT_LOG_ERR("queue::data should be ", queue::data{8}, ", but it is ",
                   _data);
      return false;
    }

    TNCT_LOG_TST("queue::data = ", _data);

    if (_queue.capacity() != 7) {
      TNCT_LOG_ERR("capacity should be 7, but it is ", _queue.capacity());
      return false;
    }

    if (_queue.occupied() != 5) {
      TNCT_LOG_ERR("there should be 5 slots occupied, but there are ",
                   _queue.occupied());
      return false;
    }

    TNCT_LOG_TST("traversing queue with 7 slots, and 5 occupied");
    _queue.traverse(_printer);

    // reading # 5
    {
      std::optional<queue::data> _maybe{_queue.get()};
      if (!_maybe.has_value()) {
        TNCT_LOG_ERR("no queue::data read, when it should");
        return false;
      }
      _data = std::move(*_maybe);
    }

    if (_data != queue::data{-4}) {
      TNCT_LOG_ERR("queue::data should be ", queue::data{-4}, ", but it is ",
                   _data);
      return false;
    }

    TNCT_LOG_TST("queue::data = ", _data);

    if (_queue.capacity() != 7) {
      TNCT_LOG_ERR("capacity should be 7, but it is ", _queue.capacity());
      return false;
    }

    if (_queue.occupied() != 4) {
      TNCT_LOG_ERR("there should be 4 slots occupied, but there are ",
                   _queue.occupied());
      return false;
    }

    TNCT_LOG_TST("traversing queue with 7 slots, and 4 occupied");
    _queue.traverse(_printer);

    // reading # 6
    {
      std::optional<queue::data> _maybe{_queue.get()};
      if (!_maybe.has_value()) {
        TNCT_LOG_ERR("no queue::data read, when it should");
        return false;
      }
      _data = std::move(*_maybe);
    }

    if (_data != queue::data{18}) {
      TNCT_LOG_ERR("queue::data should be ", queue::data{18}, ", but it is ",
                   _data);
      return false;
    }

    TNCT_LOG_TST("queue::data = ", _data);

    if (_queue.capacity() != 7) {
      TNCT_LOG_ERR("capacity should be 7, but it is ", _queue.capacity());
      return false;
    }

    if (_queue.occupied() != 3) {
      TNCT_LOG_ERR("there should be 3 slots occupied, but there are ",
                   _queue.occupied());
      return false;
    }

    TNCT_LOG_TST("traversing queue with 7 slots, and 3 occupied");
    _queue.traverse(_printer);

    // reading # 7
    {
      std::optional<queue::data> _maybe{_queue.get()};
      if (!_maybe.has_value()) {
        TNCT_LOG_ERR("no queue::data read, when it should");
        return false;
      }
      _data = std::move(*_maybe);
    }

    if (_data != queue::data{455}) {
      TNCT_LOG_ERR("queue::data should be ", queue::data{455}, ", but it is ",
                   _data);
      return false;
    }

    TNCT_LOG_TST("queue::data = ", _data);

    if (_queue.capacity() != 7) {
      TNCT_LOG_ERR("capacity should be 7, but it is ", _queue.capacity());
      return false;
    }

    if (_queue.occupied() != 2) {
      TNCT_LOG_ERR("there should be 2 slots occupied, but there are ",
                   _queue.occupied());
      return false;
    }

    TNCT_LOG_TST("traversing queue with 7 slots, and 2 occupied");
    _queue.traverse(_printer);

    // reading # 8
    {
      std::optional<queue::data> _maybe{_queue.get()};
      if (!_maybe.has_value()) {
        TNCT_LOG_ERR("no queue::data read, when it should");
        return false;
      }
      _data = std::move(*_maybe);
    }

    if (_data != queue::data{-1}) {
      TNCT_LOG_ERR("queue::data should be ", queue::data{-1}, ", but it is ",
                   _data);
      return false;
    }

    TNCT_LOG_TST("queue::data = ", _data);

    if (_queue.capacity() != 7) {
      TNCT_LOG_ERR("capacity should be 7, but it is ", _queue.capacity());
      return false;
    }

    if (_queue.occupied() != 1) {
      TNCT_LOG_ERR("there should be 1 slots occupied, but there are ",
                   _queue.occupied());
      return false;
    }

    TNCT_LOG_TST("traversing queue with 7 slots, and 1 occupied");
    _queue.traverse(_printer);

    // reading # 9
    {
      std::optional<queue::data> _maybe{_queue.get()};
      if (!_maybe.has_value()) {
        TNCT_LOG_ERR("no queue::data read, when it should");
        return false;
      }
      _data = std::move(*_maybe);
    }

    if (_data != queue::data{23}) {
      TNCT_LOG_ERR("queue::data should be ", queue::data{23}, ", but it is ",
                   _data);
      return false;
    }

    TNCT_LOG_TST("queue::data = ", _data);

    if (_queue.capacity() != 7) {
      TNCT_LOG_ERR("capacity should be 7, but it is ", _queue.capacity());
      return false;
    }

    if (_queue.occupied() != 0) {
      TNCT_LOG_ERR("there should be 0 slots occupied, but there are ",
                   _queue.occupied());
      return false;
    }

    TNCT_LOG_TST("traversing queue with 7 slots, and 0 occupied");
    _queue.traverse(_printer);

    // reading from an empty queue
    {
      std::optional<queue::data> _maybe{_queue.get()};
      if (_maybe.has_value()) {
        TNCT_LOG_ERR("there is queue::data, when it should not be");
        return false;
      }
      _data = std::move(*_maybe);
    }

    TNCT_LOG_TST("no queue::data to be read");

    return true;
  }
};

struct queue_004 {
  static std::string desc() { return "Corner case: add - get - get"; }

  bool operator()(const program::alg::options &) {
    auto _printer = [](const queue::data &p_data) -> void {
      TNCT_LOG_TST(p_data);
    };
    queue _queue;

    if (_queue.capacity() != 0) {
      TNCT_LOG_ERR("capacity should be 0, but it is ", _queue.capacity());
      return false;
    }

    if (_queue.occupied() != 0) {
      TNCT_LOG_ERR("there should be 0 slots occupied, but there are ",
                   _queue.occupied());
      return false;
    }

    TNCT_LOG_TST("traversing queue with 0 slots, and no occupied");
    _queue.traverse(_printer);

    // addition # 1
    _queue.add(10);
    if (_queue.capacity() != 1) {
      TNCT_LOG_ERR("capacity should be 1, but it is ", _queue.capacity());
      return false;
    }

    if (_queue.occupied() != 1) {
      TNCT_LOG_ERR("there should be 1 slot occupied, but there are ",
                   _queue.occupied());
      return false;
    }

    TNCT_LOG_TST("traversing queue with 1 slots, and one occupied");
    _queue.traverse(_printer);

    // reading # 1
    queue::data _data;
    {
      std::optional<queue::data> _maybe{_queue.get()};
      if (!_maybe.has_value()) {
        TNCT_LOG_ERR("no data read, when it should");
        return false;
      }
      _data = std::move(*_maybe);
    }
    if (_data != queue::data{10}) {
      TNCT_LOG_ERR("data should be ", queue::data{10}, ", but it is ", _data);
      return false;
    }

    TNCT_LOG_TST("data = ", _data);

    if (_queue.capacity() != 1) {
      TNCT_LOG_ERR("capacity should be 1, but it is ", _queue.capacity());
      return false;
    }

    if (_queue.occupied() != 0) {
      TNCT_LOG_ERR("there should be 0 slots occupied, but there are ",
                   _queue.occupied());
      return false;
    }

    TNCT_LOG_TST("traversing queue with 1 slots, and 0 occupied");
    _queue.traverse(_printer);

    // reading # 2
    {
      std::optional<queue::data> _maybe{_queue.get()};
      if (_maybe.has_value()) {
        _data = std::move(*_maybe);
        TNCT_LOG_ERR("data ", _data, " read, when it should not");
        return false;
      }
      TNCT_LOG_TST("no data read, as expected");
    }

    return true;
  }
};

int main(int argc, char **argv) {
  using namespace tenacitas;

  log::alg::set_debug_level();
  log::alg::set_max_file_name_lenght(18);

  lib::test::alg::tester _tester(argc, argv);
  //  run_test(_tester, alg::queue_000);
  //  run_test(_tester, alg::queue_001);
  //  run_test(_tester, alg::queue_002);
  run_test(_tester, queue_003);
  run_test(_tester, queue_004);
}
