#ifndef TNCT_INTERPRETER_DAT_TOKEN_H
#define TNCT_INTERPRETER_DAT_TOKEN_H

#include <utility>

#include "tnct/interpreter/dat/lexema.h"
#include "tnct/interpreter/dat/type.h"

namespace tnct::interpreter::dat {

template <std::size_t t_lexema_size>
using token_t = std::pair<lexema_t<t_lexema_size>, type>;

// template <std::size_t t_lexema_size>
// std::ostream &operator<<(std::ostream &p_out,
//                          const token_t<t_lexema_size> &p_token) {
//   p_out << '[' << p_token.first << ',' << p_token.second << ']';
//   return p_out;
// }

} // namespace tnct::interpreter::dat

#endif
