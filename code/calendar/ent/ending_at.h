#ifndef TENACITAS_PERIODICITY_ENTITIES_ENDING_AT_H
#define TENACITAS_PERIODICITY_ENTITIES_ENDING_AT_H

#include <periodicity/entities/ending.h>
#include <periodicity/entities/types_of_ending.h>

namespace tenacitas {
namespace periodicity {
namespace entities {

template<>
struct ending_t<types_of_ending::at>
{};

} // namespace entities
} // namespace periodicity
} // namespace tenacitas
#endif // ENDING_AT_H
