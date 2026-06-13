#ifndef TNCT_INTERPRETER_DAT_TERMINAL_RECOGNITION_H
#define TNCT_INTERPRETER_DAT_TERMINAL_RECOGNITION_H

#include <string>

#include "tnct/interpreter/dat/terminal.h"

namespace tnct::interpreter::dat {

template <std::size_t t_lexema_size> struct terminal_recognition_t {
  std::string::const_iterator end;
  terminal_t<t_lexema_size> terminal;
};

} // namespace tnct::interpreter::dat

#endif
