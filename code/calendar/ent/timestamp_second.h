#ifndef TENACITAS_CALENDAR_ENTITIES_TIMESTAMP_SECOND_H
#define TENACITAS_CALENDAR_ENTITIES_TIMESTAMP_SECOND_H

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
#include <calendar/ent/timestamp.h>
#include <calendar/ent/weekday.h>
#include <calendar/ent/year.h>

namespace tenacitas {
namespace calendar {
namespace ent {

///
/// \brief specialization of \p timestamp_t template class, with precision up to
/// \p second
///
template<>
struct timestamp_t<second>
{
  ///
  /// \brief timestamp_t default constructor creates a timestamp for now
  ///
  inline timestamp_t()
    : m_time(time(nullptr))
    , m_tm()
  {
    struct tm* _tm = localtime(&m_time);
    memcpy(&m_tm, _tm, sizeof(struct tm));
  }

  ///
  /// \brief timestamp creates a timestamp_t by informing day, month, year,
  /// hour, minute and second \param p_year \param p_month \param p_day \param
  /// p_hour \param p_minute \param p_second
  ///
  timestamp_t(year p_year,
              month p_month,
              day p_day,
              hour p_hour = hour::h00,
              minute p_minute = minute::m00,
              second p_second = second::s00);
  ///
  /// \brief timestamp_t copy constructor
  /// \param p_timestamp_t timestamp_t to be copied
  ///
  inline timestamp_t(const timestamp_t& p_timestamp_t)
    : m_time(p_timestamp_t.m_time)
    , m_tm()
  {
    memcpy(&m_tm, &p_timestamp_t.m_tm, sizeof(struct tm));
  }

  ///
  /// \brief timestamp_t move constructor
  /// \param p_timestamp_t timestamp_t to be moved
  ///
  inline timestamp_t(timestamp_t&& p_timestamp_t)
    : m_time(std::move(p_timestamp_t.m_time))
    , m_tm()
  {
    memcpy(&m_tm, &p_timestamp_t.m_tm, sizeof(struct tm));
  }

  ///
  /// \brief operator = copy assignment
  /// \param p_timestamp_t
  /// \return
  ///
  timestamp_t& operator=(const timestamp_t& p_timestamp_t);

  ///
  /// \brief operator = move assignment
  /// \param p_timestamp_t
  /// \return
  ///
  timestamp_t& operator=(timestamp_t&& p_timestamp_t);

