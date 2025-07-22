/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_SRC_TRAITS_ASYNC_EVENT_H
#define TENACITAS_SRC_TRAITS_ASYNC_EVENT_H

#include <type_traits>

#include "tenacitas/src/traits/has_output_operator.h"

namespace tenacitas::src::traits::async {

template <typename t>
concept event = std::default_initializable<t> && std::copy_constructible<t> &&
    std::move_constructible<t> && has_output_operator<t> && std::is_class_v<t>;

}

#endif
