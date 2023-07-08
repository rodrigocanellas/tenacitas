#ifndef TENACITAS_LIB_MESSENGER_CPT_DESERIALIZER_H
#define TENACITAS_LIB_MESSENGER_CPT_DESERIALIZER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <concepts>

#include <tenacitas.lib/src/async/cpt/event.h>
#include <tenacitas.lib/src/communication/cpt/message.h>

namespace tenacitas::lib::communication::cpt {

template <typename t>
concept deserializer = requires(t p_t) {

  typename t::event;
  requires async::cpt::event<typename t::event>;

  typename t::message;
  requires message<typename t::message>;

  {
    p_t(typename t::message())
    } -> std::same_as<std::optional<typename t::event>>;
};

} // namespace tenacitas::lib::communication::cpt

#endif
