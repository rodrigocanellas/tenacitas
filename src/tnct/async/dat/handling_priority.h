/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_ASYNC_DAT_HANDLING_PRIORITY_H
#define TNCT_ASYNC_DAT_HANDLING_PRIORITY_H

#include <cstdint>
#include <iostream>

namespace tnct::async::dat
{

/// \brief Defines the handling_priority of an event publishing among other
/// publishings for that event. The smallest the value, the lowest the
/// handling_priority
enum class handling_priority : uint8_t
{
  lowest = 1,
  very_low,
  low,
  medium,
  high,
  very_high,
  highest
};

/// \brief Output operator for \p handling_priority
inline std::ostream &operator<<(std::ostream     &p_out,
                                handling_priority p_handling_priority)
{
  switch (p_handling_priority)
  {
  case handling_priority::lowest:
    p_out << "lowest";
    break;
  case handling_priority::very_low:
    p_out << "very low";
    break;
  case handling_priority::low:
    p_out << "low";
    break;
  case handling_priority::medium:
    p_out << "medium";
    break;
  case handling_priority::high:
    p_out << "high";
    break;
  case handling_priority::very_high:
    p_out << "very high";
    break;
  case handling_priority::highest:
    p_out << "highest";
    break;
  default:
    p_out << "UNDEFINED";
    break;
  }
  return p_out;
}

// bool operator>=(handling_priority p_left, handling_priority
// p_right)
// {
//     return p_right >= p_left;
// }

} // namespace tnct::async::dat

#endif
