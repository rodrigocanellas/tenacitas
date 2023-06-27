#ifndef TENACITAS_LIB_MATH_ALG_FACTORIAL_H
#define TENACITAS_LIB_MATH_ALG_FACTORIAL_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <exception>
#include <optional>

namespace tenacitas::lib::math::alg {

template <typename t_ret, typename t_val>
std::optional<t_ret> factorial(t_val p_val) {

  t_ret ret = 1;

  for (t_val _i = 1; _i <= p_val; ++_i) {
    ret *= _i;
  }

  return {ret};
}

} // namespace tenacitas::lib::math::alg
#endif
