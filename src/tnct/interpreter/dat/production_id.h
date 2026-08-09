/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_INTERPRETER_DAT_PRODUCTION_ID_H
#define TNCT_INTERPRETER_DAT_PRODUCTION_ID_H

#include <utility>

#include "tnct/interpreter/dat/grammar_id.h"
#include "tnct/string/dat/fixed_size_string.h"

namespace tnct::interpreter::dat {

using production_id =
    std::pair<grammar_id, tnct::string::dat::fixed_size_string<15>>;

} // namespace tnct::interpreter::dat

#endif
