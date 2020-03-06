#include <calendar/unix/timestamp.h>

namespace tenacitas {
namespace calendar {
namespace unix {

using namespace tenacitas::calendar::ent;

timestamp::timestamp(year p_year,
           month p_month,
           day p_day,
           hour p_hour,
           minute p_minute,
           second p_second)
{
  struct tm _tm;

  _tm.tm_year =
      (years(p_year) - years(1900)).get<decltype(_tm.tm_year)>();

  _tm.tm_mon = months(p_month).get<decltype(_tm.tm_mon)>();
  _tm.tm_mday = days(p_day).get<decltype(_tm.tm_mday)>();
  _tm.tm_hour = hours(p_hour).get<decltype(_tm.tm_hour)>();
  _tm.tm_min = minutes(p_minute).get<decltype(_tm.tm_min)>();
  _tm.tm_sec = seconds(p_second).get<decltype(_tm.tm_sec)>();
  m_time = mktime(&_tm);
  if (m_time == -1) {
    throw std::runtime_error(
          std::to_string(_tm.tm_mday) + "/" + std::to_string(_tm.tm_mon + 1) +
          "/" + std::to_string(_tm.tm_year) + " is not a valid date");
  }
}

timestamp&
timestamp::operator=(const timestamp& p_timestamp)
{
  if (this != &p_timestamp) {
    m_time = p_timestamp.m_time;
  }
  return *this;
}

timestamp&
timestamp::operator=(timestamp&& p_timestamp)
{
  if (this != &p_timestamp) {
    m_time = std::move(p_timestamp.m_time);
  }
  return *this;
}

timestamp&
timestamp::operator+=(seconds p_seconds)
{
  m_time += p_seconds.get<decltype(m_time)>();
  return *this;
}

timestamp
timestamp::operator+(seconds p_seconds)
{
  timestamp _new(*this);
  _new += p_seconds;
  return _new;
}

timestamp&
timestamp::operator-=(seconds p_seconds)
{
  m_time -= p_seconds.get<decltype(m_time)>();
  return *this;
}

timestamp
timestamp::operator-(seconds p_seconds)
{
  timestamp _new(*this);
  _new -= p_seconds;
  return _new;
}

timestamp&
timestamp::operator+=(minutes p_minutes)
{
  m_time += (minute::get_seconds() * p_minutes.get<seconds>()).get<decltype(m_time)>();
  return *this;
}

timestamp
timestamp::operator+(minutes p_minutes)
{
  timestamp _new(*this);
  _new += p_minutes;
  return _new;
}

timestamp&
timestamp::operator-=(minutes p_minutes)
{
  m_time -= (minute::get_seconds() * p_minutes.get<seconds>()).get<decltype(m_time)>();
  return *this;
}

timestamp
timestamp::operator-(minutes p_minutes)
{
  timestamp _new(*this);
  _new -= p_minutes;
  return _new;
}

timestamp&
timestamp::operator+=(days p_days)
{
  m_time += (day::get_seconds() * p_days.get<seconds>()).get<decltype(m_time)>();
  return *this;
}

timestamp
timestamp::operator+(days p_days)
{
  timestamp _new(*this);
  _new += p_days;
  return _new;
}

timestamp&
timestamp::operator-=(days p_days)
{
  m_time -= (day::get_seconds() * p_days.get<seconds>()).get<decltype(m_time)>();
  return *this;
}

timestamp
timestamp::operator-(days p_days)
{
  timestamp _new(*this);
  _new -= p_days;
  return _new;
}

timestamp&
timestamp::operator+=(weekdays p_weeks)
{
  m_time += (weekday::get_seconds() * p_weeks.get<seconds>()).get<decltype(m_time)>();
  return *this;
}

timestamp
timestamp::operator+(weekdays p_weeks)
{
  timestamp _new(*this);
  _new += p_weeks;
  return _new;
}

timestamp&
timestamp::operator-=(weekdays p_weeks)
{
  m_time -= (weekday::get_seconds() * p_weeks.get<seconds>()).get<decltype(m_time)>();
  return *this;
}

timestamp
timestamp::operator-(weekdays p_weeks)
{
  timestamp _new(*this);
  _new -= p_weeks;
  return _new;
}

timestamp&
timestamp::operator+=(months p_months)
{
  struct tm* _tm = localtime(&m_time);

  _tm->tm_mon += p_months.get<decltype(_tm->tm_mon)>() % 12;
  _tm->tm_year += p_months.get<decltype(_tm->tm_year)>() / 12;
  m_time = mktime(_tm);
  return *this;
}

timestamp
timestamp::operator+(months p_months)
{
  timestamp _new(*this);
  _new += p_months;
  return _new;
}

timestamp&
timestamp::operator-=(months p_months)
{
  struct tm* _tm = localtime(&m_time);
  _tm->tm_mon -= p_months.get<decltype(_tm->tm_mon)>() % 12;
  _tm->tm_year -= p_months.get<decltype(_tm->tm_year)>() / 12;
  m_time = mktime(_tm);
  return *this;
}

timestamp
timestamp::operator-(months p_months)
{
  timestamp _new(*this);
  _new -= p_months;
  return _new;
}

timestamp
timestamp::operator+=(years p_years)
{
  struct tm* _tm = localtime(&m_time);
  _tm->tm_year += p_years.get<decltype(_tm->tm_year)>();
  m_time = mktime(_tm);
  return *this;
}

timestamp
timestamp::operator+(years p_years)
{
  timestamp _new(*this);
  _new += p_years;
  return _new;
}

timestamp
timestamp::operator-=(years p_years)
{
  struct tm* _tm = localtime(&m_time);
  _tm->tm_year -= p_years.get<decltype(_tm->tm_year)>();
  m_time = mktime(_tm);
  return *this;
}

timestamp
timestamp::operator-(years p_years)
{
  timestamp _new(*this);
  _new -= p_years;
  return _new;
}

} // namespace ent
} // namespace calendar
} // namespace tenacitas
