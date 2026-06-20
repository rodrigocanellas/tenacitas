/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Caellas - rodrigo.caellas at gmail.com

#ifndef TNCT_INTERPRETER_TST_STANTDARD_RECOGNIZERS_TEST_H
#define TNCT_INTERPRETER_TST_STANTDARD_RECOGNIZERS_TEST_H

#include <iterator>
#include <optional>
#include <string>

#include "tnct/format/bus/fmt.h"
#include "tnct/interpreter/bus/non_terminal_recognizer.h"
#include "tnct/interpreter/bus/non_terminal_standard_recognizers.h"
#include "tnct/interpreter/tst/common.h"
#include "tnct/log/bus/cerr.h"
#include "tnct/log/cpt/macros.h"
#include "tnct/program/bus/options.h"

using tnct::format::bus::fmt;

namespace tnct::interpreter::tst {

struct non_terminal_standard_recognizers_000 {
  static std::string desc() { return "Test correct scan"; }

  bool operator()(const program::bus::options &) {

    log::cerr _logger;

    const std::string _text{"abcdefgh"};
    bus::word_recognizer _word_recognizer{word_type};

    std::optional<dat::non_terminal_recognition> _res{
        _word_recognizer(_text.begin(), _text.end())};

    if (!_res) {
      TNCT_LOG_ERR(_logger, "word not recognized, but it should");
      return false;
    }

    const std::string _scanned{_res->non_terminal.value};
    if (_scanned != _text) {
      TNCT_LOG_ERR(_logger, fmt("word scanned is ", _scanned,
                                ", but it should have been ", _text));
      return false;
    }

    TNCT_LOG_TST(_logger, fmt("word scanned is ", _scanned, " as it should"));
    return true;
  }
};

struct non_terminal_standard_recognizers_001 {
  static std::string desc() { return "Empty text"; }

  bool operator()(const program::bus::options &) {

    log::cerr _logger;

    const std::string _text;
    bus::word_recognizer _word_recognizer{word_type};

    std::optional<dat::non_terminal_recognition> _res{
        _word_recognizer(_text.begin(), _text.end())};

    if (_res) {
      TNCT_LOG_ERR(_logger, "word recognized, but it should not have");
      return false;
    }

    return true;
  }
};

struct non_terminal_standard_recognizers_002 {
  static std::string desc() {
    return "Recognizing a word in a text with a number";
  }

  bool operator()(const program::bus::options &) {

    log::cerr _logger;

    const std::string _text{"abc4efg"};
    bus::word_recognizer _word_recognizer{word_type};

    std::optional<dat::non_terminal_recognition> _res{
        _word_recognizer(_text.begin(), _text.end())};

    if (!_res) {
      TNCT_LOG_ERR(_logger, "word not recognized, but it should");
      return false;
    }

    const std::string _expected{"abc4efg"};
    const std::string _scanned{_res->non_terminal.value};
    if (_scanned != _expected) {
      TNCT_LOG_ERR(_logger, fmt("word scanned is ", _scanned,
                                ", but it should have been ", _expected));
      return false;
    }

    TNCT_LOG_TST(_logger, fmt("word scanned is ", _scanned, " as it should"));

    return true;
  }
};

struct non_terminal_standard_recognizers_003 {
  static std::string desc() {
    return "Scanning not from the beginnig of the text";
  }

  bool operator()(const program::bus::options &) {

    log::cerr _logger;

    const std::string _text{"abc4efg"};
    bus::word_recognizer _word_recognizer{word_type};

    std::string::const_iterator _begin{std::next(_text.begin(), 4)};
    std::optional<dat::non_terminal_recognition> _res{
        _word_recognizer(_begin, _text.end())};

    if (!_res) {
      TNCT_LOG_ERR(_logger, "word not recognized, but it should");
      return false;
    }

    const std::string _expected{"efg"};
    const std::string _scanned{_res->non_terminal.value};
    if (_scanned != _expected) {
      TNCT_LOG_ERR(_logger, fmt("word scanned is ", _scanned,
                                ", but it should have been ", _expected));
      return false;
    }

    TNCT_LOG_TST(_logger, fmt("word scanned is ", _scanned, " as it should"));

    return true;
  }
};

struct non_terminal_standard_recognizers_004 {
  static std::string desc() {
    return "Scanning all non-tokens in a text without spaces";
  }

