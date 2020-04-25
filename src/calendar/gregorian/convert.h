#ifndef TENACITAS_CALENDAR_GREGORIAN_CONVERT_H
#define TENACITAS_CALENDAR_GREGORIAN_CONVERT_H

namespace tenacitas {
namespace calendar {
namespace gregorian {

template <typename t_from, typename t_to>
constexpr t_to convert(const t_from &p_from = t_from(1));

} // namespace gregorian
} // namespace calendar
} // namespace tenacitas

#endif // CONVERT_H
