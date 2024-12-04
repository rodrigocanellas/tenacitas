/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_ASY_EVENT_HANDLING_H
#define TENACITAS_LIB_ASY_EVENT_HANDLING_H

#include <iostream>

#include "tenacitas.lib/async/event_handler.h"
#include "tenacitas.lib/async/event_priority.h"
#include "tenacitas.lib/async/handling_id.h"
#include "tenacitas.lib/sta/event.h"
#include "tenacitas.lib/sta/is_pair.h"
#include "tenacitas.lib/sta/queue.h"

namespace tenacitas::lib::sta {

// Define handling_definition concept
template <typename t>
concept event_handling_definition =
    requires(t p_t, const typename t::event &evt_lvalue, std::ostream &out) {

  typename t::event;
  requires sta::event<typename t::event>;

  {p_t.id};
  requires std::same_as < std::add_const_t<async::handling_id>,
  decltype(p_t.id) > ;

  typename t::queue;
  requires sta::queue<typename t::queue>;

  { p_t.add_event(evt_lvalue) } -> std::same_as<bool>;

  {
    p_t.add_subscriber(std::declval<async::event_handler<typename t::event>>())
    } -> std::same_as<void>;

  {
    p_t.add_subscriber(
        std::declval<size_t>(),
        std::declval<std::function<async::event_handler<typename t::event>()>>())
    } -> std::same_as<void>;

  { p_t.stop() } -> std::same_as<void>;

  { p_t.amount_added() } -> std::same_as<size_t>;

  { p_t.is_stopped() } -> std::same_as<bool>;

  { p_t.get_event_priority() } -> std::same_as<async::event_priority>;

  {
    p_t.set_event_priority(std::declval<async::event_priority>())
    } -> std::same_as<void>;

  { p_t.get_occupied() } -> std::same_as<size_t>;

  { p_t.clear() } -> std::same_as<void>;

  { out << p_t } -> std::same_as<std::ostream &>;
};

} // namespace tenacitas::lib::sta

#endif // EVENTS_DEFINITION_H
