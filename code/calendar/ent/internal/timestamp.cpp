#include <calendar/ent/timestamp.h>

namespace tenacitas {
namespace calendar {
namespace ent {

timestamp::timestamp(year p_year,
                                 month p_month,
                                 day p_day,
                                 hour p_hour,
                                 minute p_minute,
                                 second p_second)
{
  struct tm* _tm = localtime(&m_time);

  _tm->tm_year =
      (amount<year>(p_year) - amount<year>(1900)).get<decltype(_tm->tm_year)>();

  _tm->tm_mon = amount<month>(p_month).get<decltype(_tm->tm_mon)>();
  _tm->tm_mday = amount<day>(p_day).get<decltype(_tm->tm_mday)>();
  _tm->tm_hour = amount<hour>(p_hour).get<decltype(_tm->tm_hour)>();
  _tm->tm_min = amount<minute>(p_minute).get<decltype(_tm->tm_min)>();
  _tm->tm_sec = amount<second>(p_second).get<decltype(_tm->tm_sec)>();
  m_time = mktime(_tm);
  if (m_time == -1) {
    throw std::runtime_error(
          std::to_string(_tm->tm_mday) + "/" + std::to_string(_tm->tm_mon + 1) +
          "/" + std::to_string(_tm->tm_year) + " is not a valid date");
  }
}

timestamp&
timestamp::operator=(const timestamp& p_timestamp_t)
{
  if (this != &p_timestamp_t) {
    m_time = p_timestamp_t.m_time;
  }
  return *this;
}

timestamp&
timestamp::operator=(timestamp&& p_timestamp_t)
{
  if (this != &p_timestamp_t) {
    m_time = std::move(p_timestamp_t.m_time);
  }
  return *this;
}

timestamp&
timestamp::operator+=(amount<second> p_seconds)
{
  m_time += p_seconds.get<decltype(m_time)>();
  return *this;
}

timestamp
timestamp::operator+(amount<second> p_seconds)
{
  timestamp _new(*this);
  _new += p_seconds;
  return _new;
}

timestamp&
timestamp::operator-=(amount<second> p_seconds)
{
  m_time -= p_seconds.get<decltype(m_time)>();
  return *this;
}

timestamp
timestamp::operator-(amount<second> p_seconds)
{
  timestamp _new(*this);
  _new -= p_seconds;
  return _new;
}

timestamp&
timestamp::operator+=(amount<minute> p_minutes)
{
  m_time += (minute::seconds() * p_minutes.get<amount<second>>()).get<decltype(m_time)>();
  return *this;
}

timestamp
timestamp::operator+(amount<minute> p_minutes)
{
  timestamp _new(*this);
  _new += p_minutes;
  return _new;
}

timestamp&
timestamp::operator-=(amount<minute> p_minutes)
{
  m_time -= (minute::seconds() * p_minutes.get<amount<second>>()).get<decltype(m_time)>();
  return *this;
}

timestamp
timestamp::operator-(amount<minute> p_minutes)
{
  timestamp _new(*this);
  _new -= p_minutes;
  return _new;
}

timestamp&
timestamp::operator+=(amount<day> p_days)
{
  m_time += (day::seconds() * p_days.get<amount<second>>()).get<decltype(m_time)>();
  return *this;
}

timestamp
timestamp::operator+(amount<day> p_days)
{
  timestamp _new(*this);
  _new += p_days;
  return _new;
}

timestamp&
timestamp::operator-=(amount<day> p_days)
{
  m_time -= (day::seconds() * p_days.get<amount<second>>()).get<decltype(m_time)>();
  return *this;
}

timestamp
timestamp::operator-(amount<day> p_days)
{
  timestamp _new(*this);
  _new -= p_days;
  return _new;
}

timestamp&
timestamp::operator+=(amount<weekday> p_weeks)
{
  m_time += (weekday::seconds() * p_weeks.get<amount<second>>()).get<decltype(m_time)>();
  return *this;
}

timestamp
timestamp::operator+(amount<weekday> p_weeks)
{
  timestamp _new(*this);
  _new += p_weeks;
  return _new;
}

timestamp&
timestamp::operator-=(amount<weekday> p_weeks)
{
  m_time -= (weekday::seconds() * p_weeks.get<amount<second>>()).get<decltype(m_time)>();
  return *this;
}

timestamp
timestamp::operator-(amount<weekday> p_weeks)
{
  timestamp _new(*this);
  _new -= p_weeks;
  return _new;
}

timestamp&
timestamp::operator+=(amount<month> p_months)
{
  struct tm* _tm = localtime(&m_time);

  _tm->tm_mon += p_months.get<decltype(_tm->tm_mon)>() % 12;
  _tm->tm_year += p_months.get<decltype(_tm->tm_year)>() / 12;
  m_time = mktime(_tm);
  return *this;
}

timestamp
timestamp::operator+(amount<month> p_months)
{
  timestamp _new(*this);
  _new += p_months;
  return _new;
}

timestamp&
timestamp::operator-=(amount<month> p_months)
{
  struct tm* _tm = localtime(&m_time);
  _tm->tm_mon -= p_months.get<decltype(_tm->tm_mon)>() % 12;
  _tm->tm_year -= p_months.get<decltype(_tm->tm_year)>() / 12;
  m_time = mktime(_tm);
  return *this;
}

timestamp
timestamp::operator-(amount<month> p_months)
{
  timestamp _new(*this);
  _new -= p_months;
  return _new;
}

timestamp
timestamp::operator+=(amount<year> p_years)
{
  struct tm* _tm = localtime(&m_time);
  _tm->tm_year += p_years.get<decltype(_tm->tm_year)>();
  m_time = mktime(_tm);
  return *this;
}

timestamp
timestamp::operator+(amount<year> p_years)
{
  timestamp _new(*this);
  _new += p_years;
  return _new;
}

timestamp
timestamp::operator-=(amount<year> p_years)
{
  struct tm* _tm = localtime(&m_time);
  _tm->tm_year -= p_years.get<decltype(_tm->tm_year)>();
  m_time = mktime(_tm);
  return *this;
}

timestamp
timestamp::operator-(amount<year> p_years)
{
  timestamp _new(*this);
  _new -= p_years;
  return _new;
}

} // namespace ent
} // namespace calendar
} // namespace tenacitas
