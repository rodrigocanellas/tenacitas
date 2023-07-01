#ifndef TENACITAS_LIB_CONCEPTS_CPT_ID_H
#define TENACITAS_LIB_CONCEPTS_CPT_ID_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas.lib.async directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <concepts>

namespace tenacitas::lib::number::cpt {

template <typename t>
concept id =
    std::unsigned_integral<std::remove_const_t<std::remove_reference_t<t>>>;

} // namespace tenacitas::lib::number::cpt

#endif
