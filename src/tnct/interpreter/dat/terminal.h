/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_INTERPRETER_DAT_TERMINAL_H
#define TNCT_INTERPRETER_DAT_TERMINAL_H

#include "tnct/interpreter/dat/lexema.h"
#include "tnct/interpreter/dat/type.h"

namespace tnct::interpreter::dat {

template <std::size_t t_lexema_size> struct terminal_t {

  lexema_reference_t<t_lexema_size> lexema_ref;
  dat::type type{dat::unknow_type};
};

template <std::size_t t_lexema_size>
std::ostream &operator<<(std::ostream &p_out,
                         const terminal_t<t_lexema_size> &p_token) {
  p_out << '[' << p_token.type << ',' << p_token.lexema_ref.get() << ']';
  return p_out;
}

} // namespace tnct::interpreter::dat

#endif
