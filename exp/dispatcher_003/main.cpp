/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

/// \example dispatcher_003/main.cpp

#include <iostream>

#include <tenacitas.lib/asy/dispatcher.h>

#include <tenacitas.lib/exp/dispatcher_003/asy/asy.h>
#include <tenacitas.lib/exp/dispatcher_003/cfg/cfg.h>
#include <tenacitas.lib/exp/dispatcher_003/dom/fun.h>
#include <tenacitas.lib/exp/dispatcher_003/per/per.h>
#include <tenacitas.lib/exp/dispatcher_003/uix/uix.h>

int main(int argc, char **argv) {
  // tenacitas::lib::log::set_trace_level();
  try {

    asy::dispatcher::ptr _dispatcher{asy::dispatcher::create()};

    cfg::options _options{argc, argv};

    per::saver _save(_options);

    dom::processor _processor(std::move(_save));
    return uix::start(_options, _dispatcher, std::move(_processor));

  } catch (std::exception &_ex) {
    TNCT_LOG_ERR(_ex.what());
    cfg::help(argv[0], std::cout);
  }
}
