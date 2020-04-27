#ifndef TENACITAS_CALENDAR_GREGORIAN_MONTHS_H
#define TENACITAS_CALENDAR_GREGORIAN_MONTHS_H

#include <calendar/gregorian/amount.h>
#include <calendar/gregorian/convert.h>
#include <calendar/gregorian/days.h>
#include <calendar/gregorian/hours.h>
#include <calendar/gregorian/minutes.h>
#include <calendar/gregorian/month.h>
#include <calendar/gregorian/seconds.h>
#include <calendar/gregorian/year.h>

namespace tenacitas {
namespace calendar {
namespace gregorian {

typedef amount_t<month> months;

template <typename t_to>
constexpr inline t_to convert(const year &p_year, const month &p_month);

template <>
constexpr inline days convert<days>(const year &p_year, const month &p_month) {
  if ((p_month == month::jan) || (p_month == month::mar) ||
      (p_month == month::may) || (p_month == month::jul) ||
      (p_month == month::ago) || (p_month == month::oct) ||
      (p_month == month::dec)) {
    return days(31);
  }
  if ((p_month == month::apr) || (p_month == month::jun) ||
      (p_month == month::sep) || (p_month == month::nov)) {
    return days(30);
  }
  if (p_year.is_leap()) {
    return days(29);
  }
  return days(28);
}

template <>
constexpr inline minutes convert<minutes>(const year &p_year,
                                          const month &p_month) {
  return convert<days, minutes>(convert<days>(p_year, p_month));
}

template <>
constexpr inline seconds convert<seconds>(const year &p_year,
                                          const month &p_month) {
  return convert<minutes, seconds>(
      convert<days, minutes>(convert<days>(p_year, p_month)));
}

template <>
constexpr inline days convert<months, days>(const months &p_months) {
  return days(p_months * 30);
}

template <>
constexpr inline hours convert<months, hours>(const months &p_months) {
  return convert<days, hours>(convert<months, days>(p_months));
}

template <>
constexpr inline minutes convert<months, minutes>(const months &p_months) {
  return convert<hours, minutes>(
      convert<days, hours>(convert<months, days>(p_months)));
}

template <>
constexpr inline seconds convert<months, seconds>(const months &p_months) {
  return convert<minutes, seconds>(convert<hours, minutes>(
      convert<days, hours>(convert<months, days>(p_months))));
}

} // namespace gregorian
} // namespace calendar
} // namespace tenacitas

#endif // MONTHS_H
