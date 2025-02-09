/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TRAITS_EVENT_H
#define TNCT_TRAITS_EVENT_H

#include "tnct/traits/has_output_operator.h"

namespace tnct::traits {

template <typename t>
concept event = std::default_initializable<t> && std::copy_constructible<t> &&
    std::move_constructible<t> && has_output_operator<t>;

}

#endif
