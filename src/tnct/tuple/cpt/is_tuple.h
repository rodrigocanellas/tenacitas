/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TUPLE_CPT_IS_TUPLE_H
#define TNCT_TUPLE_CPT_IS_TUPLE_H

#include "tnct/tuple/internal/is_tuple_helper.h"

namespace tnct::tuple::cpt
{

template <typename t_tuple>
concept is_tuple = internal::is_tuple_helper_v<t_tuple>;

} // namespace tnct::tuple::cpt
#endif
