#ifndef TENACITAS_CALENDAR_ENT_TIMESTAMP_UNIX_H
#define TENACITAS_CALENDAR_ENT_TIMESTAMP_UNIX_H

#include <cstring>
#include <cstdint>
#include <ctime>
#include <iomanip>
#include <iostream>

#include <calendar/unix/amounts.h>
#include <calendar/unix/times.h>

namespace tenacitas {
namespace calendar {
namespace ent {

///
/// \brief specialization of \p timestamp template class, with precision up to
/// \p second
///
struct timestamp_unix
{
  ///
  /// \brief timestamp_unix default constructor creates a timestamp_unix for now
  ///
  inline timestamp_unix()
  {
//    std::timespec_get(&m_value, TIME_UTC);
     if (clock_gettime(CLOCK_REALTIME, &m_value) == -1) {
       throw std::runtime_error("error getting time: " +
                                std::string(std::strerror(errno)));
     }
     std::cout << "TIME = " << std::asctime(gmtime(&(m_value.tv_sec))) << std::endl;
  }

  ///
  /// \brief timestamp_unix creates a timestamp_unix by informing day, month, year,
  /// hour, minute and second \param p_year \param p_month \param p_day \param
  /// p_hour \param p_minute \param p_second
  ///
  timestamp_unix(year p_year,
            month p_month,
            day p_day,
            hour p_hour = hour::h00,
            minute p_minute = minute::m00,
            second p_second = second::s00);
  ///
  /// \brief timestamp_unix copy constructor
  /// \param p_timestamp_unix timestamp_unix to be copied
  ///
  inline timestamp_unix(const timestamp_unix& p_timestamp_unix)
    : m_value(p_timestamp_unix.m_value)
  {}

  ///
  /// \brief timestamp_unix move constructor
  /// \param p_timestamp_unix timestamp_unix to be moved
  ///
  inline timestamp_unix(timestamp_unix&& p_timestamp_unix)
    : m_value(std::move(p_timestamp_unix.m_value))
  {}

  ///
  /// \brief operator = copy assignment
  /// \param p_timestamp_unix
  /// \return
  ///
  timestamp_unix& operator=(const timestamp_unix& p_timestamp_unix);

  ///
  /// \brief operator = move assignment
  /// \param p_timestamp_unix
  /// \return
  ///
  timestamp_unix& operator=(timestamp_unix&& p_timestamp_unix);

