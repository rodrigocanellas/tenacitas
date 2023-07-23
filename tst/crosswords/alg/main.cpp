/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <chrono>
#include <cstdint>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

#include <tenacitas/crosswords/alg/assembler.h>
#include <tenacitas/crosswords/alg/dispatcher.h>
#include <tenacitas/crosswords/typ/grid.h>
#include <tenacitas/log/alg/logger.h>
#include <tenacitas/program/alg/options.h>
#include <tenacitas/test/alg/tester.h>

using namespace tenacitas::lib;
namespace alg {

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

  bool operator()(const program::alg::options &) {
    using namespace crosswords;
    try {
      crosswords::typ::entries _entries{{"open", "expl 1"}};

      crosswords::typ::entries::const_entry_ite _begin{_entries.begin()};

      crosswords::typ::permutation _permutation;
      _permutation.push_back(_begin);

      crosswords::typ::grid _grid(_permutation, crosswords::typ::index{2},
                                  crosswords::typ::index{3});

    } catch (std::exception &_ex) {
      TNCT_LOG_TST(_ex.what());
      return true;
    }

    TNCT_LOG_ERR("There should have been an exception");
    return false;
  }
};

struct test_001 {
  static std::string desc() {
    return "organizing 'entries' with one entry in a 'grid'";
  }

  bool operator()(const program::alg::options &) {
    using namespace crosswords;

    crosswords::typ::entries _entries{{"open", "expl 1"}};

    crosswords::typ::entries::const_entry_ite _begin{_entries.begin()};

    crosswords::typ::permutation _permutation;
    _permutation.push_back(_begin);

    crosswords::typ::grid _grid(_permutation, crosswords::typ::index{11},
                                crosswords::typ::index{8});

    crosswords::bus::internal::first_word_positioner _first_word_positioner;

    bool _stop{false};

    _first_word_positioner(_stop, _grid);

    TNCT_LOG_TST(_grid);

    auto _layout{_grid.begin()};

    return (_layout->get_orientation() == crosswords::typ::orientation::hori) &&
           (_layout->get_row() == 0) && (_layout->get_col() == 0) &&
           _grid.organized();
  }
};

struct test_003 {
  static std::string desc() {
    return "Find intersection between 'open' and 'never'";
  }

  bool operator()(const program::alg::options &) {
    using namespace crosswords;
    bool _stop{false};
    auto _vector =
        crosswords::bus::internal::find_intersections(_stop, "open", "never");
    if (_vector.empty()) {
      TNCT_LOG_ERR("intersect not found");
      return false;
    }

    TNCT_LOG_TST("intersect found: ", print(_vector));
    return (_vector[0] == crosswords::typ::coordinate{1, 2}) &&
           (_vector[1] == crosswords::typ::coordinate{3, 2}) &&
           (_vector[2] == crosswords::typ::coordinate{0, 3});
  }
};

struct test_004 {
  static std::string desc() {
    return "Do not find intersection between 'open' and 'black'";
  }

  bool operator()(const program::alg::options &) {
    using namespace crosswords;
    bool _stop{false};
    auto _vector =
        crosswords::bus::internal::find_intersections(_stop, "open", "black");
    if (!_vector.empty()) {
      TNCT_LOG_ERR("intersect found: ", print(_vector));
      return false;
    }
    TNCT_LOG_TST("intersect not found");
    return true;
  }
};

struct test_005 {
  static std::string desc() {
    return "Find intersection between 'open' and 'old'";
  }

  bool operator()(const program::alg::options &) {
    using namespace crosswords;
    bool _stop{false};
    auto _vector =
        crosswords::bus::internal::find_intersections(_stop, "open", "old");
    if (_vector.empty()) {
      TNCT_LOG_ERR("intersect not found");
      return false;
    }
    TNCT_LOG_TST("intersect found: ", print(_vector));
    return (_vector[0] == crosswords::typ::coordinate{0, 0});
  }
};

struct test_006 {
  static std::string desc() {
    return "Find intersection between 'open' and 'abcn'";
  }

  bool operator()(const program::alg::options &) {
    using namespace crosswords;
    bool _stop{false};
    auto _vector =
        crosswords::bus::internal::find_intersections(_stop, "open", "abcn");
    if (_vector.empty()) {
      TNCT_LOG_ERR("intersect not found");
      return false;
    }
    TNCT_LOG_TST("intersect found: ", print(_vector));
    return (_vector[0] == crosswords::typ::coordinate{3, 3});
  }
};

struct test_007 {
  static std::string desc() {
    return "Check that word 'never' can not be positioned horizontally because "
           "it reaches maximum number of cols";
  }

  bool operator()(const program::alg::options &) {
    using namespace crosswords;

    crosswords::typ::entries _entries{{"open", "expl 1"}, {"never", "expl 2"}};

    crosswords::typ::entries::const_entry_ite _begin{_entries.begin()};

    crosswords::typ::permutation _permutation;
    _permutation.push_back(_begin);
    _permutation.push_back(std::next(_begin));

    crosswords::typ::grid _grid(_permutation, crosswords::typ::index{5},
                                crosswords::typ::index{11});

    _grid.set(_grid.begin(), crosswords::typ::index{1},
              crosswords::typ::index{8}, crosswords::typ::orientation::vert);

    TNCT_LOG_TST(_grid);

    crosswords::typ::grid::layout_ite _layout = std::next(_grid.begin());

    return !crosswords::bus::internal::position_horizontally(
        _grid, {1, 2}, _grid.begin(), _layout);
  }
};

struct test_007a {
  static std::string desc() {
    return "Check that word 'never' can not be positioned horizontally because "
           "first letter will be before column 0";
  }

  bool operator()(const program::alg::options &) {
    using namespace crosswords;

    crosswords::typ::entries _entries{{"open", "expl 1"}, {"never", "expl 2"}};

    crosswords::typ::entries::const_entry_ite _begin{_entries.begin()};

    crosswords::typ::permutation _permutation;
    _permutation.push_back(_begin);
    _permutation.push_back(std::next(_begin));

    crosswords::typ::grid _grid(_permutation, crosswords::typ::index{5},
                                crosswords::typ::index{11});

    _grid.set(_grid.begin(), crosswords::typ::index{0},
              crosswords::typ::index{0}, crosswords::typ::orientation::vert);

    TNCT_LOG_TST(_grid);

    crosswords::typ::grid::layout_ite _layout = std::next(_grid.begin());

    return !crosswords::bus::internal::position_horizontally(
        _grid, {1, 2}, _grid.begin(), _layout);
  }
};

struct test_008 {
  static std::string desc() {
    return "Check that the word 'never' can not be positioned horizontally "
           "because a position it would occupy it is occupied";
  }

