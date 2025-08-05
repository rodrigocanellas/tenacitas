/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_SRC_TRAITS_ASYNC_HAS_ADD_HANDLING_METHOD_H
#define TENACITAS_SRC_TRAITS_ASYNC_HAS_ADD_HANDLING_METHOD_H

#include "tenacitas/src/async/handling_name.h"
#include "tenacitas/src/async/handling_priority.h"
#include "tenacitas/src/async/result.h"
#include "tenacitas/src/traits/async/event.h"
#include "tenacitas/src/traits/async/handling_function.h"
#include "tenacitas/src/traits/container/queue.h"

namespace tenacitas::src::traits::async
{

template <typename t, typename t_event, typename t_class>
concept handling_function_ptr =
    event<t_event> && std::same_as<t, void (t_class::*)(t_event &&)>;

template <typename t, typename t_event, typename t_handler, typename t_queue>
concept has_add_handling_method =
    traits::async::event<t_event>
    && traits::async::handling_function<t_handler, t_event>
    && traits::container::queue<t_queue, t_event>
    && requires(t &d, src::async::handling_name &&p_handling_name,
                t_handler &&p_handling_function, t_queue &&p_queue,
                src::async::handling_priority &&p_handling_priority,
                std::size_t                     p_num_handling_functions) {
         {
           d.template add_handling<t_event, std::remove_cvref_t<t_queue>,
                                   std::remove_cvref_t<t_handler>>(
               p_handling_name, std::move(p_handling_function),
               std::move(p_queue), p_handling_priority,
               p_num_handling_functions)
         } -> std::same_as<src::async::result>;
       };

} // namespace tenacitas::src::traits::async

#endif
