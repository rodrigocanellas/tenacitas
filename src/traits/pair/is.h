/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_SRC_TRAITS_PAIR_IS_H
#define TENACITAS_SRC_TRAITS_PAIR_IS_H

#include <type_traits>
#include <utility>

namespace tenacitas::src::traits::pair {

/// \brief Type trait that identifies if a type is not a \p pair
template <typename> struct is : std::false_type {};

/// \brief Type trait that identifies if a type is a pair
template <typename... t> struct is<std::pair<t...>> : std::true_type {};

template <typename t> inline constexpr bool is_v = is<t>::value;

} // namespace tenacitas::src::traits

#endif
