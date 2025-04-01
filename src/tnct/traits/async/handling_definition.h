/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TRAITS_ASYNC_HANDLING_DEFINITION_H
#define TNCT_TRAITS_ASYNC_HANDLING_DEFINITION_H

#include <type_traits>

#include "tnct/traits/async/event.h"
#include "tnct/traits/async/handler.h"
#include "tnct/traits/async/handling_priority.h"
#include "tnct/traits/container/queue.h"
#include "tnct/traits/string/fixed_size_string.h"

namespace tnct::traits::async {

template <typename t>
concept handling_definition = requires(t p_t) {

  typename t::event;
  typename t::handler;
  typename t::queue;
  typename t::priority;
  typename t::id;

  {
    p_t.get_id()
    } -> std::same_as<std::add_lvalue_reference_t<
        std::add_const_t<std::remove_cvref_t<typename t::id>>>>;

  { p_t.get_handler() } -> std::same_as<std::remove_cv_t<typename t::handler>>;

  { p_t.get_queue() } -> std::same_as<std::remove_cv_t<typename t::queue>>;

  { p_t.get_num_handlers() } -> std::same_as<std::size_t>;

  { p_t.get_priority() } -> std::same_as<typename t::priority>;
}
&&

    traits::async::event<std::remove_cvref_t<typename t::event>> &&

        traits::container::queue<std::remove_cvref_t<typename t::queue>,
                                 std::remove_cvref_t<typename t::event>> &&

            traits::async::handler<std::remove_cvref_t<typename t::handler>,
                                   std::remove_cvref_t<typename t::event>> &&

                traits::string::fixed_size_string<
                    std::remove_cvref_t<typename t::id>> &&

                    traits::async::handling_priority<
                        std::remove_cvref_t<typename t::priority>>;

} // namespace tnct::traits::async

#endif
