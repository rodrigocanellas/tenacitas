/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_CPT_INDEX_DEFINITION_H
#define TNCT_CONTAINER_CPT_INDEX_DEFINITION_H

#include <concepts>
#include <type_traits>

namespace tnct::container::cpt {

template <typename t>
concept field_definition = requires {
  typename t::object_type;

  typename t::field_type;

  typename t::field_getter;

  typename t::field_setter;

  typename t::index_id;

  requires std::same_as<std::remove_cv_t<decltype(t::is_calculated)>, bool>;

  requires(std::is_move_constructible_v<typename t::object_type>);

  requires(
      std::is_invocable_r_v<typename t::field_type, typename t::field_getter,
                            const typename t::object_type &>);

  requires(
      std::is_invocable_r_v<void, typename t::field_setter,
                            typename t::object_type &, typename t::field_type>);
};

} // namespace tnct::container::cpt

#endif