  bool operator()(const program::alg::options &) {
    using namespace crosswords;

    crosswords::typ::entries _entries{
        {"open", "expl 1"}, {"never", "expl 2"}, {"extra", "expl 3"}};

    crosswords::typ::entries::const_entry_ite _begin{_entries.begin()};

    crosswords::typ::permutation _permutation;
    _permutation.push_back(_begin);
    _permutation.push_back(std::next(_begin));
    _permutation.push_back(std::next(_begin, 2));

    crosswords::typ::grid _grid(_permutation, crosswords::typ::index{5},
                                crosswords::typ::index{11});

    _grid.set(_grid.begin(), crosswords::typ::index{0},
              crosswords::typ::index{1}, crosswords::typ::orientation::vert);
    _grid.set(std::next(_grid.begin(), 2), crosswords::typ::index{0},
              crosswords::typ::index{2}, crosswords::typ::orientation::vert);

    TNCT_LOG_TST(_grid);

    crosswords::typ::grid::layout_ite _layout = std::next(_grid.begin());

    return !crosswords::bus::internal::position_horizontally(
        _grid, {1, 2}, _grid.begin(), _layout);
  }
};

struct test_009 {
  static std::string desc() {
    return "Check that the word 'never' can be positioned horizontally";
  }

  bool operator()(const program::alg::options &) {
    using namespace crosswords;

    crosswords::typ::entries _entries{{"open", "expl 1"}, {"never", "expl 2"}};

    crosswords::typ::entries::const_entry_ite _begin{_entries.begin()};

    crosswords::typ::permutation _permutation;
    _permutation.push_back(_begin);
    _permutation.push_back(std::next(_begin));

    crosswords::typ::grid _grid(_permutation, crosswords::typ::index{7},
                                crosswords::typ::index{11});

    _grid.set(_grid.begin(), crosswords::typ::index{0},
              crosswords::typ::index{4}, crosswords::typ::orientation::vert);

    TNCT_LOG_TST(_grid);

    crosswords::typ::grid::layout_ite _layout = std::next(_grid.begin());

    if (!crosswords::bus::internal::position_horizontally(
            _grid, {1, 2}, _grid.begin(), _layout)) {
      TNCT_LOG_ERR('\'', _layout->get_word(),
                   "' was not positioned, but it should have");
      return false;
    }

    TNCT_LOG_TST(_grid);
    return true;
  }
};

struct test_010 {
  static std::string desc() {
    return "Check that that word 'never' can not be positioned vertically "
           "because it reaches maximum number of rows";
  }

  bool operator()(const program::alg::options &) {
    using namespace crosswords;

    crosswords::typ::entries _entries{{"open", "expl 1"}, {"never", "expl 2"}};

    crosswords::typ::entries::const_entry_ite _begin{_entries.begin()};

    crosswords::typ::permutation _permutation;
    _permutation.push_back(_begin);
    _permutation.push_back(std::next(_begin));

    crosswords::typ::grid _grid(_permutation, crosswords::typ::index{11},
                                crosswords::typ::index{5});

    _grid.set(_grid.begin(), crosswords::typ::index{8},
              crosswords::typ::index{0}, crosswords::typ::orientation::hori);

    TNCT_LOG_TST(_grid);

    crosswords::typ::grid::layout_ite _layout = std::next(_grid.begin());

    return !crosswords::bus::internal::position_vertically(
        _grid, {1, 2}, _grid.begin(), _layout);
  }
};

struct test_010a {
  static std::string desc() {
    return "Check that that word 'never' can not be positioned vertically "
           "because its first letter would be before 0";
  }

  bool operator()(const program::alg::options &) {
    using namespace crosswords;

    crosswords::typ::entries _entries{{"open", "expl 1"}, {"never", "expl 2"}};

    crosswords::typ::entries::const_entry_ite _begin{_entries.begin()};

    crosswords::typ::permutation _permutation;
    _permutation.push_back(_begin);
    _permutation.push_back(std::next(_begin));

    crosswords::typ::grid _grid(_permutation, crosswords::typ::index{11},
                                crosswords::typ::index{5});

    _grid.set(_grid.begin(), crosswords::typ::index{0},
              crosswords::typ::index{1}, crosswords::typ::orientation::hori);

    TNCT_LOG_TST(_grid);

    crosswords::typ::grid::layout_ite _layout = std::next(_grid.begin());

    return !crosswords::bus::internal::position_vertically(
        _grid, {1, 2}, _grid.begin(), _layout);
  }
};

struct test_011 {
  static std::string desc() {
    return "Check that the word 'never' can not be positioned vertically "
           "because a position it would occupy it is occupied";
  }

  bool operator()(const program::alg::options &) {
    using namespace crosswords;

    crosswords::typ::entries _entries{
        {"open", "expl 1"}, {"never", "expl 2"}, {"extra", "expl 3"}};

    crosswords::typ::entries::const_entry_ite _begin{_entries.begin()};

    crosswords::typ::permutation _permutation;
    _permutation.push_back(_begin);
    _permutation.push_back(std::next(_begin));
    _permutation.push_back(std::next(_begin, 2));

    crosswords::typ::grid _grid(_permutation, crosswords::typ::index{11},
                                crosswords::typ::index{5});

    _grid.set(_grid.begin(), crosswords::typ::index{1},
              crosswords::typ::index{0}, crosswords::typ::orientation::hori);
    _grid.set(std::next(_grid.begin(), 2), crosswords::typ::index{2},
              crosswords::typ::index{0}, crosswords::typ::orientation::hori);

    TNCT_LOG_TST(_grid);

    crosswords::typ::grid::layout_ite _layout = std::next(_grid.begin());

    return !crosswords::bus::internal::position_vertically(
        _grid, {1, 2}, _grid.begin(), _layout);
  }
};

struct test_012 {
  static std::string desc() {
    return "Check that that word 'never' can be positioned vertically";
  }

  bool operator()(const program::alg::options &) {
    using namespace crosswords;

    crosswords::typ::entries _entries{{"open", "expl 1"}, {"never", "expl 2"}};

    crosswords::typ::entries::const_entry_ite _begin{_entries.begin()};

    crosswords::typ::permutation _permutation;
    _permutation.push_back(_begin);
    _permutation.push_back(std::next(_begin));

    crosswords::typ::grid _grid(_permutation, crosswords::typ::index{11},
                                crosswords::typ::index{5});

    _grid.set(_grid.begin(), crosswords::typ::index{1},
              crosswords::typ::index{0}, crosswords::typ::orientation::hori);

    TNCT_LOG_TST(_grid);

    crosswords::typ::grid::layout_ite _layout = std::next(_grid.begin());

    if (!crosswords::bus::internal::position_vertically(
            _grid, {1, 2}, _grid.begin(), _layout)) {
      TNCT_LOG_ERR('\'', _layout->get_word(),
                   "' was not positioned, but it should have");
      return false;
    }
    TNCT_LOG_TST(_grid);
    return true;
  }
};

