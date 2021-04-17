/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

/// \example messenger_000

#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <iostream>
#include <mutex>

#include <tenacitas/async.h>
#include <tenacitas/number.h>

using namespace tenacitas;
using namespace std::chrono_literals;

// message to be sent
struct temperature {
  explicit temperature(float p_value = 0.0) : value(p_value) {}

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const temperature &p_temperature) {
    p_out << p_temperature.value;
    return p_out;
  }

  float value{0.0};
};

// simulates a temperature sensor generating values
struct temperature_sensor {
  temperature_sensor(uint16_t p_max)
      : m_max(p_max), m_temperature_generator(m_id, m_timeout, m_interval,
                                              [this]() { generator(); }) {}

  // starts to generate temperatures
  void start() { m_temperature_generator.start(); }

private:
  // type for the asynchronous loop that will call the 'generator' method
  typedef async::sleeping_loop_t<void> temperature_generator;

private:
  // function to be executed inside the sleeping loop, that will generate the
  // temperatures
  void generator() {
    if (m_counter >= m_max) {
      if (!m_reported) {
        std::cout << "all temperatures generated" << std::endl;
        m_reported = true;
      }
      return;
    }
    ++m_counter;
    m_temperature += 0.2;
    async::send(temperature{m_temperature});
  }

private:
  // counter which will be incremented by the function executed in the sleeping
  // loop
  uint16_t m_counter{0};

  // max number of temperatures value of the counter
  const uint16_t m_max;

  // temperature to be sent
  float m_temperature = 23.1;

  // timeout for the function executed in the sleeping loop
  const std::chrono::seconds m_timeout{1s};

  // interval of execution of the sleeping loop
  const std::chrono::milliseconds m_interval{500ms};

  // asynchronous loop that will generate temperature at each m_interval
  temperature_generator m_temperature_generator;

  // to avoid writing more than one time that all the temperatures were
  // generated
  bool m_reported{false};

  number::id m_id;
};

int main() {

  // condition variable and mutex to wait for the sleeping loop to finish
  std::condition_variable _cond;
  std::mutex _mutex;

  // number of temperatures to be generated
  const uint16_t _max{20};

  // counter of temperatures received
  uint16_t _counter_received{0};

  // function that will handle temperatures sent
  std::function<void(const temperature &)> _temperature_handler =
      [&_counter_received, &_cond](const temperature &p_temperature) -> void {
    std::cout << ++_counter_received << " - " << p_temperature << std::endl;
    std::this_thread::sleep_for(1s);

    if (_counter_received >= _max) {
      _cond.notify_one();
    }
  };

  // adding the function that will handle temperatures to an unamed queue of
  // temperature handlers; 2s is the time _temperature_handler has to execute
  async::add_handler(_temperature_handler, 2s);

  // declaring the temperature sensor
  temperature_sensor _sensor{_max};

  // starting the sensor
  _sensor.start();

  // waits for the function  that handles temperatures to notify that the
  // expected number of temperatures was generated
  std::unique_lock<std::mutex> _lock(_mutex);
  _cond.wait(_lock);
}
