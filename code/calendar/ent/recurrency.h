#ifndef TENACITAS_PERIODICITY_ENTITIES_RECURRENCY_H
#define TENACITAS_PERIODICITY_ENTITIES_RECURRENCY_H

#include <periodicity/entities/types_of_ending.h>
#include <periodicity/entities/types_of_repetition.h>

namespace tenacitas {
namespace periodicity {
namespace entities {

template<types_of_repetition t_type_of_repetition,
         types_of_ending t_type_of_ending>
struct recurrency_t;

} // namespace entities
} // namespace periodicity
} // namespace tenacitas
#endif // RECURRENCY_H
