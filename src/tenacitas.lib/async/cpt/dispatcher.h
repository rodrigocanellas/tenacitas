#ifndef TENACITAS_LIB_ASYNC_CPT_DISPATCHER_H
#define TENACITAS_LIB_ASYNC_CPT_DISPATCHER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas.lib.async directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <concepts>
#include <memory>

#include <tenacitas.lib/async/cpt/event.h>
#include <tenacitas.lib/async/typ/priority.h>
#include <tenacitas.lib/async/typ/queue_id.h>

namespace tenacitas::lib::async::cpt {

// template <typename t>
// concept dispatcher = requires(t p_t) {
//   typename t::ptr;

//  !std::default_initializable<t>;
//  !std::copy_constructible<t>;
//  !std::move_constructible<t>;

//  { p_t.stop() } -> std::same_as<void>;

//  {

//      p_t.template add_queue<int>(typ::priority()) } ->
//      std::same_as<typ::queue_id>;
//};

} // namespace tenacitas::lib::async::cpt

#endif
