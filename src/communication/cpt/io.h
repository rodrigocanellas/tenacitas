#ifndef TENACITAS_LIB_MESSENGER_CPT_IO_H
#define TENACITAS_LIB_MESSENGER_CPT_IO_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <concepts>

#include <tenacitas.lib/src/async/cpt/concepts.h>
#include <tenacitas.lib/src/calendar/cpt/chrono_convertible.h>
#include <tenacitas.lib/src/communication/cpt/message.h>
#include <tenacitas.lib/src/communication/typ/connection_id.h>

namespace tenacitas::lib::communication::cpt {

template <typename t, typename t_message>
concept io = requires(t p_t) {

  typename t_message;
  requires message<t_message>;

  { p_t.send(typ::connection_id{}, t_message{}) } -> std::same_as<void>;

  { p_t.receive(typ::connection_id{}) } -> std::same_as<t_message>;

  { p_t.receive_timeout } -> calendar::cpt::convertible_to_ms;

  { p_t.send_timeout } -> calendar::cpt::convertible_to_ms;
};

} // namespace tenacitas::lib::communication::cpt

#endif
