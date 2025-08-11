/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TUPLE_INTERNAL_IS_TUPLE_HELPER_H
#define TNCT_TUPLE_INTERNAL_IS_TUPLE_HELPER_H

#include <type_traits>

namespace tnct::tuple::internal
{

/// \brief Type trait that identifies if an object is not a tuple
template <typename>
struct is_tuple_helper : std::false_type
{
};

/// \brief Type trait that identifies if an object is a tuple
template <typename... t>
struct is_tuple_helper<std::tuple<t...>> : std::true_type
{
};

template <typename t>
inline constexpr bool is_tuple_helper_v = is_tuple_helper<t>::value;

} // namespace tnct::tuple::internal
#endif
