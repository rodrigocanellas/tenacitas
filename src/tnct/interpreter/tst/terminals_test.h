/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Caellas - rodrigo.caellas at gmail.com

#ifndef TNCT_INTERPRETER_TST_TERMINALS_H
#define TNCT_INTERPRETER_TST_TERMINALS_H

#include <string>

#include "tnct/format/bus/fmt.h"
#include "tnct/interpreter/dat/terminal.h"
#include "tnct/interpreter/dat/terminals.h"
#include "tnct/interpreter/tst/common.h"
#include "tnct/log/bus/cerr.h"
#include "tnct/log/cpt/macros.h"
#include "tnct/program/bus/options.h"

using tnct::format::bus::fmt;

namespace tnct::interpreter::tst {

std::optional<std::string::const_iterator>
scan(std::string::const_iterator p_begin, std::string::const_iterator p_end,
     lexema &&p_expected, const terminals &p_terminals, log::cerr &p_logger) {

  std::optional<terminal_recognition> _res{
      p_terminals.recognize(p_begin, p_end)};

  if (!_res) {
    TNCT_LOG_ERR(p_logger,
                 fmt("no terminal found, but ", p_expected, " was expected"));
    return std::nullopt;
  }

  if (_res->terminal.lexema.get() != p_expected) {
    TNCT_LOG_ERR(p_logger,
                 fmt(_res->terminal.lexema.get(),
                     " was found, but it should have been ", p_expected));
    return std::nullopt;
  }

  TNCT_LOG_INF(p_logger, fmt(_res->terminal.lexema.get(), " was found"));

  return {_res->end};
}

struct terminals_000 {
  static std::string desc() { return "Test correct scan"; }

  bool operator()(const program::bus::options &) {

    log::cerr _logger;

    terminals _terminals;

    _terminals.add({{
        "=",
        _comparision_operator,
    }});

    const std::string _text{"="};

    std::optional<terminal_recognition> _res{
        _terminals.recognize(_text.begin(), _text.end())};

    if (!_res) {
      TNCT_LOG_ERR(_logger, "not terminal was found, but it should have been");
      return false;
    }

    if (_res->terminal.lexema.get() != lexema{"="}) {
      TNCT_LOG_ERR(_logger, fmt(_res->terminal.lexema.get(),
                                " was found, but it should have been '<'"));
      return false;
    }

    TNCT_LOG_INF(_logger, fmt(_res->terminal.lexema.get(), " was found"));
    return true;
  }
};

struct terminals_001 {
  static std::string desc() { return "Empty string"; }

  bool operator()(const program::bus::options &) {

    constexpr std::size_t lexema_size{10};

    using terminals = dat::terminals_t<lexema_size>;

    log::cerr _logger;

    terminals _terminals;

    _terminals.add({{
                        "==",
                        _comparision_operator,

                    },
                    {
                        "<",
                        _comparision_operator,

                    }});

    const std::string _text;

    std::optional<terminal_recognition> _res{
        _terminals.recognize(_text.begin(), _text.end())};

    if (_res) {
      TNCT_LOG_ERR(_logger,
                   "'a terminal was recognized, but it shold not have");
      return false;
    }

    return true;
  }
};

struct terminals_002 {
  static std::string desc() { return "Recognizes two terminals"; }

  bool operator()(const program::bus::options &) {

    constexpr std::size_t lexema_size{10};

    using lexema = dat::lexema_t<lexema_size>;
    using terminals = dat::terminals_t<lexema_size>;

    log::cerr _logger;

    terminals _terminals;

    _terminals.add({{
                        "==",
                        _comparision_operator,
                    },
                    {
                        "=",
                        _comparision_operator,
                    }});

    const std::string _text{"= =="};

    std::string::const_iterator _ite{_text.begin()};

    std::optional<std::string::const_iterator> _maybe{
        scan(_ite, _text.end(), lexema{"="}, _terminals, _logger)};
    if (!_maybe) {
      return false;
    }
    _ite = *_maybe;

    _maybe = scan(_ite, _text.end(), lexema{"=="}, _terminals, _logger);
    if (!_maybe) {
      return false;
    }
    _ite = *_maybe;

    std::optional<terminal_recognition> _res{
        _terminals.recognize(_ite, _text.end())};
    if (_res) {
      TNCT_LOG_DEB(_logger, fmt(_res->terminal.lexema.get(),
                                " was found, but none should have been"));
      return false;
    }

    return true;
  }
};

struct terminals_003 {
  static std::string desc() { return "Recognizes '< = == ='"; }