  ///
  /// \brief operator <<
  /// \param p_out
  /// \param p_ts
  /// \return
  ///
  inline friend std::ostream& operator<<(std::ostream& p_out,
                                         const timestamp_unix& p_ts)
  {
    p_out << "["

          << std::setw(2) << std::setfill('0') << p_ts.get_day() << "/"

          << std::setw(2) << std::setfill('0') << p_ts.get_month() << "/"

          << std::setw(4) << std::setfill('0') << p_ts.get_year() << ","

          << p_ts.get_weekday() << ","

          << std::setw(2) << std::setfill('0') << p_ts.get_hour() << ":"

          << std::setw(2) << std::setfill('0') << p_ts.get_minute() << ":"

          << std::setw(2) << std::setfill('0') << p_ts.get_second()

          << "]";

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
  /// \throw if the resulting \p timestamp_unix is invalid
  ///
  inline void set_day(day p_day)
  {
    struct tm* _tm = gmtime(&(m_value.tv_sec));
    _tm->tm_mday = days(p_day).get<decltype(_tm->tm_mday)>();
    m_value.tv_sec = mktime(_tm);
  }

  inline void set_day_month(day p_day, month p_month)
  {
    struct tm* _tm = gmtime(&(m_value.tv_sec));
    _tm->tm_mday = days(p_day).get<decltype(_tm->tm_mday)>();
    _tm->tm_mday = months(p_month).get<decltype(_tm->tm_mon)>();
    m_value.tv_sec = mktime(_tm);
  }


  ///
  /// \brief get_second
  /// \return
  ///
  inline second get_second() const
  {
    struct tm* _tm = gmtime(&(m_value.tv_sec));
    return second::create(_tm->tm_sec);
  }

  ///
  /// \brief get_minute
  /// \return
  ///
  inline minute get_minute() const
  {
    struct tm* _tm = gmtime(&(m_value.tv_sec));
    return minute::create(_tm->tm_min);
  }

  ///
  /// \brief get_hour
  /// \return
  ///
  inline hour get_hour() const
  {
    struct tm* _tm = gmtime(&(m_value.tv_sec));
    return hour::create(_tm->tm_hour);
  }

  ///
  /// \brief get_day
  /// \return
  ///
  inline day get_day() const
  {
    struct tm* _tm = gmtime(&(m_value.tv_sec));
    return day::create(_tm->tm_mday);
  }

  ///
  /// \brief get_weekday
  /// \return
  ///
  inline weekday get_weekday() const
  {
    struct tm* _tm = gmtime(&(m_value.tv_sec));
    return weekday::create(_tm->tm_wday);
  }

  ///
  /// \brief get_month
  /// \return
  ///
  inline month get_month() const
  {
    struct tm* _tm = gmtime(&(m_value.tv_sec));
    return month::create(_tm->tm_mon);
  }

  ///
  /// \brief get_year
  /// \return
  ///
  inline year get_year() const
  {
    struct tm* _tm = gmtime(&(m_value.tv_sec));
    return year(_tm->tm_year + 1900);
  }

  ///
  /// \brief operator ==
  /// \param p_ts
  /// \return
  ///
  inline bool operator==(const timestamp_unix& p_ts) const
  {
    return ( (m_value.tv_sec == p_ts.m_value.tv_sec) &&
             (m_value.tv_nsec == p_ts.m_value.tv_nsec) );
  }

  ///
  /// \brief operator !=
  /// \param p_ts
  /// \return
  ///
  bool operator!=(const timestamp_unix& p_ts) const
  {
    if (m_value.tv_sec != p_ts.m_value.tv_sec) {
      return true;
    }
    if (m_value.tv_nsec != p_ts.m_value.tv_nsec) {
      return true;
    }
    return false;
  }

  ///
  /// \brief operator >
  /// \param p_ts
  /// \return
  ///
  bool operator>(const timestamp_unix& p_ts) const
  {
    if (m_value.tv_sec > p_ts.m_value.tv_sec) {
      return true;
    }
    if (m_value.tv_sec < p_ts.m_value.tv_sec) {
      return false;
    }
    if (m_value.tv_nsec > p_ts.m_value.tv_nsec) {
      return true;
    }
    return false;
  }

  ///
  /// \brief operator <
  /// \param p_ts
  /// \return
  ///
  bool operator<(const timestamp_unix& p_ts) const
  {
    if (m_value.tv_sec < p_ts.m_value.tv_sec) {
      return true;
    }
    if (m_value.tv_sec > p_ts.m_value.tv_sec) {
      return false;
    }
    if (m_value.tv_nsec < p_ts.m_value.tv_nsec) {
      return true;
    }
    return false;
  }

  ///
  /// \brief operator >=
  /// \param p_ts
  /// \return
  ///
  bool operator>=(const timestamp_unix& p_ts) const
  {
    if (m_value.tv_sec < p_ts.m_value.tv_sec) {
      return false;
    }
    if (m_value.tv_sec > p_ts.m_value.tv_sec) {
      return true;
    }
    if (m_value.tv_nsec < p_ts.m_value.tv_nsec) {
      return false;
    }
    return true;
  }

  ///
  /// \brief operator <=
  /// \param p_ts
  /// \return
  ///
  inline bool operator<=(const timestamp_unix& p_ts) const
  {
    if (m_value.tv_sec > p_ts.m_value.tv_sec) {
      return false;
    }
    if (m_value.tv_sec < p_ts.m_value.tv_sec) {
      return true;
    }
    if (m_value.tv_nsec > p_ts.m_value.tv_nsec) {
      return false;
    }
    return true;
  }

  ///
  /// \brief operator +=
  /// \param p_seconds
  /// \return
  ///
  inline timestamp_unix& operator+=(seconds p_seconds)
  {
    m_value.tv_sec += p_seconds.get<decltype(m_value.tv_sec)>();
    return *this;
  }

  ///
  /// \brief operator +
  /// \param p_seconds
  /// \return
  ///
  timestamp_unix operator+(seconds p_seconds);

  ///
  /// \brief operator -=
  /// \param p_seconds
  /// \return
  ///
  timestamp_unix& operator-=(seconds p_seconds)
  {
    m_value.tv_sec -= p_seconds.get<decltype(m_value.tv_sec)>();
    return *this;
  }

  ///
  /// \brief operator -
  /// \param p_seconds
  /// \return
  ///
  timestamp_unix operator-(seconds p_seconds);

  ///
  /// \brief operator +=
  /// \param p_minutes
  /// \return
  ///
  inline timestamp_unix& operator+=(minutes p_minutes)
  {
    m_value.tv_sec += (minute::get_seconds() * p_minutes).get<decltype(m_value.tv_sec)>();
    return *this;
  }

  ///
  /// \brief operator +
  /// \param p_minutes
  /// \return
  ///
  timestamp_unix operator+(minutes p_minutes);

  ///
  /// \brief operator -=
  /// \param p_minutes
  /// \return
  ///
  inline timestamp_unix& operator-=(minutes p_minutes)
  {
    m_value.tv_sec -= (minute::get_seconds() * p_minutes).get<decltype(m_value.tv_sec)>();
    return *this;
  }

  ///
  /// \brief operator -
  /// \param p_minutes
  /// \return
  ///
  timestamp_unix operator-(minutes p_minutes);

  ///
  /// \brief operator +=
  /// \param p_days
  /// \return
  ///
  inline timestamp_unix& operator+=(days p_days)
  {
    m_value.tv_sec += (day::get_seconds() * p_days).get<decltype(m_value.tv_sec)>();
    return *this;
  }

  ///
  /// \brief operator +
  /// \param p_days
  /// \return
  ///
  timestamp_unix operator+(days p_days);

  ///
  /// \brief operator -=
  /// \param p_days
  /// \return
  ///
  inline timestamp_unix& operator-=(days p_days)
  {
    m_value.tv_sec -= (day::get_seconds() * p_days).get<decltype(m_value.tv_sec)>();
    return *this;
  }

  ///
  /// \brief operator -
  /// \param p_days
  /// \return
  ///
  timestamp_unix operator-(days p_days);

  ///
  /// \brief operator +=
  /// \param p_weeks
  /// \return
  ///
  inline timestamp_unix& operator+=(weekdays p_weeks)
  {
    m_value.tv_sec += (weekday::get_seconds() * p_weeks).get<decltype(m_value.tv_sec)>();
    return *this;
  }

  ///
  /// \brief operator +
  /// \param p_weeks
  /// \return
  ///
  timestamp_unix operator+(weekdays p_weeks);

  ///
  /// \brief operator -=
  /// \param p_weeks
  /// \return
  ///
  timestamp_unix& operator-=(weekdays p_weeks)
  {
    m_value.tv_sec -= (weekday::get_seconds() * p_weeks).get<decltype(m_value.tv_sec)>();
    return *this;
  }

  ///
  /// \brief operator -
  /// \param p_weeks
  /// \return
  ///
  timestamp_unix operator-(weekdays p_weeks);

  ///
  /// \brief operator +=
  /// \param p_months
  /// \return
  ///
  timestamp_unix& operator+=(months p_months);

  ///
  /// \brief operator +
  /// \param p_months
  /// \return
  ///
  timestamp_unix operator+(months p_months);

  ///
  /// \brief operator -=
  /// \param p_months
  /// \return
  ///
  timestamp_unix& operator-=(months p_months);

  ///
  /// \brief operator -
  /// \param p_months
  /// \return
  ///
  timestamp_unix operator-(months p_months);

  ///
  /// \brief operator +=
  /// \param p_years
  /// \return
  ///
  timestamp_unix operator+=(years p_years);

  ///
  /// \brief operator +
  /// \param p_years
  /// \return
  ///
  timestamp_unix operator+(years p_years);

  ///
  /// \brief operator -=
  /// \param p_years
  /// \return
  ///
  timestamp_unix operator-=(years p_years);

  ///
  /// \brief operator -
  /// \param p_years
  /// \return
  ///
  timestamp_unix operator-(years p_years);

private:
  struct timespec m_value;
//  time_t m_value = -1;
};

} // namespace unix
} // namespace calendar
} // namespace tenacitas

#endif // timestamp_SECOND_H
