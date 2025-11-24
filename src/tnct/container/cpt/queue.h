/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_CONTAINER_CPT_QUEUE_H
#define TNCT_CONTAINER_CPT_QUEUE_H

#include <concepts>
#include <cstddef>
#include <optional>

#include "tnct/ostream/cpt/has_output_operator.h"

namespace tnct::container::cpt
{

template <typename t, typename t_data>
concept queue =

    std::same_as<typename t::data, t_data> &&

    std::copyable<t_data> &&

    ostream::cpt::has_output_operator<t_data> &&

    std::copyable<t> &&

    std::same_as<typename t::data, t_data> &&

    requires(t p_t, t_data &&p_data) {
      {
        p_t.push(std::move(p_data))
      } -> std::same_as<void>;
    } &&

    requires(t p_t, const t_data &p_data) {
      {
        p_t.push(p_data)
      } -> std::same_as<void>;
    } &&

    requires(t p_t) {
      {
        p_t.pop()
      } -> std::same_as<std::optional<t_data>>;

      {
        p_t.full()
      } -> std::same_as<bool>;

      {
        p_t.empty()
      } -> std::same_as<bool>;

      {
        p_t.capacity()
      } -> std::same_as<std::size_t>;

      {
        p_t.occupied()
      } -> std::same_as<std::size_t>;

      {
        p_t.clear()
      } -> std::same_as<void>;
    };

} // namespace tnct::container::cpt

#endif
