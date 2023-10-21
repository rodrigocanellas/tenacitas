#ifndef TENACITAS_LIB_TRAITS_ALG_TUPLE_H
#define TENACITAS_LIB_TRAITS_ALG_TUPLE_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <functional>
#include <tuple>

#include <tenacitas.lib/src/traits/cpt/is_tuple.h>

/// \brief master namespace
namespace tenacitas::lib::traits::alg {

/// \brief Type trait that identifies if a type is not a tuple
template <typename> struct is_tuple : std::false_type {};

/// \brief Type trait that identifies if a type is a tuple
template <typename... t> struct is_tuple<std::tuple<t...>> : std::true_type {};

/// \brief Alias for a \p is_tuple value
template <typename t> constexpr bool is_tuple_v = is_tuple<t>::value;

template <size_t t_idx, typename t_type, cpt::tuple_like t_tuple>
constexpr ssize_t idx() {
  if constexpr (t_idx >= std::tuple_size_v<t_tuple>) {
    return -1;
  } else if constexpr (std::is_same_v<t_type,
                                      std::tuple_element_t<t_idx, t_tuple>>) {
    return static_cast<ssize_t>(t_idx);
  } else {
    return idx<t_idx + 1, t_type, t_tuple>();
  }
}

/// \brief Index of \p t_type in \p t_tuple, or static_assert if not found
// template <typename t_type, cpt::tuple_like t_tuple> constexpr size_t index()
// {
//   return idx<0, t_type, t_tuple>();
// }

template <typename t_type, cpt::tuple_like t_types> struct index {
  static constexpr ssize_t value = idx<0, t_type, t_types>();
};

template <typename t_type, cpt::tuple_like t_types>
constexpr ssize_t index_v = index<t_type, t_types>::value;

// template <typename t_type, typename t_tuple> struct ind;

// template <typename t_type, typename... t_types>
// struct ind<t_type, std::tuple<t_type, t_types...>> {
//   static const std::size_t value = 0;
// };

// template <typename t, typename u, typename... t_types>
// struct ind<t, std::tuple<u, t_types...>> {
//   static const std::size_t value = 1 + ind<t,
//   std::tuple<t_types...>>::value;
// };

// template <typename t, typename... t_types>
// constexpr std::size_t ind_v = ind<t, t_types...>::value;

template <size_t t_idx, typename... t_function, typename... t_data>
requires(... and std::is_invocable_r_v<
         void, t_function,
         t_data &>) void traverse(std::tuple<t_function...> p_function,
                                  std::tuple<t_data...> &p_data) {
  std::get<t_idx>(p_function)(std::get<t_idx>(p_data));
}

template <size_t... t_idxs, typename... t_function, typename... t_data>
requires(... and std::is_invocable_r_v<
         void, t_function,
         t_data &>) void traverse(std::index_sequence<t_idxs...>,
                                  std::tuple<t_function...> p_function,
                                  std::tuple<t_data...> &p_data) {
  (traverse<t_idxs>(p_function, p_data), ...);
}

template <typename... t_function, typename... t_data>
requires(... and std::is_invocable_r_v<
         void, t_function,
         t_data &>) void traverse(std::tuple<t_function...> p_function,
                                  std::tuple<t_data...> &p_data) {
  traverse(std::make_index_sequence<sizeof...(t_data)>{}, p_function, p_data);
}

template <size_t t_idx, typename t_function, typename... t_data>
requires(... and std::is_invocable_r_v<
         void, t_function, t_data &>) void traverse(t_function p_function,
                                                    std::tuple<t_data...>
                                                        &p_data) {
  p_function(std::get<t_idx>(p_data));
}

template <size_t... t_idxs, typename t_function, typename... t_data>
requires(... and std::is_invocable_r_v<
         void, t_function,
         t_data &>) void traverse(std::index_sequence<t_idxs...>,
                                  t_function p_function,
                                  std::tuple<t_data...> &p_data) {
  (traverse<t_idxs>(p_function, p_data), ...);
}

template <typename t_function, typename... t_data>
requires(... and std::is_invocable_r_v<
         void, t_function, t_data &>) void traverse(t_function p_function,
                                                    std::tuple<t_data...>
                                                        &p_data) {
  traverse(std::make_index_sequence<sizeof...(t_data)>{}, p_function, p_data);
}

} // namespace tenacitas::lib::traits::alg

#endif // TUPLE_H
