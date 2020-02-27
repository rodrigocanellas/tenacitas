#ifndef TENACITAS_PERIODICITY_ENTITIES_TYPES_OF_ENDING_H
#define TENACITAS_PERIODICITY_ENTITIES_TYPES_OF_ENDING_H

#include <cstdint>

namespace tenacitas {
namespace periodicity {
namespace entities {

enum class types_of_ending : uint8_t
{
  never = 1,
  at = 2,
  number = 3
};
} // namespace entities
} // namespace periodicity
} // namespace tenacitas
#endif // STOP
