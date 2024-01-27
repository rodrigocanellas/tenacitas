/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_STA_EVENT_H
#define TENACITAS_LIB_STA_EVENT_H

#include <concepts>

namespace tenacitas::lib::sta {

template <typename t>
concept event = std::default_initializable<t> && std::copy_constructible<t>;

}

#endif
