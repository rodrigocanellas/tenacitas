#ifndef TENACITAS_LIB_ASYNC_INTERNAL_EVT_DESERIALIZE_H
#define TENACITAS_LIB_ASYNC_INTERNAL_EVT_DESERIALIZE_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <tenacitas.lib/src/communication/cpt/concepts.h>

namespace tenacitas::lib::src::communication::internal::evt {

template <cpt::message t_message, cpt::deserializer t_deserializer>
struct deserialize {
  deserialize() = default;
  deserialize(const t_message &p_message) : message(p_message) {}

  t_message message;
};

} // namespace tenacitas::lib::src::communication::internal::evt

#endif
