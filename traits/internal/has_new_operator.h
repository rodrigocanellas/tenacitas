/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_TRAITS_INTERNAL_HAS_NEW_OPERATOR_H
#define TENACITAS_LIB_TRAITS_INTERNAL_HAS_NEW_OPERATOR_H

#include <cstdint>

#include <tenacitas.lib/traits/tuple_like.h>

namespace tenacitas::lib::traits::internal {

/// Type trait to check if a class has new operator
///
/// Code adapted from ChatGPT
template <typename T> class has_new_operator {
  using yes = uint8_t;
  using no = uint16_t;

  template <typename C> static yes &test(decltype(new C()));
  template <typename C> static no &test(...);

public:
  static const bool value = sizeof(test<T>(0)) == sizeof(yes);
};
} // namespace tenacitas::lib::traits::internal

#endif
