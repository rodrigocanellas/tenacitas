/// \example Basic use of tenacitas::concurrent::loop_t

#include <chrono>
#include <cstdint>
#include <iostream>
#include <string>

#include <concurrent/fixed_size_queue.h>
#include <concurrent/internal/log.h>
#include <concurrent/loop.h>
#include <concurrent/producer_consumer.h>
#include <concurrent/result.h>
#include <logger/cerr/log.h>
#include <status/result.h>
#include <tester/test.h>

using namespace tenacitas;
typedef tenacitas::logger::cerr::log log;

void printer(const int16_t &p_i) { std::cout << p_i << " "; }

typedef concurrent::fixed_size_queue_t<int16_t> queue;
typedef concurrent::producer_consumer_t<queue, log> producer_consumer;

struct work {
  status::result operator()(int16_t &&p_value) {
    concurrent_log_info(log, "value = ", p_value);
    return status::ok;
  }
};

struct producer_consumer_000 {
  bool operator()() {
    producer_consumer _pc({queue(10)});
    _pc.add(work(), std::chrono::milliseconds(500));
    _pc.start();
    _pc.add(-8);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    return true;
  }

  static std::string desc() {
    return "Simple test, creating a producer_consumer, adding a single data, "
           "and starting";
  }
};

// struct queue_001 {
//  typedef concurrent::queue_t<int16_t, 5, log> queue;

//  bool operator()() {

//    queue _queue;
//    producer _producer(&_queue);
//    consumer _consumer(&_queue);

//    std::thread _t1([&_producer]() { _producer(); });
//    std::thread _t2([&_consumer]() { _consumer(); });

//    concurrent_log_debug(log, "Going to sleep");
//    std::this_thread::sleep_for(std::chrono::seconds(5));
//    concurrent_log_debug(log, "Waking up");

//    _producer.stop();
//    _consumer.stop();

//    _t1.join();
//    _t2.join();

//    return true;
//  }

//  static std::string desc() { return "1 Consumer e 1 producer"; }
//};

// struct queue_002 {
//  typedef concurrent::queue_t<int16_t, 5, log> queue;

//  bool operator()() {

//    queue _queue;
//    producer _producer(&_queue);
//    consumer _consumer(&_queue);

//    std::thread _t1([&_producer]() { _producer(); });

//    concurrent_log_debug(log, "Going to sleep 1");
//    std::this_thread::sleep_for(std::chrono::seconds(5));
//    concurrent_log_debug(log, "Waking up 1");

//    std::thread _t2([&_consumer]() { _consumer(); });

//    concurrent_log_debug(log, "Going to sleep 2");
//    std::this_thread::sleep_for(std::chrono::seconds(5));
//    concurrent_log_debug(log, "Waking up 2");

//    _producer.stop();
//    _consumer.stop();

//    _t1.join();
//    _t2.join();

//    return true;
//  }

//  static std::string desc() {
//    return "1 Consumer e 1 producer, consumer started 5 seconds after
//    producer";
//  }
//};

int main(int argc, char **argv) {
  log::set_debug();
  tester::test _test(argc, argv);
  run_test(_test, producer_consumer_000);
  //  run_test(_test, queue_001);
  //  run_test(_test, queue_002);
}
