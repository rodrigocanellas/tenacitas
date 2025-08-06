/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_SRC_ASYNC_TRAITS_IS_EVENT_H
#define TENACITAS_SRC_ASYNC_TRAITS_IS_EVENT_H

#include <type_traits>

#include "tenacitas/src/ostream/traits/has_output_operator.h"

namespace tenacitas::src::async::traits
{

template <typename t>
concept is_event =
    std::default_initializable<t> && std::copyable<t> && std::movable<t>
    && src::ostream::traits::has_output_operator<t> && std::is_class_v<t>;

}

#endif
