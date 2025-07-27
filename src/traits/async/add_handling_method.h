/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_SRC_TRAITS_ASYNC_HAS_ADD_HANDLING_METHOD_H
#define TENACITAS_SRC_TRAITS_ASYNC_HAS_ADD_HANDLING_METHOD_H

#include "tenacitas/src/traits/async/event.h"
#include "tenacitas/src/traits/async/handling_function.h"
#include "tenacitas/src/traits/async/handling_name.h"
#include "tenacitas/src/traits/async/handling_priority.h"
#include "tenacitas/src/traits/async/result.h"
#include "tenacitas/src/traits/container/queue.h"

namespace tenacitas::src::traits::async
{

template <typename t, typename t_event, typename t_handling_name,
          typename t_handling_function, typename t_queue,
          typename t_handling_priority, typename t_result>
concept has_add_handling_method =
    traits::async::event<t_event>
    && traits::async::handling_name<t_handling_name>
    && traits::async::handling_function<t_handling_function, t_event>
    && traits::container::queue<t_queue, t_event>
    && traits::async::handling_priority<t_handling_priority>
    && traits::async::result<t_result>
    && requires(t &d, t_handling_name &&p_handling_name,
                t_handling_function &&p_handling_function, t_queue &&p_queue,
                t_handling_priority &&p_handling_priority,
                std::size_t           p_num_handling_functions) {
         {
           d.template add_handling<t_event, std::remove_cvref_t<t_queue>,
                                   std::remove_cvref_t<t_handling_function>>(
               std::move(p_handling_name), std::move(p_handling_function),
               std::move(p_queue), p_handling_priority,
               p_num_handling_functions)
         } -> std::same_as<t_result>;
       };

} // namespace tenacitas::src::traits::async

#endif
