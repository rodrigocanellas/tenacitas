/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_ASYNC_EXP_DISPATCHER_HANDLER_H
#define TENACITAS_LIB_ASYNC_EXP_DISPATCHER_HANDLER_H

#include <chrono>

#include "tenacitas.lib/async/exp/dispatcher/event.h"
#include "tenacitas.lib/async/exp/dispatcher/event_handled.h"
#include "tenacitas.lib/async/exp/dispatcher/event_id.h"
#include "tenacitas.lib/async/exp/dispatcher/handler_type_id.h"
#include "tenacitas.lib/async/exp/dispatcher/handling_id.h"
#include "tenacitas.lib/async/exp/dispatcher/logger.h"
#include "tenacitas.lib/format/fmt.h"
#include "tenacitas.lib/traits/dispatcher.h"

namespace tenacitas::lib::async::exp {

template <event_id t_event_id, handler_type_id t_type_id,
          traits::dispatcher<async::exp::logger, async::exp::event_handled>
              t_dispatcher>
struct handler {
  using event = async::exp::event<t_event_id>;

  handler(logger &p_logger, t_dispatcher &p_dispatcher,
          const handling_id &p_handling_id,
          std::chrono::milliseconds p_sleep_to_simulate_work)
      : m_logger(p_logger), m_dispatcher(p_dispatcher),
        m_handling_id(p_handling_id),
        m_sleep_to_simulate_work(p_sleep_to_simulate_work) {}

  void operator()(event &&p_event) {
    std::this_thread::sleep_for(m_sleep_to_simulate_work);
    m_dispatcher.template publish<event_handled>(event_handled{
        m_handling_id, t_event_id, t_type_id, std::this_thread::get_id()});
    ++m_num_events;
    TNCT_LOG_TST(m_logger,
                 format::fmt('\'', p_event, "', handling ", m_handling_id,
                             ", handler type #", type_id, ", handler # ",
                             std::this_thread::get_id(), ": handling event #",
                             m_num_events));
  }

  static constexpr async::exp::handler_type_id type_id{t_type_id};
  static constexpr async::exp::event_id event_id{t_event_id};

private:
  logger &m_logger;
  t_dispatcher &m_dispatcher;
  handling_id m_handling_id;
  std::chrono::milliseconds m_sleep_to_simulate_work;

  uint32_t m_num_events{0};
};

} // namespace tenacitas::lib::async::exp

#endif
