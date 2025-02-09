/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TRAITS_QUEUE_H
#define TNCT_TRAITS_QUEUE_H

#include <cstddef>
#include <optional>
#include <type_traits>
#include <utility>

#include "tnct/traits/has_output_operator.h"

namespace tnct::traits {

template <typename t, typename t_data>
concept queue = requires(t p_t) {
  typename t::data;

  std::is_same_v<typename t::data, t_data>;

  std::is_copy_constructible_v<typename t::data>;

  std::is_move_constructible_v<typename t::data>;

  !std::is_default_constructible_v<typename t::data>;

  has_output_operator<typename t::data>;

  std::is_copy_constructible_v<t>;

  std::is_move_constructible_v<t>;

  !std::is_default_constructible_v<t>;

  std::is_move_assignable_v<t>;

  std::is_copy_assignable_v<t>;

  {
    p_t.push(std::move(std::declval<typename t::data>()))
    } -> std::same_as<void>;

  {
    p_t.push(std::declval<
             std::add_const_t<std::add_lvalue_reference_t<typename t::data>>>())
    } -> std::same_as<void>;

  { p_t.pop() } -> std::same_as<std::optional<typename t::data>>;

  { p_t.full() } -> std::same_as<bool>;

  { p_t.empty() } -> std::same_as<bool>;

  { p_t.capacity() } -> std::same_as<size_t>;

  { p_t.occupied() } -> std::same_as<size_t>;

  { p_t.clear() } -> std::same_as<void>;
};

} // namespace tnct::traits

#endif
