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

  if (_res->terminal.lexema_ref.get() != p_expected) {
    TNCT_LOG_ERR(p_logger,
                 fmt(_res->terminal.lexema_ref.get(),
                     " was found, but it should have been ", p_expected));
    return std::nullopt;
  }

  TNCT_LOG_INF(p_logger, fmt(_res->terminal.lexema_ref.get(), " was found"));

  return {_res->end};
}

struct terminals_000 {
  static std::string desc() { return "Test correct scan"; }

  bool operator()(const program::bus::options &) {

    log::cerr _logger;

    terminals _terminals;

    _terminals.import({{
        "=",
        comparision_operator,
    }});

    const std::string _text{"="};

    std::optional<terminal_recognition> _res{
        _terminals.recognize(_text.begin(), _text.end())};

    if (!_res) {
      TNCT_LOG_ERR(_logger, "not terminal was found, but it should have been");
      return false;
    }

    if (_res->terminal.lexema_ref.get() != lexema{"="}) {
      TNCT_LOG_ERR(_logger, fmt(_res->terminal.lexema_ref.get(),
                                " was found, but it should have been '<'"));
      return false;
    }

    TNCT_LOG_INF(_logger, fmt(_res->terminal.lexema_ref.get(), " was found"));
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

    _terminals.import({{
                           "==",
                           comparision_operator,

                       },
                       {
                           "<",
                           comparision_operator,

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

    _terminals.import({{
                           "==",
                           comparision_operator,
                       },
                       {
                           "=",
                           comparision_operator,
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
      TNCT_LOG_DEB(_logger, fmt(_res->terminal.lexema_ref.get(),
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

    _terminals.import({{
                           "==",
                           comparision_operator,
                       },
                       {
                           "<",
                           comparision_operator,
                       },
                       {
                           "=",
                           assignment_operator,
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
      TNCT_LOG_DEB(_logger, fmt(_res->terminal.lexema_ref.get(),
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

    _terminals.import({{
                           "if",
                           reserved_word,
                       },
                       {
                           "(",
                           expression_delimeter,
                       },
                       {
                           ")",
                           expression_delimeter,
                       },
                       {
                           "==",
                           comparision_operator,
                       },
                       {
                           "<",
                           comparision_operator,
                       },
                       {
                           "=",
                           assignment_operator,
                       },
                       {
                           "!",
                           unary_operator,
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
      TNCT_LOG_DEB(_logger, fmt(_res->terminal.lexema_ref.get(),
                                " was found, but none should have been"));
      return false;
    }

    return true;
  }
};

struct terminals_005 {
  static std::string desc() {
    return "terminals_t returns nullopt for a text containing only spaces";
  }

  bool operator()(const program::bus::options &) {
    using terminals = dat::terminals_t<10>;
    terminals _terminals;
    _terminals.add("=", dat::type{1});

    const std::string _text{"   "};
    return !_terminals.recognize(_text.begin(), _text.end());
  }
};

struct terminals_006 {
  static std::string desc() {
    return "terminals_t recognizes the longest terminal";
  }

  bool operator()(const program::bus::options &) {
    using terminals = dat::terminals_t<10>;
    terminals _terminals;
    _terminals.add("=", dat::type{1});
    _terminals.add("==", dat::type{2});

    const std::string _text{"== x"};
    std::optional<terminals::terminal_recognition> _res{
        _terminals.recognize(_text.begin(), _text.end())};

    return _res &&
           (_res->terminal.lexema_ref.get() == terminals::lexema{"=="}) &&
           (_res->terminal.type == dat::type{2}) &&
           (_res->end == std::next(_text.begin(), 2));
  }
};

struct terminals_007 {
  static std::string desc() {
    return "terminals_t can import terminals from another terminals_t";
  }

  bool operator()(const program::bus::options &) {
    using terminals = dat::terminals_t<10>;
    terminals _source;
    _source.add("(", dat::type{1});
    _source.add(")", dat::type{1});

    terminals _target;
    _target.import(_source);

    const std::string _text{"("};
    std::optional<terminals::terminal_recognition> _res{
        _target.recognize(_text.begin(), _text.end())};

    return _res && (_res->terminal.lexema_ref.get() == terminals::lexema{"("});
  }
};

struct terminals_008 {
  static std::string desc() {
    return "terminals_t::add ignores duplicate lexemas";
  }

  bool operator()(const program::bus::options &) {
    using terminals = dat::terminals_t<10>;
    terminals _terminals;
    _terminals.add("=", dat::type{1});
    _terminals.add("=", dat::type{2});

    const std::string _text{"="};
    std::optional<terminals::terminal_recognition> _res{
        _terminals.recognize(_text.begin(), _text.end())};

    return _res && (_res->terminal.type == dat::type{1});
  }
};

} // namespace tnct::interpreter::tst
#endif
