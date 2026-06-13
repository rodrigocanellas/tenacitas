#ifndef TNCT_INTERPRETER_DAT_NON_TERMINAL_H
#define TNCT_INTERPRETER_DAT_NON_TERMINAL_H

#include <iostream>
#include <string>

#include "tnct/interpreter/dat/type.h"

namespace tnct::interpreter::dat {

struct non_terminal {
  std::string value;
  dat::type type{dat::unknow_type};
};

std::ostream &operator<<(std::ostream &p_out, const non_terminal &p_non_token) {
  p_out << '[' << p_non_token.type << ',' << p_non_token.value << ']';
  return p_out;
}

} // namespace tnct::interpreter::dat

#endif
