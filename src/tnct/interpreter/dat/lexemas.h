/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

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
