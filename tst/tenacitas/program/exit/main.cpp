/// \example

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <iostream>

#include <tenacitas/async.h>
#include <tenacitas/logger.h>
#include <tenacitas/macros.h>
#include <tenacitas/message.h>
#include <tenacitas/program.h>
#include <tenacitas/tester.h>

using namespace tenacitas;
using namespace std::chrono_literals;

struct exit_000 {
  static std::string desc() { return "Tests 'application' exit"; }

  bool operator()() {
    logger::set_debug_level();

    logger::cerr<> _log{"main"};

    //  _log.set_debug_level();

    const uint16_t _max{10};
    uint16_t _counter{0};
    auto _function = [&_counter, &_log]() -> void {
      if (_counter > _max) {
        DEB(_log, "about to publish 'exit_app'");
        async::messenger_t<message::exit_app>::publish({});
      } else {
        DEB(_log, _counter++);
      }
    };

    async::sleeping_loop_t<void> _loop(200ms, 1s, _function);

    program::application _app(2s, [&_loop]() { _loop.start(); });

    return true;
  }
};

struct exit_001 {
  static std::string desc() {
    return "Tests 'application' exit, with 'function' taking too long to "
           "finish";
  }

  bool operator()() {
    logger::set_debug_level();

    logger::cerr<> _log{"main"};

    //  _log.set_debug_level();

    const uint16_t _max{10};
    uint16_t _counter{0};
    auto _function = [&_counter, &_log]() -> void {
      if (_counter > _max) {

        DEB(_log, "about to publish 'exit_app'");
        async::messenger_t<message::exit_app>::publish({});
        DEB(_log, "sleeping...");
        std::this_thread::sleep_for(5s);
        DEB(_log, "waking up...");
      } else {
        DEB(_log, _counter++);
      }
    };

    async::sleeping_loop_t<void> _loop(200ms, 1s, _function);

    program::application _app(2s, [&_loop]() { _loop.start(); });

    return true;
  }
};

int main(int argc, char **argv) {
  logger::set_debug_level();

  tester::test _test(argc, argv);
  run_test(_test, exit_000);
  run_test(_test, exit_001);
}
