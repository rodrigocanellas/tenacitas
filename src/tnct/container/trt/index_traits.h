/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_TRT_INDEX_TRAITS_H
#define TNCT_CONTAINER_TRT_INDEX_TRAITS_H

#include <map>

namespace tnct::container::trt {

class std_map_id {};
class std_multimap_id {};

struct no_index_id {};
struct no_index_type {};
struct no_iterator_type {};

template <typename t_index_id, typename t_key, typename t_value>
struct index_traits;

template <typename t_key, typename t_value>
struct index_traits<std_map_id, t_key, t_value> {
  using index = std::map<t_key, t_value>;
  using iterator = index::iterator;
};

template <typename t_key, typename t_value>
struct index_traits<std_multimap_id, t_key, t_value> {
  using index = std::multimap<t_key, t_value>;
  using iterator = index::iterator;
};

template <typename t_key, typename t_value>
struct index_traits<no_index_id, t_key, t_value> {
  using index = no_index_type;
  using iterator = no_iterator_type;
};

} // namespace tnct::container::trt

#endif
