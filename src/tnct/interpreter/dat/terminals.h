#ifndef TNCT_INTERPRETER_DAT_terminals_H
#define TNCT_INTERPRETER_DAT_terminals_H

#include <algorithm>
#include <iterator>
#include <vector>

#include "tnct/interpreter/dat/lexema.h"
#include "tnct/interpreter/dat/token.h"
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
  using token = token_t<t_lexema_size>;

  struct recognition {
    std::string::const_iterator ite;
    token_t<t_lexema_size> token;
  };

  terminals_t() = default;
  terminals_t(const terminals_t &) = delete;
  terminals_t(terminals_t &&) = default;
  ~terminals_t() = default;

  terminals_t &operator=(const terminals_t &) = delete;
  terminals_t &operator=(terminals_t &&) = default;

  /// Adds a group of lexema objects that should be recognized directly from the
  /// input text and their common associated type
  void add(std::initializer_list<std::pair<std::string_view, dat::type>>
               &&p_terminals) {
    import(p_terminals.begin(), p_terminals.end());
  }

  void add(dat::type p_type, std::string_view p_string) {
    m_container.emplace(p_string, p_type);
  }

  std::optional<recognition>
  recognize(std::string::const_iterator p_begin,
            std::string::const_iterator p_end_of_text) const {
    std::string::const_iterator _begin = p_begin;

    if ((_begin == p_end_of_text) || (*_begin == '\0') || (*_begin == '\n')) {
      return std::nullopt;
    }

    while (*_begin == ' ') {
      ++_begin;
    }

    std::string::const_iterator _ite = std::next(_begin);
    std::optional<token> _res;
    std::optional<token> _aux;

    while (true) {

      _aux = is_defined(_begin.base(), _ite.base());

      if (_aux) {
        // so far the string between _begin and _ite is being recognized, but
        // we can not decide on the first recognition. For example, if "=" and
        // "==" are terminals the string is "== b", then the first "=" will be
        // recognized, but the actual token is "=="
        _res = _aux;
      } else if ((_res) && (!_res->lexema.get().empty())) {
        return {{--_ite, {std::move(*_res)}}};
      }

      // any of these conditions makes us stop
      if ((_ite == p_end_of_text) || (*_ite == '\0') || (*_ite == ' ') ||
          (*_ite == '\n')) {
        break;
      }

      ++_ite;
    }
    if (_res) {
      return {{_ite, {std::move(*_res)}}};
    }
    return std::nullopt;
  }

  void import(const terminals_t &p_terminals) {
    import(p_terminals.begin(), p_terminals.end());
  }

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const terminals_t &p_terminals) {
    for (const typename container::value_type &_value_type :
         p_terminals.m_container) {
      p_out << _value_type << ' ';
    }
    return p_out;
  }

private:
  /// Tries to retrieve the type associated to a lexema and a reference to the
  /// lexema
  std::optional<token> is_defined(typename lexema::const_iterator p_begin,
                                  typename lexema::const_iterator p_end) const {
    if (static_cast<decltype(t_lexema_size)>(std::distance(p_begin, p_end)) >
        t_lexema_size) {
      return std::nullopt;
    }

    auto _cmp{[&](const typename container::value_type &p_token) {
      const bool _res{p_token.first.equals(p_begin, p_end)};
      return _res;
    }};

    const auto _ite{std::find_if(m_container.begin(), m_container.end(), _cmp)};

    if (_ite == m_container.end()) {
      return std::nullopt;
    }
    return std::optional<token>{
        {token{lexema_reference<t_lexema_size>{_ite->first},
               dat::type{_ite->second}}}};
  }

  template <typename t_iterator>
  void import(t_iterator p_begin, t_iterator p_end) {

    for (t_iterator _ite = p_begin; _ite != p_end; ++_ite) {
      if (std::find_if(m_container.begin(), m_container.end(),
                       [&](const typename container::value_type &p_value) {
                         return (lexema{_ite->first} == p_value.first);
                       }) == m_container.end()) {
        m_container.push_back({lexema{_ite->first}, _ite->second});
      }
    }
  }

  using container = std::vector<std::pair<lexema, dat::type>>;

  container m_container;
};

} // namespace tnct::interpreter::dat

#endif
