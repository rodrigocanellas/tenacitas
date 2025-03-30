/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TRAITS_ASYNC_RESULT_H
#define TNCT_TRAITS_ASYNC_RESULT_H

#include "tnct/traits/enum.h"
#include "tnct/traits/has_output_operator.h"

namespace tnct::traits::async {

template <typename t>
concept result = enum_like<t> && has_output_operator<t>;

} // namespace tnct::traits::async

#endif
