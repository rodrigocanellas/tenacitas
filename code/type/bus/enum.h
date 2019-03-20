#ifndef TENACITAS_TYPE_BUS_ENUM_H
#define TENACITAS_TYPE_BUS_ENUM_H

#include <type_traits>

namespace tenacitas {
namespace type {
namespace bus {

template<typename t_enum>
inline typename std::underlying_type<t_enum>::type
e2t(const t_enum& p_value)
{
  return static_cast<typename std::underlying_type<t_enum>::type>(p_value);
}

} // namespace bus
} // namespace type
} // namespace tenacitas

#endif // ENUM_H
