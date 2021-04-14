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
      : m_max(p_max), m_temperature_generator(m_timeout, m_interval,
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
};

// thread-safe std::cout printer
struct printer {

  printer() = default;
  printer(const printer &) {}
  void operator()(uint16_t p_id, const temperature p_temperature) {
    std::lock_guard<std::mutex> _lock(m_mutex);
    std::cout << '[' << p_id << ',' << p_temperature << ']' << std::endl;
  }

private:
  std::mutex m_mutex;
};

// handler of the temperature sent by the sensor
struct temperature_handler {

  temperature_handler(const temperature_handler &p_th)
      : m_id(p_th.m_id), m_printer(p_th.m_printer), m_sleep(p_th.m_sleep) {}

  // p_id allows to distinguish between handlers
  // p_printer is used to print the id of the handler and temperature handled
  // p_sleep allows to define that a handler takes more time to consume a
  // temperature than other
  temperature_handler(uint16_t p_id, printer &p_printer,
                      std::chrono::milliseconds p_sleep)
      : m_id(p_id), m_printer(p_printer), m_sleep(p_sleep) {}

  void operator()(const temperature &p_temperature) {
    std::this_thread::sleep_for(m_sleep);
    ++m_counter;
    m_printer(m_id, p_temperature);
  }

  // counter of the number of temperatures handled
  inline uint16_t counter() const { return m_counter; }

private:
  uint16_t m_id;
  printer &m_printer;
  std::chrono::milliseconds m_sleep;
  uint16_t m_counter{0};
};

int main() {

  // number of temperatures to be generated
  const uint16_t _max{20};

  // thread-safe printer
  printer _printer;

  // handler 1, that will sleep 250ms while handling
  temperature_handler _handler_1{1, _printer, 250ms};

  // handler 2, that will sleep 1250ms while handling
  temperature_handler _handler_2{2, _printer, 1250ms};

  // lambda 1, so that _handler_1 can be used later
  auto _h1 = [&_handler_1](const temperature &p_temperature) -> void {
    _handler_1(p_temperature);
  };

  // lambda 2, so that _handler_2 can be used later
  auto _h2 = [&_handler_2](const temperature &p_temperature) -> void {
    _handler_2(p_temperature);
  };

  // adds a handler to a handlers group, and save the id of this handlers group
  async::id _handlers_id = async::add_handler<temperature>(_h1, 2s);

  // adds another handler to _handlers_id
  async::add_handler<temperature>(_handlers_id, _h2);

  // declaring the temperature sensor
  temperature_sensor _sensor{_max};

  // starting the sensor
  _sensor.start();

  // busy wait for all the temperatures to be handled
  while ((_handler_1.counter() + _handler_2.counter()) != _max) {
    std::this_thread::sleep_for(50ms);
  }
}
