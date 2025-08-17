#ifndef TENACITAS_TST_CROSSWORDS_ASSEMBLER_TEST_H
#define TENACITAS_TST_CROSSWORDS_ASSEMBLER_TEST_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include "tnct/crosswords/bus/internal/assembler.h"
#include "tnct/crosswords/evt/dispatcher.h"
#include "tnct/format/fmt.h"
#include "tnct/log/cerr.h"
#include "tnct/program/options.h"

using namespace std::chrono_literals;

namespace tnct::crosswords::tst
{

struct assembler_test_000
{
  static std::string desc()
  {
    return "Fail to solve grid with one word";
  }

  bool operator()(const program::options &)
  {

    using logger     = log::cerr;
    using dispatcher = crosswords::evt::dispatcher;
    using assembler  = crosswords::bus::internal::assembler<logger, dispatcher>;
    using crosswords::dat::entries;
    using crosswords::dat::index;

    logger     _logger;
    dispatcher _dispatcher{_logger};

    entries _entries{{{"viravira", "expl viravira"}}};

    assembler _solver(_logger, _dispatcher);

    auto _start{std::chrono::high_resolution_clock::now()};
    auto _grid{_solver.start(_entries, index{11}, index{11}, 1s)};
    auto _end{std::chrono::high_resolution_clock::now()};
    std::chrono::duration<double> diff = _end - _start;
    TNCT_LOG_TST(_logger, format::fmt("time: ", diff.count()));
    if (!_grid)
    {
      TNCT_LOG_TST(_logger, "Could not solve, as expected");
      return true;
    }
    TNCT_LOG_ERR(_logger,
                 format::fmt("solved", *_grid, ", but it should have not"));
    return false;
  }
};

struct assembler_test_001
{
  static std::string desc()
  {
    return "Solve grid with two words";
  }

  bool operator()(const program::options &)
  {
    using logger     = log::cerr;
    using dispatcher = crosswords::evt::dispatcher;
    using assembler  = crosswords::bus::internal::assembler<logger, dispatcher>;
    using crosswords::dat::entries;
    using crosswords::dat::index;

    logger     _logger;
    dispatcher _dispatcher{_logger};

    entries _entries{{"viravira", "expl viravira"}, {"exumar", "expl exumar"}};

    assembler _solver(_logger, _dispatcher);

    auto _start{std::chrono::high_resolution_clock::now()};
    auto _grid{_solver.start(_entries, crosswords::dat::index{11},
                             crosswords::dat::index{11}, 1s)};
    auto _end{std::chrono::high_resolution_clock::now()};
    std::chrono::duration<double> diff = _end - _start;
    TNCT_LOG_TST(_logger, format::fmt("time: ", diff.count()));
    if (_grid)
    {
      TNCT_LOG_TST(_logger, format::fmt("SOLVED!", *_grid));
      return true;
    }
    TNCT_LOG_ERR(_logger, "not solved");
    return false;
  }
};

struct assembler_test_002
{
  static std::string desc()
  {
    return "Trying to solve a grid with 18 words";
  }

  bool operator()(const program::options &)
  {

    using logger     = log::cerr;
    using dispatcher = crosswords::evt::dispatcher;
    using assembler  = crosswords::bus::internal::assembler<logger, dispatcher>;
    using crosswords::dat::entries;
    using crosswords::dat::index;

    entries _entries{
        {"viravira", "expl viravira"}, {"exumar", "expl exumar"},
        {"rapina", "expl rapina"},     {"tamara", "expl tamara"},
        {"teatro", "expl teatro"},     {"badalar", "expl badalar"},
        {"farelos", "expl farelos"},   {"afunilar", "expl afunilar"},
        {"sibliar", "expl sibliar"},   {"renovar", "expl renovar"},
        {"lesante", "expl lesante"},   {"sideral", "expl sideral"},
        {"salutar", "expl salutar"},   {"aguipa", "expl aguipa"},
        {"aresta", "expl aresta"},     {"avivar", "expl avivar"},
        {"crepom", "expl crepom"},     {"debute", "expl debute"}};

    logger     _logger;
    dispatcher _dispatcher{_logger};

    assembler _solver(_logger, _dispatcher);

    auto _start{std::chrono::high_resolution_clock::now()};
    auto _grid{_solver.start(_entries, crosswords::dat::index{11},
                             crosswords::dat::index{11}, 2s)};
    auto _end{std::chrono::high_resolution_clock::now()};
    std::chrono::duration<double> diff = _end - _start;
    TNCT_LOG_TST(_logger, format::fmt("time: ", diff.count()));
    if (_grid)
    {
      TNCT_LOG_TST(_logger,
                   format::fmt("SOLVED!!! permutation ",
                               _grid->get_permutation_number(), *_grid));
      return true;
    }
    TNCT_LOG_ERR(_logger, "Could not solve... 8(");
    return false;
  }
};

} // namespace tnct::crosswords::tst

#endif
