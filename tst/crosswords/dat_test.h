/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_TST_CROSSWORDS_TST_DAT_H
#define TENACITAS_TST_CROSSWORDS_TST_DAT_H

#include <memory>
#include <string>

#include "tnct/crosswords/dat/entries.h"
#include "tnct/crosswords/dat/grid.h"
#include "tnct/crosswords/dat/permutation.h"
#include "tnct/log/cerr.h"
#include "tnct/program/options.h"
#include "tnct/traits/log/logger.h"

namespace tenacitas::crosswords::tst::dat {

template <typename t_first, typename t_second>
std::string print(const std::vector<std::pair<t_first, t_second>> &p_vector) {
  std::stringstream _stream;
  _stream << '[';
  for (auto _pair : p_vector) {
    _stream << '(' << _pair.first << ',' << _pair.second << ')';
  }
  _stream << ']';
  return _stream.str();
}

struct test_000 {
  static std::string desc() {
    return "organizing 'entries' with one entry in a 'grid' not big enough";
  }

  bool operator()(const src::program::options &) {
    log::cerr _logger;
    try {
      auto _entries{std::make_shared<crosswords::dat::entries>(
          crosswords::dat::entries{{"open", "expl 1"}})};

      crosswords::dat::entries::const_entry_ite _begin{_entries->begin()};

      crosswords::dat::permutation _permutation;
      _permutation.push_back(_begin);

      crosswords::dat::grid _grid(_entries, _permutation,
                                  crosswords::dat::index{2},
                                  crosswords::dat::index{3});

    } catch (std::exception &_ex) {
      TNCT_LOG_TST(_logger, _ex.what());
      return true;
    }

    TNCT_LOG_ERR(_logger, "There should have been an exception");
    return false;
  }
};

} // namespace tenacitas::crosswords::tst::dat
#endif
