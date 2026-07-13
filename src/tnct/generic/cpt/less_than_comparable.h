/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_GENERIC_CPT_LESS_THAN_COMPARABLE_H
#define TNCT_GENERIC_CPT_LESS_THAN_COMPARABLE_H

#include <concepts>

namespace tnct::generic::cpt {
template <typename T>
concept less_than_comparable = requires(const T &a, const T &b) {
  { a < b } -> std::convertible_to<bool>;
};

} // namespace tnct::generic::cpt

#endif
