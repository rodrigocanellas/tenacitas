#ifndef TENACITAS_PERIODICITY_ENTITIES_REPETITION_H
#define TENACITAS_PERIODICITY_ENTITIES_REPETITION_H

#include <cstdint>

namespace tenacitas {
namespace periodicity {
namespace entities {

enum class types_of_repetition : uint8_t
{
  micro = 1,
  milli = 2,
  second = 3,
  minute = 4,
  hour = 5,
  day = 6,
  week = 7,
  month = 8,
  year = 9
};
} // namespace entities
} // namespace periodicity
} // namespace tenacitas
#endif // REPETITION_H
