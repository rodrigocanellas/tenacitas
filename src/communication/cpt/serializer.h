#ifndef TENACITAS_LIB_MESSENGER_CPT_SERIALIZER_H
#define TENACITAS_LIB_MESSENGER_CPT_SERIALIZER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <concepts>

#include <tenacitas.lib/src/async/cpt/concepts.h>
#include <tenacitas.lib/src/communication/cpt/message.h>

namespace tenacitas::lib::communication::cpt {

template <typename t>
concept serializer = requires(t p_t) {

  typename t::event;
  requires async::cpt::event<typename t::event>;

  typename t::message;
  requires message<typename t::message>;

  { p_t(typename t::event()) } -> std::same_as<typename t::message>;
};

} // namespace tenacitas::lib::communication::cpt

#endif
