/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TRAITS_TUPLE_LIKE_H
#define TNCT_TRAITS_TUPLE_LIKE_H

#include "tnct/traits/is_tuple.h"

namespace tnct::traits {

template <typename t_tuple>
concept tuple_like = traits::is_tuple_v<t_tuple>;

} // namespace tnct::traits
#endif
