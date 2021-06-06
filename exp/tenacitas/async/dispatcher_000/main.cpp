/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

/// \example dispatcher_000

#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <iostream>
#include <mutex>

#include <tenacitas/async.h>
#include <tenacitas/number.h>
#include <tenacitas/type.h>

using namespace tenacitas;
using namespace std::chrono_literals;

// event to be dispatched
struct temperature {
    explicit temperature(float p_value = 0.0)
        : value(p_value) {}

    friend std::ostream &operator<<(std::ostream &p_out,
                                    const temperature &p_temperature) {
        p_out << p_temperature.value;
        return p_out;
    }

    float value {0.0};
};

// simulates a temperature sensor generating 'temperature' events
struct temperature_sensor {
    // p_max is the max number of 'temperature' events to be dispatched
    temperature_sensor(uint16_t p_max)
        : m_max(p_max)
        , m_temperature_generator(
              [this](type::ptr<bool>) { generator(); }, m_timeout, m_interval) {
    }

    // starts to dispatch 'temperature' events
    void start() { m_temperature_generator.start(); }

private:
    // type for the asynchronous loop that will call the 'generator' method
    typedef async::sleeping_loop_t<> temperature_generator;

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
        m_temperature += 0.2;
        async::dispatch(temperature {m_temperature});
    }

private:
    // counter which will be incremented by the function executed in the
    // sleeping loop
    uint16_t m_counter {0};

    // max number of 'temperature' events to be dispatched
    const uint16_t m_max;

    // temperature to be sent
    float m_temperature = 23.1;

    // timeout for the function executed in the sleeping loop
    const std::chrono::seconds m_timeout {1s};

    // interval of execution of the sleeping loop
    const std::chrono::milliseconds m_interval {500ms};

    // asynchronous loop that will generate temperature at each m_interval
    temperature_generator m_temperature_generator;

    // to avoid writing more than one time that all the temperatures were
    // generated
    bool m_reported {false};
};

// event dispatched when all temperatures are handled
struct all_handled {
    friend std::ostream &operator<<(std::ostream &p_out, const all_handled &) {
        p_out << "all_handled";
        return p_out;
    }
};

// handles a 'temperature' event
struct temperature_handler_0 {
    temperature_handler_0(uint16_t p_max)
        : m_max(p_max) {}
    void operator()(type::ptr<bool>, temperature &&p_temperature) {
        std::cout << ++m_counter << " - " << p_temperature << std::endl;
        std::this_thread::sleep_for(1s);

        if ((m_counter) >= m_max) {
            async::dispatch(all_handled {});
        }
    }

private:
    uint16_t m_max {0};
    uint16_t m_counter {0};
};

int main() {
    // condition variable and mutex to wait for the sleeping loop to finish
    std::condition_variable _cond;
    std::mutex _mutex;

    // number of temperatures to be generated
    const uint16_t _max {20};

    // adding the function that will handle temperatures to an unamed queue of
    // temperature handlers; 2s is the time _temperature_handler has to execute
    async::add_handler<temperature>(temperature_handler_0 {_max}, 2s);

    // declaring the temperature sensor
    temperature_sensor _sensor {_max};

    // starting the sensor
    _sensor.start();

    // handles the all_handled event, dispatched when all the temperatures were
    // handled, and notifies that the program may finish
    async::add_handler<all_handled>(
        [&](type::ptr<bool>, all_handled &&) -> void {
            std::cout << "all temperatures handled" << std::endl;
            _cond.notify_one();
            return;
        },
        100ms);

    // waits for the function  that handles temperatures to notify that the
    // expected number of temperatures was generated
    std::unique_lock<std::mutex> _lock(_mutex);
    _cond.wait(_lock);
}
