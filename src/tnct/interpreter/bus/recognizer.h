
#ifndef TNCT_INTERPRETER_BUS_RECOGNIZER_H
#define TNCT_INTERPRETER_BUS_RECOGNIZER_H

#include <functional>
#include <optional>
#include <string>

#include "tnct/interpreter/dat/non_terminal_recognition.h"

namespace tnct::interpreter::bus {

using recognizer = std::function<std::optional<dat::non_terminal_recognition>(
    std::string::const_iterator p_begin, std::string::const_iterator p_end)>;

} // namespace tnct::interpreter::bus
#endif
