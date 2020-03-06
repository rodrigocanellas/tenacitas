#ifndef TENACITAS_CALENDAR_BUS_YEARLY_REPETITION_T_H
#define TENACITAS_CALENDAR_BUS_YEARLY_REPETITION_T_H

#include <calendar/ent/day.h>
#include <calendar/ent/month.h>
#include <calendar/ent/year.h>
#include <calendar/ent/amount.h>

namespace tenacitas {
namespace calendar {
namespace bus {


template<typename t_timestamp>
struct yearly_repetition_t
{
  yearly_repetition_t(ent::day p_day, ent::month p_month, uint16_t p_at_each = 1)
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
  ent::day m_day;
  ent::month m_month;
  ent::amount<ent::year> m_at_each;
};

} // namespace bus
} // namespace calendar
} // namespace tenacitas

#endif // yearly_repetition_t_H