struct test_013 {
  static std::string desc() {
    return "Position a word vertically in intersection other than the first "
           "with a positioned word";
  }

  bool operator()(const program::alg::options &) {
    using namespace crosswords;
    crosswords::typ::entries _entries{
        {"open", "expl 1"}, {"never", "expl 2"}, {"extra", "dummy"}};

    crosswords::typ::entries::const_entry_ite _begin{_entries.begin()};

    crosswords::typ::permutation _permutation;
    _permutation.push_back(_begin);
    _permutation.push_back(std::next(_begin));
    _permutation.push_back(std::next(_begin, 2));

    crosswords::typ::grid _grid(_permutation, crosswords::typ::index{11},
                                crosswords::typ::index{5});

    crosswords::bus::internal::first_word_positioner _first_word_positioner;

    _grid.set(_grid.begin(), crosswords::typ::index{1},
              crosswords::typ::index{0}, crosswords::typ::orientation::hori);

    _grid.set(std::next(_grid.begin(), 2), crosswords::typ::index{1},
              crosswords::typ::index{2}, crosswords::typ::orientation::vert);

    TNCT_LOG_TST(_grid);
    bool _stop{false};
    if (!crosswords::bus::internal::position(_stop, _grid, _grid.begin(),
                                             std::next(_grid.begin()))) {
      TNCT_LOG_ERR('\'', std::next(_grid.begin(), 1)->get_word(),
                   "' was not positioned, but it should have");
      return false;
    }

    TNCT_LOG_TST(_grid);

    return true;
  }
};

struct test_014 {
  static std::string desc() {
    return "Position a word horizontally in intersection other than the first "
           "with a positioned word";
  }

  bool operator()(const program::alg::options &) {
    using namespace crosswords;
    crosswords::typ::entries _entries{
        {"open", "expl 1"}, {"never", "expl 2"}, {"extra", "dummy"}};

    crosswords::typ::entries::const_entry_ite _begin{_entries.begin()};

    crosswords::typ::permutation _permutation;
    _permutation.push_back(_begin);
    _permutation.push_back(std::next(_begin));
    _permutation.push_back(std::next(_begin, 2));

    crosswords::typ::grid _grid(_permutation, crosswords::typ::index{5},
                                crosswords::typ::index{11});

    _grid.set(_grid.begin(), crosswords::typ::index{0},
              crosswords::typ::index{1}, crosswords::typ::orientation::vert);

    _grid.set(std::next(_grid.begin(), 2), crosswords::typ::index{2},
              crosswords::typ::index{1}, crosswords::typ::orientation::hori);

    TNCT_LOG_TST(_grid);
    bool _stop{false};
    if (!crosswords::bus::internal::position(_stop, _grid, _grid.begin(),
                                             std::next(_grid.begin()))) {
      TNCT_LOG_ERR('\'', std::next(_grid.begin(), 1)->get_word(),
                   "' was not positioned, but it should have");
      return false;
    }

    TNCT_LOG_TST(_grid);

    return true;
  }
};

struct test_015 {
  static std::string desc() { return "Organizes a simple grid"; }

  bool operator()(const program::alg::options &) {
    using namespace crosswords;

    crosswords::typ::entries _entries{
        {"mouth", "expl 1"}, {"open", "expl 2"}, {"never", "expl 3"}};

    crosswords::typ::permutation _permutation;
    _permutation.push_back(_entries.begin());
    _permutation.push_back(std::next(_entries.begin()));
    _permutation.push_back(std::next(_entries.begin(), 2));

    auto _grid{std::make_shared<crosswords::typ::grid>(
        _permutation, crosswords::typ::index{11}, crosswords::typ::index{11})};
    auto _dispatcher{crosswords::alg::dispatcher::create()};
    crosswords::bus::internal::organizer _organize;
    if (_organize(_grid)) {
      TNCT_LOG_TST(*_grid);
      return true;
    }
    return false;
  }
};

struct test_016 {
  static std::string desc() { return "Fails to organizes grid not "; }

  bool operator()(const program::alg::options &) {
    using namespace crosswords;

    crosswords::typ::entries _entries{{"mouth", "expl 1"}, {"never", "expl 3"}};

    TNCT_LOG_TST(_entries);

    crosswords::typ::permutation _permutation;
    _permutation.push_back(_entries.begin());
    _permutation.push_back(std::next(_entries.begin()));

    auto _grid{std::make_shared<crosswords::typ::grid>(
        _permutation, crosswords::typ::index{11}, crosswords::typ::index{11})};

    auto _dispatcher{crosswords::alg::dispatcher::create()};
    crosswords::bus::internal::organizer _organize;

    if (!_organize(_grid)) {
      TNCT_LOG_TST("It was not possible to organize the grid, as expected");
      return true;
    }
    TNCT_LOG_ERR("Grid organized, but it should not have been", _grid);
    return false;
  }
};

struct test_017 {
  static std::string desc() {
    return "Organizes grid that will require the first word to be repositioned";
  }

  bool operator()(const program::alg::options &) {
    using namespace crosswords;

    crosswords::typ::entries _entries{{"mouth", "expl 1"},
                                      {"xoxxxxxx", "expl 2"}};

    crosswords::typ::permutation _permutation;
    _permutation.push_back(_entries.begin());
    _permutation.push_back(std::next(_entries.begin()));

    auto _grid{std::make_shared<crosswords::typ::grid>(
        _permutation, crosswords::typ::index{5}, crosswords::typ::index{8})};

    auto _dispatcher{crosswords::alg::dispatcher::create()};
    crosswords::bus::internal::organizer _organize;

    if (_organize(_grid)) {
      TNCT_LOG_TST(*_grid);

      crosswords::typ::grid::const_layout_ite _first = _grid->begin();
      crosswords::typ::grid::const_layout_ite _second =
          std::next(_grid->begin());

      return (_first->get_row() == 0) && (_first->get_col() == 1) &&
             (_first->get_orientation() ==
              crosswords::typ::orientation::vert) &&
             (_second->get_row() == 1) && (_second->get_col() == 0) &&
             (_second->get_orientation() == crosswords::typ::orientation::hori);
    }
    return false;
  }
};

struct test_018 {
  static std::string desc() {
    return "Fails to organize because first two words do not intersect";
  }

