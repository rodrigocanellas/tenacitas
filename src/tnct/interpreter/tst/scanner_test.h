/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_INTERPRETER_TST_SCANNER_H
#define TNCT_INTERPRETER_TST_SCANNER_H

#include "tnct/format/bus/fmt.h"
#include "tnct/interpreter/bus/non_terminal_standard_recognizers.h"
#include "tnct/interpreter/bus/scanner.h"
#include "tnct/interpreter/dat/symbol.h"
#include "tnct/interpreter/tst/common.h"
#include "tnct/log/bus/cerr.h"
#include "tnct/log/cpt/macros.h"
#include "tnct/program/bus/options.h"

using tnct::format::bus::fmt;

namespace tnct::interpreter::tst {

using scanner = bus::scanner_t<lexema_size>;

std::optional<symbol> scan_terminal(scanner &p_scanner, log::cerr &p_logger,
                                    lexema &&p_expected, dat::type p_type) {

  std::optional<symbol> _symbol{p_scanner.get_symbol()};

  if (!_symbol) {
    TNCT_LOG_ERR(p_logger, "no symbol found, but a word was expected");
    return std::nullopt;
  }

  if (!_symbol->is_terminal()) {
    TNCT_LOG_ERR(p_logger, "expected a terminal, but got a non terminal");
    return std::nullopt;
  }

  const lexema _scanned{_symbol->get_lexema()->get()};

  if (_scanned != p_expected) {
    TNCT_LOG_ERR(p_logger,
                 fmt("it was expected ", p_expected, ", but got ", _scanned));
    return std::nullopt;
  }

  if (_symbol->get_type() != p_type) {
    TNCT_LOG_ERR(p_logger, fmt("expected type ", p_type, ", but got ",
                               _symbol->get_type()));
    return std::nullopt;
  }

  TNCT_LOG_INF(p_logger, fmt("word scanned is ", _scanned,
                             " as it should, and type scanned is ",
                             _symbol->get_type(), " as it should"));

  return _symbol;
}

std::optional<symbol> scan_eot(scanner &p_scanner, log::cerr &p_logger) {
  std::optional<symbol> _symbol{p_scanner.get_symbol()};

  if (!_symbol) {
    TNCT_LOG_ERR(p_logger, "no symbol found, but a word was expected");
    return std::nullopt;
  }

  if (!_symbol->is_terminal()) {
    TNCT_LOG_ERR(p_logger, "expected a non terminal, but got a terminal");
    return std::nullopt;
  }

  const lexema _scanned{_symbol->get_lexema()->get()};

  if (_scanned != lexema::end_of_text) {
    TNCT_LOG_ERR(p_logger, fmt("it was expected ", lexema::end_of_text,
                               ", but got ", _scanned));
    return std::nullopt;
  }

  if (_symbol->get_type() != dat::end_of_text) {
    TNCT_LOG_ERR(p_logger, fmt("expected type ", dat::end_of_text, ", but got ",
                               _symbol->get_type()));
    return std::nullopt;
  }

  TNCT_LOG_INF(p_logger, "eot found as expected");

  return _symbol;
}

std::optional<symbol> scan_non_terminal(scanner &p_scanner, log::cerr &p_logger,
                                        std::string &&p_expected,
                                        dat::type p_type) {
  std::optional<symbol> _symbol{p_scanner.get_symbol()};

  if (!_symbol) {
    TNCT_LOG_ERR(p_logger, "no symbol found, but a word was expected");
    return std::nullopt;
  }

  if (_symbol->is_terminal()) {
    TNCT_LOG_ERR(p_logger, "expected a non terminal, but got a terminal");
    return std::nullopt;
  }

  const std::string _scanned{_symbol->get_value()->get()};

  if (_scanned != p_expected) {
    TNCT_LOG_ERR(p_logger,
                 fmt("it was expected ", p_expected, ", but got ", _scanned));
    return std::nullopt;
  }

  if (_symbol->get_type() != p_type) {
    TNCT_LOG_ERR(p_logger, fmt("expected type ", p_type, ", but got ",
                               _symbol->get_type()));
    return std::nullopt;
  }

  TNCT_LOG_INF(p_logger, fmt("word scanned is ", _scanned,
                             " as it should, and type scanned is ",
                             _symbol->get_type(), " as it should"));

  return _symbol;
}

struct scanner_000 {
  static std::string desc() { return "Scans 'hello'"; }

