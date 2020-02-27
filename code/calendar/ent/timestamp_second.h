#ifndef TENACITAS_CALENDAR_ENTITIES_TIMESTAMP_SECOND_H
#define TENACITAS_CALENDAR_ENTITIES_TIMESTAMP_SECOND_H

#include <cstdint>
#include <ctime>
#include <iostream>

#include <calendar/entities/amount.h>
#include <calendar/entities/day.h>
#include <calendar/entities/hour.h>
#include <calendar/entities/minute.h>
#include <calendar/entities/month.h>
#include <calendar/entities/second.h>
#include <calendar/entities/timestamp.h>
#include <calendar/entities/weekday.h>
#include <calendar/entities/year.h>

namespace tenacitas {
namespace calendar {
namespace entities {

template<>
struct timestamp<second>
{
  timestamp()
    : m_time(time(nullptr))
    , m_tm()
  {
    struct tm* _tm = localtime(&m_time);
    memcpy(&m_tm, _tm, sizeof(struct tm));
  }

  timestamp(year p_year,
            month p_month,
            day p_day,
            hour p_hour = hour::h00,
            minute p_minute = minute::m00,
            second p_second = second::s00)
  {
    m_tm.tm_year =
      (amount<year>(p_year) - amount<year>(1900)).get<decltype(m_tm.tm_year)>();

    m_tm.tm_mon = amount<month>(p_month).get<decltype(m_tm.tm_mon)>();
    m_tm.tm_mday = amount<day>(p_day).get<decltype(m_tm.tm_mday)>();
    m_tm.tm_hour = amount<hour>(p_hour).get<decltype(m_tm.tm_hour)>();
    m_tm.tm_min = amount<minute>(p_minute).get<decltype(m_tm.tm_min)>();
    m_tm.tm_sec = amount<second>(p_second).get<decltype(m_tm.tm_sec)>();
    m_time = mktime(&m_tm);
    if (m_time == -1) {
      throw std::runtime_error(
        std::to_string(m_tm.tm_mday) + "/" + std::to_string(m_tm.tm_mon + 1) +
        "/" + std::to_string(m_tm.tm_year) + " is not a valid date");
    }
  }

  timestamp(const timestamp& p_timestamp)
    : m_time(p_timestamp.m_time)
    , m_tm()
  {
    memcpy(&m_tm, &p_timestamp.m_tm, sizeof(struct tm));
  }

  timestamp(timestamp&& p_timestamp)
    : m_time(std::move(p_timestamp.m_time))
    , m_tm()
  {
    memcpy(&m_tm, &p_timestamp.m_tm, sizeof(struct tm));
  }

  timestamp& operator=(const timestamp& p_timestamp)
  {
    if (this != &p_timestamp) {
      m_time = p_timestamp.m_time;
      memcpy(&m_tm, &p_timestamp.m_tm, sizeof(struct tm));
    }
    return *this;
  }

  timestamp& operator=(timestamp&& p_timestamp)
  {
    if (this != &p_timestamp) {
      m_time = std::move(p_timestamp.m_time);
      memcpy(&m_tm, &p_timestamp.m_tm, sizeof(struct tm));
    }
    return *this;
  }

  inline friend std::ostream& operator<<(std::ostream& p_out,
                                         const timestamp& p_ts)
  {
    p_out << "[" << p_ts.get_day() << "/" << p_ts.get_month() << "/"
          << p_ts.get_year() << "," << p_ts.get_weekday() << ","
          << p_ts.get_hour() << ":" << p_ts.get_minute() << ":"
          << p_ts.get_second() << "]";
    return p_out;
  }

  inline second get_second() const { return second::create(m_tm.tm_sec); }
  inline minute get_minute() const { return minute::create(m_tm.tm_min); }
  inline hour get_hour() const { return hour::create(m_tm.tm_hour); }
  inline day get_day() const { return day::create(m_tm.tm_mday); }
  inline weekday get_weekday() const { return weekday::create(m_tm.tm_wday); }
  inline month get_month() const { return month::create(m_tm.tm_mon); }
  inline year get_year() const { return year(m_tm.tm_year + 1900); }

  inline bool operator==(const timestamp& p_ts) const
  {
    return m_time == p_ts.m_time;
  }

  inline bool operator!=(const timestamp& p_ts) const
  {
    return m_time != p_ts.m_time;
  }

  inline bool operator>(const timestamp& p_ts) const
  {
    return m_time > p_ts.m_time;
  }

  inline bool operator<(const timestamp& p_ts) const
  {
    return m_time < p_ts.m_time;
  }

  inline bool operator>=(const timestamp& p_ts) const
  {
    return m_time > p_ts.m_time;
  }

  inline bool operator<=(const timestamp& p_ts) const
  {
    return m_time < p_ts.m_time;
  }

  timestamp& operator+=(amount<second> p_seconds)
  {
    m_time += p_seconds.get<decltype(m_time)>();
    struct tm* _tm = localtime(&m_time);
    memcpy(&m_tm, _tm, sizeof(struct tm));
    return *this;
  }

