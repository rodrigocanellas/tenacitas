/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_ASYNC_TRAITS_HAS_PUBLISH_METHOD_H
#define TNCT_ASYNC_TRAITS_HAS_PUBLISH_METHOD_H

#include <concepts>

#include "tnct/async/result.h"
#include "tnct/async/traits/is_event.h"

namespace tnct::async::traits
{

template <typename t_event>
using event_type = std::remove_cvref_t<t_event>;

template <typename t_event>
using const_lvalue_event_type =
    std::add_const_t<std::add_lvalue_reference_t<event_type<t_event>>>;

template <typename t, typename t_event, typename... t_params>
concept has_publish_method =

    is_event<event_type<t_event>> &&

    (
        // const lvalue reference
        requires(t p_t) {
          {
            p_t.template publish<event_type<t_event>>(
                std::declval<const_lvalue_event_type<t_event>>())
          } -> std::same_as<async::result>;
        } ||

        // variadic parameters
        requires(t p_t, t_params &&...p_params) {
          {
            p_t.template publish<event_type<t_event>>(
                std::forward<t_params>(p_params)...)
          } -> std::same_as<async::result>;
        }

    );

} // namespace tnct::async::traits

#endif
