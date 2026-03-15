#ifndef TNCT_MATH_CPT_NUMBER_H
#define TNCT_MATH_CPT_NUMBER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <cmath>
#include <concepts>
#include <type_traits>

namespace tnct::math::cpt
{

template <typename T>
concept number = std::integral<std::remove_cvref_t<T>>
                 || std::floating_point<std::remove_cvref_t<T>>;
}

#endif
