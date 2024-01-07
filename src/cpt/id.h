/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_CPT_ID_H
#define TENACITAS_LIB_CPT_ID_H

#include <concepts>

namespace tenacitas::lib::cpt {

template <typename t>
concept id =
    std::unsigned_integral<std::remove_const_t<std::remove_reference_t<t>>>;

}

namespace tlc = tenacitas::lib::cpt;

#endif
