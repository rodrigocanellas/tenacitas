/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

/// \example sleeping_loop_001

#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <iostream>
#include <mutex>

#include <tnct/lib/alg/dispatcher.h>
#include <tnct/lib/alg/sleeping_loop.h>
#include <tnct/lib/dat/queue_id.h>

using namespace std::chrono_literals;

// event to be dispatched
struct temperature {

  temperature(float p_value = 0.0) : value(p_value) {}
  friend std::ostream &operator<<(std::ostream &p_out,
                                  const temperature &p_temp) {
    p_out << "temperature = " << p_temp.value;

    return p_out;
  }
  float value{0.0};
};

// event dispatched every time a temperature_subscriber handles a temperature
// event
struct temperature_handled {
  friend std::ostream &operator<<(std::ostream &p_out,
                                  const temperature_handled &) {
    p_out << "temperature_handled";
    return p_out;
  }
};

using dispatcher = tla::dispatcher<temperature, temperature_handled>;

// simulates a temperature sensor generating 'temperature' events
struct temperature_sensor {
  using events_published = std::tuple<temperature>;

  // p_dispatcher is responsible for publishing 'temperature' events to be
  // handled by object that subscribed for
  //
  // p_max is the max number of 'temperature' events to be dispatched
  temperature_sensor(dispatcher::ptr p_dispatcher, uint16_t p_max)
      : m_dispatcher(p_dispatcher), m_max(p_max),
        m_temperature_generator([this]() { generator(); }, m_interval) {}

  // starts to dispatch 'temperature' events
  void start() { m_temperature_generator.start(); }

private:
  // type for the asynchronous loop that will call the 'generator' method
  typedef tla::sleeping_loop temperature_generator;

private:
  // function to be executed inside the sleeping loop, that will dispatch the
  // 'temperature' events
  void generator() {
    if (m_counter >= m_max) {
      if (!m_reported) {
        std::cout << "all temperatures generated" << std::endl;
        m_reported = true;
      }
      return;
    }
    ++m_counter;
    m_temperature.value += 0.2;
    m_dispatcher->publish<temperature_sensor, temperature>(m_temperature);
  }

private:
  // publish/subscriber
  dispatcher::ptr m_dispatcher;

  // max number of 'temperature' events to be dispatched
  const uint16_t m_max;

  // counter which will be incremented by the function executed in the
  // sleeping loop
  uint16_t m_counter{0};

  // temperature to be sent
  temperature m_temperature = 23.1;

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

  void operator()(std::string &&p_str) {
    std::lock_guard<std::mutex> _lock(m_mutex);
    std::cout << p_str;
  }

private:
  std::mutex m_mutex;
};

// handles a 'temperature' event
struct temperature_subscriber_0 {
  using events_published = std::tuple<temperature_handled>;

  // p_dispatcher is responsible for publishing 'temperature' events to be
  // handled by object that subscribed for
  //
  // p_id allows to distinguish between subscribers
  //
  // p_printer is used to print the id of the subscriber and temperature handled
  //
  // p_sleep allows to define that a subscriber takes more time to consume a
  // temperature than other
  explicit temperature_subscriber_0(dispatcher::ptr p_dispatcher,
                                    const char *p_id, printer &p_printer)
      : m_dispatcher(p_dispatcher), m_id(p_id), m_printer(p_printer) {}

  void operator()(temperature &&p_temperature) {
    std::this_thread::sleep_for(m_sleep);
    ++m_counter;
    m_dispatcher->publish<temperature_subscriber_0, temperature_handled>();

    std::stringstream _stream;
    _stream << '[' << m_id << ',' << m_counter << ',' << p_temperature << ']'
            << std::endl;
    m_printer(_stream.str());
  }

private:
  // publish/subscriber
  dispatcher::ptr m_dispatcher;
  std::string m_id;
  printer &m_printer;
  std::chrono::milliseconds m_sleep{250ms};
  uint16_t m_counter{0};
};

//  waits for the number of temperature_handled events equals the maximum number
//  of temperature events
struct wait {

  using events_subscribed = std::tuple<temperature_handled>;

  // p_dispatcher is responsible for publishing 'temperature' events to be
  // handled by object that subscribed for
  //
  // p_max is the max number of 'temperature' events to be dispatched
  wait(dispatcher::ptr p_dispatcher, uint16_t p_max, printer &p_printer)
      : m_dispatcher(p_dispatcher), m_max(p_max), m_printer(p_printer) {
    m_dispatcher->subscribe<wait, temperature_handled>(
        [this](auto p_event) -> void { subscriber(std::move(p_event)); });
  }

  void operator()() {
    std::unique_lock<std::mutex> _lock(m_mutex);
    m_cond.wait(_lock);
    return;
  }

private:
  void subscriber(temperature_handled &&) {
    ++m_counter;
    if (m_counter >= m_max) {
      std::stringstream _stream;
      _stream << m_counter << " events were handled\n";
      m_printer(_stream.str());
      m_cond.notify_one();
    }
  }

private:
  dispatcher::ptr m_dispatcher;
  const uint16_t m_max;
  printer &m_printer;
  uint16_t m_counter{0};
  std::condition_variable m_cond;
  std::mutex m_mutex;
};

struct start {
  using events_subscribed = std::tuple<temperature>;

  void operator()() {
    // publisher/subscriber
    dispatcher::ptr _dispatcher{dispatcher::create()};

    // number of temperatures to be generated
    const uint16_t _max{25};

    // thread-safe printer
    printer _printer;

    {
      std::stringstream _stream;
      _stream << _max << " events will be generated\n";
      _printer(_stream.str());
    }

    // adds a subscriber to a publishing, and save the id of this publishing
    tld::queue_id _queue_id = _dispatcher->subscribe<start, temperature>(
        temperature_subscriber_0{_dispatcher, "0a", _printer});

    // adds another subscriber to the publishing
    _dispatcher->subscribe<start, temperature>(
        _queue_id, temperature_subscriber_0{_dispatcher, "0b", _printer});

    // declaring the temperature sensor
    temperature_sensor _sensor{_dispatcher, _max};

    // starting the sensor
    _sensor.start();

    // waits for all the temperature events to be handled
    wait(_dispatcher, _max, _printer)();
  }
};

int main() { start()(); }
