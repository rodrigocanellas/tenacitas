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
namespace typ {

using data = std::pair<int16_t, double>;

using data_ptr = std::shared_ptr<data>;

using queue = container::typ::circular_queue_t<data_ptr>;
} // namespace typ

namespace alg {
struct producer {
  producer(typ::queue &p_queue) : m_queue(p_queue) {}

  void operator()() {
    TNCT_LOG_TST("starting producer");
    while (true) {
      if (m_stop) {
        break;
      }
      m_d += 0.01;
      m_i++;
      TNCT_LOG_TST("adding ", m_i, ",", m_d);
      m_queue.add(std::make_shared<typ::data>(typ::data{m_i, m_d}));
      TNCT_LOG_TST(m_i, ",", m_d, " added");
      std::this_thread::sleep_for(std::chrono::milliseconds(400));
    }
  }

  void stop() { m_stop = true; }

private:
  typ::queue &m_queue;
  int16_t m_i = 100;
  double m_d = 3.14;
  bool m_stop = false;
};

struct consumer {
  consumer(typ::queue &p_queue) : m_queue(p_queue) {}

  void operator()() {
    TNCT_LOG_TST("starting consumer");
    while (true) {
      if ((m_stop) && (m_queue.empty())) {
        break;
      }
      std::optional<typ::data_ptr> _maybe{m_queue.get()};

      if (_maybe.has_value()) {

        std::shared_ptr<const typ::data> _data(std::move(*_maybe));

        TNCT_LOG_TST("getting ", *_data);
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(950));
    }
  }

  void stop() { m_stop = true; }

private:
  typ::queue &m_queue;
  bool m_stop = false;
};

struct queue_000 {

  bool operator()(const program::alg::options &) {
    typ::queue _queue(20);
    _queue.add(std::make_shared<typ::data>(typ::data{9, -4.32}));
    _queue.traverse([](std::shared_ptr<const typ::data> p_data) -> void {
      TNCT_LOG_TRA(*p_data);
    });

    std::optional<typ::data_ptr> _maybe{_queue.get()};

    if (!_maybe.has_value()) {
      std::shared_ptr<const typ::data> _data(std::move(*_maybe));

      if (*_data != typ::data(9, -4.32)) {
        TNCT_LOG_ERR("Expected [9, -4.32], but got ", *_data);
        return false;
      }
      TNCT_LOG_INF("got ", *_data);
      if (!_queue.empty()) {
        TNCT_LOG_ERR("Queue should be empty, but it is not");
        return false;
      }
    }
    return true;
  }

  static std::string desc() {
    return "Simple test, inserting and getting a element, and testing if "
           "the queue becomes empty";
  }
};

struct queue_001 {

  bool operator()(const program::alg::options &) {

    typ::queue _queue(20);
    producer _producer(_queue);
    consumer _consumer(_queue);

    std::thread _t1([&_producer]() { _producer(); });
    std::thread _t2([&_consumer]() { _consumer(); });

    TNCT_LOG_TST("going to sleep");
    std::this_thread::sleep_for(std::chrono::seconds(5));
    TNCT_LOG_TST("waking up");

    _producer.stop();
    _consumer.stop();

    _t1.join();
    _t2.join();

    return true;
  }

  static std::string desc() {
    return "1 consumer e 1 producer. Main thread sleeps for 5 seconds";
  }
};

struct queue_002 {

  bool operator()(const program::alg::options &) {

    typ::queue _queue(20);
    producer _producer(_queue);
    consumer _consumer(_queue);

    std::thread _t1([&_producer]() { _producer(); });

    TNCT_LOG_TST("going to sleep 1");
    std::this_thread::sleep_for(std::chrono::seconds(5));
    TNCT_LOG_TST("waking up 1");

    std::thread _t2([&_consumer]() { _consumer(); });

    TNCT_LOG_TST("going to sleep 2");
    std::this_thread::sleep_for(std::chrono::seconds(5));
    TNCT_LOG_TST("waking up 2");

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
};

struct queue_003 {
  static std::string desc() {
    return "Tests if the queue will expand as it becomes full";
  }

