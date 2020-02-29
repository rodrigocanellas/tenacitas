#ifndef TENACITAS_CALENDAR_ENT_ENDING_AT_H
#define TENACITAS_CALENDAR_ENT_ENDING_AT_H

#include <ctime>

#include <calendar/ent/timestamp.h>

namespace tenacitas {
namespace calendar {
namespace ent {

template<typename t_time_precision>
struct end_on
{
  end_on(timestamp_t<t_time_precision> p_when)
    : m_when(p_when)
  {}

  bool stop(timestamp_t<t_time_precision> p_time)
  {
    return (p_time > m_when ? true : false);
  }

  timestamp_t<t_time_precision> m_when;
};

} // namespace entities
} // namespace periodicity
} // namespace tenacitas
#endif // ENDING_AT_H
