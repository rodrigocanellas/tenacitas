/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_SRC_TRAITS_ASYNC_RESULT_H
#define TENACITAS_SRC_TRAITS_ASYNC_RESULT_H

#include "tenacitas/src/traits/has_output_operator.h"
#include "tenacitas/src/traits/scoped_enum.h"

namespace tenacitas::src::traits::async
{

template <typename t>
concept result = scoped_enum<t> && has_output_operator<t> && requires {
  {
    t::OK
  } -> std::convertible_to<t>;
};

} // namespace tenacitas::src::traits::async

#endif
