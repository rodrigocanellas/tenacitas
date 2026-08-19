/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_TRT_NO_INDEX_DEFINITION_H
#define TNCT_CONTAINER_TRT_NO_INDEX_DEFINITION_H

#include <utility>

#include "tnct/container/trt/index_definition.h"

namespace tnct::container::trt {

struct no_index_id {};

template <typename t_key, typename t_ref> class no_index_type {

public:
  using key_type = char;
  using mapped_type = char;
  using value_type = char;
  using iterator = char;
  using const_iterator = char;

  std::pair<iterator, bool> emplace(const key_type &, mapped_type) {
    return {iterator{}, false};
  }

  std::pair<iterator, iterator> equal_range(key_type) {
    return {iterator{}, iterator{}};
  }

  std::pair<const_iterator, const_iterator> equal_range(key_type) const {
    return {const_iterator{}, const_iterator{}};
  }

  iterator erase(iterator) { return iterator{}; }

  iterator begin() { return iterator{}; }

  iterator end() { return iterator{}; }

  const_iterator begin() const { return const_iterator{}; }

  const_iterator end() const { return const_iterator{}; }

  iterator find(const key_type &) { return iterator{}; }

  const_iterator find(const key_type &) const { return const_iterator{}; }
};

template <typename t_key, typename t_value>
struct index_definition<no_index_id, t_key, t_value> {
  using index_type = no_index_type<t_key, t_value>;
  static constexpr bool unique = false;
};

} // namespace tnct::container::trt

#endif
