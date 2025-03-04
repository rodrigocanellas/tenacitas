/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TRAITS_TUPLE_LIKE_H
#define TNCT_TRAITS_TUPLE_LIKE_H

#include "tnct/traits/tuple/is.h"

namespace tnct::traits::tuple {

template <typename t_tuple>
concept like = traits::tuple::is_v<t_tuple>;

} // namespace tnct::traits::tuple
#endif
