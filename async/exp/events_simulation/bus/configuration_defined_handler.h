/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_ASYNC_EXP_BUS_CONFIGURATION_DEFINED_HANDLER_H
#define TENACITAS_LIB_ASYNC_EXP_BUS_CONFIGURATION_DEFINED_HANDLER_H

#include "tenacitas.lib/async/exp/events_simulation/cfg/handlings.h"
#include "tenacitas.lib/async/exp/events_simulation/cfg/publishers.h"
#include "tenacitas.lib/async/exp/events_simulation/eve/configuration_defined.h"
#include "tenacitas.lib/format/fmt.h"
#include "tenacitas.lib/traits/logger.h"

namespace tenacitas::lib::async::exp::events_simulation::bus {

template <traits::logger t_logger> struct configuration_defined_handler {
  configuration_defined_handler(t_logger &p_logger) : m_logger(p_logger) {}

  void operator()(eve::configuration_defined &&p_configuration_defined) {
    const cfg::handlings &_cfg_handlings{p_configuration_defined.m_handlings};

    for (const cfg::handling &_cfg_handling : _cfg_handlings) {
      TNCT_LOG_INF(m_logger, format::fmt(_cfg_handling));
    }

    const cfg::publishers &_cfg_publishers{
        p_configuration_defined.m_publishers};
    for (const cfg::publisher &_cfg_publisher : _cfg_publishers) {
      TNCT_LOG_INF(m_logger, format::fmt(_cfg_publisher));
    }
  }

private:
  t_logger &m_logger;
};
} // namespace tenacitas::lib::async::exp::events_simulation::bus

#endif
