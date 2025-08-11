/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TRAITS_SCOPED_ENUM_H
#define TNCT_TRAITS_SCOPED_ENUM_H

#include <type_traits>

namespace tnct::traits
{
template <typename T>
concept scoped_enum =
    std::is_enum_v<T> && !std::is_convertible_v<T, std::underlying_type_t<T>>;

} // namespace tnct::traits

#endif
