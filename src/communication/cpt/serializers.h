#ifndef TENACITAS_LIB_MESSENGER_CPT_SERIALIZERS_H
#define TENACITAS_LIB_MESSENGER_CPT_SERIALIZERS_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <concepts>

#include <tenacitas.lib/src/async/cpt/concepts.h>
#include <tenacitas.lib/src/communication/cpt/message.h>
#include <tenacitas.lib/src/traits/cpt/is_tuple.h>

namespace tenacitas::lib::communication::cpt {

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

} // namespace tenacitas::lib::communication::cpt

#endif
