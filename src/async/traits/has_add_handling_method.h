/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_SRC_ASYNC_TRAITS_HAS_ADD_HANDLING_METHOD_H
#define TENACITAS_SRC_ASYNC_TRAITS_HAS_ADD_HANDLING_METHOD_H

#include "tenacitas/src/async/handling_name.h"
#include "tenacitas/src/async/handling_priority.h"
#include "tenacitas/src/async/result.h"
#include "tenacitas/src/async/traits/is_event.h"
#include "tenacitas/src/async/traits/is_handler.h"
#include "tenacitas/src/traits/container/queue.h"

namespace tenacitas::src::async::traits
{

// template <typename t, typename t_event, typename t_class>
// concept handler_ptr =
//     event<t_event> && std::same_as<t, void (t_class::*)(t_event &&)>;

template <typename t, typename t_event, typename t_queue, typename t_handler>
concept has_add_handling_method =
    src::async::traits::is_event<t_event>
    && src::traits::container::queue<t_queue, t_event>
    && src::async::traits::is_handler<t_handler, t_event>
    && requires(t &d, src::async::handling_name &&p_handling_name,
                t_queue &&p_queue, t_handler &&p_handler,
                src::async::handling_priority &&p_handling_priority,
                std::size_t                     p_num_handlers) {
         {
           d.template add_handling<t_event, std::remove_cvref_t<t_queue>,
                                   std::remove_cvref_t<t_handler>>(
               p_handling_name, std::move(p_queue),
               std::move(p_handler), p_handling_priority,
               p_num_handlers)
         } -> std::same_as<src::async::result>;
       };

} // namespace tenacitas::src::async::traits

#endif
