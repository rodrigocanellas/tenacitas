/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_LIB_CPT_EVENT_H
#define TNCT_LIB_CPT_EVENT_H

#include <concepts>

namespace tnct::lib::cpt {

template <typename t>
concept event = std::default_initializable<t> && std::copy_constructible<t>;

}

namespace tlc = tnct::lib::cpt;

#endif
