#ifndef TENACITAS_LIB_ASYNC_TYP_PRIORITY_H
#define TENACITAS_LIB_ASYNC_TYP_PRIORITY_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <cstdint>
#include <functional>
#include <iostream>

#include <tenacitas.lib/src/number/typ/id.h>

namespace tenacitas::lib::src::async::typ {
/// \brief Defines the priority of an event publishing among other publishings for
/// that event. The smallest the value, the lowest the priority
enum class priority : uint8_t {
  lowest = 1,
  very_low,
  low,
  medium,
  high,
  very_high,
  highest
};

/// \brief Output operator for \p priority
inline std::ostream &operator<<(std::ostream &p_out, priority p_priority) {
  switch (p_priority) {
  case priority::lowest:
    p_out << "lowest";
    break;
  case priority::very_low:
    p_out << "very low";
    break;
  case priority::low:
    p_out << "low";
    break;
  case priority::medium:
    p_out << "medium";
    break;
  case priority::high:
    p_out << "high";
    break;
  case priority::very_high:
    p_out << "very high";
    break;
  case priority::highest:
    p_out << "highest";
    break;
  }
  return p_out;
}

} // namespace tenacitas::lib::src::async::typ

#endif
