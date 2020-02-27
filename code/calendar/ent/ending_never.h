#ifndef TENACITAS_PERIODICITY_ENTITIES_ENDING_NEVER_H
#define TENACITAS_PERIODICITY_ENTITIES_ENDING_NEVER_H

#include <periodicity/entities/ending.h>
#include <periodicity/entities/types_of_ending.h>

namespace tenacitas {
namespace periodicity {
namespace entities {

template<>
struct ending_t<types_of_ending::never>
{};

} // namespace entities
} // namespace periodicity
} // namespace tenacitas
#endif // ENDING_NEVER_H
