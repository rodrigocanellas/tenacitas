/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_TRAITS_DISPATCHER_H
#define TENACITAS_LIB_TRAITS_DISPATCHER_H

// #include <tuple>

#include "tenacitas.lib/traits/event.h"
#include "tenacitas.lib/traits/has_new_operator.h"
#include "tenacitas.lib/traits/is_smart_ptr.h"
#include "tenacitas.lib/traits/logger.h"
#include "tenacitas.lib/traits/tuple_like.h"

namespace tenacitas::lib::traits {

template <typename t>
concept dispatcher = requires(t p_t) {
  typename t::events;

  !std::copy_constructible<t>;

  !std::move_constructible<t>;

  std::default_initializable<t>;

  !has_new_operator_v<t>;

  // t::events is a std::tuple
  tuple_like<typename t::events>;

  // every type in t::events is an event
  []<std::size_t... t_idx>(std::index_sequence<t_idx...>) {
    return (event<std::tuple_element<t_idx, typename t::events>> && ...);
  }
  (std::make_index_sequence<std::tuple_size_v<typename t::events>>());
};

} // namespace tenacitas::lib::traits

// template <typename t, typename t_logger, typename... t_events>
// concept dispatcher = requires(t p_t) {
//   typename t::events;
//   typename t::logger;

//   !std::copy_constructible<t>;

//   !std::move_constructible<t>;

//   std::default_initializable<t>;

//   no_new_operator<t>;

//   logger<typename t::logger>;

//   // t::events is a std::tuple
//   tuple_like<typename t::events>;

//   // every type in t::events is an event
//   []<std::size_t... t_idx>(std::index_sequence<t_idx...>) {
//     return (event<std::tuple_element<t_idx, typename t::events>> && ...);
//   }
//   (std::make_index_sequence<std::tuple_size_v<typename t::events>>());

//   // every type in t_events is a event
//   []<std::size_t... t_idx>(std::index_sequence<t_idx...>) {
//     return (event<std::tuple_element<t_idx, std::tuple<t_events...>>> &&
//     ...);
//   }
//   (std::make_index_sequence<std::tuple_size_v<std::tuple<t_events...>>>());

//   // every event in t_events exists in t::events
//   (tuple_traverse<std::tuple<t_events...>>(
//        []<std::tuple<t_events...>, size_t t_idx_1>() {
//          return tuple_traverse<typename t::events>(
//              []<typename t::events, size_t t_idx_2>() {
//                // the traverse stops if the lambda returns false
//                return !std::equal<
//                    std::tuple_element_t<t_idx_1, std::tuple<t_events...>>,
//                    std::tuple_element_t<t_idx_2, typename t::events>>;
//              });
//        }) != -1);
// };

#endif
