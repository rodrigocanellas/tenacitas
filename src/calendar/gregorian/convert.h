#ifndef TENACITAS_CALENDAR_GREGORIAN_CONVERT_H
#define TENACITAS_CALENDAR_GREGORIAN_CONVERT_H

#include <calendar/gregorian/days.h>
#include <calendar/gregorian/hours.h>
#include <calendar/gregorian/minutes.h>
#include <calendar/gregorian/month.h>
#include <calendar/gregorian/months.h>
#include <calendar/gregorian/seconds.h>
#include <calendar/gregorian/weeks.h>
#include <calendar/gregorian/year.h>

namespace tenacitas {
namespace calendar {
namespace gregorian {

template <typename t_from, typename t_to>
constexpr t_to convert(const t_from &p_from = t_from(1));

template <> constexpr inline seconds convert(const minutes &p_minutes) {
  return seconds(p_minutes * 60);
}

template <> constexpr inline minutes convert(const hours &p_hours) {
  return minutes(p_hours * 60);
}

template <> constexpr inline seconds convert(const hours &p_hours) {
  return convert<minutes, seconds>(convert<hours, minutes>(p_hours));
}

template <> constexpr inline hours convert(const days &p_days) {
  return hours(p_days * 24);
}

template <> constexpr inline minutes convert(const days &p_days) {
  return convert<hours, minutes>(convert<days, hours>(p_days));
}

template <> constexpr inline seconds convert(const days &p_days) {
  return convert<minutes, seconds>(
      convert<hours, minutes>(convert<days, hours>(p_days)));
}

template <> constexpr inline days convert(const weeks &p_weeks) {
  return days(p_weeks * 7);
}

template <> constexpr inline hours convert(const weeks &p_weeks) {
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

#endif // CONVERT_H
