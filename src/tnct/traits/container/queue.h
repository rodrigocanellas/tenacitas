/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TRAITS_CONTAINER_QUEUE_H
#define TNCT_TRAITS_CONTAINER_QUEUE_H

#include <concepts>
#include <cstddef>
#include <optional>
#include <utility>

#include "tnct/traits/has_output_operator.h"

namespace tnct::traits::container {

template <typename t, typename t_data>
concept queue = std::copy_constructible<typename t::data> &&
    std::move_constructible<typename t::data> &&
    // std::assignable_from<typename t::data &, typename t::data> &&
    has_output_operator<typename t::data> && std::copy_constructible<t> &&
    std::move_constructible<t> && std::assignable_from<t &, t> &&

    requires(t p_t) {

  typename t::data;

  { p_t.push(std::declval<typename t::data>()) } -> std::same_as<void>;
  { p_t.push(std::declval<const typename t::data &>()) } -> std::same_as<void>;
  { p_t.pop() } -> std::same_as<std::optional<typename t::data>>;
  { p_t.full() } -> std::same_as<bool>;
  { p_t.empty() } -> std::same_as<bool>;
  { p_t.capacity() } -> std::same_as<std::size_t>;
  { p_t.occupied() } -> std::same_as<std::size_t>;
  { p_t.clear() } -> std::same_as<void>;
};

} // namespace tnct::traits::container

#endif
