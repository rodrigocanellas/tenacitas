#ifndef TENACITAS_CALENDAR_GREGORIAN_AMOUNTS_H
#define TENACITAS_CALENDAR_GREGORIAN_AMOUNTS_H

#include <cstdint>
#include <tuple>

#include <calendar/gregorian/amount.h>
#include <calendar/gregorian/convert.h>
#include <calendar/gregorian/day.h>
#include <calendar/gregorian/hour.h>
#include <calendar/gregorian/minute.h>
#include <calendar/gregorian/month.h>
#include <calendar/gregorian/second.h>
#include <calendar/gregorian/week.h>
#include <calendar/gregorian/year.h>

#define months_per_year (365 / static_cast<double>(12))

namespace tenacitas {
namespace calendar {
namespace gregorian {

typedef amount_t<second> seconds;
typedef amount_t<minute> minutes;
typedef amount_t<hour> hours;
typedef amount_t<day> days;
typedef amount_t<week> weeks;
typedef amount_t<month> months;
typedef amount_t<year> years;

// from seconds

template <>
constexpr inline minutes convert<seconds, minutes>(seconds p_seconds) {
  return minutes(p_seconds.value() / static_cast<double>(60));
}

template <> constexpr inline hours convert<seconds, hours>(seconds p_seconds) {
  //  return convert<seconds, minutes>(p_seconds.value()) /
  //  static_cast<double>(60);
  return hours(p_seconds.value() / static_cast<double>(60 * 60));
}

template <> constexpr inline days convert<seconds, days>(seconds p_seconds) {
  //  return convert<seconds, hours>(p_seconds.value()) /
  //  static_cast<double>(24);
  return days(p_seconds.value() / static_cast<double>(60 * 60 * 24));
}

template <> constexpr inline weeks convert<seconds, weeks>(seconds p_seconds) {
  //  return convert<seconds, days>(p_seconds.value()) / static_cast<double>(7);
  return weeks(p_seconds.value() / static_cast<double>(60 * 60 * 24 * 7));
}

template <>
constexpr inline months convert<seconds, months>(seconds p_seconds) {
  //  return convert<seconds, days>(p_seconds.value()) / static_cast<double>(7);
  return months(p_seconds.value() / (60 * 60 * 24 * months_per_year));
}

template <> constexpr inline years convert<seconds, years>(seconds p_seconds) {
  //  return convert<seconds, days>(p_seconds.value()) / static_cast<double>(7);
  return years(p_seconds.value() / static_cast<double>(60 * 60 * 24 * 365));
}

// from minutes

template <>
constexpr inline seconds convert<minutes, seconds>(minutes p_minutes) {
  return seconds(p_minutes.value() * static_cast<double>(60));
}

template <> constexpr inline hours convert<minutes, hours>(minutes p_minutes) {
  return hours(p_minutes.value() / static_cast<double>(60));
}

template <> constexpr inline days convert<minutes, days>(minutes p_minutes) {
  return days(p_minutes.value() / static_cast<double>(60 * 24));
}

template <> constexpr inline weeks convert<minutes, weeks>(minutes p_minutes) {
  return weeks(p_minutes.value() / static_cast<double>(60 * 24 * 7));
}

template <>
constexpr inline months convert<minutes, months>(minutes p_minutes) {
  return months(p_minutes.value() / (60 * 24 * months_per_year));
}

template <> constexpr inline years convert<minutes, years>(minutes p_minutes) {
  return years(p_minutes.value() / static_cast<double>(60 * 24 * 365));
}

// from hours

template <> constexpr inline days convert<hours, days>(hours p_hours) {
  return days(p_hours.value() / static_cast<double>(24));
}

template <> constexpr inline weeks convert<hours, weeks>(hours p_hours) {
  return weeks(p_hours.value() / static_cast<double>(24 * 7));
}

template <> constexpr inline months convert<hours, months>(hours p_hours) {
  return months(p_hours.value() / (24 * months_per_year));
}

template <> constexpr inline years convert<hours, years>(hours p_hours) {
  return years(p_hours.value() / static_cast<double>(24 * 365));
}

template <> constexpr inline minutes convert<hours, minutes>(hours p_hours) {
  return minutes(p_hours.value() * static_cast<double>(60));
}

template <> constexpr inline seconds convert<hours, seconds>(hours p_hours) {
  return seconds(p_hours.value() * static_cast<double>(60 * 60));
}

// from days

template <> constexpr inline weeks convert<days, weeks>(days p_days) {
  return weeks(p_days.value() / static_cast<double>(7));
}

template <> constexpr inline months convert<days, months>(days p_days) {
  return months(p_days.value() / months_per_year);
}

template <> constexpr inline years convert<days, years>(days p_days) {
  return years(p_days.value() / static_cast<double>(365));
}

template <> constexpr inline hours convert<days, hours>(days p_days) {
  return hours(p_days.value() * static_cast<double>(24));
}

template <> constexpr inline minutes convert<days, minutes>(days p_days) {
  return minutes(p_days.value() * static_cast<double>(24 * 60));
}

template <> constexpr inline seconds convert<days, seconds>(days p_days) {
  return seconds(p_days.value() * static_cast<double>(24 * 60 * 60));
}

// from weeks

template <> constexpr inline months convert<weeks, months>(weeks p_weeks) {
  return months(p_weeks.value() / (months_per_year / 7));
}

template <> constexpr inline years convert<weeks, years>(weeks p_weeks) {
  return years(p_weeks.value() / (365 / static_cast<double>(7)));
}

template <> constexpr inline days convert<weeks, days>(weeks p_weeks) {
  return days(p_weeks.value() * static_cast<double>(7));
}

template <> constexpr inline hours convert<weeks, hours>(weeks p_weeks) {
  return hours(p_weeks.value() * static_cast<double>(7 * 24));
}

template <> constexpr inline minutes convert<weeks, minutes>(weeks p_weeks) {
  return minutes(p_weeks.value() * static_cast<double>(7 * 24 * 60));
}

template <> constexpr inline seconds convert<weeks, seconds>(weeks p_weeks) {
  return seconds(p_weeks.value() * static_cast<double>(7 * 24 * 60 * 60));
}

// from months

template <> constexpr inline years convert<months, years>(months p_months) {
  return years(p_months.value() / static_cast<double>(12));
}

template <> constexpr inline weeks convert<months, weeks>(months p_months) {
  return weeks(p_months.value() * (months_per_year / 7));
}

template <> constexpr inline days convert<months, days>(months p_months) {
  return days(p_months.value() * months_per_year);
}

template <> constexpr inline hours convert<months, hours>(months p_months) {
  return hours(p_months.value() * 24 * months_per_year);
}

template <> constexpr inline minutes convert<months, minutes>(months p_months) {
  return minutes(p_months.value() * 60 * 24 * months_per_year);
}

template <> constexpr inline seconds convert<months, seconds>(months p_months) {
  return seconds(p_months.value() * 60 * 60 * 24 * months_per_year);
}

// from years

template <> constexpr inline months convert<years, months>(years p_years) {
  return months(p_years.value() * static_cast<double>(12));
}

template <> constexpr inline weeks convert<years, weeks>(years p_years) {
  return weeks(p_years.value() * (365 / static_cast<double>(7)));
}

template <> constexpr inline days convert<years, days>(years p_years) {
  return days(p_years.value() * static_cast<double>(365));
}

template <> constexpr inline hours convert<years, hours>(years p_years) {
  return hours(p_years.value() * 365 * static_cast<double>(24));
}

template <> constexpr inline minutes convert<years, minutes>(years p_years) {
  return minutes(p_years.value() * 365 * 24 * static_cast<double>(60));
}

template <> constexpr inline seconds convert<years, seconds>(years p_years) {
  return seconds(p_years.value() * 365 * 24 * 60 * static_cast<double>(60));
}

//// partition
// template <typename t_amount>
// std::tuple<years, months, weeks, days, hours, minutes, seconds>
// partition(t_amount p_amount) {

//}

// template <typename t_to>
// constexpr inline t_to convert(const year &p_year, const month &p_month);

// template <>
// constexpr inline days convert<days>(const year &p_year, const month &p_month)
// {
//  if ((p_month == month::jan) || (p_month == month::mar) ||
//      (p_month == month::may) || (p_month == month::jul) ||
//      (p_month == month::ago) || (p_month == month::oct) ||
//      (p_month == month::dec)) {
//    return days(31);
//  }
//  if ((p_month == month::apr) || (p_month == month::jun) ||
//      (p_month == month::sep) || (p_month == month::nov)) {
//    return days(30);
//  }
//  if (p_year.is_leap()) {
//    return days(29);
//  }
//  return days(28);
//}

// template <>
// constexpr inline minutes convert<minutes>(const year &p_year,
//                                          const month &p_month) {
//  return convert<days, minutes>(convert<days>(p_year, p_month));
//}

// template <>
// constexpr inline seconds convert<seconds>(const year &p_year,
//                                          const month &p_month) {
//  return convert<minutes, seconds>(
//      convert<days, minutes>(convert<days>(p_year, p_month)));
//}

} // namespace gregorian
} // namespace calendar
} // namespace tenacitas
#endif // TENACITAS_CALENDAR_CONVERSIONS_H
