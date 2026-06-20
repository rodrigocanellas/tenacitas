#ifndef TNCT_INTERPRETER_BUS_TERMINAL_RECOGNIZER_H
#define TNCT_INTERPRETER_BUS_TERMINAL_RECOGNIZER_H

#include <algorithm>
#include <functional>
#include <iterator>

#include "tnct/interpreter/dat/lexema.h"
#include "tnct/interpreter/dat/terminal.h"
#include "tnct/interpreter/dat/terminal_recognition.h"
#include "tnct/interpreter/dat/terminals.h"

namespace tnct::interpreter::bus {

template <std::size_t t_lexema_size> class terminal_recognizer_t {

public:
  using lexema = dat::lexema_t<t_lexema_size>;
  using terminal = dat::terminal_t<t_lexema_size>;
  using terminals = dat::terminals_t<t_lexema_size>;
  using terminal_recognition = dat::terminal_recognition_t<t_lexema_size>;

  using is_delimeter = std::function<bool(std::string::const_iterator p_ite,
                                          std::string::const_iterator p_end)>;

  terminal_recognizer_t(
      const terminals &p_terminals,
      is_delimeter p_is_delimeter =
          [](std::string::const_iterator p_ite,
             std::string::const_iterator p_end) {
            return ((p_ite == p_end) || (*p_ite == '\0') || (*p_ite == ' ') ||
                    (*p_ite == '\n') || (*p_ite == '\t'));
          })
      : m_terminals(p_terminals), m_is_delimeter(p_is_delimeter) {}

  terminal_recognizer_t() = delete;

  terminal_recognizer_t(const terminal_recognizer_t &) = delete;
  terminal_recognizer_t(terminal_recognizer_t &&) = default;
  ~terminal_recognizer_t() = default;

  terminal_recognizer_t &operator=(const terminal_recognizer_t &) = delete;
  terminal_recognizer_t &operator=(terminal_recognizer_t &&) = default;

  std::optional<terminal_recognition>
  operator()(std::string::const_iterator p_begin,
             std::string::const_iterator p_end_of_text) const {

    if (m_is_delimeter(p_begin, p_end_of_text)) {
      return std::nullopt;
    }

    std::string::const_iterator _ite = std::next(p_begin);
    std::optional<terminal> _res;
    std::optional<terminal> _aux;

    while (true) {

      _aux = m_terminals.get().find(p_begin.base(), _ite.base());

      if (_aux) {
        // so far the string between _begin and _ite is being recognized,
        // but we can not decide on the first terminal_recognition. For
        // example, if
        // "=" and
        // "==" are terminals the string is "== b", then the first "=" will
        // be recognized, but the actual token is "=="
        _res = _aux;
      } else if ((_res) && (!_res->lexema_ref.get().empty())) {
        return {{--_ite, {std::move(*_res)}}};
      }

      // any of these conditions makes us stop
      if (m_is_delimeter(_ite, p_end_of_text)) {
        break;
      }

      ++_ite;
    }
    if (_res) {
      return {terminal_recognition{.end{_ite}, .terminal{std::move(*_res)}}};
    }
    return std::nullopt;
  }

private:
  std::reference_wrapper<const terminals> m_terminals;
  is_delimeter m_is_delimeter;
};

} // namespace tnct::interpreter::bus

#endif
