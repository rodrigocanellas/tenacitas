#ifndef TENACITAS_CALENDAR_UNIX_TIMESTAMP_H
#define TENACITAS_CALENDAR_UNIX_TIMESTAMP_H

#include <cstdint>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>

#include <calendar/ent/day.h>
#include <calendar/ent/days.h>
#include <calendar/ent/hour.h>
#include <calendar/ent/hours.h>
#include <calendar/ent/minute.h>
#include <calendar/ent/minutes.h>
#include <calendar/ent/month.h>
#include <calendar/ent/months.h>
#include <calendar/ent/second.h>
#include <calendar/ent/seconds.h>
#include <calendar/ent/weekday.h>
#include <calendar/ent/weekdays.h>
#include <calendar/ent/year.h>
#include <calendar/ent/years.h>

namespace tenacitas {
namespace calendar {
namespace unix {

///
/// \brief specialization of \p timestamp template class, with precision up to
/// \p second
///
struct timestamp {
  ///
  /// \brief timestamp default constructor creates a timestamp for now
  ///
  inline timestamp() {
    //    std::timespec_get(&m_value, TIME_UTC);
    if (clock_gettime(CLOCK_REALTIME, &m_value) == -1) {
      throw std::runtime_error("error getting time: " +
                               std::string(std::strerror(errno)));
    }
    std::cout << "TIME = " << std::asctime(gmtime(&(m_value.tv_sec)))
              << std::endl;
  }

  ///
  /// \brief timestamp creates a timestamp by informing day, month, year,
  /// hour, minute and second \param p_year \param p_month \param p_day \param
  /// p_hour \param p_minute \param p_second
  ///
  timestamp(ent::year p_year, ent::month p_month, ent::day p_day,
            ent::hour p_hour = ent::hour::h00,
            ent::minute p_minute = ent::minute::m00,
            ent::second p_second = ent::second::s00);
  ///
  /// \brief timestamp copy constructor
  /// \param p_timestamp timestamp to be copied
  ///
  inline timestamp(const timestamp &p_timestamp)
      : m_value(p_timestamp.m_value) {}

  ///
  /// \brief timestamp move constructor
  /// \param p_timestamp timestamp to be moved
  ///
  inline timestamp(timestamp &&p_timestamp)
      : m_value(std::move(p_timestamp.m_value)) {}

  ///
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

