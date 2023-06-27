#ifndef TENACITAS_LIB_TRAITS_ALG_TUPLE_H
#define TENACITAS_LIB_TRAITS_ALG_TUPLE_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <tuple>

/// \brief master namespace
namespace tenacitas::lib::traits::alg {

/// \brief Type trait that identifies if a type is not a tuple
template <typename> struct is_tuple : std::false_type {};

/// \brief Type trait that identifies if a type is a tuple
template <typename... t> struct is_tuple<std::tuple<t...>> : std::true_type {};

/// \brief Alias for a \p is_tuple value
template <typename t> constexpr bool is_tuple_v = is_tuple<t>::value;

template <typename t_type, typename t_tuple> struct index;

template <typename t_type, typename... t_types>
struct index<t_type, std::tuple<t_type, t_types...>> {
  static const std::size_t value = 0;
};

template <typename t, typename u, typename... t_types>
struct index<t, std::tuple<u, t_types...>> {
  static const std::size_t value = 1 + index<t, std::tuple<t_types...>>::value;
};

template <typename t, typename... t_types>
constexpr std::size_t index_v = index<t, t_types...>::value;

} // namespace tenacitas::lib::traits::alg

#endif // TUPLE_H