  bool operator()(const program::alg::options &) {
    using namespace crosswords;

    crosswords::typ::entries _entries{{"chat", "expl chat"},
                                      {"debute", "expl debute"},
                                      {"regis", "expl regis"},
                                      {"gases", "expl gases"},
                                      {"exumar", "expl exumar"},
                                      {"dias", "expl dias"},
                                      {"pai", "expl pai"},
                                      {"lesante", "expl lesante"},
                                      {"afunilar", "expl afunilar"},
                                      {"ma", "expl ma"},
                                      {"afunilar", "expl afunilar"},
                                      {"atoba", "expl atoba"},
                                      {"ot", "expl ot"},
                                      {"viravira", "expl viravira"},
                                      {"sideral", "expl sideral"},
                                      {"gim", "expl gim"},
                                      {"oval", "expl oval"},
                                      {"rapina", "expl rapina"},
                                      {"lados", "expl lados"},
                                      {"rotor", "expl rotor"},
                                      {"aresta", "aresta"},
                                      {"poxa", "expl poxa"},

                                      {"crepom", "expl crepom"},
                                      {"hexa", "expl hexa"},
                                      {"aguipa ", "expl aguipa"},
                                      {"tim", "expl tim"},
                                      {"salutar", "expl salutar"},
                                      {"renovar", "expl renovar"},
                                      {"eg", "expl eg"},
                                      {"badalar", "expl badalar"},
                                      {"usina", "expl usina"},
                                      {"teatro", "expl teatro"},
                                      {"esse", "expl esse"},
                                      {"sola", "expl sola"},
                                      {"avivar", "expl avivar"},
                                      {"idade", "expl idade"},
                                      {"farelos", "expl farelos"},
                                      {"st", "expl st"},
                                      {"sibliar", "expl sibliar"},
                                      {"pop", "expl pop"},
                                      {"agito", "expl agito"},
                                      {"inox", "expl inox"},
                                      {"tamara", "expl tamara"}

    };

    crosswords::typ::permutation _permutation;
    for (crosswords::typ::entries::const_entry_ite _entry = _entries.begin();
         _entry != _entries.end(); ++_entry) {
      _permutation.push_back(_entry);
    }

    auto _grid{std::make_shared<crosswords::typ::grid>(
        _permutation, crosswords::typ::index{11}, crosswords::typ::index{11})};

    auto _dispatcher{crosswords::alg::dispatcher::create()};
    crosswords::bus::internal::organizer _organize;

    if (!_organize(_grid)) {

      TNCT_LOG_TST(
          "It was not be possible to organize because two first words do not "
          "match, as expected");

      return true;
    }
    TNCT_LOG_ERR(
        "It should not be possible to organize because two first words do not "
        "match");
    return false;
  }
};

struct test_019 {
  static std::string desc() {
    return "Organizes a more complex grid, where it will be necessary to "
           "change first word position";
  }

  bool operator()(const program::alg::options &) {
    using namespace crosswords;

    crosswords::typ::entries _entries{
        {"chat", "expl chat"},
        {"crepom", "expl crepom"},
        {"debute", "expl debute"}
        //        ,
        //                          {"regis", "expl regis"},
        //                          {"gases", "expl gases"},
        //                          {"exumar", "expl exumar"},
        //                          {"dias", "expl dias"},
        //                          {"pai", "expl pai"},
        //                          {"lesante", "expl lesante"},
        //                          {"afunilar", "expl afunilar"},
        //                          {"ma", "expl ma"},
        //                          {"afunilar", "expl afunilar"},
        //                          {"atoba", "expl atoba"},
        //                          {"ot", "expl ot"},
        //                          {"viravira", "expl viravira"},
        //                          {"sideral", "expl sideral"},
        //                          {"gim", "expl gim"},
        //                          {"oval", "expl oval"},
        //                          {"rapina", "expl rapina"},
        //                          {"lados", "expl lados"},
        //                          {"rotor", "expl rotor"},
        //                          {"aresta", "aresta"},
        //                          {"poxa", "expl poxa"},

        //                          {"hexa", "expl hexa"},
        //                          {"aguipa ", "expl aguipa"},
        //                          {"tim", "expl tim"},
        //                          {"salutar", "expl salutar"},
        //                          {"renovar", "expl renovar"},
        //                          {"eg", "expl eg"},
        //                          {"badalar", "expl badalar"},
        //                          {"usina", "expl usina"},
        //                          {"teatro", "expl teatro"},
        //                          {"esse", "expl esse"},
        //                          {"sola", "expl sola"},
        //                          {"avivar", "expl avivar"},
        //                          {"idade", "expl idade"},
        //                          {"farelos", "expl farelos"},
        //                          {"st", "expl st"},
        //                          {"sibliar", "expl sibliar"},
        //                          {"pop", "expl pop"},
        //                          {"agito", "expl agito"},
        //                          {"inox", "expl inox"},
        //                          {"tamara", "expl tamara"}

    };

    crosswords::typ::permutation _permutation;
    for (crosswords::typ::entries::const_entry_ite _entry = _entries.begin();
         _entry != _entries.end(); ++_entry) {
      _permutation.push_back(_entry);
    }

    auto _grid{std::make_shared<crosswords::typ::grid>(
        _permutation, crosswords::typ::index{11}, crosswords::typ::index{11})};

    auto _dispatcher{crosswords::alg::dispatcher::create()};
    crosswords::bus::internal::organizer _organize;

    if (!_organize(_grid)) {

      TNCT_LOG_ERR("It should be possible to organize");
      return false;
    }
    TNCT_LOG_TST("FINAL GRID", *_grid);

    return true;
  }
};

struct test_020 {
  static std::string desc() {
    return "Sorts 'entries' by size, from longest to shortest";
  }