  bool operator()(const program::bus::options &) {

    const std::string _text{"abc efg"};
    TNCT_LOG_TST(m_logger, fmt("text is '", _text, "'"));

    bus::word_recognizer _word_recognizer{word_type};
    bus::decimal_integer_number_recognizer _decimal_integer_number_recognizer{
        integer_type};

    std::string::const_iterator _ite{_text.begin()};

    std::optional<dat::non_terminal_recognition> _scanned{
        scan(_ite, _text.end(), _word_recognizer, "abc")};
    if (!_scanned) {
      return false;
    }
    _ite = ++_scanned->end;

    _scanned = scan(_scanned->end, _text.end(), _word_recognizer, "efg");
    if (!_scanned) {
      return false;
    }

    // end of text reached

    _ite = ++_scanned->end;

    _scanned = _word_recognizer(_ite, _text.end());
    if (_scanned) {
      TNCT_LOG_TST(m_logger, fmt(_scanned->non_terminal.value,
                                 " was found, but none should have been"));
      return false;
    }
    return true;
  }

private:
  //  template <cpt::non_terminal_recognizer t_recognizer>
  std::optional<dat::non_terminal_recognition>
  scan(std::string::const_iterator p_begin, std::string::const_iterator p_end,
       bus::non_terminal_recognizer p_recognizer, std::string &&p_expected) {

    std::string::const_iterator _ite{p_begin};

    std::optional<dat::non_terminal_recognition> _res{
        p_recognizer(_ite, p_end)};

    if (!_res) {
      TNCT_LOG_ERR(m_logger, fmt(p_expected, " not recognized, but it should"));
      return std::nullopt;
    }

    const std::string _scanned{_res->non_terminal.value};

    if (_scanned != p_expected) {
      TNCT_LOG_ERR(m_logger, fmt("word scanned is ", _scanned,
                                 ", but it should have been ", p_expected));
      return std::nullopt;
    }
    TNCT_LOG_TST(m_logger, fmt("word scanned is ", _scanned, " as it should"));

    return _res;
  }

  log::cerr m_logger;
};

struct non_terminal_standard_recognizers_005 {
  static std::string desc() {
    return "real_number_recognizer recognizes only digits-separator-digits";
  }

  bool operator()(const program::bus::options &) {
    log::cerr _logger;

    bus::real_number_recognizer _recognizer{real_type};

    const std::string _valid{"123.45abc"};
    std::optional<dat::non_terminal_recognition> _res{
        _recognizer(_valid.begin(), _valid.end())};

    if (!_res) {
      TNCT_LOG_ERR(_logger, "real number not recognized");
      return false;
    }

    if (_res->non_terminal.value != "123.45") {
      TNCT_LOG_ERR(_logger,
                   fmt("expected 123.45, got ", _res->non_terminal.value));
      return false;
    }

    if (_res->non_terminal.type != real_type) {
      TNCT_LOG_ERR(_logger, "unexpected real number type");
      return false;
    }

    return true;
  }
};

struct non_terminal_standard_recognizers_006 {
  static std::string desc() {
    return "real_number_recognizer rejects incomplete real numbers";
  }

  bool operator()(const program::bus::options &) {
    constexpr dat::type _real_type{100};
    bus::real_number_recognizer _recognizer{_real_type};

    for (const std::string &_text : {std::string{"123"}, std::string{".123"},
                                     std::string{"123."}, std::string{"."}}) {
      if (_recognizer(_text.begin(), _text.end())) {
        return false;
      }
    }

    return true;
  }
};

struct non_terminal_standard_recognizers_007 {
  static std::string desc() {
    return "decimal_integer_number_recognizer stops before first non-digit";
  }

  bool operator()(const program::bus::options &) {
    constexpr dat::type _integer_type{101};
    const std::string _text{"123abc"};
    bus::decimal_integer_number_recognizer _recognizer{_integer_type};

    std::optional<dat::non_terminal_recognition> _res{
        _recognizer(_text.begin(), _text.end())};

    return _res && (_res->non_terminal.value == "123") &&
           (_res->end == std::next(_text.begin(), 3));
  }
};

struct non_terminal_standard_recognizers_008 {
  static std::string desc() {
    return "word_recognizer rejects text that does not start with a letter";
  }

  bool operator()(const program::bus::options &) {
    constexpr dat::type _word_type{102};
    const std::string _text{"_abc"};
    bus::word_recognizer _recognizer{_word_type};
    return !_recognizer(_text.begin(), _text.end());
  }
};

} // namespace tnct::interpreter::tst
#endif
