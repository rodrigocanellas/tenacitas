/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_INTERPRETER_BUS_NON_TERMINAL_RECOGNIZER_H
#define TNCT_INTERPRETER_BUS_NON_TERMINAL_RECOGNIZER_H

#include <functional>
#include <optional>
#include <string>

#include "tnct/interpreter/dat/non_terminal_recognition.h"

namespace tnct::interpreter::bus {

using non_terminal_recognizer =
    std::function<std::optional<dat::non_terminal_recognition>(
        std::string::const_iterator p_begin,
        std::string::const_iterator p_end)>;

} // namespace tnct::interpreter::bus
#endif