  bool operator()(const program::bus::options &) {

    log::cerr _logger;
    scanner _scanner;

    bus::word_recognizer _word_recognizer{word_type};

    _scanner.add_recognizer(std::move(_word_recognizer));

    const std::string _text{"hello"};

    _scanner.set_text_to_scan(_text.begin(), _text.end());

    std::optional<symbol> _symbol{_scanner.get_symbol()};

    if (!_symbol) {
      TNCT_LOG_ERR(_logger, "no symbol found, but it was expected");
      return false;
    }

    if (_symbol->is_terminal()) {
      TNCT_LOG_ERR(_logger,
                   "terminal symbol found, but it was expected a non terminal");
    }

    const std::string &_value{_symbol->get_value()->get()};

    if (_value != "hello") {
      TNCT_LOG_ERR(_logger,
                   fmt("'hello' was expected, but '", _value, "' was scanned"));
      return false;
    }

    TNCT_LOG_TST(_logger, fmt("'", _value, "' was scanned, as expected"));

    return true;
  }
};

struct scanner_001 {
  static std::string desc() { return "Scans '<'"; }

  bool operator()(const program::bus::options &) {

    log::cerr _logger;
    scanner _scanner;

    _scanner.add_terminal("<", comparision_operator);

    const std::string _text{"<"};

    _scanner.set_text_to_scan(_text.begin(), _text.end());

    std::optional<symbol> _symbol{_scanner.get_symbol()};

    if (!_symbol) {
      TNCT_LOG_ERR(_logger, "no symbol found, but it was expected");
      return false;
    }

    if (!_symbol->is_terminal()) {
      TNCT_LOG_ERR(_logger,
                   "non terminal symbol found, but it was expected a terminal");
    }

    const lexema &_lexema{_symbol->get_lexema()->get()};

    if (_lexema != "<") {
      TNCT_LOG_ERR(_logger, fmt("'hello' was expected, but '", _lexema,
                                "' was scanned"));
      return false;
    }

    TNCT_LOG_TST(_logger, fmt("'", _lexema, "' was scanned, as expected"));

    return true;
  }
};

struct scanner_002 {
  static std::string desc() { return "Empty text"; }

  bool operator()(const program::bus::options &) {

    log::cerr _logger;
    scanner _scanner;

    const std::string _text;
    bus::word_recognizer _word_recognizer{word_type};

    _scanner.set_text_to_scan(_text.begin(), _text.end());

    std::optional<symbol> _res{_scanner.get_symbol()};

    if (!_res) {
      TNCT_LOG_ERR(_logger, "no symbol found, but it was expected a terminal "
                            "symbol with dat::end_of_text type");
      return false;
    }

    if (!_res->is_terminal()) {
      TNCT_LOG_ERR(_logger, "expected a terminal, but got a non terminal");
      return false;
    }

    if (_res->get_type() != dat::end_of_text) {
      TNCT_LOG_ERR(_logger, fmt("expected a terminal with dat::end_of_text "
                                "type, but got a non terminal ",
                                _res->get_type()));
      return false;
    }
    TNCT_LOG_TST(_logger, fmt("dat::type is ", _res->get_type()));
    return true;
  }
};

struct scanner_003 {
  static std::string desc() { return "Text only with spaces"; }

  bool operator()(const program::bus::options &) {

    log::cerr _logger;
    scanner _scanner;

    const std::string _text{"   "};
    bus::word_recognizer _word_recognizer{word_type};

    _scanner.set_text_to_scan(_text.begin(), _text.end());

    std::optional<symbol> _res{_scanner.get_symbol()};

    if (!_res) {
      TNCT_LOG_ERR(_logger, "no symbol found, but it was expected a terminal "
                            "symbol with dat::end_of_text type");
      return false;
    }

    if (!_res->is_terminal()) {
      TNCT_LOG_ERR(_logger, "expected a terminal, but got a non terminal");
      return false;
    }

    if (_res->get_type() != dat::end_of_text) {
      TNCT_LOG_ERR(_logger, fmt("expected a terminal with dat::end_of_text "
                                "type, but got a non terminal ",
                                _res->get_type()));
      return false;
    }
    TNCT_LOG_TST(_logger, fmt("dat::type is ", _res->get_type()));
    return true;
  }
};

struct scanner_004 {
  static std::string desc() { return "Text only with two new-line chars"; }

