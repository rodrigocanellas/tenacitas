/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TRAITS_LOG_WRITER_H
#define TNCT_TRAITS_LOG_WRITER_H

#include <string_view>

namespace tnct::traits {

template <typename t>
concept log_writer = requires(t p_t) {
  { p_t.operator()(std::declval<std::string_view>()) } -> std::same_as<void>;
};

} // namespace tnct::traits
#endif
