/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_ASY_EVENT_EVENT_PRIORITY_H
#define TENACITAS_LIB_ASY_EVENT_EVENT_PRIORITY_H

#include <cstdint>
#include <iostream>

namespace tenacitas::lib::asy {

/// \brief Defines the event_priority of an event publishing among other
/// publishings for that event. The smallest the value, the lowest the
/// event_priority
enum class event_priority : uint8_t {
  lowest = 1,
  very_low,
  low,
  medium,
  high,
  very_high,
  highest
};

/// \brief Output operator for \p event_priority
inline std::ostream &operator<<(std::ostream &p_out,
                                event_priority p_event_priority) {
  switch (p_event_priority) {
  case event_priority::lowest:
    p_out << "lowest";
    break;
  case event_priority::very_low:
    p_out << "very low";
    break;
  case event_priority::low:
    p_out << "low";
    break;
  case event_priority::medium:
    p_out << "medium";
    break;
  case event_priority::high:
    p_out << "high";
    break;
  case event_priority::very_high:
    p_out << "very high";
    break;
  case event_priority::highest:
    p_out << "highest";
    break;
  default:
    p_out << "UNDEFINED";
    break;
  }
  return p_out;
}

} // namespace tenacitas::lib::asy

#endif