  bool operator()(const program::bus::options &) {

    log::cerr _logger;
    scanner _scanner;

    const std::string _text{"\n\n"};
    _scanner.set_text_to_scan(_text.begin(), _text.end());

    bus::word_recognizer _word_recognizer{word_type};

    std::optional<symbol> _res{_scanner.get_symbol()};

    if (!_res) {
      TNCT_LOG_ERR(
          _logger,
          "no symbol found, but it was expected a dat::end_of_text type");
      return false;
    }

    if (_res->get_type() != dat::end_of_text) {
      TNCT_LOG_ERR(
          _logger,
          fmt("it was expected a dat::end_of_text type, but it was found ",
              _res->get_type()));
      return false;
    }

    // if (!_res->is_terminal()) {
    //   TNCT_LOG_ERR(_logger, "expected a terminal, but got a non terminal");
    //   return false;
    // }

    // if (_res->get_type() != dat::end_of_text) {
    //   TNCT_LOG_ERR(_logger, fmt("expected a terminal with dat::end_of_text "
    //                             "type, but got a non terminal ",
    //                             _res->get_type()));
    //   return false;
    // }

    // if (_scanner.get_current_line() != 3) {
    //   TNCT_LOG_ERR(_logger, fmt("line should be 3, but it is ",
    //                             _scanner.get_current_line()));
    //   return false;
    // }
    // TNCT_LOG_TST(_logger, fmt("dat::type is ", _res->get_type()));
    return true;
  }
};

struct scanner_005 {
  static std::string desc() {
    return "Recognizing a word in a text with a number";
  }

  bool operator()(const program::bus::options &) {

    log::cerr _logger;
    scanner _scanner;

    const std::string _text{"abc4efg"};
    _scanner.set_text_to_scan(_text.begin(), _text.end());

    _scanner.add_recognizer(bus::word_recognizer{word_type});

    std::optional<symbol> _symbol{_scanner.get_symbol()};

    if (!_symbol) {
      TNCT_LOG_ERR(_logger, "no symbol found, but a word was expected");
      return false;
    }

    if (_symbol->is_terminal()) {
      TNCT_LOG_ERR(_logger, "expected a non terminal, but got a terminal");
      return false;
    }

    const std::string _expected{"abc4efg"};
    const std::string _scanned{_symbol->get_value()->get()};

    if (_scanned != _expected) {
      TNCT_LOG_ERR(_logger,
                   fmt("it was expected ", _expected, ", but got ", _scanned));
      return false;
    }

    TNCT_LOG_INF(_logger, fmt("word scanned is ", _scanned, " as it should"));

    return true;
  }
};

struct scanner_006 {
  static std::string desc() {
    return "Scanning all non-tokens in a text without spaces";
  }

  bool operator()(const program::bus::options &) {

    log::cerr _logger;
    scanner _scanner;

    const std::string _text{"abc 4 efg"};
    _scanner.set_text_to_scan(_text.begin(), _text.end());

    _scanner.add_recognizer(bus::word_recognizer{word_type});
    _scanner.add_recognizer(
        bus::decimal_integer_number_recognizer{integer_type});

    std::optional<symbol> _symbol{
        scan_non_terminal(_scanner, _logger, "abc", word_type)};

    if (!_symbol) {
      return false;
    }

    _symbol = scan_non_terminal(_scanner, _logger, "4", integer_type);
    if (!_symbol) {
      return false;
    }

    _symbol = scan_non_terminal(_scanner, _logger, "efg", word_type);
    if (!_symbol) {
      return false;
    }

    _symbol = scan_eot(_scanner, _logger);
    if (!_symbol) {
      return false;
    }

    return true;
  }
};

struct scanner_007 {
  static std::string desc() {
    return "Scanning all non-tokens in a text witht spaces";
  }

  bool operator()(const program::bus::options &) {
    log::cerr _logger;
    scanner _scanner;

    const std::string _text{"abc 4 efg"};
    _scanner.set_text_to_scan(_text.begin(), _text.end());

    _scanner.add_recognizer(bus::word_recognizer{word_type});
    _scanner.add_recognizer(
        bus::decimal_integer_number_recognizer{integer_type});

    std::optional<symbol> _symbol{
        scan_non_terminal(_scanner, _logger, "abc", word_type)};

    if (!_symbol) {
      return false;
    }

    _symbol = scan_non_terminal(_scanner, _logger, "4", integer_type);
    if (!_symbol) {
      return false;
    }

    _symbol = scan_non_terminal(_scanner, _logger, "efg", word_type);
    if (!_symbol) {
      return false;
    }

    _symbol = scan_eot(_scanner, _logger);
    if (!_symbol) {
      return false;
    }

    return true;
  }
};

struct scanner_008 {
  static std::string desc() { return "Scans '<'"; }

  bool operator()(const program::bus::options &) {
    log::cerr _logger;
    scanner _scanner;

    _scanner.add_terminal("<", comparision_operator);

    const std::string _text{"<"};

    _scanner.set_text_to_scan(_text.begin(), _text.end());

    std::optional<symbol> _symbol{
        scan_terminal(_scanner, _logger, lexema{"<"}, comparision_operator)};

    return _symbol.has_value();
  }
};

struct scanner_009 {
  static std::string desc() { return "Recognizes '= =='"; }

