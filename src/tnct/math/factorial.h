#ifndef TNCT_MATH_FACTORIAL_H
#define TNCT_MATH_FACTORIAL_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

// #include <limits>
#include <optional>
#include <type_traits>

namespace tnct::math
{

template <typename t_ret, typename t_val>
std::optional<t_ret> factorial(t_val p_val)
{
  static_assert(std::is_integral_v<t_ret> && std::is_integral_v<t_val>,
                "factorial requires integral types");

  if (p_val < 0)
    return std::nullopt; // factorial not defined for negatives

  t_ret ret = 1;

  for (t_val i = 1; i <= p_val; ++i)
  {
    // Detect potential overflow before multiplication
    // if (ret > std::numeric_limits<t_ret>::max() / i)
    //   return std::nullopt;

    ret *= static_cast<t_ret>(i);
  }

  return ret;
}

} // namespace tnct::math
#endif
