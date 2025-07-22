/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_SRC_TRAITS_ENUM_H
#define TENACITAS_SRC_TRAITS_ENUM_H

#include <type_traits>

namespace tenacitas::src::traits {

template <typename t>
concept enum_like = std::is_enum_v<t>;

} // namespace tenacitas::src::traits

#endif
