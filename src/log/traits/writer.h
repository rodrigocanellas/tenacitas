/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_SRC_LOG_TRAITS_WRITER_H
#define TENACITAS_SRC_LOG_TRAITS_WRITER_H

#include <string_view>

namespace tenacitas::src::log::traits
{

template <typename t>
concept writer = requires(t p_t) {
  {
    p_t.operator()(std::declval<std::string_view>())
  } -> std::same_as<void>;
};

} // namespace tenacitas::src::log::traits
#endif
