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

/// Helper class to avoid multiple uses of the expression
/// 'std::tuple_element_t<t_key_pos, t_object>'
template <std::size_t t_key_pos, tuple::cpt::is_tuple t_object>
using index_key_at = std::tuple_element_t<t_key_pos, t_object>;

/// Helper class to make the use of tnct::container::cpt::index_traits more
/// readable
///
/// \tparam t_index_id is a class that will be used to identify a
/// tnct::container::cpt::index_traits
/// \tparam t_key_pos is the position in t_object that will be used as key in
/// the index
/// \tparam is the tuple that implements the object to be indexed
template <typename t_index_id, std::size_t t_key_pos,
          tuple::cpt::is_tuple t_object>
using index_type =
    typename index_traits<t_index_id, index_key_at<t_key_pos, t_object>,
                          t_object>::type;

template <typename t, typename t_object>
concept index_definition = requires {
  /// Identifies the type of index used.
  ///
  /// This will be used in \p index_type
  typename t::index_id;

  /// Defines the position in the tuple t_object that will be used as key in the
  /// index
  requires std::same_as<std::remove_cv_t<decltype(t::key_pos)>, std::size_t>;

  requires tuple::cpt::is_tuple<t_object>;

  std::integral_constant<std::size_t, t::key_pos>{};

  /// The index type deduced from t::index_id, t::key_pos and t_object satifies
  /// the requirements defined in tnct::container::cpt::index
  requires(index<index_type<typename t::index_id, t::key_pos, t_object>,
                 index_key_at<t::key_pos, t_object>, t_object>);

  requires(tuple::cpt::index_within_tuple<t_object, t::key_pos>);
};

} // namespace tnct::container::cpt

#endif
