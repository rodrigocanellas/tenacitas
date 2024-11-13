/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_TRAITS_IS_TUPLE_H
#define TENACITAS_LIB_TRAITS_IS_TUPLE_H

#include <tuple>

namespace tenacitas::lib::traits {

/// \brief Type trait that identifies if an object is not a tuple
template <typename> struct is_tuple : std::false_type {};

/// \brief Type trait that identifies if an object is a tuple
template <typename... t> struct is_tuple<std::tuple<t...>> : std::true_type {};

template <typename t> inline constexpr bool is_tuple_v = is_tuple<t>::value;

} // namespace tenacitas::lib::traits

#endif
