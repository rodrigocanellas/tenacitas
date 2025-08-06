/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_EXP_ASYNC_DISPATCHER_HANDLER_H
#define TENACITAS_EXP_ASYNC_DISPATCHER_HANDLER_H

#include <chrono>

#include "tenacitas/exp/async/dispatcher_000/event.h"
#include "tenacitas/exp/async/dispatcher_000/event_handled.h"
#include "tenacitas/exp/async/dispatcher_000/event_id.h"
#include "tenacitas/exp/async/dispatcher_000/handler_type_id.h"
#include "tenacitas/exp/async/dispatcher_000/logger.h"
#include "tenacitas/src/async/handling_name.h"
#include "tenacitas/src/async/result.h"
#include "tenacitas/src/async/traits/has_publish_method.h"
#include "tenacitas/src/format/fmt.h"
#include "tenacitas/src/log/traits/logger.h"

namespace tenacitas::exp::async
{

template <
    event_id t_event_id, handler_type_id t_type_id,
    src::async::traits::has_publish_method<async::event_handled> t_dispatcher>
struct handler
{
  using event = async::event<t_event_id>;

  using events_handled = std::tuple<event>;

  handler(logger &p_logger, t_dispatcher &p_dispatcher,
          src::async::handling_name p_handling_name,
          std::chrono::milliseconds p_sleep_to_simulate_work)
      : m_logger(p_logger), m_dispatcher(p_dispatcher),
        m_handling_name(p_handling_name),
        m_sleep_to_simulate_work(p_sleep_to_simulate_work)
  {
    TNCT_LOG_TST(m_logger, src::format::fmt(
                               "handler ", p_handling_name,
                               ", sleep = ", m_sleep_to_simulate_work.count()))
  }

  void operator()(event && /*p_event*/)
  {
    //    std::this_thread::sleep_for(m_sleep_to_simulate_work);
    busy_wait();

    auto _result{m_dispatcher.template publish<event_handled>(
        m_handling_name, t_event_id, t_type_id, std::this_thread::get_id())};

    if (_result != src::async::result::OK)
    {
      TNCT_LOG_ERR(m_logger, src::format::fmt("error: ", _result));
      return;
    }

    ++m_num_events;
    // TNCT_LOG_TST(m_logger,
    //              src::format::fmt("handling ", m_handling_id, ", handler type
    //              #",
    //                          type_id, ", handler #",
    //                          std::this_thread::get_id(),
    //                          ": handling event #", m_num_events));
  }

  static constexpr async::handler_type_id type_id{t_type_id};
  static constexpr async::event_id        event_id{t_event_id};

private:
  void busy_wait()
  {
    const auto start = std::chrono::high_resolution_clock::now();
    for (;;)
    {
      if ((std::chrono::high_resolution_clock::now() - start)
          >= m_sleep_to_simulate_work)
      {
        break;
      }
    }
  }

private:
  logger                   &m_logger;
  t_dispatcher             &m_dispatcher;
  src::async::handling_name m_handling_name;
  std::chrono::milliseconds m_sleep_to_simulate_work;

  uint32_t m_num_events{0};
};

} // namespace tenacitas::exp::async

#endif
