/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_ASYNC_EXP_EVE_CONFIGURATION_DEFINED_H
#define TENACITAS_LIB_ASYNC_EXP_EVE_CONFIGURATION_DEFINED_H

#include <iostream>

#include "tenacitas.lib/async/exp/events_simulation/cfg/handlings.h"
#include "tenacitas.lib/async/exp/events_simulation/cfg/publishers.h"

namespace tenacitas::lib::async::exp::events_simulation::eve {

struct configuration_defined {
  configuration_defined() = default;
  configuration_defined(const configuration_defined &) = default;
  configuration_defined(configuration_defined &&) = default;
  configuration_defined(cfg::handlings &&p_handlings,
                        cfg::publishers &&p_publishers)
      : m_handlings(std::move(p_handlings)),
        m_publishers(std::move(p_publishers)) {}
  configuration_defined &operator=(const configuration_defined &) = default;
  configuration_defined &operator=(configuration_defined &&) = default;

  friend std::ostream &
  operator<<(std::ostream &p_out,
             const configuration_defined &p_configuration_defined) {
    p_out << "handlings : { ";
    for (const auto &_handling : p_configuration_defined.m_handlings) {
      p_out << _handling << ',';
    }
    p_out << "} ";
    p_out << "subscribers : {";
    for (const auto &_subsciber : p_configuration_defined.m_publishers) {
      p_out << _subsciber << ',';
    }
    p_out << '}';
    return p_out;
  }

  cfg::handlings m_handlings;
  cfg::publishers m_publishers;
};
} // namespace tenacitas::lib::async::exp::events_simulation::eve

#endif
