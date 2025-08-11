#ifndef TENACITAS_SRC_CROSSWORDS_ASY_DISPATCHER_H
#define TENACITAS_SRC_CROSSWORDS_ASY_DISPATCHER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include "tenacitas/src/async/dispatcher.h"
#include "tenacitas/src/crosswords/asy/events.h"
#include "tenacitas/src/log/cerr.h"

namespace tenacitas::src::crosswords::asy {

using dispatcher = tenacitas::src::async::dispatcher<
    tenacitas::src::log::cerr, asy::grid_create_timeout, asy::grid_to_organize,
    asy::grid_permutations_tried, asy::grid_create_solved,
    asy::grid_create_start, asy::grid_create_stop, asy::grid_create_unsolved,
    asy::grid_create_status, asy::grid_attempt_configuration>;
}

#endif
