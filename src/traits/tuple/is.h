/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_SRC_TRAITS_TUPLE_IS_H
#define TENACITAS_SRC_TRAITS_TUPLE_IS_H

#include <tuple>

namespace tenacitas::src::traits::tuple {

/// \brief Type trait that identifies if an object is not a tuple
template <typename> struct is : std::false_type {};

/// \brief Type trait that identifies if an object is a tuple
template <typename... t> struct is<std::tuple<t...>> : std::true_type {};

template <typename t> inline constexpr bool is_v = is<t>::value;

} // namespace tenacitas::src::traits::tuple

#endif
