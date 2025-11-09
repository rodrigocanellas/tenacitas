/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TUPLE_CPT_CONTAINS_TUPLE_H
#define TNCT_TUPLE_CPT_CONTAINS_TUPLE_H

#include <tuple>

#include "tnct/tuple/internal/bus/contains_tuple_helper.h"

namespace tnct::tuple::cpt
{

// code reviewed by ChatGPT from a previous version I wrote
template <typename t_container_tuple, typename t_contained_tuple>
concept contains_tuple =
    (std::tuple_size_v<t_container_tuple> > 0)
    && (std::tuple_size_v<t_contained_tuple> >= 0)
    && internal::bus::contains_tuple_helper<t_container_tuple,
                                            t_contained_tuple>();

} // namespace tnct::tuple::cpt

#endif
