/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

/// \example sleeping_loop_000

#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <iostream>
#include <mutex>

// tenacitas::async::sleeping_loop is defined in 'tenacitas/async.h'
#include <tenacitas/async.h>
#include <tenacitas/number.h>

using namespace tenacitas;
using namespace std::chrono_literals;

int main() {

  number::id _id;

  // this the function to be executed in this sleeping loop does not require
  // parameters
  typedef async::sleeping_loop_t<void> sleeping_loop;

  // timeout for the function executed in the sleeping loop
  const std::chrono::milliseconds _timeout{200ms};

  // interval of execution of the sleeping loop
  const std::chrono::seconds _interval{1s};

  // counter which will be incremented by the function executed in the sleeping
  // loop
  uint16_t _counter{0};

  // max value of the counter
  const uint16_t _max{20};

  // condition variable and mutex to wait for the sleeping loop to finish
  std::condition_variable _cond;
  std::mutex _mutex;

  // function to be executed inside the sleeping loop
  std::function<void()> _function = [&_counter, &_cond]() -> void {
    if (_counter >= _max) {
      _cond.notify_one();
    } else {
      ++_counter;
      std::cout << "counter = " << _counter << std::endl;
    }
  };

  // sleeping loop object
  sleeping_loop _sleeping_loop(_id, _timeout, _interval, _function);

  // starts the loop asynchronously
  _sleeping_loop.start();

  // waits for the function to be executed inside the sleeping loop to notify
  // that the counter reached the maximum value
  std::unique_lock<std::mutex> _lock(_mutex);
  _cond.wait(_lock);
}
