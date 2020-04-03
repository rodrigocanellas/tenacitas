#ifndef TENACITAS_CALENDAR_BUS_YEARLY_REPETITION_T_H
#define TENACITAS_CALENDAR_BUS_YEARLY_REPETITION_T_H

#include <calendar/_ent/day.h>
#include <calendar/_ent/month.h>
#include <calendar/_ent/year.h>
#include <calendar/_ent/years.h>

namespace tenacitas {
namespace calendar {
namespace _bus {


template<typename t_timestamp>
struct yearly_repetition_t
{
  yearly_repetition_t(_ent::day p_day, _ent::month p_month, uint16_t p_at_each = 1)
    : m_day(p_day)
    , m_month(p_month)
    , m_at_each(p_at_each)
  {}

  t_timestamp next(const t_timestamp &p_time, bool p_first = false)
  {
    t_timestamp _time (p_time);
    if (m_month > _time.get_month()) {
      _time.set_day_month(m_day, m_month);
    } else if (m_month == _time.get_month()) {
      if (m_day > _time.get_day()) {
        _time.set_day(m_day);
      } else if (m_day <= _time.get_day()) {
        // _day < _tm->tm_mday
        _time.set_day(m_day);
      }
    } else {
      _time.set_day_month(m_day, m_month);
    }

    if (!p_first) {
      _time += m_at_each;
    }
    return _time;
  }

private:
  _ent::day m_day;
  _ent::month m_month;
  _ent::years m_at_each;
};

} // namespace _bus
} // namespace calendar
} // namespace tenacitas

#endif // yearly_repetition_t_H
