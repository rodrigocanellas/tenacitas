#ifndef TENACITAS_CALENDAR_ENT_RECURRENCY_DAY_H
#define TENACITAS_CALENDAR_ENT_RECURRENCY_DAY_H

#include <calendar/ent/number_of_days.h>
#include <calendar/ent/recurrency.h>
#include <calendar/ent/types_of_repetition.h>

namespace tenacitas {
namespace calendar {
namespace ent {

template<>
struct recurrency_t<types_of_repetition::day, ending_types::never>
{

  inline recurrency_t(number_of_days p_number_of_days)
    : m_number_of_days(p_number_of_days)
  {}

  inline number_of_days get_number_of_days() { return m_number_of_days; }

private:
  number_of_days m_number_of_days;
};

} // namespace entities
} // namespace periodicity
} // namespace tenacitas
#endif // RECURRENCY_DAY_H
