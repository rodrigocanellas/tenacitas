/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

/// \example application_000

#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>

#include <tenacitas/async.h>
#include <tenacitas/event.h>

#include <tenacitas/number.h>
#include <tenacitas/program.h>
#include <tenacitas/type.h>

using namespace tenacitas;
using namespace std::chrono_literals;

// message to be sent
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

// simulates a temperature sensor generating values
struct temperature_sensor {
    temperature_sensor()
        : m_temperature_generator(
              [this](type::ptr<bool> p_bool) { generator(p_bool); },
              m_timeout,
              m_interval) {

        async::add_handler<event::exit_app>(
            [this](type::ptr<bool>, event::exit_app &&) -> void { stop(); }, 1s,
            async::priority {255});
    }

    // starts to generate temperatures
    void start() { m_temperature_generator.start(); }

    void stop() { m_temperature_generator.stop(); }

private:
    // type for the asynchronous loop that will call the 'generator' method
    typedef async::sleeping_loop_t<> temperature_generator;

private:
    // function to be executed inside the sleeping loop, that will generate the
    // temperatures
    void generator(type::ptr<bool>) {
        m_temperature += 0.2;
        async::dispatch(temperature {m_temperature});
    }

private:
    // temperature to be sent
    float m_temperature = 23.1;

    // timeout for the function executed in the sleeping loop
    const std::chrono::seconds m_timeout {1s};

    // interval of execution of the sleeping loop
    const std::chrono::milliseconds m_interval {500ms};

    // asynchronous loop that will generate temperature at each m_interval
    temperature_generator m_temperature_generator;
};

// thread-safe std::cout printer
struct printer {

    printer() = default;
    printer(const printer &) {}
    void
    operator()(char p_id, uint16_t p_counter, const temperature p_temperature) {
        std::lock_guard<std::mutex> _lock(m_mutex);
        std::cout << '[' << p_id << ',' << p_counter << "," << p_temperature
                  << ']' << std::endl;
    }

private:
    std::mutex m_mutex;
};

// handler of the temperature sent by the sensor
struct temperature_handler {

    temperature_handler(const temperature_handler &p_th)
        : m_id(p_th.m_id)
        , m_printer(p_th.m_printer)
        , m_sleep(p_th.m_sleep) {}

    // p_id allows to distinguish between handlers
    // p_printer is used to print the id of the handler and temperature handled
    // p_sleep allows to define that a handler takes more time to consume a
    // temperature than other
    explicit temperature_handler(char p_id,
                                 printer &p_printer,
                                 std::chrono::milliseconds p_sleep)
        : m_id(p_id)
        , m_printer(p_printer)
        , m_sleep(p_sleep) {}

    void operator()(type::ptr<bool>, temperature &&p_temperature) {
        std::this_thread::sleep_for(m_sleep);
        ++m_counter;
        m_printer(m_id, m_counter, p_temperature);
    }

private:
    char m_id;
    printer &m_printer;
    std::chrono::milliseconds m_sleep;
    uint16_t m_counter {0};
};

void app() {

    std::condition_variable _cond;
    std::mutex _mutex;

    async::add_handler<event::exit_app>(
        [&_cond](type::ptr<bool>, event::exit_app &&) -> void {
            _cond.notify_one();
        },
        100ms);

    std::thread _th([&]() -> void {
        std::cout << "Press Q at any time to stop the application" << std::endl;
        char _c;
        std::cin >> _c;
        while (true) {
            if ((_c == 'Q') || (_c == 'q')) {
                break;
            }
            _cond.notify_one();
        }
        async::dispatch(event::exit_app());
    });

    // thread-safe printer
    printer _printer;

    // adds a handler, that will sleep for 250ms, to a handling, and saves the
    // id of this handling
    async::handling_id _handling_id = async::add_handler<temperature>(
        temperature_handler {'A', _printer, 250ms}, 2s);

    // adds another handler, which will sleepfor 1250ms, to _handling_id, so
    // _handler_1 and _handler_2 will compete with each other to handle
    // temperature messages, in the handling _handling_id
    async::add_handler<temperature>(
        _handling_id, temperature_handler {'B', _printer, 1250ms});

    // add handler that will sleep for 750ms to another handling
    async::add_handler<temperature>(temperature_handler {'C', _printer, 750ms},
                                    2s);

    {
        // declaring the temperature sensor
        temperature_sensor _sensor;

        // starting the sensor
        _sensor.start();

        {
            std::unique_lock<std::mutex> _lock(_mutex);
            _cond.wait(_lock);
            _th.join();
            _sensor.stop();
        }
    }
}

int main() { program::application _program(2s, app); }
