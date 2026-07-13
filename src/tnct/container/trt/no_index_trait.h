/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_TRT_NO_INDEX_TRAIT_H
#define TNCT_CONTAINER_TRT_NO_INDEX_TRAIT_H

#include "tnct/container/trt/index_traits.h"

namespace tnct::container::trt {

struct no_index_trait_id {};

struct no_index_type {};
struct no_iterator_type {};

template <typename t_key, typename t_value>
struct index_traits<no_index_trait_id, t_key, t_value> {
  using index = no_index_type;
  using iterator = no_iterator_type;
  static constexpr bool unique = false;
};

} // namespace tnct::container::trt

#endif
