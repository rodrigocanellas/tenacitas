/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TRAITS_HANDLINGS_H
#define TNCT_TRAITS_HANDLINGS_H

#include "tnct/traits/event.h"

#include "tnct/traits/logger.h"

#include "tnct/traits/tuple_like.h"

namespace tnct::traits {

template <typename t>
concept handlings = requires(t p_t) {
  typename t::event;
  typename t::logger;

  event<typename t::event>;
  logger<typename t::logger>;
};

} // namespace tnct::traits

#endif
