/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TRAITS_ASYNC_SUBSCRIBER_H
#define TNCT_TRAITS_ASYNC_SUBSCRIBER_H

#include "tnct/traits/async/event.h"

namespace tnct::traits::async {

// code reviewed by ChatGPT from a previous version I wrote
template <typename t, typename t_event>
concept handler = event<t_event> && requires(t p_t, t_event &&evt) {
  { p_t(std::move(evt)) } -> std::same_as<void>;
}

&& !requires(t p_t, const t_event &evt) {
  p_t(evt);
} // Prevents const t_event&

&&!requires(t p_t, t_event evt) {
  p_t(evt);
} // Prevents t_event (by value)

;

// template <typename t, typename t_event>
// concept handler = event<t_event> &&
//     std::is_invocable_r_v<void, t, t_event &&>; // Ensures correct signature

} // namespace tnct::traits::async

#endif
