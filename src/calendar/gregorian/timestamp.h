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
  timestamp() = default;

  ///
  /// \brief timestamp
  /// \param p_year
  /// \param p_month
  /// \param p_day
  /// \param p_hour
  /// \param p_minute
  /// \param p_second
  ///
  timestamp(year p_year, month p_month, day p_day, hour p_hour = hour::_00,
            minute p_minute = minute::_00, second p_second = second::_00);
  ///
  /// \brief timestamp copy constructor
  /// \param p_timestamp timestamp to be copied
  ///
  inline timestamp(const timestamp &p_timestamp)
      : m_days(p_timestamp.m_days), m_secs(p_timestamp.m_secs) {}

  ///
  /// \brief timestamp move constructor
  /// \param p_timestamp timestamp to be moved
  ///
  timestamp(timestamp &&p_timestamp)
      : m_days(std::move(p_timestamp.m_days)),
        m_secs(std::move(p_timestamp.m_secs)) {}

  /// \brief operator = copy assignment
  /// \param p_timestamp
  /// \return
  ///
  timestamp &operator=(const timestamp &p_timestamp);

  ///
  /// \brief operator = move assignment
  /// \param p_timestamp
  /// \return
  ///
  timestamp &operator=(timestamp &&p_timestamp);

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
  second get_second();

  ///
  /// \brief get_minute
  /// \return
  ///
  minute get_minute();

  ///
  /// \brief get_hour
  /// \return
  ///
  hour get_hour();

  ///
  /// \brief get_day
  /// \return
  ///
  day get_day();

  ///
  /// \brief get_weekday
  /// \return
  ///
  weekday get_weekday();

  ///
  /// \brief get_month
  /// \return
  ///
  month get_month();

  ///
  /// \brief get_year
  /// \return
  ///
  year get_year();

  ///
  /// \brief now
  /// \return
  ///
  static timestamp now();

  ///
  /// \brief operator <<
  /// \param p_out
  /// \param p_ts
  /// \return
  ///
  friend std::ostream &operator<<(std::ostream &p_out, timestamp &p_ts);

  ///
  /// \brief operator ==
  /// \param p_ts
  /// \return
  ///
  bool operator==(const timestamp &p_ts) const {
    return ((m_days == p_ts.m_days) && (m_secs == p_ts.m_secs));
  }
  ///
  /// \brief operator !=
  /// \param p_ts
  /// \return
  ///
  bool operator!=(const timestamp &p_ts) const {
    return ((m_days != p_ts.m_days) || (m_secs != p_ts.m_secs));
  }

  ///
  /// \brief operator >
  /// \param p_ts
  /// \return
  ///
  bool operator>(const timestamp &p_ts) const;

  ///
  /// \brief operator <
  /// \param p_ts
  /// \return
  ///
  bool operator<(const timestamp &p_ts) const;

  ///
  /// \brief operator >=
  /// \param p_ts
  /// \return
  ///
  bool operator>=(const timestamp &p_ts) const;

  ///
  /// \brief operator <=
  /// \param p_ts
  /// \return
  ///
  bool operator<=(const timestamp &p_ts) const;

  //  ///
  //  /// \brief operator +=
  //  /// \param p_seconds
  //  /// \return
  //  ///
  //  timestamp &operator+=(seconds p_seconds);

  ///
  /// \brief operator +
  /// \param p_seconds
  /// \return
  ///
  template <typename t_amount> timestamp operator+(t_amount p_amount) {
    timestamp _timestamp(*this);

    days _days_1(p_amount);
    auto _int = _days_1.integer();
    auto _dec = _days_1.decimal();

    seconds _secs_in_day((days(_dec)));

    seconds _secs(_timestamp.m_secs + _secs_in_day);

    if (days(_secs) > days(1)) {
      _timestamp.m_days += days(_int + 1);
      _timestamp.m_secs = _secs - seconds(days(1));
    } else {
      _timestamp.m_days += days(_int);
      _timestamp.m_secs += _secs_in_day.integer();
    }
    return _timestamp;
  }

  //  ///
  //  /// \brief operator -=
  //  /// \param p_seconds
  //  /// \return
  //  ///
  //  timestamp &operator-=(seconds p_seconds);

  ///
  /// \brief operator -
  /// \param p_amount
  /// \return
  ///
  template <typename t_amount> timestamp operator-(t_amount p_amount) {
    timestamp _timestamp(*this);

    days _days_1(p_amount);
    auto _int = _days_1.integer();
    auto _dec = _days_1.decimal();

    seconds _secs_in_day((days(_dec)));

    seconds _secs(_timestamp.m_secs - _secs_in_day);

    if (days(_secs) < 0) {
      _timestamp.m_days -= days(_int + 1);
      _timestamp.m_secs = seconds(days(1)) + _secs;
    } else {
      _timestamp.m_days -= days(_int);
      _timestamp.m_secs += _secs_in_day.integer();
    }

    return _timestamp;
  }

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
  constexpr void dMyhms2days_secs(year p_year, month p_month, day p_day,
                                  hour p_hour, minute p_minute,
                                  second p_second);

  constexpr void days_secs2dMywhms();

private:
  /// \brief amount of days since epoch
  days m_days = {0};

  /// \brief amount of seconds in the day
  seconds m_secs = {0};

  /// \brief indicates that the parts of the timestamp - m_year, m_month, m_day,
  /// m_hour, m_minute, m_second, m_weekday - need to be updated
  bool m_needs_update = {true};

  year m_year = year(1970);
  month m_month = month::jan;
  day m_day = day::_01;
  hour m_hour = hour::_00;
  minute m_minute = minute::_00;
  second m_second = second::_00;
  weekday m_weekday = weekday::thu;
};

} // namespace gregorian
} // namespace calendar
} // namespace tenacitas

#endif
