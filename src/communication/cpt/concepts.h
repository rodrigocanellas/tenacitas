#ifndef TENACITAS_LIB_MESSENGER_CPT_CONCEPTS_H
#define TENACITAS_LIB_MESSENGER_CPT_CONCEPTS_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <concepts>
#include <optional>
#include <ranges>
#include <tuple>
#include <vector>

#include <tenacitas.lib/src/async/cpt/concepts.h>
#include <tenacitas.lib/src/calendar/cpt/chrono_convertible.h>
#include <tenacitas.lib/src/communication/typ/channel_id.h>
#include <tenacitas.lib/src/communication/typ/connection_id.h>
#include <tenacitas.lib/src/traits/cpt/is_tuple.h>

namespace tenacitas::lib::src::communication::cpt {

template <typename t>
concept message = requires(t p_t) {

  typename t::value_type;

  requires(sizeof(typename t::value_type) == 1);

  std::ranges::bidirectional_range<t>;
};

template <typename t>
concept serializer = requires(t p_t) {

  typename t::event;
  requires async::cpt::event<typename t::event>;

  typename t::message;
  requires message<typename t::message>;

  { p_t(typename t::event()) } -> std::same_as<typename t::message>;
};

template <typename t, typename t_message>
concept serializers =

    traits::cpt::tuple_like<t> &&

    message<t_message> &&

        []<std::size_t... t_idx>(std::index_sequence<t_idx...>) {
  return (

      (

          std::is_same_v<t_message,
                         typename std::tuple_element_t<t_idx, t>::message>

          &&

          std::is_invocable_r_v<t_message, std::tuple_element_t<t_idx, t>,
                                typename std::tuple_element_t<t_idx, t>::event>

          )

      &&

      ...);
}
(std::make_index_sequence<std::tuple_size_v<t>>());

template <typename t>
concept deserializer = requires(t p_t) {

  typename t::event;
  requires async::cpt::event<typename t::event>;

  typename t::message;
  requires message<typename t::message>;

  {
    p_t(typename t::message())
    } -> std::same_as<std::optional<typename t::event>>;
};

template <typename t, typename t_message>
concept deserializers =

    traits::cpt::tuple_like<t> &&

    message<t_message> &&

        []<std::size_t... t_idx>(std::index_sequence<t_idx...>) {
  return (

      (

          std::is_same_v<t_message,
                         typename std::tuple_element_t<t_idx, t>::message>

          &&

          std::is_invocable_r_v<
              std::optional<typename std::tuple_element_t<t_idx, t>::event>,
              std::tuple_element_t<t_idx, t>, const t_message &>

          )

      &&

      ...);
}
(std::make_index_sequence<std::tuple_size_v<t>>())

    ;

template <typename t, typename t_message>
concept io = requires(t p_t) {

  typename t_message;
  requires message<t_message>;

  { p_t.send(typ::connection_id{}, t_message{}) } -> std::same_as<void>;

  { p_t.receive(typ::connection_id{}) } -> std::same_as<t_message>;

  { p_t.receive_timeout } -> calendar::cpt::convertible_to_ms;

  { p_t.send_timeout } -> calendar::cpt::convertible_to_ms;
};

} // namespace tenacitas::lib::src::communication::cpt

#endif
