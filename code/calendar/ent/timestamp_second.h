#ifndef TENACITAS_CALENDAR_ENTITIES_TIMESTAMP_SECOND_H
#define TENACITAS_CALENDAR_ENTITIES_TIMESTAMP_SECOND_H

#include <cstdint>
#include <ctime>
#include <iostream>

#include <calendar/ent/amount.h>
#include <calendar/ent/day.h>
#include <calendar/ent/hour.h>
#include <calendar/ent/minute.h>
#include <calendar/ent/month.h>
#include <calendar/ent/second.h>
#include <calendar/ent/timestamp.h>
#include <calendar/ent/weekday.h>
#include <calendar/ent/year.h>

namespace tenacitas {
namespace calendar {
namespace ent {

///
/// \brief specialization of \p timestamp template class, with precision up to
/// \p second
///
template<>
struct timestamp<second>
{
  ///
  /// \brief timestamp default constructor creates a timestamp for now
  ///
  inline timestamp()
    : m_time(time(nullptr))
    , m_tm()
  {
    struct tm* _tm = localtime(&m_time);
    memcpy(&m_tm, _tm, sizeof(struct tm));
  }

  ///
  /// \brief timestamp creates a timestamp by informing day, month, year, hour,
  /// minute and second \param p_year \param p_month \param p_day \param p_hour
  /// \param p_minute
  /// \param p_second
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
    : m_time(p_timestamp.m_time)
    , m_tm()
  {
    memcpy(&m_tm, &p_timestamp.m_tm, sizeof(struct tm));
  }

  ///
  /// \brief timestamp move constructor
  /// \param p_timestamp timestamp to be moved
  ///
  inline timestamp(timestamp&& p_timestamp)
    : m_time(std::move(p_timestamp.m_time))
    , m_tm()
  {
    memcpy(&m_tm, &p_timestamp.m_tm, sizeof(struct tm));
  }

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
    p_out << "[" << p_ts.get_day() << "/" << p_ts.get_month() << "/"
          << p_ts.get_year() << "," << p_ts.get_weekday() << ","
          << p_ts.get_hour() << ":" << p_ts.get_minute() << ":"
          << p_ts.get_second() << "]";
    return p_out;
  }

  ///
  /// \brief get_second
  /// \return
  ///
  inline second get_second() const { return second::create(m_tm.tm_sec); }

  ///
  /// \brief get_minute
  /// \return
  ///
  inline minute get_minute() const { return minute::create(m_tm.tm_min); }

  ///
  /// \brief get_hour
  /// \return
  ///
  inline hour get_hour() const { return hour::create(m_tm.tm_hour); }

  ///
  /// \brief get_day
  /// \return
  ///
  inline day get_day() const { return day::create(m_tm.tm_mday); }

  ///
  /// \brief get_weekday
  /// \return
  ///
  inline weekday get_weekday() const { return weekday::create(m_tm.tm_wday); }

  ///
  /// \brief get_month
  /// \return
  ///
  inline month get_month() const { return month::create(m_tm.tm_mon); }

  ///
  /// \brief get_year
  /// \return
  ///
  inline year get_year() const { return year(m_tm.tm_year + 1900); }

  ///
  /// \brief operator ==
  /// \param p_ts
  /// \return
  ///
  inline bool operator==(const timestamp& p_ts) const
  {
    return m_time == p_ts.m_time;
  }

  ///
  /// \brief operator !=
  /// \param p_ts
  /// \return
  ///
  inline bool operator!=(const timestamp& p_ts) const
  {
    return m_time != p_ts.m_time;
  }

  ///
  /// \brief operator >
  /// \param p_ts
  /// \return
  ///
  inline bool operator>(const timestamp& p_ts) const
  {
    return m_time > p_ts.m_time;
  }

  ///
  /// \brief operator <
  /// \param p_ts
  /// \return
  ///
  inline bool operator<(const timestamp& p_ts) const
  {
    return m_time < p_ts.m_time;
  }

