#ifndef TENACITAS_CALENDAR_GREGORIAN_HOURS_H
#define TENACITAS_CALENDAR_GREGORIAN_HOURS_H

#include <cstdint>

#include <calendar/gregorian/amount.h>
#include <calendar/gregorian/convert.h>
#include <calendar/gregorian/hour.h>
#include <calendar/gregorian/minutes.h>
#include <calendar/gregorian/seconds.h>

namespace tenacitas {
namespace calendar {
namespace gregorian {

typedef amount_t<hour> hours;

template <>
constexpr inline minutes convert<hours, minutes>(const hours &p_hours) {
  return minutes(p_hours * 60);
}

template <>
constexpr inline seconds convert<hours, seconds>(const hours &p_hours) {
  return convert<minutes, seconds>(convert<hours, minutes>(p_hours));
}

} // namespace gregorian
} // namespace calendar
} // namespace tenacitas

#endif // HOURS_H
