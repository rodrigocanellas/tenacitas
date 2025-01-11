/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_ASYNC_EXP_BUS_SIMULATION_HANDLER_H
#define TENACITAS_LIB_ASYNC_EXP_BUS_SIMULATION_HANDLER_H

#include <chrono>
#include <string>
#include <string_view>
#include <thread>

#include "tenacitas.lib/async/exp/events_simulation/typ/handler_id.h"
#include "tenacitas.lib/async/exp/events_simulation/typ/handling_id.h"
#include "tenacitas.lib/traits/event.h"
#include "tenacitas.lib/traits/logger.h"

namespace tenacitas::lib::async::exp::events_simulation::bus {

template <traits::event t_simulation, traits::logger t_logger>
struct simulation_event_handler {
  simulation_event_handler(t_logger &p_logger,
                           const typ::handling_id &p_handling_id,
                           std::string_view p_event,
                           std::chrono::milliseconds p_sleep)
      : m_logger(p_logger), m_handling_id(p_handling_id), m_event(p_event),
        m_sleep(p_sleep) {}

  void operator()(t_simulation &&) { std::this_thread::sleep_for(m_sleep); }

private:
  t_logger &m_logger;
  typ::handling_id m_handling_id;
  std::string m_event;
  std::chrono::milliseconds m_sleep;
  typ::handler_id id;
};
} // namespace tenacitas::lib::async::exp::events_simulation::bus

#endif
