/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_STRING_BUS_TO_NUMBER_H
#define TNCT_STRING_BUS_TO_NUMBER_H

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <optional>
#include <string>
#include <string_view>

#include "tnct/math/cpt/number.h"

namespace tnct::string::bus {

template <math::cpt::number t_number>
static std::optional<t_number> to_number(std::string_view p_str) {
  try {

    if constexpr (std::is_unsigned_v<t_number>) {
      char *_end{nullptr};
      const std::uint64_t _number{std::strtoull(p_str.data(), &_end, 10)};
      if (*_end != '\0') {
        return std::nullopt;
      }
      if (_number >= std::numeric_limits<t_number>::max()) {
        return std::nullopt;
      }
      return {static_cast<t_number>(_number)};
    } else {
      long double _number{std::stold(p_str.data())};
      if (_number >= std::numeric_limits<t_number>::max()) {
        return std::nullopt;
      }
      return {static_cast<t_number>(_number)};
    }

  } catch (...) {
  }

  return std::nullopt;
}

} // namespace tnct::string::bus

#endif
