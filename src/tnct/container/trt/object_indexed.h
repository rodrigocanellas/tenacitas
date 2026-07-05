/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_TRT_OBJECT_INDEXED_H
#define TNCT_CONTAINER_TRT_OBJECT_INDEXED_H

#include <optional>

namespace tnct::container::trt {

template <typename t_object> struct object_indexed {
  using type = t_object;
  using optional = std::optional<type>;
  using const_ref = std::reference_wrapper<const optional>;
  using ref = std::reference_wrapper<optional>;
};

} // namespace tnct::container::trt

#endif
