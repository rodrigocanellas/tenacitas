/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TRAITS_ASYNC_HAS_PUBLISH_METHOD_H
#define TNCT_TRAITS_ASYNC_HAS_PUBLISH_METHOD_H

#include <concepts>
#include <tuple>

#include "tnct/traits/async/event.h"
#include "tnct/traits/enum.h"
#include "tnct/traits/tuple/like.h"

namespace tnct::traits::async {

template <typename t, typename t_result, typename t_event>
concept has_lvalue_publish = requires(t p_t, const t_event &e) {
  {
    p_t.template publish<std::remove_cvref_t<t_event>>(e)
    } -> std::same_as<t_result>;
};

template <typename t, typename t_result, typename t_event>
concept has_rvalue_publish = requires(t p_t, t_event &&e) {
  {
    p_t.template publish<std::remove_cvref_t<t_event>>(std::forward<t_event>(e))
    } -> std::same_as<t_result>;
};

// Combined concept to ensure both lvalue and rvalue versions exist
template <typename t, typename t_result, typename t_event>
concept publish_method_like = event<t_event> && enum_like<t_result> &&
    has_lvalue_publish<t, t_result, t_event> &&
    has_rvalue_publish<t, t_result, t_event>;

namespace internal {
template <typename t, enum_like t_result, tuple::like t_events,
          std::size_t t_idx = 0>
requires(std::tuple_size_v<t_events> > 0)

    constexpr bool has_publish_method() {

  if constexpr (t_idx >= std::tuple_size_v<t_events>) {
    return false;
  }

  using event = std::tuple_element_t<t_idx, t_events>;

  if constexpr (async::event<event> && has_lvalue_publish<t, t_result, event> &&
                has_rvalue_publish<t, t_result, event>) {
    return true;
  }

  if constexpr ((t_idx + 1) < std::tuple_size_v<t_events>) {
    return has_publish_method<t, t_result, t_events, t_idx + 1>();
  }

  return false;
}
} // namespace internal

template <typename t, typename t_result, typename... t_events>
concept publishers_methods_like = enum_like<t_result> &&
    internal::has_publish_method<t, t_result, std::tuple<t_events...>>();

} // namespace tnct::traits::async

#endif
