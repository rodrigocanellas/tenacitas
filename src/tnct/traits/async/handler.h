/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TRAITS_ASYNC_HANDLER_H
#define TNCT_TRAITS_ASYNC_HANDLER_H

// #include <type_traits>

#include "tnct/traits/async/event.h"

namespace tnct::traits::async {

template <typename t, typename t_event>
concept handler =
    async::event<t_event> && std::is_invocable_r_v<void, t, t_event &&>;

// std::invocable<t,
//                std::add_rvalue_reference_t<std::remove_cvref_t<t_event>>>;

//     requires(t p_t) {

//   requires event<typename t::event>;

//   {
//     p_t((
//         std::declval<std::add_rvalue_reference_t<std::remove_cv_t<t_event>>>()))
//     } -> std::same_as<void>;
// };

} // namespace tnct::traits::async

#endif
