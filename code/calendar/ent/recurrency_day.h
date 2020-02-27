#ifndef TENACITAS_PERIODICITY_ENTITIES_RECURRENCY_DAY_H
#define TENACITAS_PERIODICITY_ENTITIES_RECURRENCY_DAY_H

#include <periodicity/entities/number_of_days.h>
#include <periodicity/entities/recurrency.h>
#include <periodicity/entities/types_of_repetition.h>

namespace tenacitas {
namespace periodicity {
namespace entities {

template<>
struct recurrency_t<types_of_repetition::day, types_of_ending::never>
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
