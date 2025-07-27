#ifndef TENACITAS_TST_CROSSWORDS_ORGANIZER_TEST_H
#define TENACITAS_TST_CROSSWORDS_ORGANIZER_TEST_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include "tenacitas/src/crosswords/asy/dispatcher.h"
#include "tenacitas/src/crosswords/bus/internal/organizer.h"
#include "tenacitas/src/format/fmt.h"
#include "tenacitas/src/log/cerr.h"
#include "tenacitas/src/program/options.h"

namespace tncts = tenacitas::src;

namespace tenacitas::tst::crosswords {

struct organizer_test_000 {
  static std::string desc() { return "Organizes a simple grid"; }

  bool operator()(const tncts::program::options &) {
    using organizer = tncts::crosswords::bus::internal::organizer<
        tncts::log::cerr, tncts::crosswords::asy::dispatcher>;
    using logger = tncts::log::cerr;
    using dispatcher = tncts::crosswords::asy::dispatcher;
    using entries = tncts::crosswords::dat::entries;
    using permutation = tncts::crosswords::dat::permutation;
    using grid = tncts::crosswords::dat::grid;
    using index = tncts::crosswords::dat::index;

    logger _logger;
    dispatcher _dispatcher{_logger};

    entries _entries{{"mouth", "expl 1"}, {"open", "expl 2"}};

    permutation _permutation;
    _permutation.push_back(_entries.begin());
    _permutation.push_back(std::next(_entries.begin()));
    _permutation.push_back(std::next(_entries.begin(), 2));

    auto _grid{std::make_shared<grid>(_permutation, index{11}, index{11})};

    organizer _organizer{_logger, _dispatcher};
    if (_organizer(_grid)) {
      TNCT_LOG_TST(_logger, tncts::format::fmt(*_grid));
      return true;
    }
    return false;
  }
};

} // namespace tenacitas::tst::crosswords

#endif
