/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_INTERPRETER_DAT_SYMBOLS_H
#define TNCT_INTERPRETER_DAT_SYMBOLS_H

#include <vector>

#include "tnct/interpreter/dat/symbol.h"

namespace tnct::interpreter::dat {

/// Collection tenacitas::interpreter::symbol objects
///
/// @tparam t_lexema_size is the maximum size of a lexema string
template <std::size_t t_lexema_size> class symbols_t final {

public:
  using symbol = symbol_t<t_lexema_size>;

private:
  using container = std::vector<symbol>;

public:
  using iterator = container::iterator;
  using const_iterator = container::const_iterator;

  symbols_t() = default;
  symbols_t(const symbols_t &) = delete;
  symbols_t(symbols_t &&) = default;
  ~symbols_t();

  symbols_t &operator=(const symbols_t &) = delete;
  symbols_t &operator=(symbols_t &&) = default;

  void add(const symbol &p_symbol) { m_container.push_back(p_symbol); }

  void add(symbol &&p_symbol) { m_container.push_back(std::move(p_symbol)); }

  iterator begin() { return m_container.begin(); }

  iterator end() { return m_container.end(); }

  const_iterator begin() const { return m_container.begin(); }

  const_iterator end() const { return m_container.end(); }

  void erase(iterator p_begin, iterator p_end) {
    m_container.erase(p_begin, p_end);
  }

private:
  container m_container;
};

} // namespace tnct::interpreter::dat

#endif
