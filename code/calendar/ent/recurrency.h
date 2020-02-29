#ifndef TENACITAS_CALENDAR_ENT_RECURRENCY_H
#define TENACITAS_CALENDAR_ENT_RECURRENCY_H


#include <calendar/ent/types_of_repetition.h>

namespace tenacitas {
namespace calendar {
namespace ent {

template<types_of_repetition t_type_of_repetition,
         ending_types t_type_of_ending>
struct recurrency_t;

} // namespace entities
} // namespace periodicity
} // namespace tenacitas
#endif // RECURRENCY_H
