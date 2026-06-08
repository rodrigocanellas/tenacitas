/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Caellas - rodrigo.caellas at gmail.com

#ifndef TNCT_INTERPRETER_TST_TOKENS_TEST_H
#define TNCT_INTERPRETER_TST_TOKENS_TEST_H

#include <string>

#include "tnct/format/bus/fmt.h"
#include "tnct/interpreter/dat/token.h"
#include "tnct/interpreter/dat/tokens.h"
#include "tnct/interpreter/dat/type.h"
#include "tnct/log/bus/cerr.h"
#include "tnct/log/cpt/macros.h"
#include "tnct/program/bus/options.h"
// #include "tnct/tuple/output.h"

using tnct::format::bus::fmt;

namespace tnct::interpreter::tst {

struct tokens_000 {
  static std::string desc() { return "Test correct scan"; }

  bool operator()(const program::bus::options &) {

    constexpr std::size_t lexema_size{10};

    using lexema = dat::lexema_t<lexema_size>;
    using tokens = dat::tokens_t<lexema_size>;
    using token = dat::token_t<lexema_size>;

    log::cerr _cerr;

    tokens _tokens;

    constexpr dat::type _comparision_operator = 1;

    _tokens.add({token{"==", _comparision_operator},
                 token{"<", _comparision_operator}});

    TNCT_LOG_INF(_cerr, fmt(_tokens));

    const std::string _text{"<"};

    std::optional<std::pair<tokens::lexema_const_reference, dat::type>> _res{
        _tokens.recognize(_text.begin(), _text.end())};

    if (!_res) {
      TNCT_LOG_ERR(_cerr, "'<' not found, but it should have");
      return false;
    }

    if (_res->first.get() != lexema{"<"}) {
      TNCT_LOG_ERR(_cerr, fmt(_res->first.get(),
                              " was found, but it should have been '<'"));
      return false;
    }

    TNCT_LOG_INF(_cerr, fmt(_res->first.get(), " was found"));
    return true;
  }
};

} // namespace tnct::interpreter::tst
#endif
