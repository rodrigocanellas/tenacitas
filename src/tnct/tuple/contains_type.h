/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TUPLE_CONTAINS_TYPE_H
#define TNCT_TUPLE_CONTAINS_TYPE_H

#include "tnct/tuple/cpt/is_tuple.h"
#include "tnct/tuple/get_type_index.h"

namespace tnct::tuple
{

// code reviewed by ChatGPT from a previous version I wrote
template <tuple::cpt::is_tuple t_tuple, typename t_type>
constexpr bool contains_type()
{
  return get_type_index<t_tuple, t_type>().has_value();
}

} // namespace tnct::tuple

#endif