  bool operator()(const program::alg::options &) {
    using namespace crosswords;

    crosswords::typ::entries _entries{{"chat", "expl chat"},
                                      {"crepom", "expl crepom"},
                                      {"debute", "expl debute"},
                                      {"regis", "expl regis"},
                                      {"gases", "expl gases"},
                                      {"exumar", "expl exumar"},
                                      {"dias", "expl dias"},
                                      {"pai", "expl pai"},
                                      {"lesante", "expl lesante"},
                                      {"ma", "expl ma"},
                                      {"afunilar", "expl afunilar"},
                                      {"atoba", "expl atoba"},
                                      {"ot", "expl ot"},
                                      {"viravira", "expl viravira"},
                                      {"sideral", "expl sideral"},
                                      {"gim", "expl gim"},
                                      {"oval", "expl oval"},
                                      {"rapina", "expl rapina"},
                                      {"lados", "expl lados"},
                                      {"rotor", "expl rotor"},
                                      {"aresta", "aresta"},
                                      {"poxa", "expl poxa"},
                                      {"hexa", "expl hexa"},
                                      {"aguipa", "expl aguipa"},
                                      {"tim", "expl tim"},
                                      {"salutar", "expl salutar"},
                                      {"renovar", "expl renovar"},
                                      {"eg", "expl eg"},
                                      {"badalar", "expl badalar"},
                                      {"usina", "expl usina"},
                                      {"teatro", "expl teatro"},
                                      {"esse", "expl esse"},
                                      {"sola", "expl sola"},
                                      {"avivar", "expl avivar"},
                                      {"idade", "expl idade"},
                                      {"farelos", "expl farelos"},
                                      {"st", "expl st"},
                                      {"sibliar", "expl sibliar"},
                                      {"pop", "expl pop"},
                                      {"agito", "expl agito"},
                                      {"inox", "expl inox"},
                                      {"tamara", "expl tamara"}

    };

    TNCT_LOG_TST("entries = ", _entries);

    crosswords::bus::internal::sort_entries(_entries);

    TNCT_LOG_TST("entries = ", _entries);

    crosswords::typ::entries::const_entry_ite _entry = _entries.begin();

    if ((!check(_entry++, "eg")) || (!check(_entry++, "ma")) ||
        (!check(_entry++, "ot")) || (!check(_entry++, "st")) ||
        (!check(_entry++, "gim")) || (!check(_entry++, "pai")) ||
        (!check(_entry++, "pop")) || (!check(_entry++, "tim")) ||
        (!check(_entry++, "chat")) || (!check(_entry++, "dias")) ||
        (!check(_entry++, "esse")) || (!check(_entry++, "hexa")) ||
        (!check(_entry++, "inox")) || (!check(_entry++, "oval")) ||
        (!check(_entry++, "poxa")) || (!check(_entry++, "sola")) ||
        (!check(_entry++, "agito")) || (!check(_entry++, "atoba")) ||
        (!check(_entry++, "gases")) || (!check(_entry++, "idade")) ||
        (!check(_entry++, "lados")) || (!check(_entry++, "regis")) ||
        (!check(_entry++, "rotor")) || (!check(_entry++, "usina")) ||
        (!check(_entry++, "aguipa")) || (!check(_entry++, "aresta")) ||
        (!check(_entry++, "avivar")) || (!check(_entry++, "crepom")) ||
        (!check(_entry++, "debute")) || (!check(_entry++, "exumar")) ||
        (!check(_entry++, "rapina")) || (!check(_entry++, "tamara")) ||
        (!check(_entry++, "teatro")) || (!check(_entry++, "badalar")) ||
        (!check(_entry++, "farelos")) || (!check(_entry++, "lesante")) ||
        (!check(_entry++, "renovar")) || (!check(_entry++, "salutar")) ||
        (!check(_entry++, "sibliar")) || (!check(_entry++, "sideral")) ||
        (!check(_entry++, "afunilar")) || (!check(_entry++, "viravira"))) {
      return false;
    }

    return true;
  }

private:
  bool check(crosswords::typ::entries::const_entry_ite p_entry,
             crosswords::typ::word &&p_word) {
    if (p_entry->get_word() != p_word) {
      TNCT_LOG_ERR("should be '", p_word, "', but it is '", p_entry->get_word(),
                   '\'');
      return false;
    }
    return true;
  }
};

struct test_021 {
  static std::string desc() {
    return "Fails to organize a very complex grid because first and second "
           "words do not intersect";
  }

  bool operator()(const program::alg::options &) {
    using namespace crosswords;

    crosswords::typ::entries _entries{{"chat", "expl chat"},
                                      {"crepom", "expl crepom"},
                                      {"debute", "expl debute"},
                                      {"regis", "expl regis"},
                                      {"gases", "expl gases"},
                                      {"exumar", "expl exumar"},
                                      {"dias", "expl dias"},
                                      {"pai", "expl pai"},
                                      {"lesante", "expl lesante"},
                                      {"ma", "expl ma"},
                                      {"afunilar", "expl afunilar"},
                                      {"atoba", "expl atoba"},
                                      {"ot", "expl ot"},
                                      {"viravira", "expl viravira"},
                                      {"sideral", "expl sideral"},
                                      {"gim", "expl gim"},
                                      {"oval", "expl oval"},
                                      {"rapina", "expl rapina"},
                                      {"lados", "expl lados"},
                                      {"rotor", "expl rotor"},
                                      {"aresta", "aresta"},
                                      {"poxa", "expl poxa"},
                                      {"hexa", "expl hexa"},
                                      {"aguipa", "expl aguipa"},
                                      {"tim", "expl tim"},
                                      {"salutar", "expl salutar"},
                                      {"renovar", "expl renovar"},
                                      {"eg", "expl eg"},
                                      {"badalar", "expl badalar"},
                                      {"usina", "expl usina"},
                                      {"teatro", "expl teatro"},
                                      {"esse", "expl esse"},
                                      {"sola", "expl sola"},
                                      {"avivar", "expl avivar"},
                                      {"idade", "expl idade"},
                                      {"farelos", "expl farelos"},
                                      {"st", "expl st"},
                                      {"sibliar", "expl sibliar"},
                                      {"pop", "expl pop"},
                                      {"agito", "expl agito"},
                                      {"inox", "expl inox"},
                                      {"tamara", "expl tamara"}

    };

    crosswords::bus::internal::sort_entries(_entries);

    crosswords::typ::permutation _permutation;
    for (crosswords::typ::entries::const_entry_ite _entry = _entries.begin();
         _entry != _entries.end(); ++_entry) {
      _permutation.push_back(_entry);
    }

    auto _grid{std::make_shared<crosswords::typ::grid>(
        _permutation, crosswords::typ::index{11}, crosswords::typ::index{11})};

    auto _dispatcher{crosswords::alg::dispatcher::create()};
    crosswords::bus::internal::organizer _organize;

    if (!_organize(_grid)) {
      TNCT_LOG_TST("Grid not organized, as expected", _grid);
      return true;
    }

    TNCT_LOG_ERR(
        "It should not be possible to organize, but grid is organized: ",
        *_grid);

    return false;
  }
};

struct test_023 {
  static std::string desc() {
    return "Fail to solve grid with one word and one thread";
  }

  bool operator()(const program::alg::options &) {
    using namespace crosswords;

    crosswords::typ::entries _entries{{"viravira", "expl viravira"}};

    crosswords::bus::assembler _solver(crosswords::alg::dispatcher::create());

    auto _start{std::chrono::high_resolution_clock::now()};
    std::shared_ptr<crosswords::typ::grid> _grid{_solver.start(
        _entries, crosswords::typ::index{11}, crosswords::typ::index{11}, 1)};
    auto _end{std::chrono::high_resolution_clock::now()};
    std::chrono::duration<double> diff = _end - _start;
    TNCT_LOG_TST("time: ", diff.count());
    if (!_grid) {
      TNCT_LOG_TST("Could not solve, as expected");
      return true;
    }
    TNCT_LOG_ERR("solved", *_grid, ", but it should have not");
    return false;
  }
};

struct test_024 {
  static std::string desc() {
    return "Fail to solve grid one word and 2 threads";
  }

