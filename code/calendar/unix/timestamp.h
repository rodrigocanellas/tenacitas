#ifndef TENACITAS_CALENDAR_UNIX_TIMESTAMP_H
#define TENACITAS_CALENDAR_UNIX_TIMESTAMP_H

#include <cstdint>
#include <ctime>
#include <iomanip>
#include <iostream>

#include <calendar/ent/amount.h>
#include <calendar/ent/day.h>
#include <calendar/ent/hour.h>
#include <calendar/ent/minute.h>
#include <calendar/ent/month.h>
#include <calendar/ent/second.h>
#include <calendar/ent/weekday.h>
#include <calendar/ent/year.h>

namespace tenacitas {
namespace calendar {
namespace unix {

using namespace tenacitas::calendar::ent;

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
    : m_time(time(nullptr))
  {
  }

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
    : m_time(p_timestamp.m_time)
  {
  }

  ///
  /// \brief timestamp move constructor
  /// \param p_timestamp timestamp to be moved
  ///
  inline timestamp(timestamp&& p_timestamp)
    : m_time(std::move(p_timestamp.m_time))
  {
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
    struct tm* _tm = localtime(&m_time);
    _tm->tm_mday = amount<day>(p_day).get<decltype(_tm->tm_mday)>();
    m_time = mktime(_tm);
  }

  inline void set_day_month(day p_day, month p_month) {
    struct tm* _tm = localtime(&m_time);
    _tm->tm_mday = amount<day>(p_day).get<decltype(_tm->tm_mday)>();
    _tm->tm_mday = amount<month>(p_month).get<decltype(_tm->tm_mon)>();
    m_time = mktime(_tm);

  }

  ///
  /// \brief get_second
  /// \return
  ///
  inline second get_second() const {
    struct tm* _tm = localtime(&m_time);
    return second::create(_tm->tm_sec);
  }

  ///
  /// \brief get_minute
  /// \return
  ///
  inline minute get_minute() const {
    struct tm* _tm = localtime(&m_time);
    return minute::create(_tm->tm_min);
  }

  ///
  /// \brief get_hour
  /// \return
  ///
  inline hour get_hour() const {
    struct tm* _tm = localtime(&m_time);
    return hour::create(_tm->tm_hour);
  }

  ///
  /// \brief get_day
  /// \return
  ///
  inline day get_day() const {
    struct tm* _tm = localtime(&m_time);
    return day::create(_tm->tm_mday);
  }

  ///
  /// \brief get_weekday
  /// \return
  ///
  inline weekday get_weekday() const {
    struct tm* _tm = localtime(&m_time);
    return weekday::create(_tm->tm_wday);
  }

  ///
  /// \brief get_month
  /// \return
  ///
  inline month get_month() const {
    struct tm* _tm = localtime(&m_time);
    return month::create(_tm->tm_mon);
  }

  ///
  /// \brief get_year
  /// \return
  ///
  inline year get_year() const {
    struct tm* _tm = localtime(&m_time);
    return year(_tm->tm_year + 1900);
  }

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
  //  struct timespec m_timespec;
  time_t m_time = -1;
  //  struct tm m_tm;
};

} // namespace ent
} // namespace calendar
} // namespace tenacitas

#endif // timestamp_SECOND_H
