/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_SRC_TRAITS_ASYNC_HANDLING_PRIORITY_H
#define TENACITAS_SRC_TRAITS_ASYNC_HANDLING_PRIORITY_H

#include "tenacitas/src/traits/enum.h"
#include "tenacitas/src/traits/has_output_operator.h"

namespace tenacitas::src::traits::async {

template <typename t>
concept handling_priority = enum_like<t> && has_output_operator<t>;

} // namespace tenacitas::src::traits::async

#endif
