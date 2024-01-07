/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_ALG_FACTORIAL_H
#define TENACITAS_LIB_ALG_FACTORIAL_H

#include <concepts>
#include <limits>
#include <optional>

namespace tenacitas::lib::alg {

template <typename t_type>
requires std::floating_point<t_type> std::optional<t_type>
factorial(t_type p_val) {
  t_type ret = 1;

  for (t_type _i = 1; _i <= p_val; ++_i) {
    ret *= static_cast<t_type>(_i);
    if (ret == std::numeric_limits<t_type>::infinity()) {
      // overflow
      return {};
    }
  }

  return {ret};
}

template <typename t_type>
requires std::integral<t_type> std::optional<t_type> factorial(t_type p_val) {
  t_type ret = 1;

  for (t_type _i = 1; _i <= p_val; ++_i) {
    ret *= static_cast<t_type>(_i);
    if (ret == 0) {
      // overflow
      return {};
    }
  }

  return {ret};
}

} // namespace tenacitas::lib::alg

#endif
