/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_INTERPRETER_DAT_PRODUCTIONS_H
#define TNCT_INTERPRETER_DAT_PRODUCTIONS_H

#include <map>

#include "tnct/interpreter/dat/internal/production.h"
#include "tnct/interpreter/dat/production_id.h"

namespace tnct::interpreter::dat::internal {

template <std::size_t t_lexema_size>
using productions_t =
    std::map<dat::production_id, dat::internal::production_t<t_lexema_size>>;

template <std::size_t t_lexema_size>
using productions_const_iterator_t =
    typename productions_t<t_lexema_size>::const_iterator;

} // namespace tnct::interpreter::dat::internal

#endif
