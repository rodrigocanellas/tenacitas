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

  /// \brief adds an amount of time to the current timestamp
  ///
  /// \tparam t_amount is the type amount of time to be added to this timestamp
  ///
  /// \param p_amount  is the value of the amount of time to be added to this
  /// timestamp
  ///
  /// \return this timestamp modified
  template <typename t_amount> timestamp &operator+=(t_amount p_amount) {
    add(this, p_amount);
    return *this;
  }

  /// \brief adds an amount of time to the current timestamp, creating a new
  /// timestamp
  ///
  /// \tparam t_amount is the type amount of time to be added to this timestamp
  ///
  /// \param p_amount  is the value of the amount of time to be added to this
  /// timestamp
  ///
  /// \return the new timestamp
  template <typename t_amount> timestamp operator+(t_amount p_amount) {
    timestamp _timestamp(*this);
    add(&_timestamp, p_amount);
    return _timestamp;
  }

  /// \brief substractys an amount of time to the current timestamp
  ///
  /// \tparam t_amount is the type amount of time to be substracted to this
  /// timestamp
  ///
  /// \param p_amount  is the value of the amount of time to be substracted to
  /// this timestamp
  ///
  /// \return this timestamp modified
  template <typename t_amount> timestamp &operator-=(t_amount p_amount) {
    substract(this, p_amount);
    return *this;
  }

  /// \brief subtracts an amount of time to the current timestamp, creating a
  /// new timestamp
  ///
  /// \tparam t_amount is the type amount of time to be substracted to this
  /// timestamp
  ///
  /// \param p_amount  is the value of the amount of time to be substracted to
  /// this timestamp
  ///
  /// \return the new timestamp
  template <typename t_amount> timestamp operator-(t_amount p_amount) {
    timestamp _timestamp(*this);
    substract(&_timestamp, p_amount);
    return _timestamp;
  }

private:
  template <typename t_amount> void add(timestamp *p_ts, t_amount p_amount) {
    days _days_1(p_amount);
    auto _int = _days_1.integer();
    auto _dec = _days_1.decimal();

    seconds _secs_in_day((days(_dec)));

    seconds _secs(p_ts->m_secs + _secs_in_day);

    if (days(_secs) > days(1)) {
      p_ts->m_days += days(_int + 1);
      p_ts->m_secs = _secs - seconds(days(1));
    } else {
      p_ts->m_days += days(_int);
      p_ts->m_secs += _secs_in_day.integer();
    }
  }

  template <typename t_amount>
  void substract(timestamp *p_ts, t_amount p_amount) {
    days _days_1(p_amount);
    auto _int = _days_1.integer();
    auto _dec = _days_1.decimal();

    seconds _secs_in_day((days(_dec)));

    seconds _secs(p_ts->m_secs - _secs_in_day);

    if (days(_secs) < 0) {
      p_ts->m_days -= days(_int + 1);
      p_ts->m_secs = seconds(days(1)) + _secs;
    } else {
      p_ts->m_days -= days(_int);
      p_ts->m_secs += _secs_in_day.integer();
    }
  }

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
