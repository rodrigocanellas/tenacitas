/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_LIB_CPT_ID_H
#define TNCT_LIB_CPT_ID_H

#include <concepts>

namespace tnct::lib::cpt {

template <typename t>
concept id =
    std::unsigned_integral<std::remove_const_t<std::remove_reference_t<t>>>;

}

namespace tlc = tnct::lib::cpt;

#endif
