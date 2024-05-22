/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <tenacitas.lib/traits/internal/has_new_operator.h>

#ifndef TENACITAS_LIB_TRAITS_NEW_H
#define TENACITAS_LIB_TRAITS_NEW_H

namespace tenacitas::lib::traits {

/// Concept to check if a class does not have new operator
template <typename T>
concept no_new_operator = !internal::has_new_operator<T>::value;

/// Concept to check if a class does not have new operator
template <typename T>
concept new_operator = internal::has_new_operator<T>::value;

} // namespace tenacitas::lib::traits

#endif
