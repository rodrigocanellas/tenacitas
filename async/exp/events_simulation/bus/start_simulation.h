/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_ASYNC_EXP_START_SIMULATION_H
#define TENACITAS_LIB_ASYNC_EXP_START_SIMULATION_H

#include "tenacitas.lib/async/exp/events_simulation/bus/simulation_handler.h"
#include "tenacitas.lib/async/exp/events_simulation/cfg/handlings.h"
#include "tenacitas.lib/async/exp/events_simulation/cfg/publishers.h"
#include "tenacitas.lib/async/exp/events_simulation/eve/simulation_a.h"
#include "tenacitas.lib/async/exp/events_simulation/eve/simulation_b.h"
#include "tenacitas.lib/async/exp/events_simulation/eve/simulation_c.h"
#include "tenacitas.lib/container/circular_queue.h"
#include "tenacitas.lib/format/fmt.h"
#include "tenacitas.lib/traits/dispatcher.h"
#include "tenacitas.lib/traits/event.h"
#include "tenacitas.lib/traits/logger.h"

namespace tenacitas::lib::async::exp::events_simulation::bus {

template <traits::logger t_logger,
          traits::dispatcher<t_logger, eve::simulation_a, eve::simulation_b,
                             eve::simulation_c>
              t_dispatcher>
struct start_simulation {
  start_simulation(t_logger &p_logger, t_dispatcher &p_dispatcher)
      : m_logger(p_logger), m_dispatcher(p_dispatcher) {}

  bool operator()(const cfg::handlings &p_cfg_handlings,
                  const cfg::publishers &p_cfg_publishers) {
    if (!start_handlings(p_cfg_handlings)) {
      return false;
    }
    start_publishers(p_cfg_publishers);

    return true;
  }

private:
  template <traits::event t_event>
  using queue = container::circular_queue<t_logger, t_event, 500>;

private:
  bool start_handlings(const cfg::handlings &p_cfg_handlings) {
    for (const cfg::handling &_handling : p_cfg_handlings) {
      if (_handling.m_event == "ev_a") {
        if (!start_handling<eve::simulation_a>(_handling)) {
          return false;
        }
      }
    }
    return true;
  }

  void start_publishers(const cfg::publishers & /*p_cfg_publishers*/) {}

  template <traits::event t_event>
  bool start_handling(const cfg::handling &p_handling) {
    using handler = bus::simulation_event_handler<t_event, t_logger>;
    handler _handler{m_logger, p_handling.m_id, p_handling.m_event,
                     p_handling.m_sleep_to_fake_work};

    auto _maybe{m_dispatcher.template subscribe<t_event>(
        std::move(_handler), queue<t_event>{m_logger},
        p_handling.m_num_handlers)};
    if (!_maybe) {
      TNCT_LOG_ERR(m_logger, format::fmt("error creating handling for event ",
                                         p_handling.m_event));
      return false;
    }
    TNCT_LOG_INF(m_logger, format::fmt("handling ", _maybe.value(),
                                       " created for cfg ", p_handling));
    return true;
  }

private:
  t_logger &m_logger;
  t_dispatcher &m_dispatcher;
};

} // namespace tenacitas::lib::async::exp::events_simulation::bus
#endif
