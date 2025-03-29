/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TRAITS_ASYNC_HAS_PUBLISH_METHOD_H
#define TNCT_TRAITS_ASYNC_HAS_PUBLISH_METHOD_H

#include <concepts>

#include "tnct/traits/async/event.h"
#include "tnct/traits/enum.h"
#include "tnct/traits/tuple/contains_type.h"

namespace tnct::traits::async {

template <typename t, typename t_result, typename t_event>
concept has_const_lvalue_publish_method =

    enum_like<t_result> &&

    event<t_event> &&

    tuple::contains_type<typename t::events, t_event>() &&

    requires(t p_t) {
  {
    p_t.template publish<std::remove_cvref_t<t_event>>(
        std::declval<std::add_const_t<
            std::add_lvalue_reference_t<std::remove_cvref_t<t_event>>>>())
    } -> std::same_as<t_result>;
};

template <typename t, typename t_result, typename t_event, typename... t_params>
concept has_variadic_params_publish_method =

    enum_like<t_result> &&

    event<t_event> &&

    tuple::contains_type<typename t::events, t_event>() &&

    requires(t p_t, t_params &&...p_params) {
  {
    p_t.template publish<std::remove_cvref_t<t_event>>(
        std::forward<t_params>(p_params)...)
    } -> std::same_as<t_result>;
};

} // namespace tnct::traits::async

#endif
