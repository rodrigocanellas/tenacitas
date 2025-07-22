/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_SRC_TRAITS_ASYNC_HAS_ADD_HANDLING_METHOD_H
#define TENACITAS_SRC_TRAITS_ASYNC_HAS_ADD_HANDLING_METHOD_H

// #include <concepts>
#include <cstring>
#include <type_traits>

#include "tenacitas/src/traits/async/handling_definition.h"
#include "tenacitas/src/traits/async/result.h"
#include "tenacitas/src/traits/tuple/contains_type.h"

namespace tenacitas::src::traits::async {

template <typename t, typename t_result, typename t_handling>
concept has_add_handling_method =

    traits::async::handling_definition<std::remove_cvref_t<t_handling>>

    &&

    traits::async::result<std::remove_cvref_t<t_result>>

    &&

    tuple::contains_type<typename t::events, typename t_handling::event>()

    &&

    // std::is_invocable_r_v<
    //     std::remove_cvref_t<t_result>, t,
    //     std::add_rvalue_reference_t<std::remove_cvref_t<t_handling>>>;

    requires(t p_t) {
  {
    p_t.template add_handling<std::remove_cvref_t<t_handling>>(
        std::declval<
            std::add_rvalue_reference_t<std::remove_cvref_t<t_handling>>>())

    } -> std::same_as<std::remove_cvref_t<t_result>>;
};

} // namespace tenacitas::src::traits::async

#endif
