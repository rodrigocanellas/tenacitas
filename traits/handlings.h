/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_TRAITS_HANDLINGS_H
#define TENACITAS_LIB_TRAITS_HANDLINGS_H

#include "tenacitas.lib/traits/event.h"

#include "tenacitas.lib/traits/logger.h"

#include "tenacitas.lib/traits/tuple_like.h"

namespace tenacitas::lib::traits {

template <typename t>
concept handlings = requires(t p_t) {
  typename t::event;
  typename t::logger;

  event<typename t::event>;
  logger<typename t::logger>;
};

} // namespace tenacitas::lib::traits

#endif
