/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>
#include <utility>

#include <tenacitas.lib/async.h>
#include <tenacitas.lib/logger.h>
#include <tenacitas.lib/tester.h>
#include <tenacitas.lib/type.h>

using namespace tenacitas;
using namespace tenacitas::type;

typedef std::pair<int16_t, double> data;

typedef async::internal::circular_unlimited_size_queue_t<data> queue;

struct producer {
    producer(queue &p_queue)
        : m_queue(p_queue) {}

    void operator()() {
        DEB("starting producer");
        while (true) {
            if (m_stop) {
                break;
            }
            m_d += 0.01;
            m_i++;
            DEB("adding ", m_i, ",", m_d);
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
};

struct consumer {
    consumer(queue &p_queue)
        : m_queue(p_queue) {}

    void operator()() {
        DEB("starting consumer");
        while (true) {
            if ((m_stop) && (m_queue.empty())) {
                break;
            }
            sptr<const data> _data(m_queue.get());
            if (_data) {
                DEB("getting ", *_data);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(950));
        }
    }

    void stop() { m_stop = true; }

private:
    queue &m_queue;
    bool m_stop = false;
};

struct queue_000 {

    bool operator()() {
        queue _queue(20);
        _queue.add({9, -4.32});
        _queue.traverse([](const data &p_data) -> void { DEB(p_data); });
        sptr<const data> _data = _queue.get();
        if (_data) {
            if (*_data != data(9, -4.32)) {
                ERR("Expected [9, -4.32], but got ", *_data);
                return false;
            }
            INF("got ", *_data);
            if (!_queue.empty()) {
                ERR("Queue should be empty, but it is not");
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

    bool operator()() {

        queue _queue(20);
        producer _producer(_queue);
        consumer _consumer(_queue);

        std::thread _t1([&_producer]() { _producer(); });
        std::thread _t2([&_consumer]() { _consumer(); });

        DEB("going to sleep");
        std::this_thread::sleep_for(std::chrono::seconds(5));
        DEB("waking up");

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

    bool operator()() {

        queue _queue(20);
        producer _producer(_queue);
        consumer _consumer(_queue);

        std::thread _t1([&_producer]() { _producer(); });

        DEB("going to sleep 1");
        std::this_thread::sleep_for(std::chrono::seconds(5));
        DEB("waking up 1");

        std::thread _t2([&_consumer]() { _consumer(); });

        DEB("going to sleep 2");
        std::this_thread::sleep_for(std::chrono::seconds(5));
        DEB("waking up 2");

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

    bool operator()() {
        auto _printer = [](const data &p_data) -> void { DEB(p_data); };
        queue _q {4};

        if (_q.capacity() != 4) {
            ERR("capacity should be 4, but it is ", _q.capacity());
            return false;
        }

        if (_q.occupied() != 0) {
            ERR("there should be 0 slots occupied, but there are ",
                _q.occupied());
            return false;
        }

        DEB("traversing queue with 4 slots, and no occupied");
        _q.traverse(_printer);

        // addition # 1
        _q.add({10, 3.14});
        if (_q.capacity() != 4) {
            ERR("capacity should be 4, but it is ", _q.capacity());
            return false;
        }

        if (_q.occupied() != 1) {
            ERR("there should be 1 slot occupied, but there are ",
                _q.occupied());
            return false;
        }

        DEB("traversing queue with 4 slots, and one occupied");
        _q.traverse(_printer);

        // addition # 2
        _q.add({-29, 0.93});
        if (_q.capacity() != 4) {
            ERR("capacity should be 4, but it is ", _q.capacity());
            return false;
        }

        if (_q.occupied() != 2) {
            ERR("there should be 2 slots occupied, but there are ",
                _q.occupied());
            return false;
        }

        DEB("traversing queue with 4 slots, and 2 occupied");
        _q.traverse(_printer);

        // addition # 3
        _q.add({801, -4.328});
        if (_q.capacity() != 4) {
            ERR("capacity should be 4, but it is ", _q.capacity());
            return false;
        }

        if (_q.occupied() != 3) {
            ERR("there should be 3 slots occupied, but there are ",
                _q.occupied());
            return false;
        }

        DEB("traversing queue with 4 slots, and 3 occupied");
        _q.traverse(_printer);

        // addition # 4
        _q.add({8, 1024.95});
        if (_q.capacity() != 4) {
            ERR("capacity should be 4, but it is ", _q.capacity());
            return false;
        }

        if (_q.occupied() != 4) {
            ERR("there should be 4 slots occupied, but there are ",
                _q.occupied());
            return false;
        }

        DEB("traversing queue with 4 slots, and 4 occupied");
        _q.traverse(_printer);

        // addition # 5
        _q.add({-4, -783.23});
        if (_q.capacity() != 5) {
            ERR("capacity should be 5, but it is ", _q.capacity());
            return false;
        }

        if (_q.occupied() != 5) {
            ERR("there should be 5 slots occupied, but there are ",
                _q.occupied());
            return false;
        }

        DEB("traversing queue with 5 slots, and 5 occupied");
        _q.traverse(_printer);

        // addition # 6
        _q.add({18, 333.33});
        if (_q.capacity() != 6) {
            ERR("capacity should be 6, but it is ", _q.capacity());
            return false;
        }

        if (_q.occupied() != 6) {
            ERR("there should be 6 slots occupied, but there are ",
                _q.occupied());
            return false;
        }

        DEB("traversing queue with 6 slots, and 6 occupied");
        _q.traverse(_printer);

        // addition # 7
        _q.add({455, 12.88});
        if (_q.capacity() != 7) {
            ERR("capacity should be 7, but it is ", _q.capacity());
            return false;
        }

        if (_q.occupied() != 7) {
            ERR("there should be 7 slots occupied, but there are ",
                _q.occupied());
            return false;
        }

        DEB("traversing queue with 7 slots, and 7 occupied");
        _q.traverse(_printer);

        // reading # 1
        sptr<const data> _data = _q.get();
        if (!_data) {
            ERR("no data read, when it should");
            return false;
        }

        if (*_data != data {10, 3.14}) {
            ERR("data should be ", data {10, 3.14}, ", but it is ", _data);
            return false;
        }

        DEB("data = ", _data);

        if (_q.capacity() != 7) {
            ERR("capacity should be 7, but it is ", _q.capacity());
            return false;
        }

        if (_q.occupied() != 6) {
            ERR("there should be 6 slots occupied, but there are ",
                _q.occupied());
            return false;
        }

        DEB("traversing queue with 7 slots, and 6 occupied");
        _q.traverse(_printer);

        // addition # 8, with one slot empty
        _q.add({-1, -2.3});
        if (_q.capacity() != 7) {
            ERR("capacity should be 7, but it is ", _q.capacity());
            return false;
        }

        if (_q.occupied() != 7) {
            ERR("there should be 7 slots occupied, but there are ",
                _q.occupied());
            return false;
        }

        DEB("traversing queue with 7 slots, and 7 occupied");
        _q.traverse(_printer);

        // reading # 2
        _data = _q.get();
        if (!_data) {
            ERR("no data read, when it should");
            return false;
        }

        if (*_data != data {-29, 0.93}) {
            ERR("data should be ", data {-29, 0.93}, ", but it is ", *_data);
            return false;
        }

        DEB("data = ", *_data);

        if (_q.capacity() != 7) {
            ERR("capacity should be 7, but it is ", _q.capacity());
            return false;
        }

        if (_q.occupied() != 6) {
            ERR("there should be 6 slots occupied, but there are ",
                _q.occupied());
            return false;
        }

        DEB("traversing queue with 7 slots, and 6 occupied");
        _q.traverse(_printer);

        // addition # 9, with one slot empty
        _q.add({23, 4.021});
        if (_q.capacity() != 7) {
            ERR("capacity should be 7, but it is ", _q.capacity());
            return false;
        }

        if (_q.occupied() != 7) {
            ERR("there should be 7 slots occupied, but there are ",
                _q.occupied());
            return false;
        }

        DEB("traversing queue with 7 slots, and 7 occupied");
        _q.traverse(_printer);

        // reading # 3
        _data = _q.get();
        if (!_data) {
            ERR("no data read, when it should");
            return false;
        }

        if (*_data != data {801, -4.328}) {
            ERR("data should be ", data {801, -4.328}, ", but it is ", *_data);
            return false;
        }

        DEB("data = ", *_data);

        if (_q.capacity() != 7) {
            ERR("capacity should be 7, but it is ", _q.capacity());
            return false;
        }

        if (_q.occupied() != 6) {
            ERR("there should be 6 slots occupied, but there are ",
                _q.occupied());
            return false;
        }

        DEB("traversing queue with 7 slots, and 6 occupied");
        _q.traverse(_printer);

        // reading # 4
        _data = _q.get();
        if (!_data) {
            ERR("no data read, when it should");
            return false;
        }

        if (*_data != data {8, 1024.95}) {
            ERR("data should be ", data {8, 1024.95}, ", but it is ", *_data);
            return false;
        }

        DEB("data = ", *_data);

        if (_q.capacity() != 7) {
            ERR("capacity should be 7, but it is ", _q.capacity());
            return false;
        }

        if (_q.occupied() != 5) {
            ERR("there should be 5 slots occupied, but there are ",
                _q.occupied());
            return false;
        }

        DEB("traversing queue with 7 slots, and 5 occupied");
        _q.traverse(_printer);

        // reading # 5
        _data = _q.get();
        if (!_data) {
            ERR("no data read, when it should");
            return false;
        }

        if (*_data != data {-4, -783.23}) {
            ERR("data should be ", data {-4, -783.23}, ", but it is ", *_data);
            return false;
        }

        DEB("data = ", *_data);

        if (_q.capacity() != 7) {
            ERR("capacity should be 7, but it is ", _q.capacity());
            return false;
        }

        if (_q.occupied() != 4) {
            ERR("there should be 4 slots occupied, but there are ",
                _q.occupied());
            return false;
        }

        DEB("traversing queue with 7 slots, and 4 occupied");
        _q.traverse(_printer);

        // reading # 6
        _data = _q.get();
        if (!_data) {
            ERR("no data read, when it should");
            return false;
        }

        if (*_data != data {18, 333.33}) {
            ERR("data should be ", data {18, 333.33}, ", but it is ", *_data);
            return false;
        }

        DEB("data = ", *_data);

        if (_q.capacity() != 7) {
            ERR("capacity should be 7, but it is ", _q.capacity());
            return false;
        }

        if (_q.occupied() != 3) {
            ERR("there should be 3 slots occupied, but there are ",
                _q.occupied());
            return false;
        }

        DEB("traversing queue with 7 slots, and 3 occupied");
        _q.traverse(_printer);

        // reading # 7
        _data = _q.get();
        if (!_data) {
            ERR("no data read, when it should");
            return false;
        }

        if (*_data != data {455, 12.88}) {
            ERR("data should be ", data {455, 12.88}, ", but it is ", *_data);
            return false;
        }

        DEB("data = ", *_data);

        if (_q.capacity() != 7) {
            ERR("capacity should be 7, but it is ", _q.capacity());
            return false;
        }

        if (_q.occupied() != 2) {
            ERR("there should be 2 slots occupied, but there are ",
                _q.occupied());
            return false;
        }

        DEB("traversing queue with 7 slots, and 2 occupied");
        _q.traverse(_printer);

        // reading # 8
        _data = _q.get();
        if (!_data) {
            ERR("no data read, when it should");
            return false;
        }

        if (*_data != data {-1, -2.3}) {
            ERR("data should be ", data {-1, -2.3}, ", but it is ", *_data);
            return false;
        }

        DEB("data = ", *_data);

        if (_q.capacity() != 7) {
            ERR("capacity should be 7, but it is ", _q.capacity());
            return false;
        }

        if (_q.occupied() != 1) {
            ERR("there should be 1 slots occupied, but there are ",
                _q.occupied());
            return false;
        }

        DEB("traversing queue with 7 slots, and 1 occupied");
        _q.traverse(_printer);

        // reading # 9
        _data = _q.get();
        if (!_data) {
            ERR("no data read, when it should");
            return false;
        }

        if (*_data != data {23, 4.021}) {
            ERR("data should be ", data {23, 4.021}, ", but it is ", *_data);
            return false;
        }

        DEB("data = ", *_data);

        if (_q.capacity() != 7) {
            ERR("capacity should be 7, but it is ", _q.capacity());
            return false;
        }

        if (_q.occupied() != 0) {
            ERR("there should be 0 slots occupied, but there are ",
                _q.occupied());
            return false;
        }

        DEB("traversing queue with 7 slots, and 0 occupied");
        _q.traverse(_printer);

        // reading from an empty queue
        _data = _q.get();

        if (_data) {
            ERR("read data from an empty queue");
            return false;
        }

        DEB("no data to be read");

        return true;
    }
};

int main(int argc, char **argv) {
    logger::set_debug_level();

    tester::test _test(argc, argv);
    run_test(_test, queue_000);
    run_test(_test, queue_001);
    run_test(_test, queue_002);
    run_test(_test, queue_003);
}
