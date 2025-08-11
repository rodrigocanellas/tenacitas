/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_ASYNC_TRAITS_IS_EVENT_H
#define TNCT_ASYNC_TRAITS_IS_EVENT_H

#include <type_traits>

#include "tnct/ostream/traits/has_output_operator.h"

namespace tnct::async::traits
{

template <typename t>
concept is_event =
    std::default_initializable<t> && std::copyable<t> && std::movable<t>
    && ostream::traits::has_output_operator<t> && std::is_class_v<t>;

}

#endif
