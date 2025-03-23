/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TRAITS_ASYNC_HAS_ADD_HANDLING_METHOD_H
#define TNCT_TRAITS_ASYNC_HAS_ADD_HANDLING_METHOD_H

#include <concepts>
#include <cstring>

#include "tnct/traits/async/event.h"
#include "tnct/traits/async/handler.h"
#include "tnct/traits/container/queue.h"
#include "tnct/traits/enum.h"
#include "tnct/traits/string/fixed_size_string.h"

namespace tnct::traits::async {

template <typename t, typename t_result, typename t_event, typename t_handler,
          typename t_queue, typename t_handling_priority,
          typename t_handling_id>
concept has_add_handling_method =

    tnct::traits::enum_like<t_result> &&

    tnct::traits::async::event<t_event> &&

    tnct::traits::async::handler<t_handler, t_event> &&

    tnct::traits::container::queue<t_queue, t_event> &&

    tnct::traits::enum_like<t_handling_priority> &&

    tnct::traits::string::fixed_size_string<t_handling_id> &&

    requires(t p_t) {
  {
    p_t.template add_handling<std::remove_cvref_t<t_event>>(
        std::declval<t_handling_id>(),

        std::declval<
            std::add_rvalue_reference_t<std::remove_cvref_t<t_handler>>>(),

        std::declval<
            std::add_rvalue_reference_t<std::remove_cvref_t<t_queue>>>(),

        std::declval<std::size_t>(),

        std::declval<t_handling_priority>())

    } -> std::same_as<t_result>;
};

} // namespace tnct::traits::async

#endif
