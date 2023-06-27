#ifndef TENACITAS_LIB_CONVERSIONS_TYP_BASE_H
#define TENACITAS_LIB_CONVERSIONS_TYP_BASE_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <cstdint>
#include <iostream>

namespace tenacitas::lib::conversions::typ {
enum class base : uint8_t { b2 = 2, b8 = 8, b10 = 10, b16 = 16 };

std::ostream &operator<<(std::ostream &p_out, const base &p_base) {
  switch (p_base) {
  case base::b2:
    p_out << "binary";
    break;
  case base::b8:
    p_out << "octal";
    break;
  case base::b10:
    p_out << "decimal";
    break;
  case base::b16:
    p_out << "hexadecimal";
    break;
  }

  return p_out;
}

} // namespace tenacitas::lib::conversions::typ

#endif
