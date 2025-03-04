/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TRAITS_ENUM_H
#define TNCT_TRAITS_ENUM_H

#include <type_traits>

namespace tnct::traits {

template <typename t>
concept enum_like = std::is_enum_v<t>;

} // namespace tnct::traits

#endif
