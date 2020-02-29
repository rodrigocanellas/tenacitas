#ifndef TENACITAS_CALENDAR_ENT_ENDING_AFTER_H
#define TENACITAS_CALENDAR_ENT_ENDING_AFTER_H

#include <cstdint>
#include <ctime>

#include <calendar/ent/timestamp.h>

namespace tenacitas {
namespace calendar {
namespace ent {

template<typename t_time_precision>
struct end_after
{
  end_after(uint16_t p_number)
    : m_number(p_number)
  {}

  bool stop(timestamp_t<t_time_precision>)
  {
    return (++m_counter > m_number ? true : false);
  }

  uint16_t m_number;
  uint16_t m_counter = 0;
};

} // namespace entities
} // namespace periodicity
} // namespace tenacitas
#endif // ENDING_AFTER_H
