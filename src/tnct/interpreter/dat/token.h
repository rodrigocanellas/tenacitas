#ifndef TNCT_INTERPRETER_DAT_TOKEN_H
#define TNCT_INTERPRETER_DAT_TOKEN_H

#include "tnct/interpreter/dat/lexema.h"
#include "tnct/interpreter/dat/type.h"

namespace tnct::interpreter::dat {

template <std::size_t t_lexema_size> struct token_t {

  lexema_reference<t_lexema_size> lexema;
  dat::type type{dat::unknow_type};

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const token_t<t_lexema_size> &p_token) {
    p_out << '[' << p_token.type << ',' << p_token.lexema.get() << ']';
    return p_out;
  }
};

} // namespace tnct::interpreter::dat

#endif
