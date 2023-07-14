#ifndef TENACITAS_LIB_CROSSWORDS_ALG_DISPATCHER_H
#define TENACITAS_LIB_CROSSWORDS_ALG_DISPATCHER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <iostream>
#include <memory>

#include <tenacitas.lib/src/async/alg/dispatcher.h>
#include <tenacitas.lib/src/crosswords/evt/events.h>

namespace tenacitas::lib::crosswords::alg {

using dispatcher =
    async::alg::dispatcher<evt::new_grid_to_organize, evt::assembly_finished,
                           evt::stop_organizing, evt::new_attempt>;
}

#endif
