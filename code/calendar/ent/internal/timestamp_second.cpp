#include <calendar/ent/timestamp_second.h>

namespace tenacitas {
namespace calendar {
namespace ent {

timestamp_t<second>::timestamp_t(year p_year,
                                 month p_month,
                                 day p_day,
                                 hour p_hour,
                                 minute p_minute,
                                 second p_second)
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

timestamp_t<second>&
timestamp_t<second>::operator=(const timestamp_t& p_timestamp_t)
{
  if (this != &p_timestamp_t) {
    m_time = p_timestamp_t.m_time;
    memcpy(&m_tm, &p_timestamp_t.m_tm, sizeof(struct tm));
  }
  return *this;
}

timestamp_t<second>&
timestamp_t<second>::operator=(timestamp_t&& p_timestamp_t)
{
  if (this != &p_timestamp_t) {
    m_time = std::move(p_timestamp_t.m_time);
    memcpy(&m_tm, &p_timestamp_t.m_tm, sizeof(struct tm));
  }
  return *this;
}

timestamp_t<second>&
timestamp_t<second>::operator+=(amount<second> p_seconds)
{
  m_time += p_seconds.get<decltype(m_time)>();
  struct tm* _tm = localtime(&m_time);
  memcpy(&m_tm, _tm, sizeof(struct tm));
  return *this;
}

timestamp_t<second>
timestamp_t<second>::operator+(amount<second> p_seconds)
{
  timestamp_t _new(*this);
  _new += p_seconds;
  return _new;
}

timestamp_t<second>&
timestamp_t<second>::operator-=(amount<second> p_seconds)
{
  m_time -= p_seconds.get<decltype(m_time)>();
  struct tm* _tm = localtime(&m_time);
  memcpy(&m_tm, _tm, sizeof(struct tm));
  return *this;
}

timestamp_t<second>
timestamp_t<second>::operator-(amount<second> p_seconds)
{
  timestamp_t _new(*this);
  _new -= p_seconds;
  return _new;
}

timestamp_t<second>&
timestamp_t<second>::operator+=(amount<minute> p_minutes)
{
  m_time += (minute::seconds() * p_minutes.get<amount<second>>()).get<decltype(m_time)>();
  struct tm* _tm = localtime(&m_time);
  memcpy(&m_tm, _tm, sizeof(struct tm));
  return *this;
}

timestamp_t<second>
timestamp_t<second>::operator+(amount<minute> p_minutes)
{
  timestamp_t _new(*this);
  _new += p_minutes;
  return _new;
}

timestamp_t<second>&
timestamp_t<second>::operator-=(amount<minute> p_minutes)
{
  m_time -= (minute::seconds() * p_minutes.get<amount<second>>()).get<decltype(m_time)>();
  struct tm* _tm = localtime(&m_time);
  memcpy(&m_tm, _tm, sizeof(struct tm));
  return *this;
}

timestamp_t<second>
timestamp_t<second>::operator-(amount<minute> p_minutes)
{
  timestamp_t _new(*this);
  _new -= p_minutes;
  return _new;
}

timestamp_t<second>&
timestamp_t<second>::operator+=(amount<day> p_days)
{
  m_time += (day::seconds() * p_days.get<amount<second>>()).get<decltype(m_time)>();
  struct tm* _tm = localtime(&m_time);
  memcpy(&m_tm, _tm, sizeof(struct tm));
  return *this;
}

timestamp_t<second>
timestamp_t<second>::operator+(amount<day> p_days)
{
  timestamp_t _new(*this);
  _new += p_days;
  return _new;
}

timestamp_t<second>&
timestamp_t<second>::operator-=(amount<day> p_days)
{
  m_time -= (day::seconds() * p_days.get<amount<second>>()).get<decltype(m_time)>();
  struct tm* _tm = localtime(&m_time);
  memcpy(&m_tm, _tm, sizeof(struct tm));
  return *this;
}

timestamp_t<second>
timestamp_t<second>::operator-(amount<day> p_days)
{
  timestamp_t _new(*this);
  _new -= p_days;
  return _new;
}

timestamp_t<second>&
timestamp_t<second>::operator+=(amount<weekday> p_weeks)
{
  m_time += (weekday::seconds() * p_weeks.get<amount<second>>()).get<decltype(m_time)>();
  struct tm* _tm = localtime(&m_time);
  memcpy(&m_tm, _tm, sizeof(struct tm));
  return *this;
}

timestamp_t<second>
timestamp_t<second>::operator+(amount<weekday> p_weeks)
{
  timestamp_t _new(*this);
  _new += p_weeks;
  return _new;
}

timestamp_t<second>&
timestamp_t<second>::operator-=(amount<weekday> p_weeks)
{
  m_time -= (weekday::seconds() * p_weeks.get<amount<second>>()).get<decltype(m_time)>();
  struct tm* _tm = localtime(&m_time);
  memcpy(&m_tm, _tm, sizeof(struct tm));
  return *this;
}

timestamp_t<second>
timestamp_t<second>::operator-(amount<weekday> p_weeks)
{
  timestamp_t _new(*this);
  _new -= p_weeks;
  return _new;
}

timestamp_t<second>&
timestamp_t<second>::operator+=(amount<month> p_months)
{
  m_tm.tm_mon += p_months.get<decltype(m_tm.tm_mon)>() % 12;
  m_tm.tm_year += p_months.get<decltype(m_tm.tm_year)>() / 12;
  m_time = mktime(&m_tm);
  return *this;
}

timestamp_t<second>
timestamp_t<second>::operator+(amount<month> p_months)
{
  timestamp_t _new(*this);
  _new += p_months;
  return _new;
}

timestamp_t<second>&
timestamp_t<second>::operator-=(amount<month> p_months)
{
  m_tm.tm_mon -= p_months.get<decltype(m_tm.tm_mon)>() % 12;
  m_tm.tm_year -= p_months.get<decltype(m_tm.tm_year)>() / 12;
  m_time = mktime(&m_tm);
  return *this;
}

timestamp_t<second>
timestamp_t<second>::operator-(amount<month> p_months)
{
  timestamp_t _new(*this);
  _new -= p_months;
  return _new;
}

timestamp_t<second>
timestamp_t<second>::operator+=(amount<year> p_years)
{
  m_tm.tm_year += p_years.get<decltype(m_tm.tm_year)>();
  m_time = mktime(&m_tm);
  return *this;
}

timestamp_t<second>
timestamp_t<second>::operator+(amount<year> p_years)
{
  timestamp_t _new(*this);
  _new += p_years;
  return _new;
}

timestamp_t<second>
timestamp_t<second>::operator-=(amount<year> p_years)
{
  m_tm.tm_year -= p_years.get<decltype(m_tm.tm_year)>();
  m_time = mktime(&m_tm);
  return *this;
}

timestamp_t<second>
timestamp_t<second>::operator-(amount<year> p_years)
{
  timestamp_t _new(*this);
  _new -= p_years;
  return _new;
}

} // namespace ent
} // namespace calendar
} // namespace tenacitas
