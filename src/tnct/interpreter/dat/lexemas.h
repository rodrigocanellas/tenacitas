#ifndef TNCT_INTERPRETER_DAT_LEXEMAS_H
#define TNCT_INTERPRETER_DAT_LEXEMAS_H

#include <set>

#include "tnct/interpreter/dat/lexema.h"

namespace tnct::interpreter::dat {

template <std::size_t t_lexema_size>
using lexemas_t = std::set<lexema_t<t_lexema_size>>;

template <std::size_t t_lexema_size>
using lexema_const_iterator_t =
    typename lexemas_t<t_lexema_size>::const_iterator;

} // namespace tnct::interpreter::dat

#endif
