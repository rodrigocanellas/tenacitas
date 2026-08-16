/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_CPT_MULTI_INDEX_H
#define TNCT_CONTAINER_CPT_MULTI_INDEX_H

#include <concepts>
#include <optional>
#include <tuple>
#include <type_traits>
#include <vector>

#include "tnct/container/cpt/field_definition.h"
#include "tnct/generic/cpt/less_than_comparable.h"
#include "tnct/memory/cpt/has_new_operator.h"
#include "tnct/ostream/cpt/has_output_operator.h"
#include "tnct/tuple/cpt/is_tuple.h"

namespace tnct::container::cpt {

namespace internal {

template <typename t_multi_index>
concept only_fields_definitions =

    tuple::cpt::is_tuple<typename t_multi_index::fields_definitions> &&

    []<std::size_t... t_idx>(std::index_sequence<t_idx...>) {
      return (container::cpt::field_definition<std::tuple_element_t<
                  t_idx, typename t_multi_index::fields_definitions>> &&
              ...);
    }(std::make_index_sequence<
        std::tuple_size_v<typename t_multi_index::fields_definitions>>());

template <typename t_multi_index>
concept field_types_match =
    []<std::size_t... t_idx>(std::index_sequence<t_idx...>) {
      return (std::same_as<
                  typename std::tuple_element_t<
                      t_idx,

                      typename t_multi_index::fields_definitions>::field_type,
                  typename t_multi_index::template field_t<t_idx>> &&
              ...);
    }(std::make_index_sequence<
        std::tuple_size_v<typename t_multi_index::fields_definitions>>());

template <typename t_multi_index, std::size_t t_field_pos>
concept has_get_method = requires(
    t_multi_index p_multi_index,
    const typename t_multi_index::template field_t<t_field_pos> &p_field) {
  {
    p_multi_index.template get<t_field_pos>(p_field)
  } -> std::same_as<std::vector<typename t_multi_index::record_ref>>;
};

template <typename t_multi_index>
concept has_get_methods =

    []<std::size_t... t_idx>(std::index_sequence<t_idx...>) {
      return (has_get_method<t_multi_index, t_idx> && ...);
    }(std::make_index_sequence<
        std::tuple_size_v<typename t_multi_index::fields_definitions>>());

template <typename t_multi_index, std::size_t t_field_pos>
concept has_erase_method = requires(
    t_multi_index p_multi_index,
    const typename t_multi_index::template field_t<t_field_pos> &p_field) {
  { p_multi_index.template erase<t_field_pos>(p_field) } -> std::same_as<void>;
};

template <typename t_multi_index>
concept has_erase_methods =

    []<std::size_t... t_idx>(std::index_sequence<t_idx...>) {
      return (has_erase_method<t_multi_index, t_idx> && ...);
    }(std::make_index_sequence<
        std::tuple_size_v<typename t_multi_index::fields_definitions>>());

template <typename t_multi_index, std::size_t t_field_pos>
concept has_update_method = requires(
    t_multi_index p_multi_index,
    typename t_multi_index::record_ref p_record_ref,
    const typename t_multi_index::template field_t<t_field_pos> &p_field) {
  {
    p_multi_index.template update<t_field_pos>(p_record_ref, p_field)
  } -> std::same_as<bool>;
};

template <typename t_multi_index>
concept has_update_methods =

    []<std::size_t... t_idx>(std::index_sequence<t_idx...>) {
      return (has_update_method<t_multi_index, t_idx> && ...);
    }(std::make_index_sequence<
        std::tuple_size_v<typename t_multi_index::fields_definitions>>());

} // namespace internal

template <typename t>
concept multi_index = requires(t p_t, const typename t::record &p_record,
                               typename t::object &&p_object) {
  typename t::fields_definitions;

  typename t::object;

  typename t::optional;

  typename t::record;

  typename t::record_ref;

  requires internal::only_fields_definitions<t>;

  requires internal::field_types_match<t>;

  requires std::same_as<typename t::object,
                        typename std::tuple_element_t<
                            0, typename t::fields_definitions>::object_type>;

  requires std::same_as<typename t::optional,
                        std::optional<typename t::object>>;

  requires std::same_as<typename t::record_ref,
                        std::reference_wrapper<typename t::record>>;

  requires ostream::cpt::has_output_operator<typename t::record>;

  requires generic::cpt::less_than_comparable<typename t::record>;

  requires not memory::cpt::has_new_operator_v<t>;

  requires not std::is_copy_constructible_v<t>;

  requires not std::is_move_constructible_v<t>;

  requires not std::is_copy_assignable_v<t>;

  requires not std::is_move_assignable_v<t>;

  requires internal::has_get_methods<t>;

  requires internal::has_erase_methods<t>;

  requires internal::has_update_methods<t>;

  requires ostream::cpt::has_output_operator<t>;

  {
    p_t.add(std::declval<typename t::object>())
  } -> std::same_as<std::optional<typename t::record_ref>>;

  { p_record.get_optional() } -> std::same_as<const typename t::optional &>;
};

} // namespace tnct::container::cpt

#endif
