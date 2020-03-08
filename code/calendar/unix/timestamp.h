#ifndef TENACITAS_CALENDAR_UNIX_TIMESTAMP_H
#define TENACITAS_CALENDAR_UNIX_TIMESTAMP_H

#include <cstdint>
#include <ctime>
#include <iomanip>
#include <iostream>

#include <calendar/unix/amounts.h>
#include <calendar/unix/times.h>

namespace tenacitas {
namespace calendar {
namespace unix {

///
/// \brief specialization of \p timestamp template class, with precision up to
/// \p second
///
struct timestamp
{
  ///
  /// \brief timestamp default constructor creates a timestamp for now
  ///
  inline timestamp()
    : m_value(time(nullptr))
  {}

  ///
  /// \brief timestamp creates a timestamp by informing day, month, year,
  /// hour, minute and second \param p_year \param p_month \param p_day \param
  /// p_hour \param p_minute \param p_second
  ///
  timestamp(year p_year,
            month p_month,
            day p_day,
            hour p_hour = hour::h00,
            minute p_minute = minute::m00,
            second p_second = second::s00);
  ///
  /// \brief timestamp copy constructor
  /// \param p_timestamp timestamp to be copied
  ///
  inline timestamp(const timestamp& p_timestamp)
    : m_value(p_timestamp.m_value)
  {}

  ///
  /// \brief timestamp move constructor
  /// \param p_timestamp timestamp to be moved
  ///
  inline timestamp(timestamp&& p_timestamp)
    : m_value(std::move(p_timestamp.m_value))
  {}

  ///
  /// \brief operator = copy assignment
  /// \param p_timestamp
  /// \return
  ///
  timestamp& operator=(const timestamp& p_timestamp);

  ///
  /// \brief operator = move assignment
  /// \param p_timestamp
  /// \return
  ///
  timestamp& operator=(timestamp&& p_timestamp);

  ///
  /// \brief operator <<
  /// \param p_out
  /// \param p_ts
  /// \return
  ///
  inline friend std::ostream& operator<<(std::ostream& p_out,
                                         const timestamp& p_ts)
  {
    p_out << "[" << std::setw(2) << std::setfill('0') << p_ts.get_day() << "/"
          << std::setw(2) << std::setfill('0') << p_ts.get_month() << "/"
          << std::setw(4) << std::setfill('0') << p_ts.get_year() << ","
          << p_ts.get_weekday() << "," << std::setw(2) << std::setfill('0')
          << p_ts.get_hour() << ":" << std::setw(2) << std::setfill('0')
          << p_ts.get_minute() << ":" << std::setw(2) << std::setfill('0')
          << p_ts.get_second() << "]";

    //    p_out << "{ "

    //          << "\"day\" : \"" << std::setw(2) << std::setfill('0')
    //          << p_ts.get_day() << "\", "

    //          << "\"month\" : \"" << std::setw(2) << std::setfill('0')
    //          << p_ts.get_month() << "\", "

    //          << "\"year\" : \"" << std::setw(4) << std::setfill('0')
    //          << p_ts.get_year() << "\", "

    //          << "\"weekday\" : \"" << p_ts.get_weekday() << "\", "

    //          << "\"hour\" : \"" << std::setw(2) << std::setfill('0')
    //          << p_ts.get_hour() << "\", "

    //          << "\"minute\" : \"" << std::setw(2) << std::setfill('0')
    //          << p_ts.get_minute() << "\", "

    //          << "\"second\" : \"" << std::setw(2) << std::setfill('0')
    //          << p_ts.get_second() << "\" "

    //          << " }";

    return p_out;
  }

  ///
  /// \brief set_day
  /// \param p_day
  /// \throw if the resulting \p timestamp is invalid
  ///
  inline void set_day(day p_day)
  {
    struct tm* _tm = localtime(&m_value);
    _tm->tm_mday = days(p_day).get<decltype(_tm->tm_mday)>();
    m_value = mktime(_tm);
  }

  inline void set_day_month(day p_day, month p_month)
  {
    struct tm* _tm = localtime(&m_value);
    _tm->tm_mday = days(p_day).get<decltype(_tm->tm_mday)>();
    _tm->tm_mday = months(p_month).get<decltype(_tm->tm_mon)>();
    m_value = mktime(_tm);
  }

  ///
  /// \brief get_second
  /// \return
  ///
  inline second get_second() const
  {
    struct tm* _tm = localtime(&m_value);
    return second::create(_tm->tm_sec);
  }

  ///
  /// \brief get_minute
  /// \return
  ///
  inline minute get_minute() const
  {
    struct tm* _tm = localtime(&m_value);
    return minute::create(_tm->tm_min);
  }

  ///
  /// \brief get_hour
  /// \return
  ///
  inline hour get_hour() const
  {
    struct tm* _tm = localtime(&m_value);
    return hour::create(_tm->tm_hour);
  }

  ///
  /// \brief get_day
  /// \return
  ///
  inline day get_day() const
  {
    struct tm* _tm = localtime(&m_value);
    return day::create(_tm->tm_mday);
  }

  ///
  /// \brief get_weekday
  /// \return
  ///
  inline weekday get_weekday() const
  {
    struct tm* _tm = localtime(&m_value);
    return weekday::create(_tm->tm_wday);
  }

  ///
  /// \brief get_month
  /// \return
  ///
  inline month get_month() const
  {
    struct tm* _tm = localtime(&m_value);
    return month::create(_tm->tm_mon);
  }

  ///
  /// \brief get_year
  /// \return
  ///
  inline year get_year() const
  {
    struct tm* _tm = localtime(&m_value);
    return year(_tm->tm_year + 1900);
  }

