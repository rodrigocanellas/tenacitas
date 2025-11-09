/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_ASYNC_EXP_DISPATCHER_000_HANDLER_H
#define TNCT_ASYNC_EXP_DISPATCHER_000_HANDLER_H

#include <chrono>

#include "tnct/async/exp/dispatcher_000/event.h"
#include "tnct/async/exp/dispatcher_000/event_handled.h"
#include "tnct/async/exp/dispatcher_000/event_id.h"
#include "tnct/async/exp/dispatcher_000/handler_type_id.h"
#include "tnct/async/exp/dispatcher_000/logger.h"
#include "tnct/async/dat/handling_name.h"
#include "tnct/async/dat/result.h"
#include "tnct/async/cpt/has_publish_method.h"
#include "tnct/format/bus/fmt.h"
#include "tnct/log/cpt/logger.h"
#include "tnct/log/cpt/macros.h"

namespace tnct::async::exp
{

template <event_id t_event_id, handler_type_id t_type_id,
          async::cpt::has_publish_method<async::exp::event_handled>
              t_dispatcher>
struct handler
{
  using event = async::exp::event<t_event_id>;

  using events_handled = std::tuple<event>;

  handler(logger &p_logger, t_dispatcher &p_dispatcher,
          async::dat::handling_name p_handling_name,
          std::chrono::milliseconds p_sleep_to_simulate_work)
      : m_logger(p_logger), m_dispatcher(p_dispatcher),
        m_handling_name(p_handling_name),
        m_sleep_to_simulate_work(p_sleep_to_simulate_work)
  {
    TNCT_LOG_TST(m_logger, format::bus::fmt(
                               "handler ", p_handling_name,
                               ", sleep = ", m_sleep_to_simulate_work.count()))
  }

  void operator()(event && /*p_event*/)
  {
    //    std::this_thread::sleep_for(m_sleep_to_simulate_work);
    busy_wait();

    auto _result{m_dispatcher.template publish<event_handled>(
        m_handling_name, t_event_id, t_type_id, std::this_thread::get_id())};

    if (_result != async::dat::result::OK)
    {
      TNCT_LOG_ERR(m_logger, format::bus::fmt("error: ", _result));
      return;
    }

    ++m_num_events;
    // TNCT_LOG_TST(m_logger,
    //              format::bus::fmt("handling ", m_handling_id, ", handler type
    //              #",
    //                          type_id, ", handler #",
    //                          std::this_thread::get_id(),
    //                          ": handling event #", m_num_events));
  }

  static constexpr async::exp::handler_type_id type_id{t_type_id};
  static constexpr async::exp::event_id        event_id{t_event_id};

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
  async::dat::handling_name m_handling_name;
  std::chrono::milliseconds m_sleep_to_simulate_work;

  uint32_t m_num_events{0};
};

} // namespace tnct::async::exp

#endif
