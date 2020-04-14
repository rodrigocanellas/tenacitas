#ifndef TENACITAS_CALENDAR_GREGORIAN_ENDING_AT_H
#define TENACITAS_CALENDAR_GREGORIAN_ENDING_AT_H

#include <ctime>

namespace tenacitas {
namespace calendar {
namespace greogorian {

template<typename t_timestamp>
struct end_on_t
{
  end_on_t(t_timestamp p_when)
    : m_when(p_when)
  {}

  bool stop(const t_timestamp & p_time)
  {
    return (p_time > m_when ? true : false);
  }

  t_timestamp m_when;
};

} // namespace gregorian
} // namespace periodicity
} // namespace tenacitas
#endif // ENDING_AT_H
