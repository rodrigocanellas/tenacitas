/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_ASYNC_EXP_DISPATCHER_EVENT_HANDLED_H
#define TENACITAS_LIB_ASYNC_EXP_DISPATCHER_EVENT_HANDLED_H

#include <cstdint>
#include <iostream>
#include <limits>

#include "tenacitas.lib/async/exp/dispatcher/event_id.h"
#include "tenacitas.lib/async/exp/dispatcher/handler_id.h"
#include "tenacitas.lib/async/exp/dispatcher/handler_type_id.h"
#include "tenacitas.lib/async/exp/dispatcher/handling_id.h"

namespace tenacitas::lib::async::exp {

struct event_handled {
  event_handled() = default;
  event_handled(const event_handled &) = default;
  event_handled(event_handled &&) = default;

  explicit event_handled(const handling_id &p_handling_id, event_id p_event_id,
                         handler_type_id p_handler_type_id,
                         handler_id p_handler_id)
      : handling_id(p_handling_id), event_id(p_event_id),
        handler_type_id(p_handler_type_id), handler_id(p_handler_id) {}

  event_handled &operator=(const event_handled &) = default;
  event_handled &operator=(event_handled &&) = default;

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const event_handled &p_event_handled) {
    p_out << '\'' << p_event_handled.handling_id << "' "
          << p_event_handled.event_id << ' ' << p_event_handled.handler_type_id
          << ' ' << p_event_handled.handler_id;
    return p_out;
  }

  async::exp::handling_id handling_id;
  async::exp::event_id event_id{' '};
  async::exp::handler_type_id handler_type_id{
      std::numeric_limits<uint16_t>::max()};
  async::exp::handler_id handler_id;
};

} // namespace tenacitas::lib::async::exp

#endif