  ///
  /// \brief operator >=
  /// \param p_ts
  /// \return
  ///
  inline bool operator>=(const timestamp& p_ts) const
  {
    return m_time > p_ts.m_time;
  }

  ///
  /// \brief operator <=
  /// \param p_ts
  /// \return
  ///
  inline bool operator<=(const timestamp& p_ts) const
  {
    return m_time < p_ts.m_time;
  }

  ///
  /// \brief operator +=
  /// \param p_seconds
  /// \return
  ///
  timestamp& operator+=(amount<second> p_seconds);

  ///
  /// \brief operator +
  /// \param p_seconds
  /// \return
  ///
  timestamp operator+(amount<second> p_seconds);

  ///
  /// \brief operator -=
  /// \param p_seconds
  /// \return
  ///
  timestamp& operator-=(amount<second> p_seconds);

  ///
  /// \brief operator -
  /// \param p_seconds
  /// \return
  ///
  timestamp operator-(amount<second> p_seconds);

  ///
  /// \brief operator +=
  /// \param p_minutes
  /// \return
  ///
  timestamp& operator+=(amount<minute> p_minutes);

  ///
  /// \brief operator +
  /// \param p_minutes
  /// \return
  ///
  timestamp operator+(amount<minute> p_minutes);

  ///
  /// \brief operator -=
  /// \param p_minutes
  /// \return
  ///
  timestamp& operator-=(amount<minute> p_minutes);

  ///
  /// \brief operator -
  /// \param p_minutes
  /// \return
  ///
  timestamp operator-(amount<minute> p_minutes);

  ///
  /// \brief operator +=
  /// \param p_days
  /// \return
  ///
  timestamp& operator+=(amount<day> p_days);

  ///
  /// \brief operator +
  /// \param p_days
  /// \return
  ///
  timestamp operator+(amount<day> p_days);

  ///
  /// \brief operator -=
  /// \param p_days
  /// \return
  ///
  timestamp& operator-=(amount<day> p_days);

  ///
  /// \brief operator -
  /// \param p_days
  /// \return
  ///
  timestamp operator-(amount<day> p_days);

  ///
  /// \brief operator +=
  /// \param p_weeks
  /// \return
  ///
  timestamp& operator+=(amount<weekday> p_weeks);

  ///
  /// \brief operator +
  /// \param p_weeks
  /// \return
  ///
  timestamp operator+(amount<weekday> p_weeks);

  ///
  /// \brief operator -=
  /// \param p_weeks
  /// \return
  ///
  timestamp& operator-=(amount<weekday> p_weeks);

  ///
  /// \brief operator -
  /// \param p_weeks
  /// \return
  ///
  timestamp operator-(amount<weekday> p_weeks);

  ///
  /// \brief operator +=
  /// \param p_months
  /// \return
  ///
  timestamp& operator+=(amount<month> p_months);

  ///
  /// \brief operator +
  /// \param p_months
  /// \return
  ///
  timestamp operator+(amount<month> p_months);

  ///
  /// \brief operator -=
  /// \param p_months
  /// \return
  ///
  timestamp& operator-=(amount<month> p_months);

  ///
  /// \brief operator -
  /// \param p_months
  /// \return
  ///
  timestamp operator-(amount<month> p_months);

  ///
  /// \brief operator +=
  /// \param p_years
  /// \return
  ///
  timestamp operator+=(amount<year> p_years);

  ///
  /// \brief operator +
  /// \param p_years
  /// \return
  ///
  timestamp operator+(amount<year> p_years);

  ///
  /// \brief operator -=
  /// \param p_years
  /// \return
  ///
  timestamp operator-=(amount<year> p_years);

  ///
  /// \brief operator -
  /// \param p_years
  /// \return
  ///
  timestamp operator-(amount<year> p_years);

private:
  time_t m_time = -1;
  struct tm m_tm;
};

} // namespace ent
} // namespace calendar
} // namespace tenacitas

#endif // TIMESTAMP_SECOND_H
