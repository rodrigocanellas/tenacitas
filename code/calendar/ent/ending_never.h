#ifndef TENACITAS_CALENDAR_ENT_ENDING_NEVER_H
#define TENACITAS_CALENDAR_ENT_ENDING_NEVER_H

#include <cstdint>
#include <ctime>

#include <calendar/ent/timestamp.h>

namespace tenacitas {
namespace calendar {
namespace ent {

template<typename t_time_precision>
struct end_never
{
  end_never(uint32_t p_max_events = 40)
    : m_max_events(p_max_events)
  {}

  bool stop(timestamp_t<t_time_precision>)
  {
    if (m_num == m_max_events) {
      return true;
    }
    ++m_num;
    return false;
  }

private:
  uint32_t m_num = 0;
  uint32_t m_max_events;
};

} // namespace entities
} // namespace periodicity
} // namespace tenacitas
#endif // ENDING_NEVER_H
