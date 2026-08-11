/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_INTERPRETER_DAT_ITEM_H
#define TNCT_INTERPRETER_DAT_ITEM_H

#include <optional>

#include "tnct/interpreter/dat/item_id.h"
#include "tnct/interpreter/dat/symbols.h"

template <std::size_t t_size> struct productions_t;

template <std::size_t t_lexema_size>
using productions_const_iterator_t =
    typename productions_t<t_lexema_size>::const_iterator;

template <std::size_t t_size> struct itens_t;

template <std::size_t t_lexema_size>
using itens_const_iterator_t = typename itens_t<t_lexema_size>::const_iterator;

namespace tnct::interpreter::dat::internal {

template <std::size_t t_lexema_size> struct item_t {
  using productions_const_iterator =
      productions_const_iterator_t<t_lexema_size>;
  using itens_const_iterator = itens_const_iterator_t<t_lexema_size>;
  using symbols = dat::symbols_t<t_lexema_size>;
  using symbols_const_iterator = typename symbols::const_iterator;

private:
  item_id m_id;

  productions_const_iterator m_owner;

  std::optional<symbols_const_iterator> m_symbol;

  std::optional<productions_const_iterator> m_production;

  std::optional<itens_const_iterator> m_successor;
  std::optional<itens_const_iterator> m_alternative;
};

} // namespace tnct::interpreter::dat::internal

#endif
