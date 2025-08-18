/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_LOG_CPT_WRITER_H
#define TNCT_LOG_CPT_WRITER_H

#include <string_view>

namespace tnct::log::cpt
{

template <typename t>
concept writer = requires(t p_t) {
  {
    p_t.operator()(std::declval<std::string_view>())
  } -> std::same_as<void>;
};

} // namespace tnct::log::cpt
#endif
