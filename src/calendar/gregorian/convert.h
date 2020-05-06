#ifndef TENACITAS_CALENDAR_GREGORIAN_CONVERT_H
#define TENACITAS_CALENDAR_GREGORIAN_CONVERT_H

#include <cstdint>

namespace tenacitas {
namespace calendar {
namespace gregorian {

template <typename t_from, typename t_to>
constexpr t_to convert(t_from p_from = t_from(1));

// template <typename t_from, typename... t_parts>
// constexpr void partition(t_from p_from, t_parts &... p_parts) {
//  t_from _part_0(p_from);
//  partition1(p_from, _part_0, p_parts...);
//}

// template <typename t_from, typename t_part_0, typename... t_parts>
// constexpr void partition1(t_from p_from, t_part_0 &p_part_0,
//                          t_parts &... p_parts) {

//  partition1(p_from, p_part_0);
//  partition1(p_from, p_parts...);
//}

// template <typename t_from, typename t_part_0, typename t_part_1>
// constexpr void partition(t_from p_from, t_part_0 &p_part_0, t_part_1
// &p_part_1) {
//  auto _value = p_from.value();
//  auto _int = static_cast<int64_t>(_value);
//  auto _fra = _value - _int;
//  p_part_0 = t_from(_int);
//  p_part_1 = t_to(_fra);
//}

// template <typename t_from, typename t_to>
// constexpr t_to convert(t_from &&p_from = t_from(1));

} // namespace gregorian
} // namespace calendar
} // namespace tenacitas

#endif // CONVERT_H
