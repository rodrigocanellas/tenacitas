/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_MEMORY_CPT_HAS_NEW_OPERATOR_H
#define TNCT_MEMORY_CPT_HAS_NEW_OPERATOR_H

#include <cstdint>

namespace tnct::memory::cpt
{

/// Type trait to check if a class has new operator
///
/// Code adapted from ChatGPT
template <typename T>
class has_new_operator
{
  using yes = uint8_t;
  using no  = uint16_t;

  template <typename C>
  static yes &test(decltype(new C()));
  template <typename C>
  static no &test(...);

public:
  static const bool value = sizeof(test<T>(0)) == sizeof(yes);
};

template <typename t>
constexpr bool has_new_operator_v = has_new_operator<t>::value;

} // namespace tnct::memory::cpt

#endif
