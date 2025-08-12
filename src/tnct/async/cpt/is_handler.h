/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_ASYNC_CPT_IS_HANDLER_H
#define TNCT_ASYNC_CPT_IS_HANDLER_H

#include "tnct/async/cpt/is_event.h"

namespace tnct::async::cpt
{

// code reviewed by ChatGPT from a previous version I wrote
template <typename t, typename t_event>
concept is_handler =

    is_event<t_event> &&

    requires(t p_t, t_event &&evt) {
      {
        p_t(std::move(evt))
      } -> std::same_as<void>;
    } &&

    !requires(t p_t, t_event &evt) { p_t(evt); }
    && !requires(t p_t, const t_event &evt) { p_t(evt); }
    && !requires(t p_t, t_event evt) { p_t(evt); };

} // namespace tnct::async::cpt

#endif
