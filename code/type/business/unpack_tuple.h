#ifndef TENACITAS_CONCURRENT_BUS_UNPACK_TUPLE_H
#define TENACITAS_CONCURRENT_BUS_UNPACK_TUPLE_H

#include <cstdint>
#include <functional>
#include <iostream>
#include <string>
#include <tuple>

namespace tenacitas {
namespace type {
namespace business {

template <size_t N, typename t_result>
struct call_using_tuple {
  template <typename t_work, typename t_tuple, typename... t_types>
  static t_result exec(t_work&& p_work,
                       t_tuple&& p_tuple,
                       t_types&&... p_values) {
    return call_using_tuple<N - 1, t_result>::exec(
        std::forward<t_work>(p_work), std::forward<t_tuple>(p_tuple),
        std::get<N - 1>(std::forward<t_tuple>(p_tuple)),
        std::forward<t_types>(p_values)...);
  }
};

template <typename t_result>
struct call_using_tuple<0, t_result> {
  template <typename t_work, typename t_tuple, typename... t_types>
  static t_result exec(t_work&& p_work,
                       t_tuple&& /*p_tuple*/,
                       t_types&&... p_values) {
    return std::forward<t_work>(p_work)(std::forward<t_types>(p_values)...);
  }
};

template <typename t_result, typename t_work, typename t_tuple>
t_result apply(t_work&& p_work, t_tuple&& p_tuple) {
  return call_using_tuple<std::tuple_size<t_tuple>::value, t_result>::exec(
      std::forward<t_work>(p_work), std::forward<t_tuple>(p_tuple));
}

}  // namespace business
}  // namespace type
}  // namespace tenacitas

#endif
