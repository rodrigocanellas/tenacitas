/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_ASYNC_EXP_EVE_SIMULATION_FINISHED_H
#define TENACITAS_LIB_ASYNC_EXP_EVE_SIMULATION_FINISHED_H

#include <iostream>

namespace tenacitas::lib::async::exp::events_simulation::eve {

struct simulation_finished {
  simulation_finished() = default;
  simulation_finished(const simulation_finished &) = default;
  simulation_finished(simulation_finished &&) = default;

  simulation_finished &operator=(const simulation_finished &) = default;
  simulation_finished &operator=(simulation_finished &&) = default;

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const simulation_finished &) {
    p_out << "simulation_finished";
    return p_out;
  }
};
} // namespace tenacitas::lib::async::exp::events_simulation::eve

#endif
