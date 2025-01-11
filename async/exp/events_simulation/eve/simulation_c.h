/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_ASYNC_EXP_EVE_SIMULATION_C_H
#define TENACITAS_LIB_ASYNC_EXP_EVE_SIMULATION_C_H

#include <iostream>

namespace tenacitas::lib::async::exp::events_simulation::eve {

struct simulation_c {
  simulation_c() = default;
  simulation_c(const simulation_c &) = default;
  simulation_c(simulation_c &&) = default;

  simulation_c &operator=(const simulation_c &) = default;
  simulation_c &operator=(simulation_c &&) = default;

  friend std::ostream &operator<<(std::ostream &p_out, const simulation_c &) {
    p_out << "event_c";
    return p_out;
  }
};
} // namespace tenacitas::lib::async::exp::events_simulation::eve

#endif
