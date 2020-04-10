#ifndef TENACITAS_FORMATER_MAX_STR_LENGTH_H
#define TENACITAS_FORMATER_MAX_STR_LENGTH_H

#include <cmath>
#include <cstdint>
#include <iomanip>

namespace tenacitas {
namespace number {

/// \brief maximum number of characters needed to represent a type of
/// number
///
/// <tt>max_str_length<uint16_t>()</tt> is 5, while
/// <tt>max_str_length<uint32_t>()</tt> is 10
///
/// \tparam t_num_type is the type of number
///
/// \example

template <typename t_num_type> inline uint16_t max_str_length() {
  return (
      static_cast<uint16_t>(std::log10(std::pow(2, sizeof(t_num_type) * 8))) +
      1);
}

} // namespace number
} // namespace tenacitas

#endif
