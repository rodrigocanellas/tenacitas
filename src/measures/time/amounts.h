#ifndef TENACITAS_MEASURES_AMOUNTS_H
#define TENACITAS_MEASURES_AMOUNTS_H

#include <cstdint>
#include <tuple>

#include <measures/time/day.h>
#include <measures/time/hour.h>
#include <measures/internal/amount_t.h>
#include <measures/internal/convert_t.h>
#include <measures/time/millisecond.h>
#include <measures/time/minute.h>
#include <measures/time/month.h>
#include <measures/time/second.h>
#include <measures/time/week.h>
#include <measures/time/year.h>



namespace tenacitas {
namespace measures {
namespace time {

typedef amount_t<time::millisecond> milliseconds;
typedef amount_t<time::second> seconds;
typedef amount_t<time::minute> minutes;
typedef amount_t<time::hour> hours;
typedef amount_t<time::day> days;
typedef amount_t<time::week> weeks;
typedef amount_t<time::month> months;
typedef amount_t<time::year> years;
}
}
}

namespace tenacitas {
namespace measures {


static inline double months_per_year () {return(365 / static_cast<double>(12));}

// from time::milliseconds

template <>
inline time::seconds convert<time::milliseconds, time::seconds>(time::milliseconds p_milliseconds) {
  return time::seconds(p_milliseconds / static_cast<double>(1000));
}

template <>
inline time::minutes convert<time::milliseconds, time::minutes>(time::milliseconds p_milliseconds) {
  return time::minutes(p_milliseconds / static_cast<double>(1000 * 60));
}

template <>
inline time::hours convert<
time::milliseconds, time::hours>(time::milliseconds p_milliseconds) {
  return time::hours(p_milliseconds / static_cast<double>(1000 * 60 * 60));
}

template <>
inline time::days convert<time::milliseconds, time::days>(time::milliseconds p_milliseconds) {
  return time::days(p_milliseconds / static_cast<double>(1000 * 60 * 60 * 24));
}

template <>
inline time::weeks convert<time::milliseconds, time::weeks>(time::milliseconds p_milliseconds) {
  return time::weeks(p_milliseconds / static_cast<double>(1000 * 60 * 60 * 24 * 7));
}

template <>
inline time::months convert<time::milliseconds, time::months>(time::milliseconds p_milliseconds) {
  return time::months(p_milliseconds / static_cast<double>(1000 * 60 * 60 * 24) *
                months_per_year());
}

template <>
inline time::years convert<time::milliseconds, time::years>(time::milliseconds p_milliseconds) {
  return time::years(p_milliseconds.value() / static_cast<double>(1000) * 60 * 60 * 24 * 365);
}

// from time::seconds

template <>
inline time::milliseconds convert<time::seconds, time::milliseconds>(time::seconds p_seconds) {
  return time::milliseconds(p_seconds.value() * static_cast<double>(1000));
}

template <> inline time::minutes convert<time::seconds, time::minutes>(time::seconds p_seconds) {
  return time::minutes(p_seconds.value() / static_cast<double>(60));
}

template <> inline time::hours convert<time::seconds, time::hours>(time::seconds p_seconds) {
  //  return convert<time::seconds, time::minutes>(p_seconds.value()) /
  //  static_cast<double>(60);
  return time::hours(p_seconds.value() / static_cast<double>(60 * 60));
}

template <> inline time::days convert<time::seconds, time::days>(time::seconds p_seconds) {
  //  return convert<time::seconds, time::hours>(p_seconds.value()) /
  //  static_cast<double>(24);
  return time::days(p_seconds.value() / static_cast<double>(60 * 60 * 24));
}

template <> inline time::weeks convert<time::seconds, time::weeks>(time::seconds p_seconds) {
  //  return convert<time::seconds, time::days>(p_seconds.value()) / static_cast<double>(7);
  return time::weeks(p_seconds.value() / static_cast<double>(60 * 60 * 24 * 7));
}

template <> inline time::months convert<time::seconds, time::months>(time::seconds p_seconds) {
  //  return convert<time::seconds, time::days>(p_seconds.value()) / static_cast<double>(7);
  return time::months(p_seconds.value() / (60 * 60 * 24 * months_per_year()));
}

template <> inline time::years convert<time::seconds, time::years>(time::seconds p_seconds) {
  //  return convert<time::seconds, time::days>(p_seconds.value()) / static_cast<double>(7);
  return time::years(p_seconds.value() / static_cast<double>(60 * 60 * 24 * 365));
}

// from time::minutes

template <>
inline time::milliseconds convert<time::minutes, time::milliseconds>(time::minutes p_minutes) {
  return time::milliseconds(p_minutes.value() * static_cast<double>(60 * 1000));
}

template <> inline time::seconds convert<time::minutes, time::seconds>(time::minutes p_minutes) {
  return time::seconds(p_minutes.value() * static_cast<double>(60));
}

template <> inline time::hours convert<time::minutes, time::hours>(time::minutes p_minutes) {
  return time::hours(p_minutes.value() / static_cast<double>(60));
}

template <> inline time::days convert<time::minutes, time::days>(time::minutes p_minutes) {
  return time::days(p_minutes.value() / static_cast<double>(60 * 24));
}

template <> inline time::weeks convert<time::minutes, time::weeks>(time::minutes p_minutes) {
  return time::weeks(p_minutes.value() / static_cast<double>(60 * 24 * 7));
}

template <> inline time::months convert<time::minutes, time::months>(time::minutes p_minutes) {
  return time::months(p_minutes.value() / (60 * 24 * months_per_year()));
}

template <> inline time::years convert<time::minutes, time::years>(time::minutes p_minutes) {
  return time::years(p_minutes.value() / static_cast<double>(60 * 24 * 365));
}

// from time::hours

template <> inline time::days convert<time::hours, time::days>(time::hours p_hours) {
  return time::days(p_hours.value() / static_cast<double>(24));
}

template <> inline time::weeks convert<time::hours, time::weeks>(time::hours p_hours) {
  return time::weeks(p_hours.value() / static_cast<double>(24 * 7));
}

template <> inline time::months convert<time::hours, time::months>(time::hours p_hours) {
  return time::months(p_hours.value() / (24 * months_per_year()));
}

template <> inline time::years convert<time::hours, time::years>(time::hours p_hours) {
  return time::years(p_hours.value() / static_cast<double>(24 * 365));
}

template <> inline time::minutes convert<time::hours, time::minutes>(time::hours p_hours) {
  return time::minutes(p_hours.value() * static_cast<double>(60));
}

template <> inline time::seconds convert<time::hours, time::seconds>(time::hours p_hours) {
  return time::seconds(p_hours.value() * static_cast<double>(60 * 60));
}

template <> inline time::milliseconds convert<time::hours, time::milliseconds>(time::hours p_hours) {
  return time::milliseconds(p_hours.value() * static_cast<double>(1000) * 60 * 60);
}

// from time::days

template <> inline time::weeks convert<time::days, time::weeks>(time::days p_days) {
  return time::weeks(p_days.value() / static_cast<double>(7));
}

template <> inline time::months convert<time::days, time::months>(time::days p_days) {
  return time::months(p_days.value() / months_per_year());
}

template <> inline time::years convert<time::days, time::years>(time::days p_days) {
  return time::years(p_days.value() / static_cast<double>(365));
}

template <> inline time::hours convert<time::days, time::hours>(time::days p_days) {
  return time::hours(p_days.value() * static_cast<double>(24));
}

template <> inline time::minutes convert<time::days, time::minutes>(time::days p_days) {
  return time::minutes(p_days.value() * static_cast<double>(24 * 60));
}

template <> inline time::seconds convert<time::days, time::seconds>(time::days p_days) {
  return time::seconds(p_days.value() * static_cast<double>(24 * 60 * 60));
}

template <> inline time::milliseconds convert<time::days, time::milliseconds>(time::days p_days) {
  return time::milliseconds(p_days.value() * static_cast<double>(24) * 60 * 60 *
                      1000);
}

// from time::weeks

template <> inline time::months convert<time::weeks, time::months>(time::weeks p_weeks) {
  return time::months(p_weeks.value() / (months_per_year() / 7));
}

template <> inline time::years convert<time::weeks, time::years>(time::weeks p_weeks) {
  return time::years(p_weeks.value() / (365 / static_cast<double>(7)));
}

template <> inline time::days convert<time::weeks, time::days>(time::weeks p_weeks) {
  return time::days(p_weeks.value() * static_cast<double>(7));
}

template <> inline time::hours convert<time::weeks, time::hours>(time::weeks p_weeks) {
  return time::hours(p_weeks.value() * static_cast<double>(7 * 24));
}

template <> inline time::minutes convert<time::weeks, time::minutes>(time::weeks p_weeks) {
  return time::minutes(p_weeks.value() * static_cast<double>(7 * 24 * 60));
}

template <> inline time::seconds convert<time::weeks, time::seconds>(time::weeks p_weeks) {
  return time::seconds(p_weeks.value() * static_cast<double>(7 * 24 * 60 * 60));
}

template <> inline time::milliseconds convert<time::weeks, time::milliseconds>(time::weeks p_weeks) {
  return time::milliseconds(p_weeks.value() * static_cast<double>(7) * 24 * 60 * 60 *
                      1000);
}

// from time::months

template <> inline time::years convert<time::months, time::years>(time::months p_months) {
  return time::years(p_months.value() / static_cast<double>(12));
}

template <> inline time::weeks convert<time::months, time::weeks>(time::months p_months) {
  return time::weeks(p_months.value() * (months_per_year() / 7));
}

template <> inline time::days convert<time::months, time::days>(time::months p_months) {
  return time::days(p_months.value() * months_per_year());
}

template <> inline time::hours convert<time::months, time::hours>(time::months p_months) {
  return time::hours(p_months.value() * 24 * months_per_year());
}

template <> inline time::minutes convert<time::months, time::minutes>(time::months p_months) {
  return time::minutes(p_months.value() * 60 * 24 * months_per_year());
}

template <> inline time::seconds convert<time::months, time::seconds>(time::months p_months) {
  return time::seconds(p_months.value() * 60 * 60 * 24 * months_per_year());
}

template <> inline time::milliseconds convert<time::months, time::milliseconds>(time::months p_months) {
  return time::milliseconds(p_months.value() * static_cast<double>(60) * 60 * 24 *
                      months_per_year() * 1000);
}

// from time::years

template <> inline time::months convert<time::years, time::months>(time::years p_years) {
  return time::months(p_years.value() * static_cast<double>(12));
}

template <> inline time::weeks convert<time::years, time::weeks>(time::years p_years) {
  return time::weeks(p_years.value() * (365 / static_cast<double>(7)));
}

template <> inline time::days convert<time::years, time::days>(time::years p_years) {
  return time::days(p_years.value() * static_cast<double>(365));
}

template <> inline time::hours convert<time::years, time::hours>(time::years p_years) {
  return time::hours(p_years.value() * 365 * static_cast<double>(24));
}

template <> inline time::minutes convert<time::years, time::minutes>(time::years p_years) {
  return time::minutes(p_years.value() * 365 * 24 * static_cast<double>(60));
}

template <> inline time::seconds convert<time::years, time::seconds>(time::years p_years) {
  return time::seconds(p_years.value() * 365 * 24 * 60 * static_cast<double>(60));
}

template <> inline time::milliseconds convert<time::years, time::milliseconds>(time::years p_years) {
  return time::milliseconds(p_years.value() * 365 * 24 * 60 * 1000 *
                      static_cast<double>(60));
}

//// partition
// template <typename t_amount>
// std::tuple<time::years, time::months, time::weeks, time::days, time::hours, time::minutes, time::seconds>
// partition(t_amount p_amount) {

//}

// template <typename t_to>
//  inline t_to convert(const year &p_year, const month &p_month);

// template <>
//  inline time::days convert<time::days>(const year &p_year, const month &p_month)
// {
//  if ((p_month == month::jan) || (p_month == month::mar) ||
//      (p_month == month::may) || (p_month == month::jul) ||
//      (p_month == month::ago) || (p_month == month::oct) ||
//      (p_month == month::dec)) {
//    return time::days(31);
//  }
//  if ((p_month == month::apr) || (p_month == month::jun) ||
//      (p_month == month::sep) || (p_month == month::nov)) {
//    return time::days(30);
//  }
//  if (p_year.is_leap()) {
//    return time::days(29);
//  }
//  return time::days(28);
//}

// template <>
//  inline time::minutes convert<time::minutes>(const year &p_year,
//                                          const month &p_month) {
//  return convert<time::days, time::minutes>(convert<time::days>(p_year, p_month));
//}

// template <>
//  inline time::seconds convert<time::seconds>(const year &p_year,
//                                          const month &p_month) {
//  return convert<time::minutes, time::seconds>(
//      convert<time::days, time::minutes>(convert<time::days>(p_year, p_month)));
//}


} // namespace measures
} // namespace tenacitas
#endif // TENACITAS_CALENDAR_CONVERSIONS_H
