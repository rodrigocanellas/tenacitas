#ifndef TENACITAS_LIB_ASYNC_CPT_EVENT_H
#define TENACITAS_LIB_ASYNC_CPT_EVENT_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas.lib.async directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <concepts>

namespace tenacitas::lib::async::cpt {

template <typename t>
concept event = requires(t p_t) {
  std::default_initializable<t>;
  std::copy_constructible<t>;
  std::move_constructible<t>;
};

} // namespace tenacitas::lib::async::cpt

#endif
