#ifndef TENACITAS_CALENDAR_BUS_ENDING_NEVER_H
#define TENACITAS_CALENDAR_BUS_ENDING_NEVER_H

#include <cstdint>
#include <ctime>

#include <calendar/ent/unix.h>

namespace tenacitas {
namespace calendar {
namespace bus {

template<typename t_timestamp>
struct end_never_t
{
  end_never_t(uint32_t p_max_events = 40)
    : m_max_events(p_max_events)
  {}

  bool stop(const t_timestamp&)
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

} // namespace bus
} // namespace periodicity
} // namespace tenacitas
#endif // ENDING_NEVER_H
