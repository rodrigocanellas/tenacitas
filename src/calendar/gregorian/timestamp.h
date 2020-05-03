#ifndef TENACITAS_CALENDAR_GREGORIAN_TIMESTAMP_H
#define TENACITAS_CALENDAR_GREGORIAN_TIMESTAMP_H

/// \author Rodrigo Canellas - rodrigo.canellas@gmail.com
/// \date 04/2020
///
/// \attention This implementation is based on the the code published at
/// https://howardhinnant.github.io/date_algorithms.html. Thanks Howard Hinnant,
/// for your contribution.

#include <chrono>
#include <cstdint>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <tuple>

#include <calendar/gregorian/amounts.h>
#include <calendar/gregorian/day.h>
#include <calendar/gregorian/hour.h>
#include <calendar/gregorian/minute.h>
#include <calendar/gregorian/month.h>
#include <calendar/gregorian/second.h>
#include <calendar/gregorian/weekday.h>
#include <calendar/gregorian/year.h>

namespace tenacitas {
namespace calendar {
namespace gregorian {

///
/// \brief The timestamp struct represents a certain moment in time, with
/// seconds precision
///
struct timestamp {
  ///
  /// \brief timestamp default constructor creates a timestamp for now
  ///
  timestamp();

  //  ///
  //  /// \brief timestamp creates a timestamp by informing day, month, year,
  //  /// hour, minute and second \param p_year \param p_month \param p_day
  //  \param
  //  /// p_hour \param p_minute \param p_second
  //  ///
  //  timestamp(year p_year, month p_month, day p_day, hour p_hour = hour::h00,
  //            minute p_minute = minute::m00, second p_second = second::s00);
  //  ///
  //  /// \brief timestamp copy constructor
  //  /// \param p_timestamp timestamp to be copied
  //  ///
  //  timestamp(const timestamp &p_timestamp);

  //  ///
  //  /// \brief timestamp move constructor
  //  /// \param p_timestamp timestamp to be moved
  //  ///
  //  timestamp(timestamp &&p_timestamp);

  //  explicit timestamp(days p_days);

  //  /// \brief operator = copy assignment
  //  /// \param p_timestamp
  //  /// \return
  //  ///
  //  timestamp &operator=(const timestamp &p_timestamp);

  //  ///
  //  /// \brief operator = move assignment
  //  /// \param p_timestamp
  //  /// \return
  //  ///
  //  timestamp &operator=(timestamp &&p_timestamp);

  ///
  /// \brief operator <<
  /// \param p_out
  /// \param p_ts
  /// \return
  ///
  friend std::ostream &operator<<(std::ostream &p_out, const timestamp &p_ts);

  //  ///
  //  /// \brief set_day
  //  /// \param p_day
  //  /// \throw if the resulting \p timestamp is invalid
  //  ///
  //  void set_day(day p_day);

  //  ///
  //  /// \brief set_day_month
  //  /// \param p_day
  //  /// \param p_month
  //  ///
  //  void set_day_month(day p_day, month p_month);

  ///
  /// \brief get_second
  /// \return
  ///
  second get_second() const { return m_second; }

  ///
  /// \brief get_minute
  /// \return
  ///
  minute get_minute() const { return m_minute; }

  ///
  /// \brief get_hour
  /// \return
  ///
  hour get_hour() const { return m_hour; }

  ///
  /// \brief get_day
  /// \return
  ///
  day get_day() const { return m_day; }

  //  ///
  //  /// \brief get_weekday
  //  /// \return
  //  ///
  //  weekday get_weekday() const;

  ///
  /// \brief get_month
  /// \return
  ///
  month get_month() const { return m_month; }

  ///
  /// \brief get_year
  /// \return
  ///
  year get_year() const { return m_year; }

  //  ///
  //  /// \brief operator ==
  //  /// \param p_ts
  //  /// \return
  //  ///
  //  bool operator==(const timestamp &p_ts) const;
  //  ///
  //  /// \brief operator !=
  //  /// \param p_ts
  //  /// \return
  //  ///
  //  bool operator!=(const timestamp &p_ts) const;

  //  ///
  //  /// \brief operator >
  //  /// \param p_ts
  //  /// \return
  //  ///
  //  bool operator>(const timestamp &p_ts) const;

  //  ///
  //  /// \brief operator <
  //  /// \param p_ts
  //  /// \return
  //  ///
  //  bool operator<(const timestamp &p_ts) const;

  //  ///
  //  /// \brief operator >=
  //  /// \param p_ts
  //  /// \return
  //  ///
  //  bool operator>=(const timestamp &p_ts) const;

  //  ///
  //  /// \brief operator <=
  //  /// \param p_ts
  //  /// \return
  //  ///
  //  bool operator<=(const timestamp &p_ts) const;

  //  ///
  //  /// \brief operator +=
  //  /// \param p_seconds
  //  /// \return
  //  ///
  //  timestamp &operator+=(seconds p_seconds);

  //  ///
  //  /// \brief operator +
  //  /// \param p_seconds
  //  /// \return
  //  ///
  //  timestamp operator+(seconds p_seconds);

  //  ///
  //  /// \brief operator -=
  //  /// \param p_seconds
  //  /// \return
  //  ///
  //  timestamp &operator-=(seconds p_seconds);

  //  ///
  //  /// \brief operator -
  //  /// \param p_seconds
  //  /// \return
  //  ///
  //  timestamp operator-(seconds p_seconds);