  ///
  /// \brief operator <<
  /// \param p_out
  /// \param p_ts
  /// \return
  ///
  inline friend std::ostream &operator<<(std::ostream &p_out,
                                         const timestamp &p_ts) {
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
  /// \throw if the resulting \p timestamp is invalid
  ///
  inline void set_day(ent::day p_day) {
    struct tm *_tm = gmtime(&(m_value.tv_sec));
    _tm->tm_mday = ent::days(p_day).get<decltype(_tm->tm_mday)>();
    m_value.tv_sec = mktime(_tm);
  }

  inline void set_day_month(ent::day p_day, ent::month p_month) {
    struct tm *_tm = gmtime(&(m_value.tv_sec));
    _tm->tm_mday = ent::days(p_day).get<decltype(_tm->tm_mday)>();
    _tm->tm_mday = ent::months(p_month).get<decltype(_tm->tm_mon)>();
    m_value.tv_sec = mktime(_tm);
  }

  ///
  /// \brief get_second
  /// \return
  ///
  inline ent::second get_second() const {
    struct tm *_tm = gmtime(&(m_value.tv_sec));
    return ent::second::create(_tm->tm_sec);
  }

  ///
  /// \brief get_minute
  /// \return
  ///
  inline ent::minute get_minute() const {
    struct tm *_tm = gmtime(&(m_value.tv_sec));
    return ent::minute::create(_tm->tm_min);
  }

  ///
  /// \brief get_hour
  /// \return
  ///
  inline ent::hour get_hour() const {
    struct tm *_tm = gmtime(&(m_value.tv_sec));
    return ent::hour::create(_tm->tm_hour);
  }

  ///
  /// \brief get_day
  /// \return
  ///
  inline ent::day get_day() const {
    struct tm *_tm = gmtime(&(m_value.tv_sec));
    return ent::day::create(_tm->tm_mday);
  }

  ///
  /// \brief get_weekday
  /// \return
  ///
  inline ent::weekday get_weekday() const {
    struct tm *_tm = gmtime(&(m_value.tv_sec));
    return ent::weekday::create(_tm->tm_wday);
  }

  ///
  /// \brief get_month
  /// \return
  ///
  inline ent::month get_month() const {
    struct tm *_tm = gmtime(&(m_value.tv_sec));
    return ent::month::create(_tm->tm_mon);
  }

  ///
  /// \brief get_year
  /// \return
  ///
  inline ent::year get_year() const {
    struct tm *_tm = gmtime(&(m_value.tv_sec));
    return ent::year(_tm->tm_year + 1900);
  }

  ///
  /// \brief operator ==
  /// \param p_ts
  /// \return
  ///
  inline bool operator==(const timestamp &p_ts) const {
    return ((m_value.tv_sec == p_ts.m_value.tv_sec) &&
            (m_value.tv_nsec == p_ts.m_value.tv_nsec));
  }

  ///
  /// \brief operator !=
  /// \param p_ts
  /// \return
  ///
  bool operator!=(const timestamp &p_ts) const {
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
  bool operator>(const timestamp &p_ts) const {
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
  bool operator<(const timestamp &p_ts) const {
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
  bool operator>=(const timestamp &p_ts) const {
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
  inline bool operator<=(const timestamp &p_ts) const {
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
  inline timestamp &operator+=(ent::seconds p_seconds) {
    m_value.tv_sec += p_seconds.get<decltype(m_value.tv_sec)>();
    return *this;
  }

  ///
  /// \brief operator +
  /// \param p_seconds
  /// \return
  ///
  timestamp operator+(ent::seconds p_seconds);

  ///
  /// \brief operator -=
  /// \param p_seconds
  /// \return
  ///
  timestamp &operator-=(ent::seconds p_seconds) {
    m_value.tv_sec -= p_seconds.get<decltype(m_value.tv_sec)>();
    return *this;
  }

  ///
  /// \brief operator -
  /// \param p_seconds
  /// \return
  ///
  timestamp operator-(ent::seconds p_seconds);

  ///
  /// \brief operator +=
  /// \param p_minutes
  /// \return
  ///
  inline timestamp &operator+=(ent::minutes p_minutes) {
    m_value.tv_sec += (ent::minute::get_seconds() * p_minutes)
                          .get<decltype(m_value.tv_sec)>();
    return *this;
  }

  ///
  /// \brief operator +
  /// \param p_minutes
  /// \return
  ///
  timestamp operator+(ent::minutes p_minutes);

  ///
  /// \brief operator -=
  /// \param p_minutes
  /// \return
  ///
  inline timestamp &operator-=(ent::minutes p_minutes) {
    m_value.tv_sec -= (ent::minute::get_seconds() * p_minutes)
                          .get<decltype(m_value.tv_sec)>();
    return *this;
  }

  ///
  /// \brief operator -
  /// \param p_minutes
  /// \return
  ///
  timestamp operator-(ent::minutes p_minutes);

  ///
  /// \brief operator +=
  /// \param p_days
  /// \return
  ///
  inline timestamp &operator+=(ent::days p_days) {
    m_value.tv_sec +=
        (ent::day::get_seconds() * p_days).get<decltype(m_value.tv_sec)>();
    return *this;
  }

  ///
  /// \brief operator +
  /// \param p_days
  /// \return
  ///
  timestamp operator+(ent::days p_days);

  ///
  /// \brief operator -=
  /// \param p_days
  /// \return
  ///
  inline timestamp &operator-=(ent::days p_days) {
    m_value.tv_sec -=
        (ent::day::get_seconds() * p_days).get<decltype(m_value.tv_sec)>();
    return *this;
  }

  ///
  /// \brief operator -
  /// \param p_days
  /// \return
  ///
  timestamp operator-(ent::days p_days);

  ///
  /// \brief operator +=
  /// \param p_weeks
  /// \return
  ///
  inline timestamp &operator+=(ent::weekdays p_weeks) {
    m_value.tv_sec +=
        (ent::weekday::get_seconds() * p_weeks).get<decltype(m_value.tv_sec)>();
    return *this;
  }

  ///
  /// \brief operator +
  /// \param p_weeks
  /// \return
  ///
  timestamp operator+(ent::weekdays p_weeks);

  ///
  /// \brief operator -=
  /// \param p_weeks
  /// \return
  ///
  timestamp &operator-=(ent::weekdays p_weeks) {
    m_value.tv_sec -=
        (ent::weekday::get_seconds() * p_weeks).get<decltype(m_value.tv_sec)>();
    return *this;
  }

  ///
  /// \brief operator -
  /// \param p_weeks
  /// \return
  ///
  timestamp operator-(ent::weekdays p_weeks);

  ///
  /// \brief operator +=
  /// \param p_months
  /// \return
  ///
  timestamp &operator+=(ent::months p_months);

  ///
  /// \brief operator +
  /// \param p_months
  /// \return
  ///
  timestamp operator+(ent::months p_months);

  ///
  /// \brief operator -=
  /// \param p_months
  /// \return
  ///
  timestamp &operator-=(ent::months p_months);

  ///
  /// \brief operator -
  /// \param p_months
  /// \return
  ///
  timestamp operator-(ent::months p_months);

  ///
  /// \brief operator +=
  /// \param p_years
  /// \return
  ///
  timestamp operator+=(ent::years p_years);

  ///
  /// \brief operator +
  /// \param p_years
  /// \return
  ///
  timestamp operator+(ent::years p_years);

  ///
  /// \brief operator -=
  /// \param p_years
  /// \return
  ///
  timestamp operator-=(ent::years p_years);

  ///
  /// \brief operator -
  /// \param p_years
  /// \return
  ///
  timestamp operator-(ent::years p_years);

private:
  struct timespec m_value;
  //  time_t m_value = -1;
};

} // namespace unix
} // namespace calendar
} // namespace tenacitas

#endif // timestamp_SECOND_H
