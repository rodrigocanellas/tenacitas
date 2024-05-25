/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_TRAITS_HAS_OUTPUT_OPERATOR_H
#define TENACITAS_LIB_TRAITS_HAS_OUTPUT_OPERATOR_H

#include <iostream>

namespace tenacitas::lib::traits {

template <typename T>
concept has_output_operator = requires(std::ostream &os, T value) {
  { os << value } -> std::same_as<std::ostream &>;
};

} // namespace tenacitas::lib::traits
#endif