  bool operator()(const program::bus::options &) {

    constexpr std::size_t lexema_size{10};

    using lexema = dat::lexema_t<lexema_size>;
    using terminals = dat::terminals_t<lexema_size>;

    log::cerr _logger;

    terminals _terminals;

    _terminals.add({{
                        "==",
                        _comparision_operator,
                    },
                    {
                        "<",
                        _comparision_operator,
                    },
                    {
                        "=",
                        _assignment_operator,
                    }});

    const std::string _text{"< = == ="};

    std::string::const_iterator _ite{_text.begin()};

    TNCT_LOG_INF(_logger, fmt("terminals: ", _terminals));

    std::optional<std::string::const_iterator> _maybe{
        scan(_ite, _text.end(), lexema{"<"}, _terminals, _logger)};
    if (!_maybe) {
      return false;
    }
    _ite = *_maybe;

    _maybe = scan(_ite, _text.end(), lexema{"="}, _terminals, _logger);
    if (!_maybe) {
      return false;
    }
    _ite = *_maybe;

    _maybe = scan(_ite, _text.end(), lexema{"=="}, _terminals, _logger);
    if (!_maybe) {
      return false;
    }
    _ite = *_maybe;

    _maybe = scan(_ite, _text.end(), lexema{"="}, _terminals, _logger);
    if (!_maybe) {
      return false;
    }
    _ite = *_maybe;

    std::optional<terminal_recognition> _res{
        _terminals.recognize(_ite, _text.end())};
    if (_res) {
      TNCT_LOG_DEB(_logger, fmt(_res->terminal.lexema.get(),
                                " was found, but none should have been"));
      return false;
    }

    return true;
  }
};

struct terminals_004 {
  static std::string desc() { return "Recognizes 'if!()'"; }

  bool operator()(const program::bus::options &) {

    constexpr std::size_t lexema_size{10};

    using lexema = dat::lexema_t<lexema_size>;
    using terminals = dat::terminals_t<lexema_size>;

    log::cerr _logger;

    terminals _terminals;

    _terminals.add({{
                        "if",
                        _reserved_word,
                    },
                    {
                        "(",
                        _expression_delimeter,
                    },
                    {
                        ")",
                        _expression_delimeter,
                    },
                    {
                        "==",
                        _comparision_operator,
                    },
                    {
                        "<",
                        _comparision_operator,
                    },
                    {
                        "=",
                        _assignment_operator,
                    },
                    {
                        "!",
                        _unary_operator,
                    }});

    const std::string _text{"if!()"};

    std::string::const_iterator _ite{_text.begin()};

    TNCT_LOG_INF(_logger, fmt("terminals: ", _terminals));

    std::optional<std::string::const_iterator> _maybe{
        scan(_ite, _text.end(), lexema{"if"}, _terminals, _logger)};
    if (!_maybe) {
      return false;
    }
    _ite = *_maybe;

    _maybe = scan(_ite, _text.end(), lexema{"!"}, _terminals, _logger);
    if (!_maybe) {
      return false;
    }
    _ite = *_maybe;

    _maybe = scan(_ite, _text.end(), lexema{"("}, _terminals, _logger);
    if (!_maybe) {
      return false;
    }
    _ite = *_maybe;

    _maybe = scan(_ite, _text.end(), lexema{")"}, _terminals, _logger);
    if (!_maybe) {
      return false;
    }
    _ite = *_maybe;

    std::optional<terminal_recognition> _res{
        _terminals.recognize(_ite, _text.end())};
    if (_res) {
      TNCT_LOG_DEB(_logger, fmt(_res->terminal.lexema.get(),
                                " was found, but none should have been"));
      return false;
    }

    return true;
  }
};

} // namespace tnct::interpreter::tst
#endif
