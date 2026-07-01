/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_CPT_INDEX_TRAITS_H
#define TNCT_CONTAINER_CPT_INDEX_TRAITS_H

#include <map>

#include <tnct/tuple/cpt/is_tuple.h>

namespace tnct::container::cpt {

class std_map_id;
class std_multimap_id;

template <typename t_index_id, typename t_key, typename t_value>
struct index_traits;

template <typename t_key, typename t_value>
struct index_traits<std_map_id, t_key, t_value> {
  using type = std::map<t_key, t_value>;
};

template <typename t_key, typename t_value>
struct index_traits<std_multimap_id, t_key, t_value> {
  using type = std::multimap<t_key, t_value>;
};

} // namespace tnct::container::cpt

#endif