  timestamp operator+(amount<second> p_seconds)
  {
    timestamp _new(*this);
    _new += p_seconds;
    return _new;
  }

  timestamp& operator-=(amount<second> p_seconds)
  {
    m_time -= p_seconds.get<decltype(m_time)>();
    struct tm* _tm = localtime(&m_time);
    memcpy(&m_tm, _tm, sizeof(struct tm));
    return *this;
  }

  timestamp operator-(amount<second> p_seconds)
  {
    timestamp _new(*this);
    _new -= p_seconds;
    return _new;
  }

  timestamp& operator+=(amount<minute> p_minutes)
  {
    m_time += (minute::seconds() * p_minutes).get<decltype(m_time)>();
    struct tm* _tm = localtime(&m_time);
    memcpy(&m_tm, _tm, sizeof(struct tm));
    return *this;
  }

  timestamp operator+(amount<minute> p_minutes)
  {
    timestamp _new(*this);
    _new += p_minutes;
    return _new;
  }

  timestamp& operator-=(amount<minute> p_minutes)
  {
    m_time -= (minute::seconds() * p_minutes).get<decltype(m_time)>();
    struct tm* _tm = localtime(&m_time);
    memcpy(&m_tm, _tm, sizeof(struct tm));
    return *this;
  }

  timestamp operator-(amount<minute> p_minutes)
  {
    timestamp _new(*this);
    _new -= p_minutes;
    return _new;
  }

  timestamp& operator+=(amount<day> p_days)
  {
    m_time += (day::seconds() * p_days).get<decltype(m_time)>();
    struct tm* _tm = localtime(&m_time);
    memcpy(&m_tm, _tm, sizeof(struct tm));
    return *this;
  }

  timestamp operator+(amount<day> p_days)
  {
    timestamp _new(*this);
    _new += p_days;
    return _new;
  }

  timestamp& operator-=(amount<day> p_days)
  {
    m_time -= (day::seconds() * p_days).get<decltype(m_time)>();
    struct tm* _tm = localtime(&m_time);
    memcpy(&m_tm, _tm, sizeof(struct tm));
    return *this;
  }

  timestamp operator-(amount<day> p_days)
  {
    timestamp _new(*this);
    _new -= p_days;
    return _new;
  }

  timestamp& operator+=(amount<weekday> p_weeks)
  {
    m_time += (weekday::seconds() * p_weeks).get<decltype(m_time)>();
    struct tm* _tm = localtime(&m_time);
    memcpy(&m_tm, _tm, sizeof(struct tm));
    return *this;
  }

  timestamp operator+(amount<weekday> p_weeks)
  {
    timestamp _new(*this);
    _new += p_weeks;
    return _new;
  }

  timestamp& operator-=(amount<weekday> p_weeks)
  {
    m_time -= (weekday::seconds() * p_weeks).get<decltype(m_time)>();
    struct tm* _tm = localtime(&m_time);
    memcpy(&m_tm, _tm, sizeof(struct tm));
    return *this;
  }

  timestamp operator-(amount<weekday> p_weeks)
  {
    timestamp _new(*this);
    _new -= p_weeks;
    return _new;
  }

  timestamp& operator+=(amount<month> p_months)
  {
    m_tm.tm_mon += p_months.get<decltype(m_tm.tm_mon)>() % 12;
    m_tm.tm_year += p_months.get<decltype(m_tm.tm_year)>() / 12;
    m_time = mktime(&m_tm);
    return *this;
  }

  timestamp operator+(amount<month> p_months)
  {
    timestamp _new(*this);
    _new += p_months;
    return _new;
  }

  timestamp& operator-=(amount<month> p_months)
  {
    m_tm.tm_mon -= p_months.get<decltype(m_tm.tm_mon)>() % 12;
    m_tm.tm_year -= p_months.get<decltype(m_tm.tm_year)>() / 12;
    m_time = mktime(&m_tm);
    return *this;
  }

  timestamp operator-(amount<month> p_months)
  {
    timestamp _new(*this);
    _new -= p_months;
    return _new;
  }

  timestamp operator+=(amount<year> p_years)
  {
    m_tm.tm_year += p_years.get<decltype(m_tm.tm_year)>();
    m_time = mktime(&m_tm);
    return *this;
  }

  timestamp operator+(amount<year> p_years)
  {
    timestamp _new(*this);
    _new += p_years;
    return _new;
  }

  timestamp operator-=(amount<year> p_years)
  {
    m_tm.tm_year -= p_years.get<decltype(m_tm.tm_year)>();
    m_time = mktime(&m_tm);
    return *this;
  }

  timestamp operator-(amount<year> p_years)
  {
    timestamp _new(*this);
    _new -= p_years;
    return _new;
  }

private:
  time_t m_time = -1;
  struct tm m_tm;
};

} // namespace entities
} // namespace calendar
} // namespace tenacitas

#endif // TIMESTAMP_SECOND_H
