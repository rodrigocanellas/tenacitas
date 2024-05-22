/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_TRAITS_TUPLE_TO_CONTAINER_H
#define TENACITAS_LIB_TRAITS_TUPLE_TO_CONTAINER_H

#include <tuple>

namespace tenacitas::lib::traits {

// General case: transform a single type t to t_container<t>
template <template <typename...> class t_container, typename t>
struct to_container {
  using type = t_container<t>;
};

// Recursive case: transform each type in the tuple
template <template <typename...> class t_container, typename t_tuple>
struct tuple_to_container;

template <template <typename...> class t_container, typename... ts>
struct tuple_to_container<t_container, std::tuple<ts...>> {
  using type = std::tuple<typename to_container<t_container, ts>::type...>;
};

// Convenience alias for easy usage
template <template <typename...> class t_container, typename t_tuple>
using tuple_to_container_t =
    typename tuple_to_container<t_container, t_tuple>::type;

} // namespace tenacitas::lib::traits

#endif