  bool operator()(const program::bus::options &) {
    log::cerr _logger;
    scanner _scanner;

    _scanner.add_terminal("=", comparision_operator);
    _scanner.add_terminal("==", comparision_operator);

    const std::string _text{"= =="};

    _scanner.set_text_to_scan(_text.begin(), _text.end());

    std::optional<symbol> _symbol{
        scan_terminal(_scanner, _logger, lexema{"="}, comparision_operator)};

    if (!_symbol) {
      return false;
    }

    _symbol =
        scan_terminal(_scanner, _logger, lexema{"=="}, comparision_operator);

    return _symbol.has_value();
  }
};

struct scanner_010 {
  static std::string desc() { return "Recognizes '< = == ='"; }

  bool operator()(const program::bus::options &) {
    log::cerr _logger;
    scanner _scanner;

    _scanner.add_terminal("=", comparision_operator);
    _scanner.add_terminal("==", comparision_operator);
    _scanner.add_terminal("<", comparision_operator);

    const std::string _text{"< = == ="};

    _scanner.set_text_to_scan(_text.begin(), _text.end());

    std::optional<symbol> _symbol{
        scan_terminal(_scanner, _logger, lexema{"<"}, comparision_operator)};

    if (!_symbol) {
      return false;
    }

    _symbol =
        scan_terminal(_scanner, _logger, lexema{"="}, comparision_operator);

    if (!_symbol) {
      return false;
    }

    _symbol =
        scan_terminal(_scanner, _logger, lexema{"=="}, comparision_operator);

    if (!_symbol) {
      return false;
    }

    return scan_terminal(_scanner, _logger, lexema{"="}, comparision_operator)
        .has_value();
  }
};

struct scanner_011 {
  static std::string desc() { return "Recognizes 'if!()'"; }

  bool operator()(const program::bus::options &) {
    log::cerr _logger;
    scanner _scanner;

    _scanner.add_terminals({{
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
    _scanner.set_text_to_scan(_text.begin(), _text.end());

    std::optional<symbol> _symbol{
        scan_terminal(_scanner, _logger, lexema{"if"}, reserved_word)};

    if (!_symbol) {
      return false;
    }

    _symbol = scan_terminal(_scanner, _logger, lexema{"!"}, unary_operator);

    if (!_symbol) {
      return false;
    }

    _symbol =
        scan_terminal(_scanner, _logger, lexema{"("}, expression_delimeter);

    if (!_symbol) {
      return false;
    }

    return scan_terminal(_scanner, _logger, lexema{")"}, expression_delimeter)
        .has_value();

    return true;
  }
};

struct scanner_012 {
  static std::string desc() {
    return "scanner returns end_of_text for input containing only ignored "
           "chars";
  }

  bool operator()(const program::bus::options &) {
    scanner _scanner;
    const std::string _text{" \n\t\r"};
    _scanner.set_text_to_scan(_text.begin(), _text.end());

    std::optional<symbol> _symbol{_scanner.get_symbol()};

    return _symbol && (_symbol->get_type() == dat::end_of_text);
  }
};

struct scanner_013 {
  static std::string desc() {
    return "scanner updates column after recognizing a non-terminal";
  }

  bool operator()(const program::bus::options &) {

    scanner _scanner;
    _scanner.add_recognizer(bus::word_recognizer{word_type});

    const std::string _text{"abc"};
    _scanner.set_text_to_scan(_text.begin(), _text.end());

    std::optional<symbol> _symbol{_scanner.get_symbol()};

    return _symbol && (_symbol->get_type() == word_type) &&
           (_scanner.get_current_line() == 1) &&
           (_scanner.get_current_column() == 4);
  }
};

struct scanner_014 {
  static std::string desc() {
    return "scanner updates line and column when skipping newline";
  }

  bool operator()(const program::bus::options &) {
    constexpr dat::type _word_type{200};
    scanner _scanner;
    _scanner.add_recognizer(bus::word_recognizer{_word_type});

    const std::string _text{"\nabc"};
    _scanner.set_text_to_scan(_text.begin(), _text.end());

    std::optional<symbol> _symbol{_scanner.get_symbol()};

    return _symbol && (_symbol->get_type() == _word_type) &&
           (_scanner.get_current_line() == 2) &&
           (_scanner.get_current_column() == 4);
  }
};

struct scanner_015 {
  static std::string desc() {
    return "scanner does not advance when a symbol is not recognized";
  }

  bool operator()(const program::bus::options &) {
    scanner _scanner;
    _scanner.add_terminal("=", dat::type{1});

    const std::string _text{"@"};
    _scanner.set_text_to_scan(_text.begin(), _text.end());

    std::optional<symbol> _symbol{_scanner.get_symbol()};

    return !_symbol && (_scanner.get_current_line() == 1) &&
           (_scanner.get_current_column() == 1);
  }
};

} // namespace tnct::interpreter::tst
#endif
