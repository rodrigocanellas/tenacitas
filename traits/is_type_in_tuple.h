/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_TRAITS_IS_TYPE_IN_TUPLE_H
#define TENACITAS_LIB_TRAITS_IS_TYPE_IN_TUPLE_H

#include <tuple>
#include <type_traits>

namespace tenacitas::lib::traits {
template <typename T, typename Tuple> struct is_type_in_tuple;

template <typename T>
struct is_type_in_tuple<T, std::tuple<>> : std::false_type {};

template <typename T, typename U, typename... Us>
struct is_type_in_tuple<T, std::tuple<U, Us...>>
    : std::conditional_t<std::is_same_v<T, U>, std::true_type,
                         is_type_in_tuple<T, std::tuple<Us...>>> {};

template <typename T, typename Tuple>
inline constexpr bool is_type_in_tuple_v = is_type_in_tuple<T, Tuple>::value;

} // namespace tenacitas::lib::traits
#endif
