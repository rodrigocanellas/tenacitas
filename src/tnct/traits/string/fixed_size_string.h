/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TRAITS_STRNG_FIXED_SIZE_STRING_H
#define TNCT_TRAITS_STRNG_FIXED_SIZE_STRING_H

#include <cstring>

namespace tnct::traits::string {

template <typename t>
concept fixed_size_string = requires(t p_t, const char *p_value) {

  {t{p_value}};

  {std::strlen(p_value) <= t::size};
};

} // namespace tnct::traits::string

#endif
