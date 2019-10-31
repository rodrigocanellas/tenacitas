#ifndef TENACITAS_BUSINESS_TYPE_ENUM_H
#define TENACITAS_BUSINESS_TYPE_ENUM_H

#include <type_traits>

namespace tenacitas {
namespace business {
namespace type {

template<typename t_enum>
inline typename std::underlying_type<t_enum>::type
e2t(const t_enum& p_value)
{
  return static_cast<typename std::underlying_type<t_enum>::type>(p_value);
}

} // namespace logger
} // namespace type
} // namespace tenacitas

#endif // ENUM_H
