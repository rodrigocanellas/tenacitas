/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_SRC_TRAITS_ASYNC_HANDLING_FUNCTION_H
#define TENACITAS_SRC_TRAITS_ASYNC_HANDLING_FUNCTION_H

#include "tenacitas/src/traits/async/event.h"

namespace tenacitas::src::traits::async
{

// code reviewed by ChatGPT from a previous version I wrote
template <typename t, typename t_event>
concept handling_function =

    event<t_event> &&

    requires(t p_t, t_event &&evt) {
      {
        p_t(std::move(evt))
      } -> std::same_as<void>;
    } &&

    !requires(t p_t, t_event &evt) { p_t(evt); }
    && !requires(t p_t, const t_event &evt) { p_t(evt); }
    && !requires(t p_t, t_event evt) { p_t(evt); };

} // namespace tenacitas::src::traits::async

#endif
