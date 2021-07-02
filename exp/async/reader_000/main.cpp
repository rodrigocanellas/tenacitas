/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

/// \file In this example there is a tenacitas::async::reader_t class that reads
/// int16_t numbers from the user input, until -1 in entered. A \p handler class
/// prints each number to std::cout.

#include <condition_variable>
#include <cstdint>
#include <iostream>
#include <string>
#include <thread>

#include <tenacitas.lib/async.h>
#include <tenacitas.lib/logger.h>
#include <tenacitas.lib/program.h>

using namespace tenacitas;
using namespace std::chrono_literals;

// synchronoulsy waits for the user to input a number
std::optional<int16_t> input_reader() {

    int16_t _i;
    std::cin >> _i;
    if (_i == -1) {
        std::cout.flush();
        return {};
    }
    std::cout.flush();
    return {_i};
}

// type of the asynchronous reader
typedef async::reader_t<int16_t> reader;

// function that will be executed by a tenacitas::program::application object
void Main() {

    // controls the end of \p Main
    std::mutex _mutex;
    std::condition_variable _cond;

    // handles a int16_t value the user entered
    async::add_handler<reader::data_read>(
        [](std::shared_ptr<bool>, reader::data_read &&p_data) -> void {
            std::cout << "! " << p_data.value << std::endl;
        },
        100ms);

    // handles the event that says there is no more data
    async::add_handler<reader::all_data_read>(
        [&](std::shared_ptr<bool>, reader::all_data_read &&) -> void {
            DEB("no more data");
            _cond.notify_one();
        },
        100ms);

    // the asynchronous reader object
    reader _reader(input_reader);

    std::cout << "Enter numbers at your choice, -1 to end\n";

    // starts the asynchronous reading
    _reader.start();

    // waits for \p reader::all_data_read handler to notify that \p Main may
    // exit
    std::unique_lock<std::mutex> _lock(_mutex);
    _cond.wait(_lock);

    async::dispatch(program::exit_app {});
}

int main() {
    // asynchronous application execution
    program::application _app(1s, Main);
}
