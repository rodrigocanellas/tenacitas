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

#include <measures/time/amounts.h>
#include <measures/time/day.h>
#include <measures/time/hour.h>
#include <measures/time/minute.h>
#include <measures/time/month.h>
#include <measures/time/second.h>
#include <measures/time/weekday.h>
#include <measures/time/year.h>

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
  timestamp(measures::time::year p_year, measures::time::month p_month, measures::time::day p_day, measures::time::hour p_hour = measures::time::hour::_00,
            measures::time::minute p_minute = measures::time::minute::_00, measures::time::second p_second = measures::time::second::_00);
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
  measures::time::second get_second();

  ///
  /// \brief get_minute
  /// \return
  ///
  measures::time::minute get_minute();

  ///
  /// \brief get_hour
  /// \return
  ///
  measures::time::hour get_hour();

  ///
  /// \brief get_day
  /// \return
  ///
  measures::time::day get_day();

  ///
  /// \brief get_weekday
  /// \return
  ///
  measures::time::weekday get_weekday();

  ///
  /// \brief get_month
  /// \return
  ///
  measures::time::month get_month();

  ///
  /// \brief get_year
  /// \return
  ///
  measures::time::year get_year();

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
    measures::time::days _days_1(p_amount);
    auto _int = _days_1.integer();
    auto _dec = _days_1.decimal();

    measures::time::seconds _secs_in_day((measures::time::days(_dec)));

    measures::time::seconds _secs(p_ts->m_secs + _secs_in_day);

    if (measures::time::days(_secs) > measures::time::days(1)) {
      p_ts->m_days += measures::time::days(_int + 1);
      p_ts->m_secs = _secs - measures::time::seconds(measures::time::days(1));
    } else {
      p_ts->m_days += measures::time::days(_int);
      p_ts->m_secs += _secs_in_day.integer();
    }
  }

  template <typename t_amount>
  void substract(timestamp *p_ts, t_amount p_amount) {
    measures::time::days _days_1(p_amount);
    auto _int = _days_1.integer();
    auto _dec = _days_1.decimal();

    measures::time::seconds _secs_in_day((measures::time::days(_dec)));

    measures::time::seconds _secs(p_ts->m_secs - _secs_in_day);

    if ( measures::time::days(_secs) < measures::time::days(0)) {
      p_ts->m_days -= measures::time::days(_int + 1);
      p_ts->m_secs = measures::time::seconds(measures::time::days(1)) + _secs;
    } else {
      p_ts->m_days -= measures::time::days(_int);
      p_ts->m_secs -= _secs_in_day.integer();
    }
  }

   void dMyhms2days_secs(measures::time::year p_year, measures::time::month p_month, measures::time::day p_day,
                                  measures::time::hour p_hour, measures::time::minute p_minute,
                                  measures::time::second p_second);

   void days_secs2dMywhms();

private:
  /// \brief amount of days since epoch
  measures::time::days m_days = {0};

  /// \brief amount of seconds in the day
  measures::time::seconds m_secs = {0};

  /// \brief indicates that the parts of the timestamp - m_year, m_month, m_day,
  /// m_hour, m_minute, m_second, m_weekday - need to be updated
  bool m_needs_update = {true};

  measures::time::year m_year = measures::time::year(1970);
  measures::time::month m_month = measures::time::month::jan;
  measures::time::day m_day = measures::time::day::_01;
  measures::time::hour m_hour = measures::time::hour::_00;
  measures::time::minute m_minute = measures::time::minute::_00;
  measures::time::second m_second = measures::time::second::_00;
  measures::time::weekday m_weekday = measures::time::weekday::thu;
};

} // namespace gregorian
} // namespace calendar
} // namespace tenacitas

#endif
