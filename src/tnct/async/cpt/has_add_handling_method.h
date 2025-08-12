/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_ASYNC_CPT_HAS_ADD_HANDLING_METHOD_H
#define TNCT_ASYNC_CPT_HAS_ADD_HANDLING_METHOD_H

#include "tnct/async/handling_name.h"
#include "tnct/async/handling_priority.h"
#include "tnct/async/result.h"
#include "tnct/async/cpt/is_event.h"
#include "tnct/async/cpt/is_handler.h"
#include "tnct/container/cpt/queue.h"

namespace tnct::async::cpt
{

// template <typename t, typename t_event, typename t_class>
// concept handler_ptr =
//     event<t_event> && std::same_as<t, void (t_class::*)(t_event &&)>;

template <typename t, typename t_event, typename t_queue, typename t_handler>
concept has_add_handling_method =
    async::cpt::is_event<t_event>
    && container::cpt::queue<t_queue, t_event>
    && async::cpt::is_handler<t_handler, t_event>
    && requires(t &d, async::handling_name &&p_handling_name,
                t_queue &&p_queue, t_handler &&p_handler,
                async::handling_priority &&p_handling_priority,
                std::size_t                     p_num_handlers) {
         {
           d.template add_handling<t_event, std::remove_cvref_t<t_queue>,
                                   std::remove_cvref_t<t_handler>>(
               p_handling_name, std::move(p_queue),
               std::move(p_handler), p_handling_priority,
               p_num_handlers)
         } -> std::same_as<async::result>;
       };

} // namespace tnct::async::cpt

#endif