  bool operator()(const program::alg::options &) {
    using namespace crosswords;

    crosswords::typ::entries _entries{{"viravira", "expl viravira"}};

    crosswords::bus::assembler _solver(crosswords::alg::dispatcher::create());

    auto _start{std::chrono::high_resolution_clock::now()};
    std::shared_ptr<crosswords::typ::grid> _grid{_solver.start(
        _entries, crosswords::typ::index{11}, crosswords::typ::index{11}, 2)};
    auto _end{std::chrono::high_resolution_clock::now()};
    std::chrono::duration<double> diff = _end - _start;
    TNCT_LOG_TST("time: ", diff.count());
    if (!_grid) {
      TNCT_LOG_TST("Could not solve, as expected");
      return true;
    }
    TNCT_LOG_ERR("solved", *_grid, ", but it should have not");
    return false;
  }
};

struct test_025 {
  static std::string desc() { return "Solve grid with two words and 1 thread"; }

  bool operator()(const program::alg::options &) {
    using namespace crosswords;

    crosswords::typ::entries _entries{{"viravira", "expl viravira"},
                                      {"exumar", "expl exumar"}};

    crosswords::bus::assembler _solver(crosswords::alg::dispatcher::create());

    auto _start{std::chrono::high_resolution_clock::now()};
    std::shared_ptr<crosswords::typ::grid> _grid{_solver.start(
        _entries, crosswords::typ::index{11}, crosswords::typ::index{11}, 1)};
    auto _end{std::chrono::high_resolution_clock::now()};
    std::chrono::duration<double> diff = _end - _start;
    TNCT_LOG_TST("time: ", diff.count());
    if (_grid) {
      TNCT_LOG_TST("SOLVED!", *_grid);
      return true;
    }
    TNCT_LOG_ERR("not solved");
    return false;
  }
};

struct test_026 {
  static std::string desc() { return "Solve grid with two words and 2 thread"; }

  bool operator()(const program::alg::options &) {
    using namespace crosswords;

    crosswords::typ::entries _entries{{"viravira", "expl viravira"},
                                      {"exumar", "expl exumar"}};

    crosswords::bus::assembler _solver(crosswords::alg::dispatcher::create());

    auto _start{std::chrono::high_resolution_clock::now()};
    std::shared_ptr<crosswords::typ::grid> _grid{_solver.start(
        _entries, crosswords::typ::index{11}, crosswords::typ::index{11}, 2)};
    auto _end{std::chrono::high_resolution_clock::now()};
    std::chrono::duration<double> diff = _end - _start;
    TNCT_LOG_TST("time: ", diff.count());
    if (_grid) {
      TNCT_LOG_TST("SOLVED!", *_grid);
      return true;
    }
    TNCT_LOG_ERR("not solved");
    return false;
  }
};

struct test_027 {
  static std::string desc() {
    return "Trying to solve a grid with 18 words and 5 threads ";
    // time: 0.31991967900000001
  }

  bool operator()(const program::alg::options &) {
    using namespace crosswords;

    //    crosswords::typ::entries _entries{{"chat", "expl chat"},
    //                          {"crepom", "expl crepom"},
    //                          {"debute", "expl debute"},
    //                          {"regis", "expl regis"},
    //                          {"gases", "expl gases"},
    //                          {"exumar", "expl exumar"},
    //                          {"dias", "expl dias"},
    //                          {"pai", "expl pai"},
    //                          {"lesante", "expl lesante"},
    //                          {"ma", "expl ma"},
    //                          {"afunilar", "expl afunilar"},
    //                          {"atoba", "expl atoba"},
    //                          {"ot", "expl ot"},
    //                          {"viravira", "expl viravira"},
    //                          {"sideral", "expl sideral"},
    //                          {"gim", "expl gim"},
    //                          {"oval", "expl oval"},
    //                          {"rapina", "expl rapina"},
    //                          {"lados", "expl lados"},
    //                          {"rotor", "expl rotor"},
    //                          {"aresta", "expl aresta"},
    //                          {"poxa", "expl poxa"},
    //                          {"hexa", "expl hexa"},
    //                          {"aguipa", "expl aguipa"},
    //                          {"tim", "expl tim"},
    //                          {"salutar", "expl salutar"},
    //                          {"renovar", "expl renovar"},
    //                          {"eg", "expl eg"},
    //                          {"badalar", "expl badalar"},
    //                          {"usina", "expl usina"},
    //                          {"teatro", "expl teatro"},
    //                          {"esse", "expl esse"},
    //                          {"sola", "expl sola"},
    //                          {"avivar", "expl avivar"},
    //                          {"idade", "expl idade"},
    //                          {"farelos", "expl farelos"},
    //                          {"st", "expl st"},
    //                          {"sibliar", "expl sibliar"},
    //                          {"pop", "expl pop"},
    //                          {"agito", "expl agito"},
    //                          {"inox", "expl inox"},
    //                          {"tamara", "expl tamara"}};

    crosswords::typ::entries _entries{
        {"viravira", "expl viravira"}, {"exumar", "expl exumar"},
        {"rapina", "expl rapina"},     {"tamara", "expl tamara"},
        {"teatro", "expl teatro"},     {"badalar", "expl badalar"},
        {"farelos", "expl farelos"},   {"afunilar", "expl afunilar"},
        {"sibliar", "expl sibliar"},   {"renovar", "expl renovar"},
        {"lesante", "expl lesante"},   {"sideral", "expl sideral"},
        {"salutar", "expl salutar"},   {"aguipa", "expl aguipa"},
        {"aresta", "expl aresta"},     {"avivar", "expl avivar"},
        {"crepom", "expl crepom"},     {"debute", "expl debute"}/*,
        {"usina", "expl usina"}*/};

    /*    aguipa aresta avivar crepom debute exumar rapina tamara teatro
    badalar
     * farelos viravira afunilar sibliar renovar lesante sideral salutar*/

    crosswords::bus::assembler _solver(crosswords::alg::dispatcher::create());

    auto _start{std::chrono::high_resolution_clock::now()};
    std::shared_ptr<crosswords::typ::grid> _grid{_solver.start(
        _entries, crosswords::typ::index{11}, crosswords::typ::index{11}, 2)};
    auto _end{std::chrono::high_resolution_clock::now()};
    std::chrono::duration<double> diff = _end - _start;
    TNCT_LOG_TST("time: ", diff.count());
    if (_grid) {
      TNCT_LOG_TST("SOLVED!!! permutation ", _grid->get_permutation_number(),
                   *_grid);
      return true;
    }
    TNCT_LOG_ERR("Could not solve... 8(");
    return false;
  }
};

struct test_028 {
  static std::string desc() {
    return "Trying to solve a grid with 18 words and 20 threads ";
    // time: 0.251665574
  }

