#include <calendar/unix/timestamp.h>

namespace tenacitas {
namespace calendar {
namespace unix {

timestamp::timestamp(_ent::year p_year, _ent::month p_month, _ent::day p_day,
                     _ent::hour p_hour, _ent::minute p_minute,
                     _ent::second p_second) {
  struct tm _tm;

  _tm.tm_year =
      (_ent::years(p_year) - _ent::years(1900)).get<decltype(_tm.tm_year)>();

  _tm.tm_mon = _ent::months(p_month).get<decltype(_tm.tm_mon)>();
  _tm.tm_mday = _ent::days(p_day).get<decltype(_tm.tm_mday)>();
  _tm.tm_hour = _ent::hours(p_hour).get<decltype(_tm.tm_hour)>();
  _tm.tm_min = _ent::minutes(p_minute).get<decltype(_tm.tm_min)>();
  _tm.tm_sec = _ent::seconds(p_second).get<decltype(_tm.tm_sec)>();
  time_t _time = mktime(&_tm);
  if (_time == -1) {
    throw std::runtime_error(
        std::to_string(_tm.tm_mday) + "/" + std::to_string(_tm.tm_mon + 1) +
        "/" + std::to_string(_tm.tm_year) + " is not a valid date");
  }
  m_value.tv_sec = _time;
  m_value.tv_nsec = 0;
}

timestamp &timestamp::operator=(const timestamp &p_timestamp) {
  if (this != &p_timestamp) {
    m_value.tv_sec = p_timestamp.m_value.tv_sec;
    m_value.tv_nsec = p_timestamp.m_value.tv_nsec;
  }
  return *this;
}

timestamp &timestamp::operator=(timestamp &&p_timestamp) {
  if (this != &p_timestamp) {
    m_value.tv_sec = std::move(p_timestamp.m_value.tv_sec);
    m_value.tv_nsec = std::move(p_timestamp.m_value.tv_nsec);
  }
  return *this;
}

timestamp timestamp::operator+(_ent::seconds p_seconds) {
  timestamp _new(*this);
  _new += p_seconds;
  return _new;
}

timestamp timestamp::operator-(_ent::seconds p_seconds) {
  timestamp _new(*this);
  _new -= p_seconds;
  return _new;
}

timestamp timestamp::operator+(_ent::minutes p_minutes) {
  timestamp _new(*this);
  _new += p_minutes;
  return _new;
}

timestamp timestamp::operator-(_ent::minutes p_minutes) {
  timestamp _new(*this);
  _new -= p_minutes;
  return _new;
}

timestamp timestamp::operator+(_ent::days p_days) {
  timestamp _new(*this);
  _new += p_days;
  return _new;
}

timestamp timestamp::operator-(_ent::days p_days) {
  timestamp _new(*this);
  _new -= p_days;
  return _new;
}

timestamp timestamp::operator+(_ent::weekdays p_weeks) {
  timestamp _new(*this);
  _new += p_weeks;
  return _new;
}

timestamp timestamp::operator-(_ent::weekdays p_weeks) {
  timestamp _new(*this);
  _new -= p_weeks;
  return _new;
}

timestamp &timestamp::operator+=(_ent::months p_months) {
  struct tm *_tm = gmtime(&(m_value.tv_sec));

  _tm->tm_mon += p_months.get<decltype(_tm->tm_mon)>() % 12;
  _tm->tm_year += p_months.get<decltype(_tm->tm_year)>() / 12;
  m_value.tv_sec = mktime(_tm);
  return *this;
}

timestamp timestamp::operator+(_ent::months p_months) {
  timestamp _new(*this);
  _new += p_months;
  return _new;
}

timestamp &timestamp::operator-=(_ent::months p_months) {
  struct tm *_tm = gmtime(&(m_value.tv_sec));
  _tm->tm_mon -= p_months.get<decltype(_tm->tm_mon)>() % 12;
  _tm->tm_year -= p_months.get<decltype(_tm->tm_year)>() / 12;
  m_value.tv_sec = mktime(_tm);
  return *this;
}

timestamp timestamp::operator-(_ent::months p_months) {
  timestamp _new(*this);
  _new -= p_months;
  return _new;
}

timestamp timestamp::operator+=(_ent::years p_years) {
  struct tm *_tm = gmtime(&(m_value.tv_sec));
  _tm->tm_year += p_years.get<decltype(_tm->tm_year)>();
  m_value.tv_sec = mktime(_tm);
  return *this;
}

timestamp timestamp::operator+(_ent::years p_years) {
  timestamp _new(*this);
  _new += p_years;
  return _new;
}

timestamp timestamp::operator-=(_ent::years p_years) {
  struct tm *_tm = gmtime(&(m_value.tv_sec));
  _tm->tm_year -= p_years.get<decltype(_tm->tm_year)>();
  m_value.tv_sec = mktime(_tm);
  return *this;
}

timestamp timestamp::operator-(_ent::years p_years) {
  timestamp _new(*this);
  _new -= p_years;
  return _new;
}

} // namespace unix
} // namespace calendar
} // namespace tenacitas
