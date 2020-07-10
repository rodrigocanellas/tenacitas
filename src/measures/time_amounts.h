#ifndef TENACITAS_MEASURES_AMOUNTS_H
#define TENACITAS_MEASURES_AMOUNTS_H

#include <cstdint>
#include <tuple>

#include <measures/day.h>
#include <measures/hour.h>
#include <measures/internal/amount_t.h>
#include <measures/internal/convert_t.h>
#include <measures/millisecond.h>
#include <measures/minute.h>
#include <measures/month.h>
#include <measures/second.h>
#include <measures/week.h>
#include <measures/year.h>

#define months_per_year (365 / static_cast<double>(12))

namespace tenacitas {
namespace measures {

typedef amount_t<millisecond> milliseconds;
typedef amount_t<second> seconds;
typedef amount_t<minute> minutes;
typedef amount_t<hour> hours;
typedef amount_t<day> days;
typedef amount_t<week> weeks;
typedef amount_t<month> months;
typedef amount_t<year> years;

// from milliseconds

template <>
inline seconds convert<milliseconds, seconds>(milliseconds p_milliseconds) {
  return seconds(p_milliseconds / static_cast<double>(1000));
}

template <>
inline minutes convert<milliseconds, minutes>(milliseconds p_milliseconds) {
  return minutes(p_milliseconds / static_cast<double>(1000 * 60));
}

template <>
inline hours convert<milliseconds, hours>(milliseconds p_milliseconds) {
  return hours(p_milliseconds / static_cast<double>(1000 * 60 * 60));
}

template <>
inline days convert<milliseconds, days>(milliseconds p_milliseconds) {
  return days(p_milliseconds / static_cast<double>(1000 * 60 * 60 * 24));
}

template <>
inline weeks convert<milliseconds, weeks>(milliseconds p_milliseconds) {
  return weeks(p_milliseconds / static_cast<double>(1000 * 60 * 60 * 24 * 7));
}

template <>
inline months convert<milliseconds, months>(milliseconds p_milliseconds) {
  return months(p_milliseconds / static_cast<double>(1000) * 60 * 60 * 24 *
                months_per_year);
}

template <>
inline years convert<milliseconds, years>(milliseconds p_milliseconds) {
  return years(p_milliseconds / static_cast<double>(1000) * 60 * 60 * 24 * 365);
}

// from seconds

template <>
inline milliseconds convert<seconds, milliseconds>(seconds p_seconds) {
  return milliseconds(p_seconds.value() * static_cast<double>(1000));
}

template <> inline minutes convert<seconds, minutes>(seconds p_seconds) {
  return minutes(p_seconds.value() / static_cast<double>(60));
}

template <> inline hours convert<seconds, hours>(seconds p_seconds) {
  //  return convert<seconds, minutes>(p_seconds.value()) /
  //  static_cast<double>(60);
  return hours(p_seconds.value() / static_cast<double>(60 * 60));
}

template <> inline days convert<seconds, days>(seconds p_seconds) {
  //  return convert<seconds, hours>(p_seconds.value()) /
  //  static_cast<double>(24);
  return days(p_seconds.value() / static_cast<double>(60 * 60 * 24));
}

template <> inline weeks convert<seconds, weeks>(seconds p_seconds) {
  //  return convert<seconds, days>(p_seconds.value()) / static_cast<double>(7);
  return weeks(p_seconds.value() / static_cast<double>(60 * 60 * 24 * 7));
}

template <> inline months convert<seconds, months>(seconds p_seconds) {
  //  return convert<seconds, days>(p_seconds.value()) / static_cast<double>(7);
  return months(p_seconds.value() / (60 * 60 * 24 * months_per_year));
}

template <> inline years convert<seconds, years>(seconds p_seconds) {
  //  return convert<seconds, days>(p_seconds.value()) / static_cast<double>(7);
  return years(p_seconds.value() / static_cast<double>(60 * 60 * 24 * 365));
}

// from minutes

template <>
inline milliseconds convert<minutes, milliseconds>(minutes p_minutes) {
  return milliseconds(p_minutes.value() * static_cast<double>(60 * 1000));
}

template <> inline seconds convert<minutes, seconds>(minutes p_minutes) {
  return seconds(p_minutes.value() * static_cast<double>(60));
}

template <> inline hours convert<minutes, hours>(minutes p_minutes) {
  return hours(p_minutes.value() / static_cast<double>(60));
}

template <> inline days convert<minutes, days>(minutes p_minutes) {
  return days(p_minutes.value() / static_cast<double>(60 * 24));
}

template <> inline weeks convert<minutes, weeks>(minutes p_minutes) {
  return weeks(p_minutes.value() / static_cast<double>(60 * 24 * 7));
}

template <> inline months convert<minutes, months>(minutes p_minutes) {
  return months(p_minutes.value() / (60 * 24 * months_per_year));
}

template <> inline years convert<minutes, years>(minutes p_minutes) {
  return years(p_minutes.value() / static_cast<double>(60 * 24 * 365));
}

// from hours

template <> inline days convert<hours, days>(hours p_hours) {
  return days(p_hours.value() / static_cast<double>(24));
}

template <> inline weeks convert<hours, weeks>(hours p_hours) {
  return weeks(p_hours.value() / static_cast<double>(24 * 7));
}

template <> inline months convert<hours, months>(hours p_hours) {
  return months(p_hours.value() / (24 * months_per_year));
}

template <> inline years convert<hours, years>(hours p_hours) {
  return years(p_hours.value() / static_cast<double>(24 * 365));
}

template <> inline minutes convert<hours, minutes>(hours p_hours) {
  return minutes(p_hours.value() * static_cast<double>(60));
}

template <> inline seconds convert<hours, seconds>(hours p_hours) {
  return seconds(p_hours.value() * static_cast<double>(60 * 60));
}

template <> inline milliseconds convert<hours, milliseconds>(hours p_hours) {
  return milliseconds(p_hours.value() * static_cast<double>(1000) * 60 * 60);
}

// from days

template <> inline weeks convert<days, weeks>(days p_days) {
  return weeks(p_days.value() / static_cast<double>(7));
}

template <> inline months convert<days, months>(days p_days) {
  return months(p_days.value() / months_per_year);
}

template <> inline years convert<days, years>(days p_days) {
  return years(p_days.value() / static_cast<double>(365));
}

template <> inline hours convert<days, hours>(days p_days) {
  return hours(p_days.value() * static_cast<double>(24));
}

template <> inline minutes convert<days, minutes>(days p_days) {
  return minutes(p_days.value() * static_cast<double>(24 * 60));
}

template <> inline seconds convert<days, seconds>(days p_days) {
  return seconds(p_days.value() * static_cast<double>(24 * 60 * 60));
}

template <> inline milliseconds convert<days, milliseconds>(days p_days) {
  return milliseconds(p_days.value() * static_cast<double>(24) * 60 * 60 *
                      1000);
}

// from weeks

template <> inline months convert<weeks, months>(weeks p_weeks) {
  return months(p_weeks.value() / (months_per_year / 7));
}

template <> inline years convert<weeks, years>(weeks p_weeks) {
  return years(p_weeks.value() / (365 / static_cast<double>(7)));
}

template <> inline days convert<weeks, days>(weeks p_weeks) {
  return days(p_weeks.value() * static_cast<double>(7));
}

template <> inline hours convert<weeks, hours>(weeks p_weeks) {
  return hours(p_weeks.value() * static_cast<double>(7 * 24));
}

template <> inline minutes convert<weeks, minutes>(weeks p_weeks) {
  return minutes(p_weeks.value() * static_cast<double>(7 * 24 * 60));
}

template <> inline seconds convert<weeks, seconds>(weeks p_weeks) {
  return seconds(p_weeks.value() * static_cast<double>(7 * 24 * 60 * 60));
}

template <> inline milliseconds convert<weeks, milliseconds>(weeks p_weeks) {
  return milliseconds(p_weeks.value() * static_cast<double>(7) * 24 * 60 * 60 *
                      1000);
}

// from months

template <> inline years convert<months, years>(months p_months) {
  return years(p_months.value() / static_cast<double>(12));
}

template <> inline weeks convert<months, weeks>(months p_months) {
  return weeks(p_months.value() * (months_per_year / 7));
}

template <> inline days convert<months, days>(months p_months) {
  return days(p_months.value() * months_per_year);
}

template <> inline hours convert<months, hours>(months p_months) {
  return hours(p_months.value() * 24 * months_per_year);
}

template <> inline minutes convert<months, minutes>(months p_months) {
  return minutes(p_months.value() * 60 * 24 * months_per_year);
}

template <> inline seconds convert<months, seconds>(months p_months) {
  return seconds(p_months.value() * 60 * 60 * 24 * months_per_year);
}

template <> inline milliseconds convert<months, milliseconds>(months p_months) {
  return milliseconds(p_months.value() * static_cast<double>(60) * 60 * 24 *
                      months_per_year * 1000);
}

// from years

template <> inline months convert<years, months>(years p_years) {
  return months(p_years.value() * static_cast<double>(12));
}

template <> inline weeks convert<years, weeks>(years p_years) {
  return weeks(p_years.value() * (365 / static_cast<double>(7)));
}

template <> inline days convert<years, days>(years p_years) {
  return days(p_years.value() * static_cast<double>(365));
}

template <> inline hours convert<years, hours>(years p_years) {
  return hours(p_years.value() * 365 * static_cast<double>(24));
}

template <> inline minutes convert<years, minutes>(years p_years) {
  return minutes(p_years.value() * 365 * 24 * static_cast<double>(60));
}

template <> inline seconds convert<years, seconds>(years p_years) {
  return seconds(p_years.value() * 365 * 24 * 60 * static_cast<double>(60));
}

template <> inline milliseconds convert<years, milliseconds>(years p_years) {
  return milliseconds(p_years.value() * 365 * 24 * 60 * 1000 *
                      static_cast<double>(60));
}

//// partition
// template <typename t_amount>
// std::tuple<years, months, weeks, days, hours, minutes, seconds>
// partition(t_amount p_amount) {

//}

// template <typename t_to>
//  inline t_to convert(const year &p_year, const month &p_month);

// template <>
//  inline days convert<days>(const year &p_year, const month &p_month)
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
//  inline minutes convert<minutes>(const year &p_year,
//                                          const month &p_month) {
//  return convert<days, minutes>(convert<days>(p_year, p_month));
//}

// template <>
//  inline seconds convert<seconds>(const year &p_year,
//                                          const month &p_month) {
//  return convert<minutes, seconds>(
//      convert<days, minutes>(convert<days>(p_year, p_month)));
//}

} // namespace measures
} // namespace tenacitas
#endif // TENACITAS_CALENDAR_CONVERSIONS_H
