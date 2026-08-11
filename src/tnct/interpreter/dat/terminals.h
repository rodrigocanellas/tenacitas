/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_INTERPRETER_DAT_TERMINALS_H
#define TNCT_INTERPRETER_DAT_TERMINALS_H

#include <algorithm>
#include <iterator>
#include <vector>

#include "tnct/interpreter/dat/lexema.h"
#include "tnct/interpreter/dat/terminal.h"
#include "tnct/interpreter/dat/type.h"

namespace tnct::interpreter::dat {

/// This class associates tnct::interpreter::dat::lexema_t objects to a
/// tnct::interpreter::dat::type
///
/// For example: the tnct::interpreter::dat::lexema_t list "== != < > <= >="
/// could be associated to the tnct::interpreter::dat::type
/// "relational_operator".
///
/// For example: the tnct::interpreter::dat::lexema_t list "const while do" are
/// associated to the tnct::interpreter::dat::type "reserverd_word".
///
/// These tnct::interpreter::dat::lexema_t shall be recognized
/// directly from the input text, i.e., they are defined exclusively by
/// their strings.
///
/// @tparam t_lexema_size is the maximum size of a lexema string
template <std::size_t t_lexema_size> class terminals_t {

public:
  using lexema = lexema_t<t_lexema_size>;
  using terminal = terminal_t<t_lexema_size>;

  terminals_t() = default;
  terminals_t(const terminals_t &) = delete;
  terminals_t(terminals_t &&) = default;
  ~terminals_t() = default;

  terminals_t &operator=(const terminals_t &) = delete;
  terminals_t &operator=(terminals_t &&) = default;

  void add(std::string_view p_string, dat::type p_type) {

    if (found(p_string.begin(), p_string.end()) == m_container.end()) {
      m_container.push_back({lexema{p_string}, p_type});
    }
  }

  void import(const terminals_t &p_terminals) {
    import(p_terminals.m_container.begin(), p_terminals.m_container.end());
  }

  /// Adds a group of lexema objects that should be recognized directly from the
  /// input text and their common associated type
  void import(std::initializer_list<std::pair<std::string_view, dat::type>>
                  p_terminals) {
    import(p_terminals.begin(), p_terminals.end());
  }

  /// Tries to retrieve the type associated to a lexema and a reference to the
  /// lexema
  std::optional<terminal> find(typename lexema::const_iterator p_begin,
                               typename lexema::const_iterator p_end) const {
    if (static_cast<decltype(t_lexema_size)>(std::distance(p_begin, p_end)) >
        t_lexema_size) {
      return std::nullopt;
    }

    const_iterator _ite{found(p_begin, p_end)};
    if (_ite == m_container.end()) {
      return std::nullopt;
    }

    return {terminal{.lexema_ref = {_ite->first}, .type = {_ite->second}}};
  }

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const terminals_t &p_terminals) {
    for (const typename container::value_type &_value_type :
         p_terminals.m_container) {
      p_out << _value_type.first << ',' << _value_type.second;
    }
    return p_out;
  }

private:
  using container = std::vector<std::pair<lexema, dat::type>>;
  using iterator = typename container::iterator;
  using const_iterator = typename container::const_iterator;

  template <typename t_iterator>
  void import(t_iterator p_begin, t_iterator p_end) {
    for (t_iterator _ite = p_begin; _ite != p_end; ++_ite) {
      if (found(_ite->first.begin(), _ite->first.end()) == m_container.end()) {
        m_container.push_back({lexema{_ite->first}, _ite->second});
      }
    }
  }

  const_iterator found(const char *p_begin, const char *p_end) const {

    auto _cmp{[&](const typename container::value_type &p_token) {
      const bool _res{p_token.first.equals(p_begin, p_end)};
      return _res;
    }};

    return {std::find_if(m_container.begin(), m_container.end(), _cmp)};
  }

  container m_container;
};

} // namespace tnct::interpreter::dat

#endif
