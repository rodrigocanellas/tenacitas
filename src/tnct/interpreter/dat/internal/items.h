/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_INTERPRETER_DAT_ITEMS_H
#define TNCT_INTERPRETER_DAT_ITEMS_H

#include <map>

#include "tnct/interpreter/dat/internal/item.h"
#include "tnct/interpreter/dat/item_id.h"

namespace tnct::interpreter::dat::internal {

template <std::size_t t_lexema_size>
using itens_t = std::map<dat::item_id, dat::internal::item_t<t_lexema_size>>;

template <std::size_t t_lexema_size>
using itens_const_iterator = typename itens_t<t_lexema_size>::const_iterator;

} // namespace tnct::interpreter::dat::internal

#endif