  bool operator()(const program::alg::options &) {
    using namespace crosswords;

    //    crosswords::typ::entries _entries{{"chat", "expl chat"},
    //                          {"crepom", "expl crepom"},
    //                          {"debute", "expl debute"},
    //                          {"regis", "expl regis"},
    //                          {"gases", "expl gases"},
    //                          {"exumar", "expl exumar"},
    //                          {"dias", "expl dias"},
    //                          {"pai", "expl pai"},
    //                          {"lesante", "expl lesante"},
    //                          {"ma", "expl ma"},
    //                          {"afunilar", "expl afunilar"},
    //                          {"atoba", "expl atoba"},
    //                          {"ot", "expl ot"},
    //                          {"viravira", "expl viravira"},
    //                          {"sideral", "expl sideral"},
    //                          {"gim", "expl gim"},
    //                          {"oval", "expl oval"},
    //                          {"rapina", "expl rapina"},
    //                          {"lados", "expl lados"},
    //                          {"rotor", "expl rotor"},
    //                          {"aresta", "expl aresta"},
    //                          {"poxa", "expl poxa"},
    //                          {"hexa", "expl hexa"},
    //                          {"aguipa", "expl aguipa"},
    //                          {"tim", "expl tim"},
    //                          {"salutar", "expl salutar"},
    //                          {"renovar", "expl renovar"},
    //                          {"eg", "expl eg"},
    //                          {"badalar", "expl badalar"},
    //                          {"usina", "expl usina"},
    //                          {"teatro", "expl teatro"},
    //                          {"esse", "expl esse"},
    //                          {"sola", "expl sola"},
    //                          {"avivar", "expl avivar"},
    //                          {"idade", "expl idade"},
    //                          {"farelos", "expl farelos"},
    //                          {"st", "expl st"},
    //                          {"sibliar", "expl sibliar"},
    //                          {"pop", "expl pop"},
    //                          {"agito", "expl agito"},
    //                          {"inox", "expl inox"},
    //                          {"tamara", "expl tamara"}};

    crosswords::typ::entries _entries{
            {"viravira", "expl viravira"}, {"exumar", "expl exumar"},
            {"rapina", "expl rapina"},     {"tamara", "expl tamara"},
            {"teatro", "expl teatro"},     {"badalar", "expl badalar"},
            {"farelos", "expl farelos"},   {"afunilar", "expl afunilar"},
            {"sibliar", "expl sibliar"},   {"renovar", "expl renovar"},
            {"lesante", "expl lesante"},   {"sideral", "expl sideral"},
            {"salutar", "expl salutar"},   {"aguipa", "expl aguipa"},
            {"aresta", "expl aresta"},     {"avivar", "expl avivar"},
            {"crepom", "expl crepom"},     {"debute", "expl debute"}/*,
        {"usina", "expl usina"}*/};

    crosswords::bus::assembler _solver(crosswords::alg::dispatcher::create());

    auto _start{std::chrono::high_resolution_clock::now()};
    std::shared_ptr<crosswords::typ::grid> _grid{_solver.start(
        _entries, crosswords::typ::index{11}, crosswords::typ::index{11}, 20)};
    auto _end{std::chrono::high_resolution_clock::now()};
    std::chrono::duration<double> diff = _end - _start;
    TNCT_LOG_TST("time: ", diff.count());
    if (_grid) {
      TNCT_LOG_TST("SOLVED!!! permutation ", _grid->get_permutation_number(),
                   *_grid);
      return true;
    }
    TNCT_LOG_ERR("Could not solve... 8(");
    return false;
  }
};

struct test_029 {
  static std::string desc() {
    return "Trying to solve a grid with 19 words and 20 threads ";
  }

  bool operator()(const program::alg::options &) {
    using namespace crosswords;

    //    crosswords::typ::entries _entries{{"chat", "expl chat"},
    //                          {"crepom", "expl crepom"},
    //                          {"debute", "expl debute"},
    //                          {"regis", "expl regis"},
    //                          {"gases", "expl gases"},
    //                          {"exumar", "expl exumar"},
    //                          {"dias", "expl dias"},
    //                          {"pai", "expl pai"},
    //                          {"lesante", "expl lesante"},
    //                          {"ma", "expl ma"},
    //                          {"afunilar", "expl afunilar"},
    //                          {"atoba", "expl atoba"},
    //                          {"ot", "expl ot"},
    //                          {"viravira", "expl viravira"},
    //                          {"sideral", "expl sideral"},
    //                          {"gim", "expl gim"},
    //                          {"oval", "expl oval"},
    //                          {"rapina", "expl rapina"},
    //                          {"lados", "expl lados"},
    //                          {"rotor", "expl rotor"},
    //                          {"aresta", "expl aresta"},
    //                          {"pox a", "expl poxa"},
    //                          {"hexa", "expl hexa"},
    //                          {"aguipa", "expl aguipa"},
    //                          {"tim", "expl tim"},
    //                          {"salutar", "expl salutar"},
    //                          {"renovar", "expl renovar"},
    //                          {"eg", "expl eg"},
    //                          {"badalar", "expl badalar"},
    //                          {"usina", "expl usina"},
    //                          {"teatro", "expl teatro"},
    //                          {"esse", "expl esse"},
    //                          {"sola", "expl sola"},
    //                          {"avivar", "expl avivar"},
    //                          {"idade", "expl idade"},
    //                          {"farelos", "expl farelos"},
    //                          {"st", "expl st"},
    //                          {"sibliar", "expl sibliar"},
    //                          {"pop", "expl pop"},
    //                          {"agito", "expl agito"},
    //                          {"inox", "expl inox"},
    //                          {"tamara", "expl tamara"}};

    crosswords::typ::entries _entries{
        {"viravira", "expl viravira"}, {"exumar", "expl exumar"},
        {"rapina", "expl rapina"},     {"tamara", "expl tamara"},
        {"teatro", "expl teatro"},     {"badalar", "expl badalar"},
        {"farelos", "expl farelos"},   {"afunilar", "expl afunilar"},
        {"sibliar", "expl sibliar"},   {"renovar", "expl renovar"},
        {"lesante", "expl lesante"},   {"sideral", "expl sideral"},
        {"salutar", "expl salutar"},   {"aguipa", "expl aguipa"},
        {"aresta", "expl aresta"},     {"avivar", "expl avivar"},
        {"crepom", "expl crepom"},     {"debute", "expl debute"},
        {"usina", "expl usina"}};

    crosswords::bus::assembler _solver(crosswords::alg::dispatcher::create());

    auto _start{std::chrono::high_resolution_clock::now()};
    std::shared_ptr<crosswords::typ::grid> _grid{_solver.start(
        _entries, crosswords::typ::index{11}, crosswords::typ::index{11}, 20)};
    auto _end{std::chrono::high_resolution_clock::now()};
    std::chrono::duration<double> diff = _end - _start;
    TNCT_LOG_TST("time: ", diff.count());
    if (_grid) {
      TNCT_LOG_TST("SOLVED!!! permutation ", _grid->get_permutation_number(),
                   *_grid);
      return true;
    }
    TNCT_LOG_ERR("Could not solve... 8(");
    return false;
  }
};

