#ifndef TNCT_CROSSWORDS_ASY_DISPATCHER_H
#define TNCT_CROSSWORDS_ASY_DISPATCHER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include "tnct/async/dispatcher.h"
#include "tnct/crosswords/evt/grid_attempt_configuration.h"
#include "tnct/crosswords/evt/grid_create_solved.h"
#include "tnct/crosswords/evt/grid_create_start.h"
#include "tnct/crosswords/evt/grid_create_stop.h"
#include "tnct/crosswords/evt/grid_create_unsolved.h"
#include "tnct/crosswords/evt/grid_permutations_tried.h"
#include "tnct/log/cerr.h"
#include "tnct/log/cpt/macros.h"

namespace tnct::crosswords::evt
{

using dispatcher = tnct::async::dispatcher<
    log::cerr, /*evt::grid_create_timeout, */ evt::grid_permutations_tried,
    evt::grid_create_solved, evt::grid_create_start, evt::grid_create_stop,
    evt::grid_create_unsolved, evt::grid_attempt_configuration>;
}

#endif
