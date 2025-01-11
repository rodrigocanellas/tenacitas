/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_ASYNC_EXP_EVE_SIMULATION_B_H
#define TENACITAS_LIB_ASYNC_EXP_EVE_SIMULATION_B_H

#include <iostream>

namespace tenacitas::lib::async::exp::events_simulation::eve {

struct simulation_b {
  simulation_b() = default;
  simulation_b(const simulation_b &) = default;
  simulation_b(simulation_b &&) = default;

  simulation_b &operator=(const simulation_b &) = default;
  simulation_b &operator=(simulation_b &&) = default;

  friend std::ostream &operator<<(std::ostream &p_out, const simulation_b &) {
    p_out << "event_b";
    return p_out;
  }
};
} // namespace tenacitas::lib::async::exp::events_simulation::eve

#endif
