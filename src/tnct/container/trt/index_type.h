/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_TRT_INDEX_TYPE_H
#define TNCT_CONTAINER_TRT_INDEX_TYPE_H

#include <map>

namespace tnct::container::trt {

class std_map_id;
class std_multimap_id;

template <typename t_index_id, typename t_key, typename t_value>
struct index_type;

template <typename t_key, typename t_value>
struct index_type<std_map_id, t_key, t_value> {
  using type = std::map<t_key, t_value>;
};

template <typename t_key, typename t_value>
struct index_type<std_multimap_id, t_key, t_value> {
  using type = std::multimap<t_key, t_value>;
};

template <typename t_index_id, typename t_key, typename t_value>
using index_type_t = index_type<t_index_id, t_key, t_value>::type;

} // namespace tnct::container::trt

#endif
