/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_TRAITS_HANDLER_H
#define TENACITAS_LIB_TRAITS_HANDLER_H

#include <type_traits>

#include "tenacitas.lib/traits/event.h"

namespace tenacitas::lib::traits {

template <typename t, typename t_event>
concept handler = event<t_event> &&
    std::invocable<t, std::add_rvalue_reference_t<std::remove_cv_t<t_event>>>;

//     requires(t p_t) {

//   requires event<typename t::event>;

//   {
//     p_t((
//         std::declval<std::add_rvalue_reference_t<std::remove_cv_t<t_event>>>()))
//     } -> std::same_as<void>;
// };

} // namespace tenacitas::lib::traits

#endif
