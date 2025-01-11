/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_ASYNC_EXP_EVE_SIMULATION_A_H
#define TENACITAS_LIB_ASYNC_EXP_EVE_SIMULATION_A_H

#include <iostream>

namespace tenacitas::lib::async::exp::events_simulation::eve {

struct simulation_a {
  simulation_a() = default;
  simulation_a(const simulation_a &) = default;
  simulation_a(simulation_a &&) = default;

  simulation_a &operator=(const simulation_a &) = default;
  simulation_a &operator=(simulation_a &&) = default;

  friend std::ostream &operator<<(std::ostream &p_out, const simulation_a &) {
    p_out << "simulation_a";
    return p_out;
  }
};
} // namespace tenacitas::lib::async::exp::events_simulation::eve

#endif
