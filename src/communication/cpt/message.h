#ifndef TENACITAS_LIB_MESSENGER_CPT_MESSAGE_H
#define TENACITAS_LIB_MESSENGER_CPT_MESSAGE_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <concepts>
#include <ranges>

namespace tenacitas::lib::communication::cpt {

template <typename t>
concept message = requires(t p_t) {

  typename t::value_type;

  requires(sizeof(typename t::value_type) == 1);

  std::ranges::bidirectional_range<t>;
};

} // namespace tenacitas::lib::communication::cpt

#endif // MESSAGE_H
