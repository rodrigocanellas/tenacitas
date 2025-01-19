/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_ASYNC_EXP_EVE_DISPATCHER_H
#define TENACITAS_LIB_ASYNC_EXP_EVE_DISPATCHER_H

#include "tenacitas.lib/async/dispatcher.h"
#include "tenacitas.lib/async/exp/events_simulation/eve/simulation_a.h"
#include "tenacitas.lib/async/exp/events_simulation/eve/simulation_b.h"
#include "tenacitas.lib/async/exp/events_simulation/eve/simulation_c.h"
#include "tenacitas.lib/async/exp/events_simulation/eve/simulation_finished.h"
#include "tenacitas.lib/traits/logger.h"

namespace tenacitas::lib::async::exp::events_simulation::eve {

template <traits::logger t_logger>
using dispatcher =
    async::dispatcher<t_logger, eve::simulation_finished, eve::simulation_a,
                      eve::simulation_b, eve::simulation_c>;

}

#endif
