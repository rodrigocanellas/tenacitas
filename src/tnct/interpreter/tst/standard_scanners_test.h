/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Caellas - rodrigo.caellas at gmail.com

#ifndef TNCT_INTERPRETER_TST_STANTDARD_SCANNERS_TEST_H
#define TNCT_INTERPRETER_TST_STANTDARD_SCANNERS_TEST_H

#include <iterator>
#include <string>
#include <utility>

#include "tnct/format/bus/fmt.h"
#include "tnct/interpreter/bus/standard_scanners.h"
#include "tnct/log/bus/cerr.h"
#include "tnct/log/cpt/macros.h"
#include "tnct/program/bus/options.h"

using tnct::format::bus::fmt;

namespace tnct::interpreter::tst {

struct standard_scanners_000 {
  static std::string desc() { return "Test correct scan"; }

  bool operator()(const program::bus::options &) {

    log::cerr _cerr;

    const std::string _text{"abcdefgh"};
    bus::word_scanner _word_scanner;

    std::pair<bool, std::string::const_iterator> _res{
        _word_scanner(_text.begin())};

    if (!_res.first) {
      TNCT_LOG_ERR(_cerr, "word not recognized, but it should");
      return false;
    }

    const std::string _scanned{_text.begin(), _res.second};
    if (_scanned != _text) {
      TNCT_LOG_ERR(_cerr, fmt("word scanned is ", _scanned,
                              ", but it should have been ", _text));
      return false;
    }

    TNCT_LOG_INF(_cerr, fmt("word scanned is ", _scanned, " as it should"));
    return true;
  }
};

struct standard_scanners_001 {
  static std::string desc() { return "Empty text"; }

  bool operator()(const program::bus::options &) {

    log::cerr _cerr;

    const std::string _text;
    bus::word_scanner _word_scanner;

    std::pair<bool, std::string::const_iterator> _res{
        _word_scanner(_text.begin())};

    if (_res.first) {
      TNCT_LOG_ERR(_cerr, "word recognized, but it should not have");
      return false;
    }

    return true;
  }
};

struct standard_scanners_002 {
  static std::string desc() { return "Word with a number"; }

  bool operator()(const program::bus::options &) {

    log::cerr _cerr;

    const std::string _text{"abc4efg"};
    bus::word_scanner _word_scanner;

    std::pair<bool, std::string::const_iterator> _res{
        _word_scanner(_text.begin())};

    if (!_res.first) {
      TNCT_LOG_ERR(_cerr, "word not recognized, but it should");
      return false;
    }

    const std::string _expected{"abc"};
    const std::string _scanned{_text.begin(), _res.second};
    if (_scanned != _expected) {
      TNCT_LOG_ERR(_cerr, fmt("word scanned is ", _scanned,
                              ", but it should have been ", _expected));
      return false;
    }

    TNCT_LOG_INF(_cerr, fmt("word scanned is ", _scanned, " as it should"));

    return true;
  }
};

struct standard_scanners_003 {
  static std::string desc() {
    return "Scanning not from the beginnig of the text";
  }

  bool operator()(const program::bus::options &) {

    log::cerr _cerr;

    const std::string _text{"abc4efg"};
    bus::word_scanner _word_scanner;

    std::string::const_iterator _begin{std::next(_text.begin(), 4)};
    std::pair<bool, std::string::const_iterator> _res{_word_scanner(_begin)};

    if (!_res.first) {
      TNCT_LOG_ERR(_cerr, "word not recognized, but it should");
      return false;
    }

    const std::string _expected{"efg"};
    const std::string _scanned{_begin, _res.second};
    if (_scanned != _expected) {
      TNCT_LOG_ERR(_cerr, fmt("word scanned is ", _scanned,
                              ", but it should have been ", _expected));
      return false;
    }

    TNCT_LOG_INF(_cerr, fmt("word scanned is ", _scanned, " as it should"));

    return true;
  }
};

} // namespace tnct::interpreter::tst
#endif
