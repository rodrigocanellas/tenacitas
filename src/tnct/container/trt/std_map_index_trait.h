/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_TRT_STD_MAP_INDEX_TRAIT_H
#define TNCT_CONTAINER_TRT_STD_MAP_INDEX_TRAIT_H

#include <map>

#include "tnct/container/trt/index_traits.h"

namespace tnct::container::trt {

class std_map_index_trait_id {};

template <typename t_key, typename t_value>
struct index_traits<std_map_index_trait_id, t_key, t_value> {
  using index = std::map<t_key, t_value>;
  using iterator = index::iterator;
  static constexpr bool unique = true;
};

} // namespace tnct::container::trt

#endif
