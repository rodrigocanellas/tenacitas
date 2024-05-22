/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_TRAITS_TUPLE_LIKE_H
#define TENACITAS_LIB_TRAITS_TUPLE_LIKE_H

#include <tenacitas.lib/traits/internal/has_tuple_element.h>

namespace tenacitas::lib::traits {

// \log_file Solution based on
// https://stackoverflow.com/questions/68443804/c20-concept-to-check-tuple-like-types
template <typename t_tuple>
concept tuple_like = !std::is_reference_v<t_tuple> && requires(t_tuple t) {
  typename std::tuple_size<t_tuple>::type;
  requires std::derived_from<
      std::tuple_size<t_tuple>,
      std::integral_constant<std::size_t, std::tuple_size_v<t_tuple>>>

      &&

          []<std::size_t... t_idx>(std::index_sequence<t_idx...>) {
    return (internal::has_tuple_element<t_tuple, t_idx> && ...);
  }
  (std::make_index_sequence<std::tuple_size_v<t_tuple>>());
};
} // namespace tenacitas::lib::traits
#endif
