#ifndef TENACITAS_TST_CROSSWORDS_ORGANIZER_TEST_H
#define TENACITAS_TST_CROSSWORDS_ORGANIZER_TEST_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include "tnct/crosswords/bus/internal/organizer.h"
#include "tnct/crosswords/evt/dispatcher.h"
#include "tnct/format/fmt.h"
#include "tnct/log/cerr.h"
#include "tnct/program/options.h"

using namespace tnct;

namespace tnct::crosswords::tst
{

struct organizer_test_000
{
  static std::string desc()
  {
    return "Organizes a simple grid";
  }

  bool operator()(const program::options &)
  {
    using logger     = log::cerr;
    using dispatcher = crosswords::evt::dispatcher;

    using organizer = crosswords::bus::internal::organizer<logger, dispatcher>;
    using entries   = crosswords::dat::entries;
    using permutation = crosswords::dat::permutation;
    using grid        = crosswords::dat::grid;
    using index       = crosswords::dat::index;

    logger     _logger;
    dispatcher _dispatcher{_logger};

    _logger.set_deb();

    auto _entries{std::make_shared<entries>(
        entries{{"mouth", "expl 1"}, {"open", "expl 2"}})};

    permutation _permutation;
    _permutation.push_back(_entries->begin());
    _permutation.push_back(std::next(_entries->begin()));

    auto _grid{
        std::make_shared<grid>(_entries, _permutation, index{11}, index{11})};

    organizer _organizer{_logger, _dispatcher};
    if (_organizer(_grid))
    {
      TNCT_LOG_TST(_logger, format::fmt(*_grid));
      return true;
    }
    return false;
  }
};

struct organizer_test_001
{
  static std::string desc()
  {
    return "Fails to organize a grid for 'mouth' and 'never'";
  }

  bool operator()(const program::options &)
  {

    using logger = log::cerr;
    using crosswords::evt::dispatcher;
    using organizer = crosswords::bus::internal::organizer<logger, dispatcher>;
    using crosswords::dat::entries;
    using crosswords::dat::grid;
    using crosswords::dat::index;
    using crosswords::dat::permutation;

    logger     _logger;
    dispatcher _dispatcher{_logger};

    auto _entries{std::make_shared<entries>(
        entries{{"mouth", "expl 1"}, {"never", "expl 3"}})};

    // TNCT_LOG_TST(_logger, format::fmt(*_entries));

    crosswords::dat::permutation _permutation;
    _permutation.push_back(_entries->begin());
    _permutation.push_back(std::next(_entries->begin()));

    auto _grid{
        std::make_shared<grid>(_entries, _permutation, index{11}, index{11})};

    organizer _organize{_logger, _dispatcher};

    if (!_organize(_grid))
    {
      TNCT_LOG_TST(_logger,
                   "It was not possible to organize the grid, as expected");
      return true;
    }
    TNCT_LOG_ERR(
        _logger,
        format::fmt("Grid organized, but it should not have been", *_grid));
    return false;
  }
};

struct organizer_test_002
{
  static std::string desc()
  {
    return "Organizes grid that will require the first word to be repositioned";
  }

  bool operator()(const program::options &)
  {

    using logger = log::cerr;
    using crosswords::evt::grid_create_solved;
    using crosswords::evt::grid_create_stop;
    using crosswords::evt::grid_create_timeout;
    using crosswords::evt::grid_create_unsolved;

    using dispatcher =
        async::dispatcher<logger, grid_create_solved, grid_create_stop,
                          grid_create_timeout, grid_create_unsolved>;

    using organizer = crosswords::bus::internal::organizer<logger, dispatcher>;
    using crosswords::dat::entries;
    using crosswords::dat::grid;
    using crosswords::dat::index;
    using crosswords::dat::orientation;
    using crosswords::dat::permutation;

    logger _logger;
    _logger.set_deb();
    dispatcher _dispatcher(_logger);

    auto _entries{std::make_shared<entries>(
        entries{{"mouth", "expl 1"}, {"voyager", "expl 2"}})};

    permutation _permutation;
    _permutation.push_back(_entries->begin());
    _permutation.push_back(std::next(_entries->begin()));

    auto _grid{
        std::make_shared<grid>(_entries, _permutation, index{5}, index{8})};

    organizer _organize{_logger, _dispatcher};

    if (_organize(_grid))
    {
      TNCT_LOG_TST(_logger, format::fmt(*_grid));

      grid::const_layout_ite _first  = _grid->begin();
      grid::const_layout_ite _second = std::next(_grid->begin());

      return (_first->get_row() == 0) && (_first->get_col() == 1)
             && (_first->get_orientation() == orientation::vert)
             && (_second->get_row() == 1) && (_second->get_col() == 0)
             && (_second->get_orientation() == orientation::hori);
    }
    return false;
  }
};

} // namespace tnct::crosswords::tst

#endif
