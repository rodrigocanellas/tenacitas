#ifndef TNCT_INTERPRETER_DAT_TOKENS_H
#define TNCT_INTERPRETER_DAT_TOKENS_H

#include <algorithm>
#include <map>

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
template <std::size_t t_lexema_size> class tokens_t {

public:
  using lexema = lexema_t<t_lexema_size>;
  using lexema_const_reference = typename lexema::const_reference;
  using token = token_t<t_lexema_size>;

  tokens_t() = default;
  tokens_t(const tokens_t &) = delete;
  tokens_t(tokens_t &&) = default;
  ~tokens_t();

  tokens_t &operator=(const tokens_t &) = delete;
  tokens_t &operator=(tokens_t &&) = default;

  /// Adds a group of lexema objects that should be recognized directly from the
  /// input text and their common associated type
  void add(std::initializer_list<token> p_tokens) {
    import(p_tokens.begin(), p_tokens.end());
  }

  void add(token &&p_token) {
    const auto _res{m_container.emplace(std::move(p_token))};
    if (!_res.second) {
      throw std::runtime_error(std::string{p_token.first} +
                               std::string{" already exists as a token "});
    }
  }

  void add(const token &p_tokens) { import(p_tokens.begin(), p_tokens.end()); }

  std::optional<std::pair<lexema_const_reference, dat::type>>
  recognize(std::string::const_iterator p_begin,
            std::string::const_iterator p_end_of_text) const {
    std::string::const_iterator _ite = p_begin;
    std::optional<std::pair<type, lexema_const_reference>> _res;
    std::optional<std::pair<type, lexema_const_reference>> _aux;
    while (true) {

      if (_ite == p_end_of_text) {
        return {{lexema::end_of_text, dat::end_of_text}};
      }

      /// any of these conditions makes us stop
      if ((_ite == p_end_of_text) || (*_ite == '\0') || (*_ite == ' ') ||
          (*_ite == '\n'))
        break;

      /// if the string parsed so far matches one of the tokens
      if (std::distance(_ite, p_begin) == 0) {
        lexema _lexema{std::string(1, *_ite)};
        _aux = is_defined(_lexema);
      } else {
        std::string::const_iterator aux = _ite;
        ++aux;
        lexema _lexema{_ite, aux};
        _aux = is_defined(_lexema);
      }

      if (_aux) {
        // so far the string between p_begin and _ite is being recognized, but
        // we can not decide on the first recognition. For example, if "=" and
        // "==" are tokens the string is "== b", then the first "=" will be
        // recognized, but the actual token is "=="
        _res = _aux;
      }

      ++_ite;
    }
    return _res;
  }

  void import(const tokens_t &p_tokens) {
    import(p_tokens.begin(), p_tokens.end());
  }

private:
  /// Tries to retrieve the type associated to a lexema and a reference to the
  /// lexema
  std::optional<std::pair<lexema_const_reference, type>>
  is_defined(std::string_view p_string) const {
    if (p_string.size() > t_lexema_size) {
      return std::nullopt;
    }

    const lexema _lexema{p_string};

    typename container::const_iterator _ite{m_container.find(_lexema)};
    if (_ite == m_container.end()) {
      return std::nullopt;
    }
    return {_ite->second, _ite->first};
  }

  template <typename t_iterator>
  void import(t_iterator p_begin, t_iterator p_end) {
    for (t_iterator _ite = p_begin; _ite != p_end; ++_ite) {
      const auto _res{m_container.emplace(std::move(*_ite))};
      if (!_res.second) {
        throw std::runtime_error(std::string{_ite->first} +
                                 std::string{" already exists as a token "});
      }
    }
  }

  using container = std::map<lexema, token>;

  container m_container;
};

} // namespace tnct::interpreter::dat

#endif
