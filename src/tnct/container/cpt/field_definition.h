/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_CPT_INDEX_DEFINITION_H
#define TNCT_CONTAINER_CPT_INDEX_DEFINITION_H

#include "tnct/container/cpt/index.h"
#include "tnct/container/trt/index_traits.h"
#include <tnct/tuple/cpt/is_tuple.h>

namespace tnct::container::cpt {

template <typename t_index_id, typename t_key_type, typename t_object_type>
struct satisfy_index {
  static constexpr bool value =
      index<trt::index_traits<t_index_id, t_key_type, t_object_type>,
            t_key_type, t_object_type>;
};

template <typename t_key_type, typename t_object_type>
struct satisfy_index<trt::no_index_id, t_key_type, t_object_type> {
  static constexpr bool value = true;
};

template <typename t_index_id, typename t_key_type, typename t_object_type>
static constexpr bool satisfy_index_v =
    satisfy_index<t_index_id, t_key_type, t_object_type>::value;

template <typename t, typename t_object>
concept field_definition = requires {
  typename t::object_type;

  typename t::field_type;

  typename t::field_getter;

  typename t::field_setter;

  typename t::index_id;

  requires(std::same_as<t_object, typename t::object_type>);

  requires(std::is_move_constructible_v<typename t::object_type>);

  requires(std::is_invocable_r_v<typename t::field_type,
                                 typename t::field_getter, const t_object &>);

  requires(std::is_invocable_r_v<void, typename t::field_setter, t_object &,
                                 typename t::field_type>);

  // requires(satisfy_index_v<typename t::index_id, typename t::key_type,
  //                          typename t::t_object_type>);
};

} // namespace tnct::container::cpt

#endif
