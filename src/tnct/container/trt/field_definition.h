/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_DAT_FIELD_DEFINITION_H
#define TNCT_CONTAINER_DAT_FIELD_DEFINITION_H

#include "tnct/container/trt/index_traits.h"

namespace tnct::container::trt {

template <typename t_object_type, typename t_field_type,
          typename t_field_getter, typename t_field_setter>
  requires(std::is_invocable_r_v<t_field_type, t_field_getter,
                                 const t_object_type &> &&
           std::is_invocable_r_v<void, t_field_setter, t_object_type &,
                                 t_field_type>)
struct attribute_definition {
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
struct index_definition {
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
      (trt::index_traits<t_index_id, t_field_type, t_object_type>::unique ==
       false))
struct calculated_index_definition {
  using object_type = t_object_type;
  using field_type = t_field_type;
  using field_getter = t_field_getter;
  using field_setter = decltype([](t_object_type &, field_type) {});
  using index_id = t_index_id;
  static constexpr bool is_calculated{true};
};

/* &&
            std::enable_if<
                !std::is_same_v<t_index_id, trt::no_map_id>,
                 cpt::index<

trt::index_type_t<t_index_id, t_field_type, t_object_type>,
t_field_type, t_object_type*/

} // namespace tnct::container::trt

#endif
