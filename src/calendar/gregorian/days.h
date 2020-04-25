#ifndef TENACITAS_CALENDAR_GREGORIAN_DAYS_H
#define TENACITAS_CALENDAR_GREGORIAN_DAYS_H

#include <cstdint>

#include <calendar/gregorian/amount.h>
#include <calendar/gregorian/convert.h>
#include <calendar/gregorian/day.h>
#include <calendar/gregorian/hours.h>
#include <calendar/gregorian/minutes.h>
#include <calendar/gregorian/seconds.h>

namespace tenacitas {
namespace calendar {
namespace gregorian {

typedef amount_t<day> days;

template <> constexpr inline hours convert<days, hours>(const days &p_days) {
  return hours(p_days * 24);
}

template <>
constexpr inline minutes convert<days, minutes>(const days &p_days) {
  return convert<hours, minutes>(convert<days, hours>(p_days));
}

template <>
constexpr inline seconds convert<days, seconds>(const days &p_days) {
  return convert<minutes, seconds>(
      convert<hours, minutes>(convert<days, hours>(p_days)));
}

} // namespace gregorian
} // namespace calendar
} // namespace tenacitas

#endif // DAYS_H
