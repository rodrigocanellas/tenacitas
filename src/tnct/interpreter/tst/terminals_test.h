/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Caellas - rodrigo.caellas at gmail.com

#ifndef TNCT_INTERPRETER_TST_terminals_TEST_H
#define TNCT_INTERPRETER_TST_terminals_TEST_H

#include <iterator>
#include <string>

#include "tnct/format/bus/fmt.h"
#include "tnct/interpreter/dat/terminals.h"
#include "tnct/interpreter/dat/token.h"
#include "tnct/interpreter/dat/type.h"
#include "tnct/log/bus/cerr.h"
#include "tnct/log/cpt/macros.h"
#include "tnct/program/bus/options.h"
// #include "tnct/tuple/output.h"

using tnct::format::bus::fmt;

namespace tnct::interpreter::tst {

constexpr std::size_t lexema_size{10};

using lexema = dat::lexema_t<lexema_size>;
using terminals = dat::terminals_t<lexema_size>;
using token = dat::token_t<lexema_size>;
using recognition = typename terminals::recognition;
constexpr dat::type _comparision_operator = 5;
constexpr dat::type _assignmen_operator = 2;

std::optional<std::string::const_iterator>
scan(std::string::const_iterator p_begin, std::string::const_iterator p_end,
     lexema &&p_expected, const terminals &p_terminals, log::cerr &p_logger) {

  std::string::const_iterator _ite{p_begin};

  recognition _res{p_terminals.recognize(_ite, p_end)};

  if (!_res) {
    TNCT_LOG_ERR(p_logger,
                 fmt("no token found, but ", p_expected, " was expected"));
    return std::nullopt;
  }

  if (_res->lexema.get() != p_expected) {
    TNCT_LOG_ERR(p_logger,
                 fmt(_res->lexema.get(), " was found, but it should have been ",
                     p_expected));
    return std::nullopt;
  }

  TNCT_LOG_INF(p_logger, fmt(_res->lexema.get(), " was found"));
  std::advance(_ite, _res->lexema.get().size() + 1);
  return {_ite};
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

    std::cout << "terminals:\n" << _terminals << std::endl;

    recognition _res{_terminals.recognize(_text.begin(), _text.end())};

    if (!_res) {
      TNCT_LOG_ERR(_logger, "not token was found, but it should have been");
      return false;
    }

    if (_res->lexema.get() != lexema{"="}) {
      TNCT_LOG_ERR(_logger, fmt(_res->lexema.get(),
                                " was found, but it should have been '<'"));
      return false;
    }

    TNCT_LOG_INF(_logger, fmt(_res->lexema.get(), " was found"));
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

    recognition _res{_terminals.recognize(_text.begin(), _text.end())};

    if (_res) {
      TNCT_LOG_ERR(_logger, "'a token was recognized, but it shold not have");
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

    std::cout << __FILE__ << ':' << __LINE__ << " terminals: \n"
              << _terminals << std::endl;

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

    recognition _res{_terminals.recognize(_ite, _text.end())};
    if (_res) {
      TNCT_LOG_DEB(_logger, fmt(_res->lexema.get(),
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
                        _assignmen_operator,
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

    recognition _res{_terminals.recognize(_ite, _text.end())};
    if (_res) {
      TNCT_LOG_DEB(_logger, fmt(_res->lexema.get(),
                                " was found, but none should have been"));
      return false;
    }

    return true;
  }
};

} // namespace tnct::interpreter::tst
#endif
