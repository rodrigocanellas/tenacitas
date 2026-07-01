/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_CPT_INDEX_DEFINITION_H
#define TNCT_CONTAINER_CPT_INDEX_DEFINITION_H

#include <concepts>

#include "tnct/container/cpt/index.h"
#include "tnct/container/cpt/index_traits.h"
#include "tnct/tuple/cpt/index_within_tuple.h"
#include <tnct/tuple/cpt/is_tuple.h>

namespace tnct::container::cpt {

template <std::size_t t_key_pos, typename t_object>
using index_key_at = std::tuple_element_t<t_key_pos, t_object>;

template <typename t_index_id, std::size_t t_key_pos, typename t_object>
using index_type =
    typename index_traits<t_index_id, index_key_at<t_key_pos, t_object>,
                          t_object>::type;

template <typename t, typename t_object>
concept index_definition = requires {
  typename t::index_id;

  typename t::object;

  requires std::same_as<std::remove_cv_t<decltype(t::key_pos)>, std::size_t>;

  requires tuple::cpt::is_tuple<t_object>;

  requires std::same_as<std::remove_cv_t<typename t::object>, t_object>;

  std::integral_constant<std::size_t, t::key_pos>{};

  requires(index<index_type<typename t::index_id, t::key_pos, t_object>,
                 index_key_at<t::key_pos, t_object>, t_object>);

  requires(tuple::cpt::index_within_tuple<t_object, t::key_pos>);
};

} // namespace tnct::container::cpt

#endif
