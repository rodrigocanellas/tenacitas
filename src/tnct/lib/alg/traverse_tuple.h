/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_LIB_ALG_TRAVERSE_TUPLE_TUPLE_H
#define TNCT_LIB_ALG_TRAVERSE_TUPLE_TUPLE_H

#include <cstddef>
#include <tuple>
#include <type_traits>

namespace tnct::lib::alg {

template <size_t t_idx, typename... t_function, typename... t_data>
requires(... and std::is_invocable_r_v<void, t_function, t_data &>)

    void traverse_tuple(std::tuple<t_function...> p_function,
                        std::tuple<t_data...> &p_data) {
  std::get<t_idx>(p_function)(std::get<t_idx>(p_data));
}

template <size_t... t_idxs, typename... t_function, typename... t_data>
requires(... and std::is_invocable_r_v<void, t_function, t_data &>)

    void traverse_tuple(std::index_sequence<t_idxs...>,
                        std::tuple<t_function...> p_function,
                        std::tuple<t_data...> &p_data) {
  (traverse_tuple<t_idxs>(p_function, p_data), ...);
}

template <typename... t_function, typename... t_data>
requires(... and std::is_invocable_r_v<void, t_function, t_data &>)

    void traverse_tuple(std::tuple<t_function...> p_function,
                        std::tuple<t_data...> &p_data) {
  traverse_tuple(std::make_index_sequence<sizeof...(t_data)>{}, p_function,
                 p_data);
}

template <size_t t_idx, typename t_function, typename... t_data>
requires(... and std::is_invocable_r_v<void, t_function, t_data &>)

    void traverse_tuple(t_function p_function, std::tuple<t_data...> &p_data) {
  p_function(std::get<t_idx>(p_data));
}

template <size_t... t_idxs, typename t_function, typename... t_data>
requires(... and std::is_invocable_r_v<void, t_function, t_data &>)

    void traverse_tuple(std::index_sequence<t_idxs...>, t_function p_function,
                        std::tuple<t_data...> &p_data) {
  (traverse_tuple<t_idxs>(p_function, p_data), ...);
}

template <typename t_function, typename... t_data>
requires(... and std::is_invocable_r_v<void, t_function, t_data &>)

    void traverse_tuple(t_function p_function, std::tuple<t_data...> &p_data) {
  traverse_tuple(std::make_index_sequence<sizeof...(t_data)>{}, p_function,
                 p_data);
}

} // namespace tnct::lib::alg

namespace tla = tnct::lib::alg;

#endif
