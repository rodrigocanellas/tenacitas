#ifndef TENACITAS_LIB_TRANSLATION_TYP_TYPES_H
#define TENACITAS_LIB_TRANSLATION_TYP_TYPES_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <cstdint>
#include <functional>
#include <iostream>
#include <map>
#include <string>

namespace tenacitas::lib::translation::typ {

using entry = uint32_t;
using dictionary = std::map<entry, std::string>;

inline std::ostream &operator<<(std::ostream &p_out,
                                const typ::dictionary &p_dictionary) {
  for (const dictionary::value_type &p_value : p_dictionary) {
    p_out << '(' << p_value.first << ',' << p_value.second << ')';
  }
  return p_out;
}

} // namespace tenacitas::lib::translation::typ

#endif
