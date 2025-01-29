/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_ASYNC_EXP_DISPATCHER_PUBLISHER_H
#define TENACITAS_LIB_ASYNC_EXP_DISPATCHER_PUBLISHER_H

#include <chrono>
#include <string_view>
#include <vector>

#include "tenacitas.lib/async/exp/dispatcher/event.h"
#include "tenacitas.lib/async/exp/dispatcher/logger.h"
#include "tenacitas.lib/async/sleeping_loop.h"
#include "tenacitas.lib/traits/dispatcher.h"

namespace tenacitas::lib::async::exp {

template <char t_event_id,
          traits::dispatcher<logger, exp::event<t_event_id>> t_dispacther>
struct publisher {
  using event = exp::event<t_event_id>;

  publisher(t_dispacther &p_dispatcher, logger &p_logger,
            std::chrono::milliseconds p_interval, size_t p_total_events,
            std::string_view p_id)
      : m_logger(p_logger), m_dispatcher(p_dispatcher),
        m_total_events(p_total_events), m_interval(p_interval), m_id(p_id),
        m_slepping_loop(
            m_logger, [this]() { sleeping_function(); }, m_interval, m_id)

  {}

  publisher(const publisher &) = delete;
  publisher(publisher &&p_publisher)
      : m_logger{p_publisher.m_logger}, m_dispatcher(p_publisher.m_dispatcher),
        m_total_events(p_publisher.m_total_events),
        m_num_events{p_publisher.m_num_events},
        m_interval(p_publisher.m_interval), m_id{p_publisher.m_id},
        m_slepping_loop(
            m_logger, [this]() { sleeping_function(); }, m_interval, m_id) {}
  ~publisher() { m_slepping_loop.stop(); }

  void start() {
    TNCT_LOG_TST(m_logger, format::fmt('\'', m_event, "', publisher '", m_id,
                                       "': starting publisher"));
    m_slepping_loop.start();
  }

  constexpr size_t num_events() const { return m_num_events; }

private:
  void sleeping_function() {
    if (m_num_events >= m_total_events) {
      TNCT_LOG_TST(m_logger, format::fmt('\'', m_event, "', publisher '", m_id,
                                         "': total of ", m_num_events, " of ",
                                         m_total_events,
                                         " published, stopping publisher"));
      m_slepping_loop.stop();
      return;
    }
    m_dispatcher.template publish<event>();
    ++m_num_events;
    TNCT_LOG_TST(m_logger, format::fmt('\'', m_event, "', publisher '", m_id,
                                       "': publishing event # ", m_num_events,
                                       "/", m_total_events));
  }

private:
  logger &m_logger;
  t_dispacther &m_dispatcher;
  size_t m_total_events{0};
  size_t m_num_events{0};
  std::chrono::milliseconds m_interval;
  std::string m_id{"not-assigned"};
  async::sleeping_loop<logger> m_slepping_loop;
  const event m_event{};
};

template <char t_event_id,
          traits::dispatcher<logger, event<t_event_id>> t_dispacther>
using publishers = std::vector<publisher<t_event_id, t_dispacther>>;

} // namespace tenacitas::lib::async::exp

#endif
