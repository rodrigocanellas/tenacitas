/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

/// \example dispatcher_000

#include <iostream>

#include <tnct/lib/alg/dispatcher.h>

#include "alg.h"
#include "cfg.h"
#include "dispatcher.h"
#include "sto.h"
#include "uix.h"

int main(int argc, char **argv) {
  //  tla::set_trace_level();
  try {

    alg::dispatcher::ptr _dispatcher{alg::dispatcher::create()};

    cfg::options _options{argc, argv};

    sto::saver _save(_options);

    alg::processor _processor(std::move(_save));
    return uix::start(_options, _dispatcher, std::move(_processor));

  } catch (std::exception &_ex) {
    TNCT_LOG_ERR(_ex.what());
    cfg::help(argv[0], std::cout);
  }
}
