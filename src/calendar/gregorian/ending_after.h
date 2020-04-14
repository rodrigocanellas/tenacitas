#ifndef TENACITAS_CALENDAR_GREGORIAN_ENDING_AFTER_H
#define TENACITAS_CALENDAR_GREGORIAN_ENDING_AFTER_H

#include <cstdint>
#include <ctime>

namespace tenacitas {
namespace calendar {
namespace greogorian {

template<typename t_timestamp>
struct end_after_t
{
  end_after_t(uint16_t p_number)
    : m_number(p_number)
  {}

  bool stop(const t_timestamp &)
  {
    return (++m_counter > m_number ? true : false);
  }

  uint16_t m_number;
  uint16_t m_counter = 0;
};

} // namespace gregorian
} // namespace periodicity
} // namespace tenacitas
#endif // ENDING_AFTER_H