  //  ///
  //  /// \brief operator +=
  //  /// \param p_minutes
  //  /// \return
  //  ///
  //  timestamp &operator+=(minutes p_minutes);

  //  ///
  //  /// \brief operator +
  //  /// \param p_minutes
  //  /// \return
  //  ///
  //  timestamp operator+(minutes p_minutes);

  //  ///
  //  /// \brief operator -=
  //  /// \param p_minutes
  //  /// \return
  //  ///
  //  timestamp &operator-=(minutes p_minutes);

  //  ///
  //  /// \brief operator -
  //  /// \param p_minutes
  //  /// \return
  //  ///
  //  timestamp operator-(minutes p_minutes);

  //  ///
  //  /// \brief operator +=
  //  /// \param p_days
  //  /// \return
  //  ///
  //  timestamp &operator+=(days p_days);

  //  ///
  //  /// \brief operator +
  //  /// \param p_days
  //  /// \return
  //  ///
  //  timestamp operator+(days p_days);

  //  ///
  //  /// \brief operator -=
  //  /// \param p_days
  //  /// \return
  //  ///
  //  timestamp &operator-=(days p_days);

  //  ///
  //  /// \brief operator -
  //  /// \param p_days
  //  /// \return
  //  ///
  //  timestamp operator-(days p_days);

  //  ///
  //  /// \brief operator +=
  //  /// \param p_weeks
  //  /// \return
  //  ///
  //  timestamp &operator+=(weekdays p_weeks);
  //  ///
  //  /// \brief operator +
  //  /// \param p_weeks
  //  /// \return
  //  ///
  //  timestamp operator+(weekdays p_weeks);

  //  ///
  //  /// \brief operator -=
  //  /// \param p_weeks
  //  /// \return
  //  ///
  //  timestamp &operator-=(weekdays p_weeks);

  //  ///
  //  /// \brief operator -
  //  /// \param p_weeks
  //  /// \return
  //  ///
  //  timestamp operator-(weekdays p_weeks);

  //  ///
  //  /// \brief operator +=
  //  /// \param p_months
  //  /// \return
  //  ///
  //  timestamp &operator+=(months p_months);

  //  ///
  //  /// \brief operator +
  //  /// \param p_months
  //  /// \return
  //  ///
  //  timestamp operator+(months p_months);

  //  ///
  //  /// \brief operator -=
  //  /// \param p_months
  //  /// \return
  //  ///
  //  timestamp &operator-=(months p_months);

  //  ///
  //  /// \brief operator -
  //  /// \param p_months
  //  /// \return
  //  ///
  //  timestamp operator-(months p_months);

  //  ///
  //  /// \brief operator +=
  //  /// \param p_years
  //  /// \return
  //  ///
  //  timestamp operator+=(years p_years);

  //  ///
  //  /// \brief operator +
  //  /// \param p_years
  //  /// \return
  //  ///
  //  timestamp operator+(years p_years);

  //  ///
  //  /// \brief operator -=
  //  /// \param p_years
  //  /// \return
  //  ///
  //  timestamp operator-=(years p_years);

  //  ///
  //  /// \brief operator -
  //  /// \param p_years
  //  /// \return
  //  ///
  //  timestamp operator-(years p_years);

private:
  constexpr void dmy2days(year p_year, month p_month, day p_day);

  //  template <class Int>
  //  constexpr Int days_from_civil1(Int y, unsigned m, unsigned d) noexcept {
  //    y -= m <= 2;
  //    const Int era = (y >= 0 ? y : y - 399) / 400;
  //    const unsigned yoe = static_cast<unsigned>(y - era * 400); // [0, 399]
  //    const unsigned doy =
  //        (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;         // [0, 365]
  //    const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy; // [0,
  //    146096] return era * 146097 + static_cast<Int>(doe) - 719468;
  //  }

  //  template <class Int>
  //  constexpr std::tuple<Int, unsigned, unsigned>
  //  civil_from_days(Int z) noexcept {
  //    static_assert(
  //        std::numeric_limits<unsigned>::digits >= 18,
  //        "This algorithm has not been ported to a 16 bit unsigned integer");
  //    static_assert(
  //        std::numeric_limits<Int>::digits >= 20,
  //        "This algorithm has not been ported to a 16 bit signed integer");
  //    z += 719468;
  //    const Int era = (z >= 0 ? z : z - 146096) / 146097;
  //    const unsigned doe = static_cast<unsigned>(z - era * 146097); // [0,
  //    146096] const unsigned yoe =
  //        (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365; // [0, 399]
  //    const Int y = static_cast<Int>(yoe) + era * 400;
  //    const unsigned doy = doe - (365 * yoe + yoe / 4 - yoe / 100); // [0,
  //    365] const unsigned mp = (5 * doy + 2) / 153;                      //
  //    [0, 11] const unsigned d = doy - (153 * mp + 2) / 5 + 1;              //
  //    [1, 31] const unsigned m = mp + (mp < 10 ? 3 : -9);                   //
  //    [1, 12] return std::tuple<Int, unsigned, unsigned>(y + (m <= 2), m, d);
  //  }

  constexpr void days2mdy();

  constexpr void secs2hms();

private:
  /// \brief amount of days since epoch
  days m_days = days(0);

  /// \brief amount of seconds in the day
  seconds m_secs = 0;

  year m_year = year(1970);
  month m_month = month::jan;
  day m_day = day::_01;
  hour m_hour = hour::_00;
  minute m_minute = minute::_00;
  second m_second = second::_00;
};

} // namespace gregorian
} // namespace calendar
} // namespace tenacitas

#endif
