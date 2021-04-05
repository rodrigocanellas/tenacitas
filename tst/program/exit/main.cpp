/// \example

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <iostream>

#include <tenacitas/concurrent.h>
#include <tenacitas/logger.h>
#include <tenacitas/macros.h>
#include <tenacitas/message.h>
#include <tenacitas/program.h>

using namespace tenacitas;
using namespace std::chrono_literals;

int main() {

  logger::set_debug_level();

  logger::cerr<> _log{"main"};

  program::application _app(2s);

  const uint16_t _max{20};
  uint16_t _counter{0};
  concurrent::sleeping_loop_t<void> _loop(
      200ms, 1s, [&_counter, &_log]() -> void {
        if (_counter > _max) {
          DEB(_log, "about to publish 'exit_app'");
          concurrent::messenger_t<message::exit_app>::publish({});
        }
        DEB(_log, _counter++);
      });

  _loop.start();

  DEB(_log, "loop started");

  _app.start();
}
