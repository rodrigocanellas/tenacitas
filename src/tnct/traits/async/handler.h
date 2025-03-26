/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TRAITS_ASYNC_SUBSCRIBER_H
#define TNCT_TRAITS_ASYNC_SUBSCRIBER_H


#include "tnct/traits/async/event.h"

namespace tnct::traits::async {

// code reviewed by ChatGPT from a previous version I wrote
template <typename t, typename t_event>
concept handler = event<t_event> && requires {
  { &t::operator() } -> std::same_as<void (t::*)(t_event &&)>;
};
} // namespace tnct::traits::async

#endif
