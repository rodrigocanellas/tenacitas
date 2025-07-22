/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_SRC_SRC_ASYNC_HANDLING_DEFINITION_H
#define TENACITAS_SRC_SRC_ASYNC_HANDLING_DEFINITION_H

#include "tenacitas/src/async/handling_id.h"
#include "tenacitas/src/async/handling_priority.h"
#include "tenacitas/src/traits/async/event.h"
#include "tenacitas/src/traits/async/handler.h"
#include "tenacitas/src/traits/container/queue.h"

namespace tenacitas::src::async {

template <traits::async::event t_event,
          traits::async::handler<t_event> t_handler,
          traits::container::queue<t_event> t_queue>
struct handling_definition {
  using event = t_event;
  using handler = t_handler;
  using queue = t_queue;
  using priority = handling_priority;
  using id = handling_id;

  handling_definition() = delete;
  handling_definition(const handling_id &p_handling_id, handler &&p_handler,
                      queue &&p_queue, std::size_t p_num_handlers,
                      handling_priority p_priority = handling_priority::medium)
      : m_id(p_handling_id), m_handler(std::move(p_handler)),
        m_queue(std::move(p_queue)), m_num_handlers(p_num_handlers),
        m_priority(std::move(p_priority)) {}

  constexpr const id &get_id() const { return m_id; }
  handler get_handler() const { return m_handler; }
  queue get_queue() { return m_queue; }
  constexpr std::size_t get_num_handlers() const { return m_num_handlers; }
  constexpr priority get_priority() const { return m_priority; }

private:
  id m_id;
  handler m_handler;
  queue m_queue;
  std::size_t m_num_handlers;
  priority m_priority;
};

} // namespace tenacitas::src::async

#endif
