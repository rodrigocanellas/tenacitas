#include <calendar/ent/timestamp_unix.h>

namespace tenacitas {
namespace calendar {
namespace ent {


timestamp_unix::timestamp_unix(year p_year,
                     month p_month,
                     day p_day,
                     hour p_hour,
                     minute p_minute,
                     second p_second)
{
  struct tm _tm;

  _tm.tm_year = (years(p_year) - years(1900)).get<decltype(_tm.tm_year)>();

  _tm.tm_mon = months(p_month).get<decltype(_tm.tm_mon)>();
  _tm.tm_mday = days(p_day).get<decltype(_tm.tm_mday)>();
  _tm.tm_hour = hours(p_hour).get<decltype(_tm.tm_hour)>();
  _tm.tm_min = minutes(p_minute).get<decltype(_tm.tm_min)>();
  _tm.tm_sec = seconds(p_second).get<decltype(_tm.tm_sec)>();
  time_t _time = mktime(&_tm);
  if (_time == -1) {
    throw std::runtime_error(
      std::to_string(_tm.tm_mday) + "/" + std::to_string(_tm.tm_mon + 1) + "/" +
      std::to_string(_tm.tm_year) + " is not a valid date");
  }
  m_value.tv_sec = _time;
  m_value.tv_nsec = 0;
}

timestamp_unix&
timestamp_unix::operator=(const timestamp_unix& p_timestamp_unix)
{
  if (this != &p_timestamp_unix) {
    m_value.tv_sec = p_timestamp_unix.m_value.tv_sec;
    m_value.tv_nsec = p_timestamp_unix.m_value.tv_nsec;
  }
  return *this;
}

timestamp_unix&
timestamp_unix::operator=(timestamp_unix&& p_timestamp_unix)
{
  if (this != &p_timestamp_unix) {
    m_value.tv_sec = std::move(p_timestamp_unix.m_value.tv_sec);
    m_value.tv_nsec = std::move(p_timestamp_unix.m_value.tv_nsec);
  }
  return *this;
}

timestamp_unix
timestamp_unix::operator+(seconds p_seconds)
{
  timestamp_unix _new(*this);
  _new += p_seconds;
  return _new;
}

timestamp_unix
timestamp_unix::operator-(seconds p_seconds)
{
  timestamp_unix _new(*this);
  _new -= p_seconds;
  return _new;
}

timestamp_unix
timestamp_unix::operator+(minutes p_minutes)
{
  timestamp_unix _new(*this);
  _new += p_minutes;
  return _new;
}

timestamp_unix
timestamp_unix::operator-(minutes p_minutes)
{
  timestamp_unix _new(*this);
  _new -= p_minutes;
  return _new;
}

timestamp_unix
timestamp_unix::operator+(days p_days)
{
  timestamp_unix _new(*this);
  _new += p_days;
  return _new;
}

timestamp_unix
timestamp_unix::operator-(days p_days)
{
  timestamp_unix _new(*this);
  _new -= p_days;
  return _new;
}

timestamp_unix
timestamp_unix::operator+(weekdays p_weeks)
{
  timestamp_unix _new(*this);
  _new += p_weeks;
  return _new;
}

timestamp_unix
timestamp_unix::operator-(weekdays p_weeks)
{
  timestamp_unix _new(*this);
  _new -= p_weeks;
  return _new;
}

timestamp_unix&
timestamp_unix::operator+=(months p_months)
{
  struct tm* _tm = gmtime(&(m_value.tv_sec));

  _tm->tm_mon += p_months.get<decltype(_tm->tm_mon)>() % 12;
  _tm->tm_year += p_months.get<decltype(_tm->tm_year)>() / 12;
  m_value.tv_sec = mktime(_tm);
  return *this;
}

timestamp_unix
timestamp_unix::operator+(months p_months)
{
  timestamp_unix _new(*this);
  _new += p_months;
  return _new;
}

timestamp_unix&
timestamp_unix::operator-=(months p_months)
{
  struct tm* _tm = gmtime(&(m_value.tv_sec));
  _tm->tm_mon -= p_months.get<decltype(_tm->tm_mon)>() % 12;
  _tm->tm_year -= p_months.get<decltype(_tm->tm_year)>() / 12;
  m_value.tv_sec = mktime(_tm);
  return *this;
}

timestamp_unix
timestamp_unix::operator-(months p_months)
{
  timestamp_unix _new(*this);
  _new -= p_months;
  return _new;
}

timestamp_unix
timestamp_unix::operator+=(years p_years)
{
  struct tm* _tm = gmtime(&(m_value.tv_sec));
  _tm->tm_year += p_years.get<decltype(_tm->tm_year)>();
  m_value.tv_sec = mktime(_tm);
  return *this;
}

timestamp_unix
timestamp_unix::operator+(years p_years)
{
  timestamp_unix _new(*this);
  _new += p_years;
  return _new;
}

timestamp_unix
timestamp_unix::operator-=(years p_years)
{
  struct tm* _tm = gmtime(&(m_value.tv_sec));
  _tm->tm_year -= p_years.get<decltype(_tm->tm_year)>();
  m_value.tv_sec = mktime(_tm);
  return *this;
}

timestamp_unix
timestamp_unix::operator-(years p_years)
{
  timestamp_unix _new(*this);
  _new -= p_years;
  return _new;
}

} // namespace ent
} // namespace calendar
} // namespace tenacitas
