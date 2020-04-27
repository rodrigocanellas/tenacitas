#ifndef TENACITAS_CALENDAR_GREGORIAN_YEARS_H
#define TENACITAS_CALENDAR_GREGORIAN_YEARS_H

#include <cstdint>
#include <iostream>

#include <calendar/gregorian/amount.h>
#include <calendar/gregorian/convert.h>
#include <calendar/gregorian/days.h>
#include <calendar/gregorian/hours.h>
#include <calendar/gregorian/minutes.h>
#include <calendar/gregorian/months.h>
#include <calendar/gregorian/seconds.h>
#include <calendar/gregorian/weeks.h>
#include <calendar/gregorian/year.h>

namespace tenacitas {
namespace calendar {
namespace gregorian {

typedef amount_t<year> years;

template <>
constexpr inline months convert<years, months>(const years &p_years) {
  return months(p_years * 12);
}

template <> constexpr inline weeks convert<years, weeks>(const years &p_years) {
  return weeks(p_years * 52);
}

template <> constexpr inline days convert<years, days>(const years &p_years) {
  return days(p_years * 365);
}

template <> constexpr inline hours convert<years, hours>(const years &p_years) {
  return convert<days, hours>(convert<years, days>(p_years));
}

template <>
constexpr inline minutes convert<years, minutes>(const years &p_years) {
  return convert<hours, minutes>(convert<days, hours>(
      convert<months, days>(convert<years, months>(p_years))));
}

template <>
constexpr inline seconds convert<years, seconds>(const years &p_years) {
  return convert<minutes, seconds>(convert<hours, minutes>(convert<days, hours>(
      convert<months, days>(convert<years, months>(p_years)))));
}

} // namespace gregorian
} // namespace calendar
} // namespace tenacitas

#endif // YEARS_H
