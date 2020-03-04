#ifndef TENACITAS_CALENDAR_ENT_ENDING_AT_H
#define TENACITAS_CALENDAR_ENT_ENDING_AT_H

#include <ctime>

#include <calendar/ent/timestamp.h>

namespace tenacitas {
namespace calendar {
namespace ent {


struct end_on
{
  end_on(ent::timestamp p_when)
    : m_when(p_when)
  {}

  bool stop(const ent::timestamp & p_time)
  {
    return (p_time > m_when ? true : false);
  }

  ent::timestamp m_when;
};

} // namespace entities
} // namespace periodicity
} // namespace tenacitas
#endif // ENDING_AT_H
