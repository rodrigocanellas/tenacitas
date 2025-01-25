/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_ASYNC_EXP_DISPATHER_HANDLER_H
#define TENACITAS_LIB_ASYNC_EXP_DISPATHER_HANDLER_H

#include <functional>

#include "tenacitas.lib/format/fmt.h"

#include "tenacitas.lib/async/exp/dispatcher/event.h"
#include "tenacitas.lib/async/exp/dispatcher/logger.h"

namespace tenacitas::lib::async::exp {

template <char t_event_id, uint16_t t_id> struct handler {
  using event = async::exp::event<t_event_id>;

  handler(logger &p_logger,
          std::function<uint32_t(uint32_t)> &p_event_value_transformer)
      : m_logger(p_logger),
        m_event_value_transformer(p_event_value_transformer) {}

  void operator()(event &&p_event) {
    TNCT_LOG_TST(m_logger,
                 format::fmt("event ", p_event, " handler ", id, " new value ",
                             m_event_value_transformer(p_event.value)));
  }

  static constexpr uint16_t id{t_id};

private:
  logger &m_logger;
  std::function<uint32_t(uint32_t)> m_event_value_transformer;
};

} // namespace tenacitas::lib::async::exp

#endif
