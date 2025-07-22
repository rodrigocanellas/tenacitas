/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_SRC_TRANSLATOR_TRANSLATOR_H
#define TENACITAS_SRC_TRANSLATOR_TRANSLATOR_H

#include <cstdint>
#include <functional>
#include <string_view>

namespace tenacitas::src::translator {

template <std::unsigned_integral t_index = uint32_t>
using translate =
    std::function<std::string(t_index p_entry, std::string_view p_default)>;

}


#endif
