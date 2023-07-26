#ifndef TENACITAS_LIB_TRANSLATION_ALG_TRANSLATOR_H
#define TENACITAS_LIB_TRANSLATION_ALG_TRANSLATOR_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <cstdint>
#include <functional>
#include <iostream>
#include <map>
#include <string>

#include <tenacitas.lib/src/translation/typ/types.h>

namespace tenacitas::lib::translation::alg {

using translate = std::function<std::string(const typ::entry&)>;

} // namespace tenacitas::lib::translation::alg

#endif
