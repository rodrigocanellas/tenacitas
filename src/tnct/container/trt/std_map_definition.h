/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_TRT_STD_MAP_DESCRIPTION_H
#define TNCT_CONTAINER_TRT_STD_MAP_DESCRIPTION_H

#include <map>

#include "tnct/container/trt/index_definition.h"

namespace tnct::container::trt {

class std_map_index_id {};

template <typename t_key, typename t_value>
struct index_definition<std_map_index_id, t_key, t_value> {
  using index_type = std::map<t_key, t_value>;
  static constexpr bool unique = true;
};

} // namespace tnct::container::trt

#endif
