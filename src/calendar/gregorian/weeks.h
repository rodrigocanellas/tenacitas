#ifndef TENACITAS_CALENDAR_GREGORIAN_WEEKS_H
#define TENACITAS_CALENDAR_GREGORIAN_WEEKS_H

#include <cstdint>

#include <calendar/gregorian/amount.h>
#include <calendar/gregorian/convert.h>
#include <calendar/gregorian/days.h>
#include <calendar/gregorian/hours.h>
#include <calendar/gregorian/minutes.h>
#include <calendar/gregorian/seconds.h>
#include <calendar/gregorian/week.h>

namespace tenacitas {
namespace calendar {
namespace gregorian {

typedef amount_t<week> weeks;

template <> constexpr inline days convert<weeks, days>(const weeks &p_weeks) {
  return days(p_weeks * 7);
}

template <> constexpr inline hours convert<weeks, hours>(const weeks &p_weeks) {
  return convert<days, hours>(convert<weeks, days>(p_weeks));
}

template <> constexpr inline minutes convert(const weeks &p_weeks) {
  return convert<hours, minutes>(
      convert<days, hours>(convert<weeks, days>(p_weeks)));
}

template <> constexpr inline seconds convert(const weeks &p_weeks) {
  return convert<minutes, seconds>(convert<hours, minutes>(
      convert<days, hours>(convert<weeks, days>(p_weeks))));
}

} // namespace gregorian
} // namespace calendar
} // namespace tenacitas

#endif // DAYS_H
