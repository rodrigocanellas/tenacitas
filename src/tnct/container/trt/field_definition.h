/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_TRT_FIELD_DEFINITION_H
#define TNCT_CONTAINER_TRT_FIELD_DEFINITION_H

#include <type_traits>

#include "tnct/container/trt/no_index_definition.h"

namespace tnct::container::trt {

template <typename t_object_type, typename t_field_type,
          typename t_field_getter, typename t_field_setter>
  requires(std::is_invocable_r_v<t_field_type, t_field_getter,
                                 const t_object_type &> &&
           std::is_invocable_r_v<void, t_field_setter, t_object_type &,
                                 t_field_type>)
struct attribute_field_definition {
  using object_type = t_object_type;
  using field_type = t_field_type;
  using field_getter = t_field_getter;
  using field_setter = t_field_setter;
  using index_id = trt::no_index_id;
  static constexpr bool is_calculated{false};
};

template <typename t_object_type, typename t_field_type,
          typename t_field_getter, typename t_field_setter, typename t_index_id>
  requires(std::is_invocable_r_v<t_field_type, t_field_getter,
                                 const t_object_type &> &&
           std::is_invocable_r_v<void, t_field_setter, t_object_type &,
                                 t_field_type>)
struct index_field_definition {
  using object_type = t_object_type;
  using field_type = t_field_type;
  using field_getter = t_field_getter;
  using field_setter = t_field_setter;
  using index_id = t_index_id;
  static constexpr bool is_calculated{false};
};

template <typename t_object_type, typename t_field_type,
          typename t_field_getter, typename t_index_id>
  requires(
      std::is_invocable_r_v<t_field_type, t_field_getter,
                            const t_object_type &> &&
      (trt::index_definition<t_index_id, t_field_type, t_object_type>::unique ==
       false))
struct calculated_index_definition {
  using object_type = t_object_type;
  using field_type = t_field_type;
  using field_getter = t_field_getter;
  using field_setter = decltype([](t_object_type &, field_type) {});
  using index_id = t_index_id;
  static constexpr bool is_calculated{true};
};

} // namespace tnct::container::trt

#endif