struct test_030 {
  static std::string desc() {
    return "Trying to solve a grid with 25 words and 50 threads, with 10000 "
           "attempts ";
  }

  test_030()
      : m_dispatcher(crosswords::alg::dispatcher::create()),
        m_solver(m_dispatcher) {
    using namespace crosswords;
    m_dispatcher->subscribe<crosswords::evt::new_attempt>(
        [](auto p_event) -> void {
          TNCT_LOG_TST("attempt # ", p_event.number);
        });
  }

  bool operator()(const program::alg::options &) {
    using namespace crosswords;

    crosswords::typ::entries _entries{
        {"afunilar", "expl afunilar"}, {"viravira", "expl viravira"},
        {"badalar", "expl badalar"},   {"farelos", "expl farelos"},
        {"lesante", "expl lesante"},   {"renovar", "expl renovar"},
        {"salutar", "expl salutar"},   {"sibliar", "expl sibliar"},
        {"sideral", "expl sideral"},   {"aguipa", "expl aguipa"},
        {"aresta", "expl aresta"},     {"avivar", "expl avivar"},
        {"crepom", "expl crepom"},     {"debute", "expl debute"},
        {"exumar", "expl exumar"},     {"rapina", "expl rapina"},
        {"teatro", "expl teatro"},     {"tamara", "expl tamara"},
        {"usina", "expl usina"},       {"agito", "expl agito"},
        {"atoba", "expl atoba"},       {"gases", "expl gases"},
        {"idade", "expl idade"},       {"lados", "expl lados"},
        {"regis", "expl regis"}};

    auto _start{std::chrono::high_resolution_clock::now()};
    std::shared_ptr<crosswords::typ::grid> _grid{
        m_solver.start(_entries, crosswords::typ::index{11},
                       crosswords::typ::index{11}, 50, 10000)};
    auto _end{std::chrono::high_resolution_clock::now()};
    std::chrono::duration<double> diff = _end - _start;
    TNCT_LOG_TST("time: ", diff.count());
    if (_grid) {
      TNCT_LOG_ERR("solved, but it should not have been");
      return false;
    }
    TNCT_LOG_TST("Not solved, as expected, and number of attempts = ",
                 m_solver.get_num_attempts());
    return true;
  }

private:
  crosswords::alg::dispatcher::ptr m_dispatcher{
      crosswords::alg::dispatcher::create()};
  crosswords::bus::assembler m_solver{m_dispatcher};
};

struct test_031 {
  static std::string desc() {
    return "Defines 100000 attempts to solve a grid with 25 words and 50 "
           "threads, but stops whem it reaches 5000 attempts";
  }

  test_031()
      : m_dispatcher(crosswords::alg::dispatcher::create()),
        m_solver(m_dispatcher) {
    using namespace crosswords;
    m_dispatcher->subscribe<crosswords::evt::new_attempt>(
        [this](auto p_event) -> void {
          if (p_event.number == 5000) {
            TNCT_LOG_TST("p_event->number  = ", p_event.number);
            m_solver.stop();
          }
        });
  }

  bool operator()(const program::alg::options &) {
    using namespace crosswords;

    crosswords::typ::entries _entries{
        {"afunilar", "expl afunilar"}, {"viravira", "expl viravira"},
        {"badalar", "expl badalar"},   {"farelos", "expl farelos"},
        {"lesante", "expl lesante"},   {"renovar", "expl renovar"},
        {"salutar", "expl salutar"},   {"sibliar", "expl sibliar"},
        {"sideral", "expl sideral"},   {"aguipa", "expl aguipa"},
        {"aresta", "expl aresta"},     {"avivar", "expl avivar"},
        {"crepom", "expl crepom"},     {"debute", "expl debute"},
        {"exumar", "expl exumar"},     {"rapina", "expl rapina"},
        {"teatro", "expl teatro"},     {"tamara", "expl tamara"},
        {"usina", "expl usina"},       {"agito", "expl agito"},
        {"atoba", "expl atoba"},       {"gases", "expl gases"},
        {"idade", "expl idade"},       {"lados", "expl lados"},
        {"regis", "expl regis"}};

    auto _start{std::chrono::high_resolution_clock::now()};
    std::shared_ptr<crosswords::typ::grid> _grid{
        m_solver.start(_entries, crosswords::typ::index{11},
                       crosswords::typ::index{11}, 50, 100000)};
    auto _end{std::chrono::high_resolution_clock::now()};
    std::chrono::duration<double> diff = _end - _start;
    TNCT_LOG_TST("time: ", diff.count());
    if (_grid) {
      TNCT_LOG_ERR("solved, but it should not have been");
      return false;
    }
    TNCT_LOG_TST("Not solved, as expected, and number of attempts = ",
                 m_solver.get_num_attempts());
    return true;
  }

private:
  crosswords::alg::dispatcher::ptr m_dispatcher{
      crosswords::alg::dispatcher::create()};
  crosswords::bus::assembler m_solver{m_dispatcher};
};
} // namespace alg
int main(int argc, char **argv) {

  using namespace tenacitas;

  lib::log::alg::set_debug_level();
  //  log::alg::set_file_writer("crosswords");

  lib::test::alg::tester _tester(argc, argv);

  run_test(_tester, alg::test_000);
  run_test(_tester, alg::test_001);
  run_test(_tester, alg::test_003);
  run_test(_tester, alg::test_004);
  run_test(_tester, alg::test_005);
  run_test(_tester, alg::test_006);
  run_test(_tester, alg::test_007);
  run_test(_tester, alg::test_007a);
  run_test(_tester, alg::test_008);
  run_test(_tester, alg::test_009);
  run_test(_tester, alg::test_010);
  run_test(_tester, alg::test_010a);
  run_test(_tester, alg::test_011);
  run_test(_tester, alg::test_012);
  run_test(_tester, alg::test_013);
  run_test(_tester, alg::test_014);
  run_test(_tester, alg::test_015);
  run_test(_tester, alg::test_016);
  run_test(_tester, alg::test_017);
  run_test(_tester, alg::test_018);
  run_test(_tester, alg::test_019);
  run_test(_tester, alg::test_020);
  run_test(_tester, alg::test_021);
  run_test(_tester, alg::test_023);
  run_test(_tester, alg::test_024);
  run_test(_tester, alg::test_025);
  run_test(_tester, alg::test_026);
  run_test(_tester, alg::test_027);
  run_test(_tester, alg::test_028);
  run_test(_tester, alg::test_029);
  run_test(_tester, alg::test_030);
  run_test(_tester, alg::test_031);
}
