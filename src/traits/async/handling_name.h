/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_SRC_TRAITS_ASYNC_HANDLING_NAME_H
#define TENACITAS_SRC_TRAITS_ASYNC_HANDLING_NAME_H

#include <string_view>
#include <type_traits>

namespace tenacitas::src::traits::async
{

template <typename t>
concept handling_name = std::is_convertible_v<t, std::string_view>;

} // namespace tenacitas::src::traits::async

#endif
