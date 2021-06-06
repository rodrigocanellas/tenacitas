/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

/// \example messenger_001

#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <iostream>
#include <mutex>

#include <tenacitas/async.h>
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
    // max number of 'temperature' events to be dispatched
    const uint16_t m_max;

    // counter which will be incremented by the function executed in the
    // sleeping loop
    uint16_t m_counter {0};

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

// thread-safe std::cout printer
struct printer {

    void operator()(std::string &&p_str) {
        std::lock_guard<std::mutex> _lock(m_mutex);
        std::cout << p_str;
    }

private:
    std::mutex m_mutex;
};

// event dispatched every time a temperature_handler handles a temperature event
struct temperature_handled {
    friend std::ostream &operator<<(std::ostream &p_out,
                                    const temperature_handled &) {
        p_out << "temperature_handled";
        return p_out;
    }
};

// handles a 'temperature' event
struct temperature_handler_0 {

    // p_id allows to distinguish between handlers
    // p_printer is used to print the id of the handler and temperature handled
    // p_sleep allows to define that a handler takes more time to consume a
    // temperature than other
    explicit temperature_handler_0(const char *p_id, printer &p_printer)
        : m_id(p_id)
        , m_printer(p_printer) {}

    void operator()(type::ptr<bool>, temperature &&p_temperature) {
        std::this_thread::sleep_for(m_sleep);
        ++m_counter;
        async::dispatch(temperature_handled {});

        std::stringstream _stream;
        _stream << '[' << m_id << ',' << m_counter << ',' << p_temperature
                << ']' << std::endl;
        m_printer(_stream.str());
    }

private:
    std::string m_id;
    printer &m_printer;
    std::chrono::milliseconds m_sleep {250ms};
    uint16_t m_counter {0};
};

//  waits for the number of temperature_handled events equals the maximum number
//  of temperature events
struct wait {

    wait(uint16_t p_max)
        : m_max(p_max) {
        async::add_handler<temperature_handled>(
            [this](type::ptr<bool> p_bool, temperature_handled &&p_event)
                -> void { handler(p_bool, std::move(p_event)); },
            100ms);
    }

    void operator()() {
        std::unique_lock<std::mutex> _lock(m_mutex);
        m_cond.wait(_lock);
        return;
    }

private:
    void handler(type::ptr<bool>, temperature_handled &&) {
        ++m_counter;
        if (m_counter >= m_max) {
            m_cond.notify_one();
        }
    }

private:
    const uint16_t m_max;
    uint16_t m_counter {0};
    std::condition_variable m_cond;
    std::mutex m_mutex;
};

int main() {

    // number of temperatures to be generated
    const uint16_t _max {25};

    // thread-safe printer
    printer _printer;

    // adds a handler, that will sleep 250ms, to a handling, and save the id of
    // this handling
    async::handling_id _handling_id = async::add_handler<temperature>(
        temperature_handler_0 {"0a", _printer}, 2s);

    // adds another handler to _handling_id
    async::add_handler<temperature>(_handling_id,
                                    temperature_handler_0 {"0b", _printer});

    // declaring the temperature sensor
    temperature_sensor _sensor {_max};

    // starting the sensor
    _sensor.start();

    // waits for all the temperature events to be handled
    wait _wait(_max);
    _wait();
}
