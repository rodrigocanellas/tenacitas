#ifndef TNCT_INTERPRETER_DAT_TOKENS_H
#define TNCT_INTERPRETER_DAT_TOKENS_H

#include <algorithm>
#include <iterator>
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
  ~tokens_t() = default;

  tokens_t &operator=(const tokens_t &) = delete;
  tokens_t &operator=(tokens_t &&) = default;

  /// Adds a group of lexema objects that should be recognized directly from the
  /// input text and their common associated type
  void add(std::initializer_list<token> &&p_tokens) {
    import(p_tokens.begin(), p_tokens.end());
  }

  // void add(token &&p_token) {
  //   const auto _res{m_container.emplace(p_token.first, std::move(p_token))};

  //   if (!_res.second) {
  //     throw std::runtime_error(std::string{p_token.first} +
  //                              std::string{" already exists as a token "});
  //   }
  // }

  void add(const token &p_tokens) { import(p_tokens.begin(), p_tokens.end()); }

  std::optional<std::pair<lexema_const_reference, dat::type>>
  recognize(std::string::const_iterator p_begin,
            std::string::const_iterator p_end_of_text) const {
    std::string::const_iterator _ite = p_begin;
    std::optional<std::pair<lexema_const_reference, type>> _res;
    std::optional<std::pair<lexema_const_reference, type>> _aux;

    while (true) {

      // any of these conditions makes us stop
      if ((_ite == p_end_of_text) || (*_ite == '\0') || (*_ite == ' ') ||
          (*_ite == '\n'))
        break;

      _aux = is_defined(_ite.base(), std::next(_ite).base());

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

  friend std::ostream &operator<<(std::ostream &p_out,
                                  const tokens_t &p_tokens) {
    for (const typename container::value_type &_value_type :
         p_tokens.m_container) {
      std::cout << token{_value_type.first, _value_type.second} << ' ';
    }
    return p_out;
  }

private:
  /// Tries to retrieve the type associated to a lexema and a reference to the
  /// lexema
  // std::optional<std::pair<lexema_const_reference, type>>
  // is_defined(std::string_view p_string) const {
  //   if (p_string.size() > t_lexema_size) {
  //     return std::nullopt;
  //   }

  //   const lexema _lexema{p_string};

  //   typename container::const_iterator _ite{m_container.find(_lexema)};
  //   if (_ite == m_container.end()) {
  //     return std::nullopt;
  //   }
  //   return {_ite->second, _ite->first};
  // }

  std::optional<std::pair<lexema_const_reference, type>>
  is_defined(typename lexema::const_iterator p_begin,
             typename lexema::const_iterator p_end) const {
    if (static_cast<decltype(t_lexema_size)>(std::distance(p_begin, p_end)) >
        t_lexema_size) {
      return std::nullopt;
    }

    auto _cmp{[&](const token &p_token) {
      // lexema _target{p_begin, p_end};
      // std::cout << "lexema = '" << p_token.first << "', target = '" <<
      // _target
      //           << '\'' << std::endl;
      // const auto _1b{p_token.first.begin()};
      // const auto _1e{p_token.first.end()};
      // std::cout << _1b << ',' << _1e << std::endl;
      // const bool _res(std::equal(p_token.first.begin(), p_token.first.end(),
      //                            p_begin, p_end));
      // return _res;
      // // return _target == p_token.first;
      const bool _res{p_token.first.equals(p_begin, p_end)};
      return _res;
    }};

    const auto _ite{std::find_if(m_container.begin(), m_container.end(), _cmp)};

    if (_ite == m_container.end()) {
      return std::nullopt;
    }
    return {{lexema_const_reference{_ite->first}, _ite->second}};
  }

  template <typename t_iterator>
  void import(t_iterator p_begin, t_iterator p_end) {
    for (t_iterator _ite = p_begin; _ite != p_end; ++_ite) {
      const auto _res{m_container.emplace(_ite->first, _ite->second)};
      if (!_res.second) {
        throw std::runtime_error(std::string{_ite->first} +
                                 std::string{" already exists as a token "});
      }
    }
  }

  using container = std::map<lexema, dat::type>;

  container m_container;
};

} // namespace tnct::interpreter::dat

#endif