  ///
  /// \brief operator <<
  /// \param p_out
  /// \param p_ts
  /// \return
  ///
  inline friend std::ostream& operator<<(std::ostream& p_out,
                                         const timestamp_t& p_ts)
  {
    p_out << std::setw(2) << std::setfill('0') << p_ts.get_day() << "/"
          << std::setw(2) << std::setfill('0') << p_ts.get_month() << "/"
          << std::setw(4) << std::setfill('0') << p_ts.get_year() << ","
          << p_ts.get_weekday() << "," << std::setw(2) << std::setfill('0')
          << p_ts.get_hour() << ":" << std::setw(2) << std::setfill('0')
          << p_ts.get_minute() << ":" << std::setw(2) << std::setfill('0')
          << p_ts.get_second();

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
  inline bool operator==(const timestamp_t& p_ts) const
  {
    return m_time == p_ts.m_time;
  }

  ///
  /// \brief operator !=
  /// \param p_ts
  /// \return
  ///
  inline bool operator!=(const timestamp_t& p_ts) const
  {
    return m_time != p_ts.m_time;
  }

  ///
  /// \brief operator >
  /// \param p_ts
  /// \return
  ///
  inline bool operator>(const timestamp_t& p_ts) const
  {
    return m_time > p_ts.m_time;
  }

  ///
  /// \brief operator <
  /// \param p_ts
  /// \return
  ///
  inline bool operator<(const timestamp_t& p_ts) const
  {
    return m_time < p_ts.m_time;
  }

  ///
  /// \brief operator >=
  /// \param p_ts
  /// \return
  ///
  inline bool operator>=(const timestamp_t& p_ts) const
  {
    return m_time > p_ts.m_time;
  }

  ///
  /// \brief operator <=
  /// \param p_ts
  /// \return
  ///
  inline bool operator<=(const timestamp_t& p_ts) const
  {
    return m_time < p_ts.m_time;
  }

  ///
  /// \brief operator +=
  /// \param p_seconds
  /// \return
  ///
  timestamp_t& operator+=(amount<second> p_seconds);

  ///
  /// \brief operator +
  /// \param p_seconds
  /// \return
  ///
  timestamp_t operator+(amount<second> p_seconds);

  ///
  /// \brief operator -=
  /// \param p_seconds
  /// \return
  ///
  timestamp_t& operator-=(amount<second> p_seconds);

  ///
  /// \brief operator -
  /// \param p_seconds
  /// \return
  ///
  timestamp_t operator-(amount<second> p_seconds);

  ///
  /// \brief operator +=
  /// \param p_minutes
  /// \return
  ///
  timestamp_t& operator+=(amount<minute> p_minutes);

  ///
  /// \brief operator +
  /// \param p_minutes
  /// \return
  ///
  timestamp_t operator+(amount<minute> p_minutes);

  ///
  /// \brief operator -=
  /// \param p_minutes
  /// \return
  ///
  timestamp_t& operator-=(amount<minute> p_minutes);

  ///
  /// \brief operator -
  /// \param p_minutes
  /// \return
  ///
  timestamp_t operator-(amount<minute> p_minutes);

  ///
  /// \brief operator +=
  /// \param p_days
  /// \return
  ///
  timestamp_t& operator+=(amount<day> p_days);

  ///
  /// \brief operator +
  /// \param p_days
  /// \return
  ///
  timestamp_t operator+(amount<day> p_days);

  ///
  /// \brief operator -=
  /// \param p_days
  /// \return
  ///
  timestamp_t& operator-=(amount<day> p_days);

  ///
  /// \brief operator -
  /// \param p_days
  /// \return
  ///
  timestamp_t operator-(amount<day> p_days);

  ///
  /// \brief operator +=
  /// \param p_weeks
  /// \return
  ///
  timestamp_t& operator+=(amount<weekday> p_weeks);

  ///
  /// \brief operator +
  /// \param p_weeks
  /// \return
  ///
  timestamp_t operator+(amount<weekday> p_weeks);

  ///
  /// \brief operator -=
  /// \param p_weeks
  /// \return
  ///
  timestamp_t& operator-=(amount<weekday> p_weeks);

  ///
  /// \brief operator -
  /// \param p_weeks
  /// \return
  ///
  timestamp_t operator-(amount<weekday> p_weeks);

  ///
  /// \brief operator +=
  /// \param p_months
  /// \return
  ///
  timestamp_t& operator+=(amount<month> p_months);

  ///
  /// \brief operator +
  /// \param p_months
  /// \return
  ///
  timestamp_t operator+(amount<month> p_months);

  ///
  /// \brief operator -=
  /// \param p_months
  /// \return
  ///
  timestamp_t& operator-=(amount<month> p_months);

  ///
  /// \brief operator -
  /// \param p_months
  /// \return
  ///
  timestamp_t operator-(amount<month> p_months);

  ///
  /// \brief operator +=
  /// \param p_years
  /// \return
  ///
  timestamp_t operator+=(amount<year> p_years);

  ///
  /// \brief operator +
  /// \param p_years
  /// \return
  ///
  timestamp_t operator+(amount<year> p_years);

  ///
  /// \brief operator -=
  /// \param p_years
  /// \return
  ///
  timestamp_t operator-=(amount<year> p_years);

  ///
  /// \brief operator -
  /// \param p_years
  /// \return
  ///
  timestamp_t operator-(amount<year> p_years);

private:
  time_t m_time = -1;
  struct tm m_tm;
};

} // namespace ent
} // namespace calendar
} // namespace tenacitas

#endif // TIMESTAMP_SECOND_H
