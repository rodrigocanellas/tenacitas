/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TUPLE_CPT_INDEX_WITHIN_TUPLE_H
#define TNCT_TUPLE_CPT_INDEX_WITHIN_TUPLE_H

#include "tnct/tuple/cpt/is_tuple.h"

namespace tnct::tuple::cpt {

template <tuple::cpt::is_tuple t_tuple, std::size_t t_index>
static constexpr bool index_within_tuple(std::tuple_size_v<t_tuple> > t_index);

} // namespace tnct::tuple::cpt
#endif
