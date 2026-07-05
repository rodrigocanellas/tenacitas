/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_CPT_INDEX_DEFINITION_H
#define TNCT_CONTAINER_CPT_INDEX_DEFINITION_H

#include "tnct/container/cpt/index.h"
#include "tnct/container/trt/index_type.h"
#include <tnct/tuple/cpt/is_tuple.h>

namespace tnct::container::cpt {

template <typename t, typename t_object>
concept index_definition = requires {
  /// Identifies the type of index used.
  typename t::index_id;

  typename t::object_type;

  typename t::key_type;

  typename t::key_getter;

  std::is_move_constructible_v<t_object>;

  requires(std::same_as<t_object, typename t::object_type>);

  requires(index<trt::index_type_t<typename t::index_id, typename t::key_type,
                                   typename t::object_type>,
                 typename t::key_type, typename t::object_type>);

  requires(std::is_invocable_r_v<typename t::key_type, typename t::key_getter,
                                 const t_object &>);
};

} // namespace tnct::container::cpt

#endif