  bool operator()(const program::alg::options &) {
    auto _printer = [](std::shared_ptr<const typ::data> p_data) -> void {
      TNCT_LOG_TST(*p_data);
    };
    typ::queue _q{4};

    if (_q.capacity() != 4) {
      TNCT_LOG_ERR("capacity should be 4, but it is ", _q.capacity());
      return false;
    }

    if (_q.occupied() != 0) {
      TNCT_LOG_ERR("there should be 0 slots occupied, but there are ",
                   _q.occupied());
      return false;
    }

    TNCT_LOG_TST("traversing queue with 4 slots, and no occupied");
    _q.traverse(_printer);

    // addition # 1
    _q.add(std::make_shared<typ::data>(typ::data{10, 3.14}));
    if (_q.capacity() != 4) {
      TNCT_LOG_ERR("capacity should be 4, but it is ", _q.capacity());
      return false;
    }

    if (_q.occupied() != 1) {
      TNCT_LOG_ERR("there should be 1 slot occupied, but there are ",
                   _q.occupied());
      return false;
    }

    TNCT_LOG_TST("traversing queue with 4 slots, and one occupied");
    _q.traverse(_printer);

    // addition # 2
    _q.add(std::make_shared<typ::data>(typ::data{-29, 0.93}));
    if (_q.capacity() != 4) {
      TNCT_LOG_ERR("capacity should be 4, but it is ", _q.capacity());
      return false;
    }

    if (_q.occupied() != 2) {
      TNCT_LOG_ERR("there should be 2 slots occupied, but there are ",
                   _q.occupied());
      return false;
    }

    TNCT_LOG_TST("traversing queue with 4 slots, and 2 occupied");
    _q.traverse(_printer);

    // addition # 3
    _q.add(std::make_shared<typ::data>(typ::data{801, -4.328}));
    if (_q.capacity() != 4) {
      TNCT_LOG_ERR("capacity should be 4, but it is ", _q.capacity());
      return false;
    }

    if (_q.occupied() != 3) {
      TNCT_LOG_ERR("there should be 3 slots occupied, but there are ",
                   _q.occupied());
      return false;
    }

    TNCT_LOG_TST("traversing queue with 4 slots, and 3 occupied");
    _q.traverse(_printer);

    // addition # 4
    _q.add(std::make_shared<typ::data>(typ::data{8, 1024.95}));
    if (_q.capacity() != 4) {
      TNCT_LOG_ERR("capacity should be 4, but it is ", _q.capacity());
      return false;
    }

    if (_q.occupied() != 4) {
      TNCT_LOG_ERR("there should be 4 slots occupied, but there are ",
                   _q.occupied());
      return false;
    }

    TNCT_LOG_TST("traversing queue with 4 slots, and 4 occupied");
    _q.traverse(_printer);

    // addition # 5
    _q.add(std::make_shared<typ::data>(typ::data{-4, -783.23}));
    if (_q.capacity() != 5) {
      TNCT_LOG_ERR("capacity should be 5, but it is ", _q.capacity());
      return false;
    }

    if (_q.occupied() != 5) {
      TNCT_LOG_ERR("there should be 5 slots occupied, but there are ",
                   _q.occupied());
      return false;
    }

    TNCT_LOG_TST("traversing queue with 5 slots, and 5 occupied");
    _q.traverse(_printer);

    // addition # 6
    _q.add(std::make_shared<typ::data>(typ::data{18, 333.33}));
    if (_q.capacity() != 6) {
      TNCT_LOG_ERR("capacity should be 6, but it is ", _q.capacity());
      return false;
    }

    if (_q.occupied() != 6) {
      TNCT_LOG_ERR("there should be 6 slots occupied, but there are ",
                   _q.occupied());
      return false;
    }

    TNCT_LOG_TST("traversing queue with 6 slots, and 6 occupied");
    _q.traverse(_printer);

    // addition # 7
    _q.add(std::make_shared<typ::data>(typ::data{455, 12.88}));
    if (_q.capacity() != 7) {
      TNCT_LOG_ERR("capacity should be 7, but it is ", _q.capacity());
      return false;
    }

    if (_q.occupied() != 7) {
      TNCT_LOG_ERR("there should be 7 slots occupied, but there are ",
                   _q.occupied());
      return false;
    }

    TNCT_LOG_TST("traversing queue with 7 slots, and 7 occupied");
    _q.traverse(_printer);

    // reading # 1
    std::shared_ptr<const typ::data> _data;
    {
      std::optional<typ::data_ptr> _maybe{_q.get()};
      if (!_maybe.has_value()) {
        TNCT_LOG_ERR("no data read, when it should");
        return false;
      }
      _data = std::move(*_maybe);
    }
    if (*_data != typ::data{10, 3.14}) {
      TNCT_LOG_ERR("data should be ", typ::data{10, 3.14}, ", but it is ",
                   _data);
      return false;
    }

    TNCT_LOG_TST("data = ", _data);

    if (_q.capacity() != 7) {
      TNCT_LOG_ERR("capacity should be 7, but it is ", _q.capacity());
      return false;
    }

    if (_q.occupied() != 6) {
      TNCT_LOG_ERR("there should be 6 slots occupied, but there are ",
                   _q.occupied());
      return false;
    }

    TNCT_LOG_TST("traversing queue with 7 slots, and 6 occupied");
    _q.traverse(_printer);

    // addition # 8, with one slot empty
    _q.add(std::make_shared<typ::data>(typ::data{-1, -2.3}));
    if (_q.capacity() != 7) {
      TNCT_LOG_ERR("capacity should be 7, but it is ", _q.capacity());
      return false;
    }

    if (_q.occupied() != 7) {
      TNCT_LOG_ERR("there should be 7 slots occupied, but there are ",
                   _q.occupied());
      return false;
    }

    TNCT_LOG_TST("traversing queue with 7 slots, and 7 occupied");
    _q.traverse(_printer);

    // reading # 2
    {
      std::optional<typ::data_ptr> _maybe{_q.get()};
      if (!_maybe.has_value()) {
        TNCT_LOG_ERR("no typ::data read, when it should");
        return false;
      }
      _data = std::move(*_maybe);
    }

    if (*_data != typ::data{-29, 0.93}) {
      TNCT_LOG_ERR("typ::data should be ", typ::data{-29, 0.93}, ", but it is ",
                   *_data);
      return false;
    }

    TNCT_LOG_TST("typ::data = ", *_data);

    if (_q.capacity() != 7) {
      TNCT_LOG_ERR("capacity should be 7, but it is ", _q.capacity());
      return false;
    }

    if (_q.occupied() != 6) {
      TNCT_LOG_ERR("there should be 6 slots occupied, but there are ",
                   _q.occupied());
      return false;
    }

    TNCT_LOG_TST("traversing queue with 7 slots, and 6 occupied");
    _q.traverse(_printer);

    // addition # 9, with one slot empty
    _q.add(std::make_shared<typ::data>(typ::data{23, 4.021}));
    if (_q.capacity() != 7) {
      TNCT_LOG_ERR("capacity should be 7, but it is ", _q.capacity());
      return false;
    }

    if (_q.occupied() != 7) {
      TNCT_LOG_ERR("there should be 7 slots occupied, but there are ",
                   _q.occupied());
      return false;
    }

    TNCT_LOG_TST("traversing queue with 7 slots, and 7 occupied");
    _q.traverse(_printer);

    // reading # 3
    {
      std::optional<typ::data_ptr> _maybe{_q.get()};
      if (!_maybe.has_value()) {
        TNCT_LOG_ERR("no typ::data read, when it should");
        return false;
      }
      _data = std::move(*_maybe);
    }

    if (*_data != typ::data{801, -4.328}) {
      TNCT_LOG_ERR("typ::data should be ", typ::data{801, -4.328},
                   ", but it is ", *_data);
      return false;
    }

    TNCT_LOG_TST("typ::data = ", *_data);

    if (_q.capacity() != 7) {
      TNCT_LOG_ERR("capacity should be 7, but it is ", _q.capacity());
      return false;
    }

    if (_q.occupied() != 6) {
      TNCT_LOG_ERR("there should be 6 slots occupied, but there are ",
                   _q.occupied());
      return false;
    }

    TNCT_LOG_TST("traversing queue with 7 slots, and 6 occupied");
    _q.traverse(_printer);

    // reading # 4
    {
      std::optional<typ::data_ptr> _maybe{_q.get()};
      if (!_maybe.has_value()) {
        TNCT_LOG_ERR("no typ::data read, when it should");
        return false;
      }
      _data = std::move(*_maybe);
    }

    if (!_data) {
      TNCT_LOG_ERR("no typ::data read, when it should");
      return false;
    }

    if (*_data != typ::data{8, 1024.95}) {
      TNCT_LOG_ERR("typ::data should be ", typ::data{8, 1024.95},
                   ", but it is ", *_data);
      return false;
    }

    TNCT_LOG_TST("typ::data = ", *_data);

    if (_q.capacity() != 7) {
      TNCT_LOG_ERR("capacity should be 7, but it is ", _q.capacity());
      return false;
    }

    if (_q.occupied() != 5) {
      TNCT_LOG_ERR("there should be 5 slots occupied, but there are ",
                   _q.occupied());
      return false;
    }

    TNCT_LOG_TST("traversing queue with 7 slots, and 5 occupied");
    _q.traverse(_printer);

    // reading # 5
    {
      std::optional<typ::data_ptr> _maybe{_q.get()};
      if (!_maybe.has_value()) {
        TNCT_LOG_ERR("no typ::data read, when it should");
        return false;
      }
      _data = std::move(*_maybe);
    }

    if (!_data) {
      TNCT_LOG_ERR("no typ::data read, when it should");
      return false;
    }

    if (*_data != typ::data{-4, -783.23}) {
      TNCT_LOG_ERR("typ::data should be ", typ::data{-4, -783.23},
                   ", but it is ", *_data);
      return false;
    }

    TNCT_LOG_TST("typ::data = ", *_data);

    if (_q.capacity() != 7) {
      TNCT_LOG_ERR("capacity should be 7, but it is ", _q.capacity());
      return false;
    }

    if (_q.occupied() != 4) {
      TNCT_LOG_ERR("there should be 4 slots occupied, but there are ",
                   _q.occupied());
      return false;
    }

    TNCT_LOG_TST("traversing queue with 7 slots, and 4 occupied");
    _q.traverse(_printer);

    // reading # 6
    {
      std::optional<typ::data_ptr> _maybe{_q.get()};
      if (!_maybe.has_value()) {
        TNCT_LOG_ERR("no typ::data read, when it should");
        return false;
      }
      _data = std::move(*_maybe);
    }

    if (!_data) {
      TNCT_LOG_ERR("no typ::data read, when it should");
      return false;
    }

    if (*_data != typ::data{18, 333.33}) {
      TNCT_LOG_ERR("typ::data should be ", typ::data{18, 333.33},
                   ", but it is ", *_data);
      return false;
    }

    TNCT_LOG_TST("typ::data = ", *_data);

    if (_q.capacity() != 7) {
      TNCT_LOG_ERR("capacity should be 7, but it is ", _q.capacity());
      return false;
    }

    if (_q.occupied() != 3) {
      TNCT_LOG_ERR("there should be 3 slots occupied, but there are ",
                   _q.occupied());
      return false;
    }

    TNCT_LOG_TST("traversing queue with 7 slots, and 3 occupied");
    _q.traverse(_printer);

    // reading # 7
    {
      std::optional<typ::data_ptr> _maybe{_q.get()};
      if (!_maybe.has_value()) {
        TNCT_LOG_ERR("no typ::data read, when it should");
        return false;
      }
      _data = std::move(*_maybe);
    }

    if (!_data) {
      TNCT_LOG_ERR("no typ::data read, when it should");
      return false;
    }

    if (*_data != typ::data{455, 12.88}) {
      TNCT_LOG_ERR("typ::data should be ", typ::data{455, 12.88},
                   ", but it is ", *_data);
      return false;
    }

    TNCT_LOG_TST("typ::data = ", *_data);

    if (_q.capacity() != 7) {
      TNCT_LOG_ERR("capacity should be 7, but it is ", _q.capacity());
      return false;
    }

    if (_q.occupied() != 2) {
      TNCT_LOG_ERR("there should be 2 slots occupied, but there are ",
                   _q.occupied());
      return false;
    }

    TNCT_LOG_TST("traversing queue with 7 slots, and 2 occupied");
    _q.traverse(_printer);

    // reading # 8
    {
      std::optional<typ::data_ptr> _maybe{_q.get()};
      if (!_maybe.has_value()) {
        TNCT_LOG_ERR("no typ::data read, when it should");
        return false;
      }
      _data = std::move(*_maybe);
    }

    if (!_data) {
      TNCT_LOG_ERR("no typ::data read, when it should");
      return false;
    }

    if (*_data != typ::data{-1, -2.3}) {
      TNCT_LOG_ERR("typ::data should be ", typ::data{-1, -2.3}, ", but it is ",
                   *_data);
      return false;
    }

    TNCT_LOG_TST("typ::data = ", *_data);

    if (_q.capacity() != 7) {
      TNCT_LOG_ERR("capacity should be 7, but it is ", _q.capacity());
      return false;
    }

    if (_q.occupied() != 1) {
      TNCT_LOG_ERR("there should be 1 slots occupied, but there are ",
                   _q.occupied());
      return false;
    }

    TNCT_LOG_TST("traversing queue with 7 slots, and 1 occupied");
    _q.traverse(_printer);

    // reading # 9
    {
      std::optional<typ::data_ptr> _maybe{_q.get()};
      if (!_maybe.has_value()) {
        TNCT_LOG_ERR("no typ::data read, when it should");
        return false;
      }
      _data = std::move(*_maybe);
    }

    if (!_data) {
      TNCT_LOG_ERR("no typ::data read, when it should");
      return false;
    }

    if (*_data != typ::data{23, 4.021}) {
      TNCT_LOG_ERR("typ::data should be ", typ::data{23, 4.021}, ", but it is ",
                   *_data);
      return false;
    }

    TNCT_LOG_TST("typ::data = ", *_data);

    if (_q.capacity() != 7) {
      TNCT_LOG_ERR("capacity should be 7, but it is ", _q.capacity());
      return false;
    }

    if (_q.occupied() != 0) {
      TNCT_LOG_ERR("there should be 0 slots occupied, but there are ",
                   _q.occupied());
      return false;
    }

    TNCT_LOG_TST("traversing queue with 7 slots, and 0 occupied");
    _q.traverse(_printer);

    // reading from an empty queue
    {
      std::optional<typ::data_ptr> _maybe{_q.get()};
      if (_maybe.has_value()) {
        TNCT_LOG_ERR("there is typ::data, when it should not be");
        return false;
      }
      _data = std::move(*_maybe);
    }

    TNCT_LOG_TST("no typ::data to be read");

    return true;
  }
};
} // namespace alg
int main(int argc, char **argv) {
  using namespace tenacitas;

  lib::test::alg::tester _tester(argc, argv);
  run_test(_tester, alg::queue_000);
  run_test(_tester, alg::queue_001);
  run_test(_tester, alg::queue_002);
  // run_test(_tester, queue_003);
}