  ///
  /// \brief operator ==
  /// \param p_ts
  /// \return
  ///
  inline bool operator==(const timestamp& p_ts) const
  {
    return m_value == p_ts.m_value;
  }

  ///
  /// \brief operator !=
  /// \param p_ts
  /// \return
  ///
  inline bool operator!=(const timestamp& p_ts) const
  {
    return m_value != p_ts.m_value;
  }

  ///
  /// \brief operator >
  /// \param p_ts
  /// \return
  ///
  inline bool operator>(const timestamp& p_ts) const
  {
    return m_value > p_ts.m_value;
  }

  ///
  /// \brief operator <
  /// \param p_ts
  /// \return
  ///
  inline bool operator<(const timestamp& p_ts) const
  {
    return m_value < p_ts.m_value;
  }

  ///
  /// \brief operator >=
  /// \param p_ts
  /// \return
  ///
  inline bool operator>=(const timestamp& p_ts) const
  {
    return m_value > p_ts.m_value;
  }

  ///
  /// \brief operator <=
  /// \param p_ts
  /// \return
  ///
  inline bool operator<=(const timestamp& p_ts) const
  {
    return m_value < p_ts.m_value;
  }

  ///
  /// \brief operator +=
  /// \param p_seconds
  /// \return
  ///
  inline timestamp& operator+=(seconds p_seconds)
  {
    m_value += p_seconds.get<decltype(m_value)>();
    return *this;
  }

  ///
  /// \brief operator +
  /// \param p_seconds
  /// \return
  ///
  timestamp operator+(seconds p_seconds);

  ///
  /// \brief operator -=
  /// \param p_seconds
  /// \return
  ///
  timestamp& operator-=(seconds p_seconds)
  {
    m_value -= p_seconds.get<decltype(m_value)>();
    return *this;
  }

  ///
  /// \brief operator -
  /// \param p_seconds
  /// \return
  ///
  timestamp operator-(seconds p_seconds);

  ///
  /// \brief operator +=
  /// \param p_minutes
  /// \return
  ///
  inline timestamp& operator+=(minutes p_minutes)
  {
    m_value += (minute::get_seconds() * p_minutes).get<decltype(m_value)>();
    return *this;
  }

  ///
  /// \brief operator +
  /// \param p_minutes
  /// \return
  ///
  timestamp operator+(minutes p_minutes);

  ///
  /// \brief operator -=
  /// \param p_minutes
  /// \return
  ///
  inline timestamp& operator-=(minutes p_minutes)
  {
    m_value -= (minute::get_seconds() * p_minutes).get<decltype(m_value)>();
    return *this;
  }

  ///
  /// \brief operator -
  /// \param p_minutes
  /// \return
  ///
  timestamp operator-(minutes p_minutes);

  ///
  /// \brief operator +=
  /// \param p_days
  /// \return
  ///
  inline timestamp& operator+=(days p_days)
  {
    m_value += (day::get_seconds() * p_days).get<decltype(m_value)>();
    return *this;
  }

  ///
  /// \brief operator +
  /// \param p_days
  /// \return
  ///
  timestamp operator+(days p_days);

  ///
  /// \brief operator -=
  /// \param p_days
  /// \return
  ///
  inline timestamp& operator-=(days p_days)
  {
    m_value -= (day::get_seconds() * p_days).get<decltype(m_value)>();
    return *this;
  }

  ///
  /// \brief operator -
  /// \param p_days
  /// \return
  ///
  timestamp operator-(days p_days);

  ///
  /// \brief operator +=
  /// \param p_weeks
  /// \return
  ///
  inline timestamp& operator+=(weekdays p_weeks)
  {
    m_value += (weekday::get_seconds() * p_weeks).get<decltype(m_value)>();
    return *this;
  }

  ///
  /// \brief operator +
  /// \param p_weeks
  /// \return
  ///
  timestamp operator+(weekdays p_weeks);

  ///
  /// \brief operator -=
  /// \param p_weeks
  /// \return
  ///
  timestamp& operator-=(weekdays p_weeks)
  {
    m_value -= (weekday::get_seconds() * p_weeks).get<decltype(m_value)>();
    return *this;
  }

  ///
  /// \brief operator -
  /// \param p_weeks
  /// \return
  ///
  timestamp operator-(weekdays p_weeks);

  ///
  /// \brief operator +=
  /// \param p_months
  /// \return
  ///
  timestamp& operator+=(months p_months);

  ///
  /// \brief operator +
  /// \param p_months
  /// \return
  ///
  timestamp operator+(months p_months);

  ///
  /// \brief operator -=
  /// \param p_months
  /// \return
  ///
  timestamp& operator-=(months p_months);

  ///
  /// \brief operator -
  /// \param p_months
  /// \return
  ///
  timestamp operator-(months p_months);

  ///
  /// \brief operator +=
  /// \param p_years
  /// \return
  ///
  timestamp operator+=(years p_years);

  ///
  /// \brief operator +
  /// \param p_years
  /// \return
  ///
  timestamp operator+(years p_years);

  ///
  /// \brief operator -=
  /// \param p_years
  /// \return
  ///
  timestamp operator-=(years p_years);

  ///
  /// \brief operator -
  /// \param p_years
  /// \return
  ///
  timestamp operator-(years p_years);

private:
  //  struct timespec m_timespec;
  time_t m_value = -1;
  //  struct tm m_tm;
};

} // namespace unix
} // namespace calendar
} // namespace tenacitas

#endif // timestamp_SECOND_H
