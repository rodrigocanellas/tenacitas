/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TRAITS_QUEUE_H
#define TNCT_TRAITS_QUEUE_H

#include <concepts>
#include <cstddef>
#include <optional>
#include <utility>

#include "tnct/traits/has_output_operator.h"

namespace tnct::traits {

template <typename t, typename t_data>
concept queue = requires(t p_t) {

  typename t::data;

  requires std::same_as<typename t::data, t_data>;
  requires std::copy_constructible<typename t::data>;
  requires std::move_constructible<typename t::data>;
  requires std::assignable_from<typename t::data &, typename t::data>;
  requires has_output_operator<typename t::data>;

  requires std::copy_constructible<t>;
  requires std::move_constructible<t>;
  requires std::assignable_from<t &, t>;

  { p_t.push(std::declval<typename t::data>()) } -> std::same_as<void>;
  { p_t.push(std::declval<const typename t::data &>()) } -> std::same_as<void>;
  { p_t.pop() } -> std::same_as<std::optional<typename t::data>>;
  { p_t.full() } -> std::same_as<bool>;
  { p_t.empty() } -> std::same_as<bool>;
  { p_t.capacity() } -> std::same_as<size_t>;
  { p_t.occupied() } -> std::same_as<size_t>;
  { p_t.clear() } -> std::same_as<void>;
};

} // namespace tnct::traits

#endif
