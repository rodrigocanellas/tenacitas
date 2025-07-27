/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_SRC_TRAITS_ASYNC_HANDLING_NAME_H
#define TENACITAS_SRC_TRAITS_ASYNC_HANDLING_NAME_H

#include "tenacitas/src/traits/has_output_operator.h"
#include "tenacitas/src/traits/is_hashable.h"
#include <type_traits>

namespace tenacitas::src::traits::async
{

template <typename t>
concept handling_name =
    std::is_default_constructible_v<t> && std::is_copy_constructible_v<t>
    && std::is_move_constructible_v<t> && std::is_copy_assignable_v<t>
    && std::is_move_assignable_v<t> && has_output_operator<t> && is_hashable<t>;

} // namespace tenacitas::src::traits::async

#endif
