/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_SRC_OSTREAM_TRAITS_HAS_OUTPUT_OPERATOR_H
#define TENACITAS_SRC_OSTREAM_TRAITS_HAS_OUTPUT_OPERATOR_H

#include <ostream>

namespace tenacitas::src::ostream::traits
{

// code reviewed by ChatGPT from a previous version I wrote
template <typename T, typename CharT = char,
          typename Traits = std::char_traits<CharT>>
concept has_output_operator =
    requires(std::basic_ostream<CharT, Traits> &os, const T &value) {
      {
        os << value
      } -> std::same_as<std::basic_ostream<CharT, Traits> &>;
    };

} // namespace tenacitas::src::ostream::traits
#endif
