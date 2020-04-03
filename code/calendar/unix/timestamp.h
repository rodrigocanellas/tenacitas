#ifndef TENACITAS_CALENDAR_UNIX_TIMESTAMP_H
#define TENACITAS_CALENDAR_UNIX_TIMESTAMP_H

#include <cstdint>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>

#include <calendar/_ent/day.h>
#include <calendar/_ent/days.h>
#include <calendar/_ent/hour.h>
#include <calendar/_ent/hours.h>
#include <calendar/_ent/minute.h>
#include <calendar/_ent/minutes.h>
#include <calendar/_ent/month.h>
#include <calendar/_ent/months.h>
#include <calendar/_ent/second.h>
#include <calendar/_ent/seconds.h>
#include <calendar/_ent/weekday.h>
#include <calendar/_ent/weekdays.h>
#include <calendar/_ent/year.h>
#include <calendar/_ent/years.h>

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
    //    if (std::timespec_get(&m_value, TIME_UTC) != 0) {
    //            throw std::runtime_error("error getting time: " +
    //                                     std::string(std::strerror(errno)));

    //    }
    if (clock_gettime(CLOCK_REALTIME, &m_value) == -1) {
      throw std::runtime_error("error getting time: " +
                               std::string(std::strerror(errno)));
    }
    //    std::cout << "TIME = " << std::asctime(gmtime(&(m_value.tv_sec)))
    //              << std::endl;
  }

  ///
  /// \brief timestamp creates a timestamp by informing day, month, year,
  /// hour, minute and second \param p_year \param p_month \param p_day \param
  /// p_hour \param p_minute \param p_second
  ///
  timestamp(_ent::year p_year, _ent::month p_month, _ent::day p_day,
            _ent::hour p_hour = _ent::hour::h00,
            _ent::minute p_minute = _ent::minute::m00,
            _ent::second p_second = _ent::second::s00);
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

  explicit timestamp(_ent::days p_days) : timestamp() {
    m_value.tv_sec = p_days.get<decltype(m_value.tv_sec)>() *
                     _ent::day::get_seconds().get<decltype(m_value.tv_sec)>();
  }

  operator _ent::days() const {
    return _ent::days(m_value.tv_sec /
                      _ent::day::get_seconds().get<decltype(m_value.tv_sec)>());
  }

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
  inline void set_day(_ent::day p_day) {
    struct tm *_tm = gmtime(&(m_value.tv_sec));
    _tm->tm_mday = _ent::days(p_day).get<decltype(_tm->tm_mday)>();
    m_value.tv_sec = mktime(_tm);
  }

  inline void set_day_month(_ent::day p_day, _ent::month p_month) {
    struct tm *_tm = gmtime(&(m_value.tv_sec));
    _tm->tm_mday = _ent::days(p_day).get<decltype(_tm->tm_mday)>();
    _tm->tm_mday = _ent::months(p_month).get<decltype(_tm->tm_mon)>();
    m_value.tv_sec = mktime(_tm);
  }

  ///
  /// \brief get_second
  /// \return
  ///
  inline _ent::second get_second() const {
    struct tm *_tm = gmtime(&(m_value.tv_sec));
    return _ent::second::create(_tm->tm_sec);
  }

  ///
  /// \brief get_minute
  /// \return
  ///
  inline _ent::minute get_minute() const {
    struct tm *_tm = gmtime(&(m_value.tv_sec));
    return _ent::minute::create(_tm->tm_min);
  }

  ///
  /// \brief get_hour
  /// \return
  ///
  inline _ent::hour get_hour() const {
    struct tm *_tm = gmtime(&(m_value.tv_sec));
    return _ent::hour::create(_tm->tm_hour);
  }

  ///
  /// \brief get_day
  /// \return
  ///
  inline _ent::day get_day() const {
    struct tm *_tm = gmtime(&(m_value.tv_sec));
    return _ent::day::create(_tm->tm_mday);
  }

  ///
  /// \brief get_weekday
  /// \return
  ///
  inline _ent::weekday get_weekday() const {
    struct tm *_tm = gmtime(&(m_value.tv_sec));
    return _ent::weekday::create(_tm->tm_wday);
  }

  ///
  /// \brief get_month
  /// \return
  ///
  inline _ent::month get_month() const {
    struct tm *_tm = gmtime(&(m_value.tv_sec));
    return _ent::month::create(_tm->tm_mon);
  }

  ///
  /// \brief get_year
  /// \return
  ///
  inline _ent::year get_year() const {
    struct tm *_tm = gmtime(&(m_value.tv_sec));
    return _ent::year(_tm->tm_year + 1900);
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
  inline timestamp &operator+=(_ent::seconds p_seconds) {
    m_value.tv_sec += p_seconds.get<decltype(m_value.tv_sec)>();
    return *this;
  }

  ///
  /// \brief operator +
  /// \param p_seconds
  /// \return
  ///
  timestamp operator+(_ent::seconds p_seconds);

  ///
  /// \brief operator -=
  /// \param p_seconds
  /// \return
  ///
  timestamp &operator-=(_ent::seconds p_seconds) {
    m_value.tv_sec -= p_seconds.get<decltype(m_value.tv_sec)>();
    return *this;
  }

  ///
  /// \brief operator -
  /// \param p_seconds
  /// \return
  ///
  timestamp operator-(_ent::seconds p_seconds);

  ///
  /// \brief operator +=
  /// \param p_minutes
  /// \return
  ///
  inline timestamp &operator+=(_ent::minutes p_minutes) {
    m_value.tv_sec += (_ent::minute::get_seconds() * p_minutes)
                          .get<decltype(m_value.tv_sec)>();
    return *this;
  }

  ///
  /// \brief operator +
  /// \param p_minutes
  /// \return
  ///
  timestamp operator+(_ent::minutes p_minutes);

  ///
  /// \brief operator -=
  /// \param p_minutes
  /// \return
  ///
  inline timestamp &operator-=(_ent::minutes p_minutes) {
    m_value.tv_sec -= (_ent::minute::get_seconds() * p_minutes)
                          .get<decltype(m_value.tv_sec)>();
    return *this;
  }

  ///
  /// \brief operator -
  /// \param p_minutes
  /// \return
  ///
  timestamp operator-(_ent::minutes p_minutes);

  ///
  /// \brief operator +=
  /// \param p_days
  /// \return
  ///
  inline timestamp &operator+=(_ent::days p_days) {
    m_value.tv_sec +=
        (_ent::day::get_seconds() * p_days).get<decltype(m_value.tv_sec)>();
    return *this;
  }

  ///
  /// \brief operator +
  /// \param p_days
  /// \return
  ///
  timestamp operator+(_ent::days p_days);

  ///
  /// \brief operator -=
  /// \param p_days
  /// \return
  ///
  inline timestamp &operator-=(_ent::days p_days) {
    m_value.tv_sec -=
        (_ent::day::get_seconds() * p_days).get<decltype(m_value.tv_sec)>();
    return *this;
  }

  ///
  /// \brief operator -
  /// \param p_days
  /// \return
  ///
  timestamp operator-(_ent::days p_days);

  ///
  /// \brief operator +=
  /// \param p_weeks
  /// \return
  ///
  inline timestamp &operator+=(_ent::weekdays p_weeks) {
    m_value.tv_sec += (_ent::weekday::get_seconds() * p_weeks)
                          .get<decltype(m_value.tv_sec)>();
    return *this;
  }

  ///
  /// \brief operator +
  /// \param p_weeks
  /// \return
  ///
  timestamp operator+(_ent::weekdays p_weeks);

  ///
  /// \brief operator -=
  /// \param p_weeks
  /// \return
  ///
  timestamp &operator-=(_ent::weekdays p_weeks) {
    m_value.tv_sec -= (_ent::weekday::get_seconds() * p_weeks)
                          .get<decltype(m_value.tv_sec)>();
    return *this;
  }

  ///
  /// \brief operator -
  /// \param p_weeks
  /// \return
  ///
  timestamp operator-(_ent::weekdays p_weeks);

  ///
  /// \brief operator +=
  /// \param p_months
  /// \return
  ///
  timestamp &operator+=(_ent::months p_months);

  ///
  /// \brief operator +
  /// \param p_months
  /// \return
  ///
  timestamp operator+(_ent::months p_months);

  ///
  /// \brief operator -=
  /// \param p_months
  /// \return
  ///
  timestamp &operator-=(_ent::months p_months);

  ///
  /// \brief operator -
  /// \param p_months
  /// \return
  ///
  timestamp operator-(_ent::months p_months);

  ///
  /// \brief operator +=
  /// \param p_years
  /// \return
  ///
  timestamp operator+=(_ent::years p_years);

  ///
  /// \brief operator +
  /// \param p_years
  /// \return
  ///
  timestamp operator+(_ent::years p_years);

  ///
  /// \brief operator -=
  /// \param p_years
  /// \return
  ///
  timestamp operator-=(_ent::years p_years);

  ///
  /// \brief operator -
  /// \param p_years
  /// \return
  ///
  timestamp operator-(_ent::years p_years);

private:
  struct timespec m_value;
  //  time_t m_value = -1;
};

} // namespace unix
} // namespace calendar
} // namespace tenacitas

#endif // timestamp_SECOND_H
