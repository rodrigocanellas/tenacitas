/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_INTERPRETER_DAT_NON_TERMINAL_RECOGNITION_H
#define TNCT_INTERPRETER_DAT_NON_TERMINAL_RECOGNITION_H

#include <string>

#include "tnct/interpreter/dat/non_terminal.h"

namespace tnct::interpreter::dat {

struct non_terminal_recognition {
  std::string::const_iterator end;
  dat::non_terminal non_terminal;
};

} // namespace tnct::interpreter::dat

#endif
