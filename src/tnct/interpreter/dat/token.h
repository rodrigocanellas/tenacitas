#ifndef TNCT_INTERPRETER_DAT_TOKEN_H
#define TNCT_INTERPRETER_DAT_TOKEN_H

#include <utility>

#include "tnct/interpreter/dat/lexema.h"
#include "tnct/interpreter/dat/type.h"

namespace tnct::interpreter::dat {

template <std::size_t t_lexema_size>
using token_t = std::pair<lexema_t<t_lexema_size>, type>;

} // namespace tnct::interpreter::dat

#endif
