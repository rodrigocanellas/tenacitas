/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

// #include <chrono>
// #include <sstream>
// #include <string>
// #include <vector>

// #include <tnct/crosswords/asy/bus::dispatcher.h>
// #include <tnct/crosswords/dat/grid_creator.h>
// #include <tnct/crosswords/dat/internal/assembler.h>
// #include <tnct/crosswords/mod/grid.h>

#include "tnct/crosswords/tst/assembler_test.h"
#include "tnct/crosswords/tst/dat_test.h"
#include "tnct/crosswords/tst/organizer_test.h"
#include "tnct/tester/test.h"

using namespace tnct;

// struct test_001 {
//   static std::string desc() {
//     return "organizing 'entries' with one entry in a 'grid'";
//   }

//  bool operator()(const lib::sof::program_options &) {

//    crosswords::dat::entries _entries{{"open", "expl 1"}};

//    crosswords::dat::entries::const_entry_ite _begin{_entries.begin()};

//    crosswords::dat::permutation _permutation;
//    _permutation.push_back(_begin);

//    crosswords::dat::grid _grid(_permutation, crosswords::dat::index{11},
//                                crosswords::dat::index{8});

//    crosswords::dat::internal::first_word_positioner _first_word_positioner;

//    bool _stop{false};

//    _first_word_positioner(_stop, _grid);

//    TNCT_LOG_TST(_grid);

//    auto _layout{_grid.begin()};

//    return (_layout->get_orientation() == crosswords::dat::orientation::hori)
//    &&
//           (_layout->get_row() == 0) && (_layout->get_col() == 0) &&
//           _grid.organized();
//  }
//};

// struct test_003 {
//   static std::string desc() {
//     return "Find intersection between 'open' and 'never'";
//   }

//  bool operator()(const lib::sof::program_options &) {

//    bool _stop{false};
//    auto _vector =
//        crosswords::dat::internal::find_intersections(_stop, "open",
//        "never");
//    if (_vector.empty()) {
//      TNCT_LOG_ERR("intersect not found");
//      return false;
//    }

//    TNCT_LOG_TST("intersect found: ", print(_vector));
//    return (_vector[0] == crosswords::dat::coordinate{1, 2}) &&
//           (_vector[1] == crosswords::dat::coordinate{3, 2}) &&
//           (_vector[2] == crosswords::dat::coordinate{0, 3});
//  }
//};

// struct test_004 {
//   static std::string desc() {
//     return "Do not find intersection between 'open' and 'black'";
//   }

//  bool operator()(const lib::sof::program_options &) {

//    bool _stop{false};
//    auto _vector =
//        crosswords::dat::internal::find_intersections(_stop, "open",
//        "black");
//    if (!_vector.empty()) {
//      TNCT_LOG_ERR("intersect found: ", print(_vector));
//      return false;
//    }
//    TNCT_LOG_TST("intersect not found");
//    return true;
//  }
//};

// struct test_005 {
//   static std::string desc() {
//     return "Find intersection between 'open' and 'old'";
//   }

//  bool operator()(const lib::sof::program_options &) {

//    bool _stop{false};
//    auto _vector =
//        crosswords::dat::internal::find_intersections(_stop, "open", "old");
//    if (_vector.empty()) {
//      TNCT_LOG_ERR("intersect not found");
//      return false;
//    }
//    TNCT_LOG_TST("intersect found: ", print(_vector));
//    return (_vector[0] == crosswords::dat::coordinate{0, 0});
//  }
//};

// struct test_006 {
//   static std::string desc() {
//     return "Find intersection between 'open' and 'abcn'";
//   }

//  bool operator()(const lib::sof::program_options &) {

//    bool _stop{false};
//    auto _vector =
//        crosswords::dat::internal::find_intersections(_stop, "open",
//        "abcn");
//    if (_vector.empty()) {
//      TNCT_LOG_ERR("intersect not found");
//      return false;
//    }
//    TNCT_LOG_TST("intersect found: ", print(_vector));
//    return (_vector[0] == crosswords::dat::coordinate{3, 3});
//  }
//};

// struct test_007 {
//   static std::string desc() {
//     return "Check that word 'never' can not be positioned horizontally
//     because "
//            "it reaches maximum number of cols";
//   }

//  bool operator()(const lib::sof::program_options &) {

//    crosswords::dat::entries _entries{{"open", "expl 1"}, {"never", "expl
//    2"}};

//    crosswords::dat::entries::const_entry_ite _begin{_entries.begin()};

//    crosswords::dat::permutation _permutation;
//    _permutation.push_back(_begin);
//    _permutation.push_back(std::next(_begin));

//    crosswords::dat::grid _grid(_permutation, crosswords::dat::index{5},
//                                crosswords::dat::index{11});

//    _grid.set(_grid.begin(), crosswords::dat::index{1},
//              crosswords::dat::index{8}, crosswords::dat::orientation::vert);

//    TNCT_LOG_TST(_grid);

//    crosswords::dat::grid::layout_ite _layout = std::next(_grid.begin());

//    return !crosswords::dat::internal::position_horizontally(
//        _grid, {1, 2}, _grid.begin(), _layout);
//  }
//};

// struct test_007a {
//   static std::string desc() {
//     return "Check that word 'never' can not be positioned horizontally
//     because "
//            "first letter will be before column 0";
//   }

//  bool operator()(const lib::sof::program_options &) {

//    crosswords::dat::entries _entries{{"open", "expl 1"}, {"never", "expl
//    2"}};

//    crosswords::dat::entries::const_entry_ite _begin{_entries.begin()};

//    crosswords::dat::permutation _permutation;
//    _permutation.push_back(_begin);
//    _permutation.push_back(std::next(_begin));

//    crosswords::dat::grid _grid(_permutation, crosswords::dat::index{5},
//                                crosswords::dat::index{11});

//    _grid.set(_grid.begin(), crosswords::dat::index{0},
//              crosswords::dat::index{0}, crosswords::dat::orientation::vert);

//    TNCT_LOG_TST(_grid);

//    crosswords::dat::grid::layout_ite _layout = std::next(_grid.begin());

//    return !crosswords::dat::internal::position_horizontally(
//        _grid, {1, 2}, _grid.begin(), _layout);
//  }
//};

// struct test_008 {
//   static std::string desc() {
//     return "Check that the word 'never' can not be positioned horizontally "
//            "because a position it would occupy it is occupied";
//   }

//  bool operator()(const lib::sof::program_options &) {

//    crosswords::dat::entries _entries{
//        {"open", "expl 1"}, {"never", "expl 2"}, {"extra", "expl 3"}};

//    crosswords::dat::entries::const_entry_ite _begin{_entries.begin()};

//    crosswords::dat::permutation _permutation;
//    _permutation.push_back(_begin);
//    _permutation.push_back(std::next(_begin));
//    _permutation.push_back(std::next(_begin, 2));

//    crosswords::dat::grid _grid(_permutation, crosswords::dat::index{5},
//                                crosswords::dat::index{11});

//    _grid.set(_grid.begin(), crosswords::dat::index{0},
//              crosswords::dat::index{1}, crosswords::dat::orientation::vert);
//    _grid.set(std::next(_grid.begin(), 2), crosswords::dat::index{0},
//              crosswords::dat::index{2}, crosswords::dat::orientation::vert);

//    TNCT_LOG_TST(_grid);

//    crosswords::dat::grid::layout_ite _layout = std::next(_grid.begin());

//    return !crosswords::dat::internal::position_horizontally(
//        _grid, {1, 2}, _grid.begin(), _layout);
//  }
//};

// struct test_009 {
//   static std::string desc() {
//     return "Check that the word 'never' can be positioned horizontally";
//   }

//  bool operator()(const lib::sof::program_options &) {

//    crosswords::dat::entries _entries{{"open", "expl 1"}, {"never", "expl
//    2"}};

//    crosswords::dat::entries::const_entry_ite _begin{_entries.begin()};

//    crosswords::dat::permutation _permutation;
//    _permutation.push_back(_begin);
//    _permutation.push_back(std::next(_begin));

//    crosswords::dat::grid _grid(_permutation, crosswords::dat::index{7},
//                                crosswords::dat::index{11});

//    _grid.set(_grid.begin(), crosswords::dat::index{0},
//              crosswords::dat::index{4}, crosswords::dat::orientation::vert);

//    TNCT_LOG_TST(_grid);

//    crosswords::dat::grid::layout_ite _layout = std::next(_grid.begin());

//    if (!crosswords::dat::internal::position_horizontally(
//            _grid, {1, 2}, _grid.begin(), _layout)) {
//      TNCT_LOG_ERR('\'', _layout->get_word(),
//                   "' was not positioned, but it should have");
//      return false;
//    }

//    TNCT_LOG_TST(_grid);
//    return true;
//  }
//};

// struct test_010 {
//   static std::string desc() {
//     return "Check that that word 'never' can not be positioned vertically "
//            "because it reaches maximum number of rows";
//   }

//  bool operator()(const lib::sof::program_options &) {

//    crosswords::dat::entries _entries{{"open", "expl 1"}, {"never", "expl
//    2"}};

//    crosswords::dat::entries::const_entry_ite _begin{_entries.begin()};

//    crosswords::dat::permutation _permutation;
//    _permutation.push_back(_begin);
//    _permutation.push_back(std::next(_begin));

//    crosswords::dat::grid _grid(_permutation, crosswords::dat::index{11},
//                                crosswords::dat::index{5});

//    _grid.set(_grid.begin(), crosswords::dat::index{8},
//              crosswords::dat::index{0}, crosswords::dat::orientation::hori);

//    TNCT_LOG_TST(_grid);

//    crosswords::dat::grid::layout_ite _layout = std::next(_grid.begin());

//    return !crosswords::dat::internal::position_vertically(
//        _grid, {1, 2}, _grid.begin(), _layout);
//  }
//};

// struct test_010a {
//   static std::string desc() {
//     return "Check that that word 'never' can not be positioned vertically "
//            "because its first letter would be before 0";
//   }

//  bool operator()(const lib::sof::program_options &) {

//    crosswords::dat::entries _entries{{"open", "expl 1"}, {"never", "expl
//    2"}};

//    crosswords::dat::entries::const_entry_ite _begin{_entries.begin()};

//    crosswords::dat::permutation _permutation;
//    _permutation.push_back(_begin);
//    _permutation.push_back(std::next(_begin));

//    crosswords::dat::grid _grid(_permutation, crosswords::dat::index{11},
//                                crosswords::dat::index{5});

//    _grid.set(_grid.begin(), crosswords::dat::index{0},
//              crosswords::dat::index{1}, crosswords::dat::orientation::hori);

//    TNCT_LOG_TST(_grid);

//    crosswords::dat::grid::layout_ite _layout = std::next(_grid.begin());

//    return !crosswords::dat::internal::position_vertically(
//        _grid, {1, 2}, _grid.begin(), _layout);
//  }
//};

// struct test_011 {
//   static std::string desc() {
//     return "Check that the word 'never' can not be positioned vertically "
//            "because a position it would occupy it is occupied";
//   }

//  bool operator()(const lib::sof::program_options &) {

//    crosswords::dat::entries _entries{
//        {"open", "expl 1"}, {"never", "expl 2"}, {"extra", "expl 3"}};

//    crosswords::dat::entries::const_entry_ite _begin{_entries.begin()};

//    crosswords::dat::permutation _permutation;
//    _permutation.push_back(_begin);
//    _permutation.push_back(std::next(_begin));
//    _permutation.push_back(std::next(_begin, 2));

//    crosswords::dat::grid _grid(_permutation, crosswords::dat::index{11},
//                                crosswords::dat::index{5});

//    _grid.set(_grid.begin(), crosswords::dat::index{1},
//              crosswords::dat::index{0}, crosswords::dat::orientation::hori);
//    _grid.set(std::next(_grid.begin(), 2), crosswords::dat::index{2},
//              crosswords::dat::index{0}, crosswords::dat::orientation::hori);

//    TNCT_LOG_TST(_grid);

//    crosswords::dat::grid::layout_ite _layout = std::next(_grid.begin());

//    return !crosswords::dat::internal::position_vertically(
//        _grid, {1, 2}, _grid.begin(), _layout);
//  }
//};

// struct test_012 {
//   static std::string desc() {
//     return "Check that that word 'never' can be positioned vertically";
//   }

//  bool operator()(const lib::sof::program_options &) {

//    crosswords::dat::entries _entries{{"open", "expl 1"}, {"never", "expl
//    2"}};

//    crosswords::dat::entries::const_entry_ite _begin{_entries.begin()};

//    crosswords::dat::permutation _permutation;
//    _permutation.push_back(_begin);
//    _permutation.push_back(std::next(_begin));

//    crosswords::dat::grid _grid(_permutation, crosswords::dat::index{11},
//                                crosswords::dat::index{5});

//    _grid.set(_grid.begin(), crosswords::dat::index{1},
//              crosswords::dat::index{0}, crosswords::dat::orientation::hori);

//    TNCT_LOG_TST(_grid);

//    crosswords::dat::grid::layout_ite _layout = std::next(_grid.begin());

//    if (!crosswords::dat::internal::position_vertically(
//            _grid, {1, 2}, _grid.begin(), _layout)) {
//      TNCT_LOG_ERR('\'', _layout->get_word(),
//                   "' was not positioned, but it should have");
//      return false;
//    }
//    TNCT_LOG_TST(_grid);
//    return true;
//  }
//};

// struct test_013 {
//   static std::string desc() {
//     return "Position a word vertically in intersection other than the first "
//            "with a positioned word";
//   }

//  bool operator()(const lib::sof::program_options &) {

//    crosswords::dat::entries _entries{
//        {"open", "expl 1"}, {"never", "expl 2"}, {"extra", "dummy"}};

//    crosswords::dat::entries::const_entry_ite _begin{_entries.begin()};

//    crosswords::dat::permutation _permutation;
//    _permutation.push_back(_begin);
//    _permutation.push_back(std::next(_begin));
//    _permutation.push_back(std::next(_begin, 2));

//    crosswords::dat::grid _grid(_permutation, crosswords::dat::index{11},
//                                crosswords::dat::index{5});

//    crosswords::dat::internal::first_word_positioner _first_word_positioner;

//    _grid.set(_grid.begin(), crosswords::dat::index{1},
//              crosswords::dat::index{0}, crosswords::dat::orientation::hori);

//    _grid.set(std::next(_grid.begin(), 2), crosswords::dat::index{1},
//              crosswords::dat::index{2}, crosswords::dat::orientation::vert);

//    TNCT_LOG_TST(_grid);
//    bool _stop{false};
//    if (!crosswords::dat::internal::position(_stop, _grid, _grid.begin(),
//                                             std::next(_grid.begin()))) {
//      TNCT_LOG_ERR('\'', std::next(_grid.begin(), 1)->get_word(),
//                   "' was not positioned, but it should have");
//      return false;
//    }

//    TNCT_LOG_TST(_grid);

//    return true;
//  }
//};

// struct test_014 {
//   static std::string desc() {
//     return "Position a word horizontally in intersection other than the first
//     "
//            "with a positioned word";
//   }

//  bool operator()(const lib::sof::program_options &) {

//    crosswords::dat::entries _entries{
//        {"open", "expl 1"}, {"never", "expl 2"}, {"extra", "dummy"}};

//    crosswords::dat::entries::const_entry_ite _begin{_entries.begin()};

//    crosswords::dat::permutation _permutation;
//    _permutation.push_back(_begin);
//    _permutation.push_back(std::next(_begin));
//    _permutation.push_back(std::next(_begin, 2));

//    crosswords::dat::grid _grid(_permutation, crosswords::dat::index{5},
//                                crosswords::dat::index{11});

//    _grid.set(_grid.begin(), crosswords::dat::index{0},
//              crosswords::dat::index{1}, crosswords::dat::orientation::vert);

//    _grid.set(std::next(_grid.begin(), 2), crosswords::dat::index{2},
//              crosswords::dat::index{1}, crosswords::dat::orientation::hori);

//    TNCT_LOG_TST(_grid);
//    bool _stop{false};
//    if (!crosswords::dat::internal::position(_stop, _grid, _grid.begin(),
//                                             std::next(_grid.begin()))) {
//      TNCT_LOG_ERR('\'', std::next(_grid.begin(), 1)->get_word(),
//                   "' was not positioned, but it should have");
//      return false;
//    }

//    TNCT_LOG_TST(_grid);

//    return true;
//  }
//};

// struct test_015 {
//   static std::string desc() { return "Organizes a simple grid"; }

//  bool operator()(const lib::sof::program_options &) {

//    crosswords::dat::entries _entries{
//        {"mouth", "expl 1"}, {"open", "expl 2"}};

//    crosswords::dat::permutation _permutation;
//    _permutation.push_back(_entries.begin());
//    _permutation.push_back(std::next(_entries.begin()));
//    _permutation.push_back(std::next(_entries.begin(), 2));

//    auto _grid{std::make_shared<crosswords::dat::grid>(
//        _permutation, crosswords::dat::index{11},
//        crosswords::dat::index{11})};
//    auto _dispatcher{crosswords::evt::bus::dispatcher::create()};
//    crosswords::dat::internal::organizer _organize;
//    if (_organize(_grid)) {
//      TNCT_LOG_TST(*_grid);
//      return true;
//    }
//    return false;
//  }
// };

// struct test_016 {
//   static std::string desc() { return "Fails to organizes grid not "; }

//  bool operator()(const lib::sof::program_options &) {

//    crosswords::dat::entries _entries{{"mouth", "expl 1"}, {"never", "expl
//    3"}};

//    TNCT_LOG_TST(_entries);

//    crosswords::dat::permutation _permutation;
//    _permutation.push_back(_entries.begin());
//    _permutation.push_back(std::next(_entries.begin()));

//    auto _grid{std::make_shared<crosswords::dat::grid>(
//        _permutation, crosswords::dat::index{11},
//        crosswords::dat::index{11})};

//    auto _dispatcher{crosswords::evt::bus::dispatcher::create()};
//    crosswords::dat::internal::organizer _organize;

//    if (!_organize(_grid)) {
//      TNCT_LOG_TST("It was not possible to organize the grid, as expected");
//      return true;
//    }
//    TNCT_LOG_ERR("Grid organized, but it should not have been", _grid);
//    return false;
//  }
//};

// struct test_017 {
//   static std::string desc() {
//     return "Organizes grid that will require the first word to be
//     repositioned";
//   }

//  bool operator()(const lib::sof::program_options &) {

//    crosswords::dat::entries _entries{{"mouth", "expl 1"},
//                                      {"xoxxxxxx", "expl 2"}};

//    crosswords::dat::permutation _permutation;
//    _permutation.push_back(_entries.begin());
//    _permutation.push_back(std::next(_entries.begin()));

//    auto _grid{std::make_shared<crosswords::dat::grid>(
//        _permutation, crosswords::dat::index{5}, crosswords::dat::index{8})};

//    auto _dispatcher{crosswords::evt::bus::dispatcher::create()};
//    crosswords::dat::internal::organizer _organize;

//    if (_organize(_grid)) {
//      TNCT_LOG_TST(*_grid);

//      crosswords::dat::grid::const_layout_ite _first = _grid->begin();
//      crosswords::dat::grid::const_layout_ite _second =
//          std::next(_grid->begin());

//      return (_first->get_row() == 0) && (_first->get_col() == 1) &&
//             (_first->get_orientation() ==
//              crosswords::dat::orientation::vert) &&
//             (_second->get_row() == 1) && (_second->get_col() == 0) &&
//             (_second->get_orientation() ==
//             crosswords::dat::orientation::hori);
//    }
//    return false;
//  }
//};

// struct test_018 {
//   static std::string desc() {
//     return "Fails to organize because first two words do not intersect";
//   }

//  bool operator()(const lib::sof::program_options &) {

//    crosswords::dat::entries _entries{{"chat", "expl chat"},
//                                      {"debute", "expl debute"},
//                                      {"regis", "expl regis"},
//                                      {"gases", "expl gases"},
//                                      {"exumar", "expl exumar"},
//                                      {"dias", "expl dias"},
//                                      {"pai", "expl pai"},
//                                      {"lesante", "expl lesante"},
//                                      {"afunilar", "expl afunilar"},
//                                      {"ma", "expl ma"},
//                                      {"afunilar", "expl afunilar"},
//                                      {"atoba", "expl atoba"},
//                                      {"ot", "expl ot"},
//                                      {"viravira", "expl viravira"},
//                                      {"sideral", "expl sideral"},
//                                      {"gim", "expl gim"},
//                                      {"oval", "expl oval"},
//                                      {"rapina", "expl rapina"},
//                                      {"lados", "expl lados"},
//                                      {"rotor", "expl rotor"},
//                                      {"aresta", "aresta"},
//                                      {"poxa", "expl poxa"},

//                                      {"crepom", "expl crepom"},
//                                      {"hexa", "expl hexa"},
//                                      {"aguipa ", "expl aguipa"},
//                                      {"tim", "expl tim"},
//                                      {"salutar", "expl salutar"},
//                                      {"renovar", "expl renovar"},
//                                      {"eg", "expl eg"},
//                                      {"badalar", "expl badalar"},
//                                      {"usina", "expl usina"},
//                                      {"teatro", "expl teatro"},
//                                      {"esse", "expl esse"},
//                                      {"sola", "expl sola"},
//                                      {"avivar", "expl avivar"},
//                                      {"idade", "expl idade"},
//                                      {"farelos", "expl farelos"},
//                                      {"st", "expl st"},
//                                      {"sibliar", "expl sibliar"},
//                                      {"pop", "expl pop"},
//                                      {"agito", "expl agito"},
//                                      {"inox", "expl inox"},
//                                      {"tamara", "expl tamara"}

//    };

//    crosswords::dat::permutation _permutation;
//    for (crosswords::dat::entries::const_entry_ite _entry = _entries.begin();
//         _entry != _entries.end(); ++_entry) {
//      _permutation.push_back(_entry);
//    }

//    auto _grid{std::make_shared<crosswords::dat::grid>(
//        _permutation, crosswords::dat::index{11},
//        crosswords::dat::index{11})};

//    auto _dispatcher{crosswords::evt::bus::dispatcher::create()};
//    crosswords::dat::internal::organizer _organize;

//    if (!_organize(_grid)) {

//      TNCT_LOG_TST(
//          "It was not be possible to organize because two first words do not "
//          "match, as expected");

//      return true;
//    }
//    TNCT_LOG_ERR(
//        "It should not be possible to organize because two first words do not
//        " "match");
//    return false;
//  }
//};

// struct test_019 {
//   static std::string desc() {
//     return "Organizes a more complex grid, where it will be necessary to "
//            "change first word position";
//   }

//  bool operator()(const lib::sof::program_options &) {

//    crosswords::dat::entries _entries{
//        {"chat", "expl chat"},
//        {"crepom", "expl crepom"},
//        {"debute", "expl debute"}
//        //        ,
//        //                          {"regis", "expl regis"},
//        //                          {"gases", "expl gases"},
//        //                          {"exumar", "expl exumar"},
//        //                          {"dias", "expl dias"},
//        //                          {"pai", "expl pai"},
//        //                          {"lesante", "expl lesante"},
//        //                          {"afunilar", "expl afunilar"},
//        //                          {"ma", "expl ma"},
//        //                          {"afunilar", "expl afunilar"},
//        //                          {"atoba", "expl atoba"},
//        //                          {"ot", "expl ot"},
//        //                          {"viravira", "expl viravira"},
//        //                          {"sideral", "expl sideral"},
//        //                          {"gim", "expl gim"},
//        //                          {"oval", "expl oval"},
//        //                          {"rapina", "expl rapina"},
//        //                          {"lados", "expl lados"},
//        //                          {"rotor", "expl rotor"},
//        //                          {"aresta", "aresta"},
//        //                          {"poxa", "expl poxa"},

//        //                          {"hexa", "expl hexa"},
//        //                          {"aguipa ", "expl aguipa"},
//        //                          {"tim", "expl tim"},
//        //                          {"salutar", "expl salutar"},
//        //                          {"renovar", "expl renovar"},
//        //                          {"eg", "expl eg"},
//        //                          {"badalar", "expl badalar"},
//        //                          {"usina", "expl usina"},
//        //                          {"teatro", "expl teatro"},
//        //                          {"esse", "expl esse"},
//        //                          {"sola", "expl sola"},
//        //                          {"avivar", "expl avivar"},
//        //                          {"idade", "expl idade"},
//        //                          {"farelos", "expl farelos"},
//        //                          {"st", "expl st"},
//        //                          {"sibliar", "expl sibliar"},
//        //                          {"pop", "expl pop"},
//        //                          {"agito", "expl agito"},
//        //                          {"inox", "expl inox"},
//        //                          {"tamara", "expl tamara"}

//    };

//    crosswords::dat::permutation _permutation;
//    for (crosswords::dat::entries::const_entry_ite _entry = _entries.begin();
//         _entry != _entries.end(); ++_entry) {
//      _permutation.push_back(_entry);
//    }

//    auto _grid{std::make_shared<crosswords::dat::grid>(
//        _permutation, crosswords::dat::index{11},
//        crosswords::dat::index{11})};

//    auto _dispatcher{crosswords::evt::bus::dispatcher::create()};
//    crosswords::dat::internal::organizer _organize;

//    if (!_organize(_grid)) {

//      TNCT_LOG_ERR("It should be possible to organize");
//      return false;
//    }
//    TNCT_LOG_TST("FINAL GRID", *_grid);

//    return true;
//  }
//};

// struct test_020 {
//   static std::string desc() {
//     return "Sorts 'entries' by size, from longest to shortest";
//   }

//  bool operator()(const lib::sof::program_options &) {

//    crosswords::dat::entries _entries{{"chat", "expl chat"},
//                                      {"crepom", "expl crepom"},
//                                      {"debute", "expl debute"},
//                                      {"regis", "expl regis"},
//                                      {"gases", "expl gases"},
//                                      {"exumar", "expl exumar"},
//                                      {"dias", "expl dias"},
//                                      {"pai", "expl pai"},
//                                      {"lesante", "expl lesante"},
//                                      {"ma", "expl ma"},
//                                      {"afunilar", "expl afunilar"},
//                                      {"atoba", "expl atoba"},
//                                      {"ot", "expl ot"},
//                                      {"viravira", "expl viravira"},
//                                      {"sideral", "expl sideral"},
//                                      {"gim", "expl gim"},
//                                      {"oval", "expl oval"},
//                                      {"rapina", "expl rapina"},
//                                      {"lados", "expl lados"},
//                                      {"rotor", "expl rotor"},
//                                      {"aresta", "aresta"},
//                                      {"poxa", "expl poxa"},
//                                      {"hexa", "expl hexa"},
//                                      {"aguipa", "expl aguipa"},
//                                      {"tim", "expl tim"},
//                                      {"salutar", "expl salutar"},
//                                      {"renovar", "expl renovar"},
//                                      {"eg", "expl eg"},
//                                      {"badalar", "expl badalar"},
//                                      {"usina", "expl usina"},
//                                      {"teatro", "expl teatro"},
//                                      {"esse", "expl esse"},
//                                      {"sola", "expl sola"},
//                                      {"avivar", "expl avivar"},
//                                      {"idade", "expl idade"},
//                                      {"farelos", "expl farelos"},
//                                      {"st", "expl st"},
//                                      {"sibliar", "expl sibliar"},
//                                      {"pop", "expl pop"},
//                                      {"agito", "expl agito"},
//                                      {"inox", "expl inox"},
//                                      {"tamara", "expl tamara"}

//    };

//    TNCT_LOG_TST("entries = ", _entries);

//    crosswords::dat::internal::sort_entries(_entries);

//    TNCT_LOG_TST("entries = ", _entries);

//    crosswords::dat::entries::const_entry_ite _entry = _entries.begin();

//    if ((!check(_entry++, "eg")) || (!check(_entry++, "ma")) ||
//        (!check(_entry++, "ot")) || (!check(_entry++, "st")) ||
//        (!check(_entry++, "gim")) || (!check(_entry++, "pai")) ||
//        (!check(_entry++, "pop")) || (!check(_entry++, "tim")) ||
//        (!check(_entry++, "chat")) || (!check(_entry++, "dias")) ||
//        (!check(_entry++, "esse")) || (!check(_entry++, "hexa")) ||
//        (!check(_entry++, "inox")) || (!check(_entry++, "oval")) ||
//        (!check(_entry++, "poxa")) || (!check(_entry++, "sola")) ||
//        (!check(_entry++, "agito")) || (!check(_entry++, "atoba")) ||
//        (!check(_entry++, "gases")) || (!check(_entry++, "idade")) ||
//        (!check(_entry++, "lados")) || (!check(_entry++, "regis")) ||
//        (!check(_entry++, "rotor")) || (!check(_entry++, "usina")) ||
//        (!check(_entry++, "aguipa")) || (!check(_entry++, "aresta")) ||
//        (!check(_entry++, "avivar")) || (!check(_entry++, "crepom")) ||
//        (!check(_entry++, "debute")) || (!check(_entry++, "exumar")) ||
//        (!check(_entry++, "rapina")) || (!check(_entry++, "tamara")) ||
//        (!check(_entry++, "teatro")) || (!check(_entry++, "badalar")) ||
//        (!check(_entry++, "farelos")) || (!check(_entry++, "lesante")) ||
//        (!check(_entry++, "renovar")) || (!check(_entry++, "salutar")) ||
//        (!check(_entry++, "sibliar")) || (!check(_entry++, "sideral")) ||
//        (!check(_entry++, "afunilar")) || (!check(_entry++, "viravira"))) {
//      return false;
//    }

//    return true;
//  }

// private:
//   bool check(crosswords::dat::entries::const_entry_ite p_entry,
//              crosswords::dat::word &&p_word) {
//     if (p_entry->get_word() != p_word) {
//       TNCT_LOG_ERR("should be '", p_word, "', but it is '",
//       p_entry->get_word(),
//                    '\'');
//       return false;
//     }
//     return true;
//   }
// };

// struct test_021 {
//   static std::string desc() {
//     return "Fails to organize a very complex grid because first and second "
//            "words do not intersect";
//   }

//  bool operator()(const lib::sof::program_options &) {

//    crosswords::dat::entries _entries{{"chat", "expl chat"},
//                                      {"crepom", "expl crepom"},
//                                      {"debute", "expl debute"},
//                                      {"regis", "expl regis"},
//                                      {"gases", "expl gases"},
//                                      {"exumar", "expl exumar"},
//                                      {"dias", "expl dias"},
//                                      {"pai", "expl pai"},
//                                      {"lesante", "expl lesante"},
//                                      {"ma", "expl ma"},
//                                      {"afunilar", "expl afunilar"},
//                                      {"atoba", "expl atoba"},
//                                      {"ot", "expl ot"},
//                                      {"viravira", "expl viravira"},
//                                      {"sideral", "expl sideral"},
//                                      {"gim", "expl gim"},
//                                      {"oval", "expl oval"},
//                                      {"rapina", "expl rapina"},
//                                      {"lados", "expl lados"},
//                                      {"rotor", "expl rotor"},
//                                      {"aresta", "aresta"},
//                                      {"poxa", "expl poxa"},
//                                      {"hexa", "expl hexa"},
//                                      {"aguipa", "expl aguipa"},
//                                      {"tim", "expl tim"},
//                                      {"salutar", "expl salutar"},
//                                      {"renovar", "expl renovar"},
//                                      {"eg", "expl eg"},
//                                      {"badalar", "expl badalar"},
//                                      {"usina", "expl usina"},
//                                      {"teatro", "expl teatro"},
//                                      {"esse", "expl esse"},
//                                      {"sola", "expl sola"},
//                                      {"avivar", "expl avivar"},
//                                      {"idade", "expl idade"},
//                                      {"farelos", "expl farelos"},
//                                      {"st", "expl st"},
//                                      {"sibliar", "expl sibliar"},
//                                      {"pop", "expl pop"},
//                                      {"agito", "expl agito"},
//                                      {"inox", "expl inox"},
//                                      {"tamara", "expl tamara"}

//    };

//    crosswords::dat::internal::sort_entries(_entries);

//    crosswords::dat::permutation _permutation;
//    for (crosswords::dat::entries::const_entry_ite _entry = _entries.begin();
//         _entry != _entries.end(); ++_entry) {
//      _permutation.push_back(_entry);
//    }

//    auto _grid{std::make_shared<crosswords::dat::grid>(
//        _permutation, crosswords::dat::index{11},
//        crosswords::dat::index{11})};

//    auto _dispatcher{crosswords::evt::bus::dispatcher::create()};
//    crosswords::dat::internal::organizer _organize;

//    if (!_organize(_grid)) {
//      TNCT_LOG_TST("Grid not organized, as expected", _grid);
//      return true;
//    }

//    TNCT_LOG_ERR(
//        "It should not be possible to organize, but grid is organized: ",
//        *_grid);

//    return false;
//  }
//};

// struct test_023 {
//   static std::string desc() {
//     return "Fail to solve grid with one word and one thread";
//   }

//  bool operator()(const lib::sof::program_options &) {

//    crosswords::dat::entries _entries{{"viravira", "expl viravira"}};

//    crosswords::dat::assembler _solver(crosswords::evt::bus::dispatcher::create());

//    auto _start{std::chrono::high_resolution_clock::now()};
//    std::shared_ptr<crosswords::dat::grid> _grid{_solver.start(
//        _entries, crosswords::dat::index{11}, crosswords::dat::index{11}, 1)};
//    auto _end{std::chrono::high_resolution_clock::now()};
//    std::chrono::duration<double> diff = _end - _start;
//    TNCT_LOG_TST("time: ", diff.count());
//    if (!_grid) {
//      TNCT_LOG_TST("Could not solve, as expected");
//      return true;
//    }
//    TNCT_LOG_ERR("solved", *_grid, ", but it should have not");
//    return false;
//  }
//};

// struct test_024 {
//   static std::string desc() {
//     return "Fail to solve grid one word and 2 threads";
//   }

//  bool operator()(const lib::sof::program_options &) {

//    crosswords::dat::entries _entries{{"viravira", "expl viravira"}};

//    crosswords::dat::assembler _solver(crosswords::evt::bus::dispatcher::create());

//    auto _start{std::chrono::high_resolution_clock::now()};
//    std::shared_ptr<crosswords::dat::grid> _grid{_solver.start(
//        _entries, crosswords::dat::index{11}, crosswords::dat::index{11}, 2)};
//    auto _end{std::chrono::high_resolution_clock::now()};
//    std::chrono::duration<double> diff = _end - _start;
//    TNCT_LOG_TST("time: ", diff.count());
//    if (!_grid) {
//      TNCT_LOG_TST("Could not solve, as expected");
//      return true;
//    }
//    TNCT_LOG_ERR("solved", *_grid, ", but it should have not");
//    return false;
//  }
//};

// struct test_025 {
//   static std::string desc() { return "Solve grid with two words and 1
//   thread"; }

//  bool operator()(const lib::sof::program_options &) {

//    crosswords::dat::entries _entries{{"viravira", "expl viravira"},
//                                      {"exumar", "expl exumar"}};

//    crosswords::dat::assembler _solver(crosswords::evt::bus::dispatcher::create());

//    auto _start{std::chrono::high_resolution_clock::now()};
//    std::shared_ptr<crosswords::dat::grid> _grid{_solver.start(
//        _entries, crosswords::dat::index{11}, crosswords::dat::index{11}, 1)};
//    auto _end{std::chrono::high_resolution_clock::now()};
//    std::chrono::duration<double> diff = _end - _start;
//    TNCT_LOG_TST("time: ", diff.count());
//    if (_grid) {
//      TNCT_LOG_TST("SOLVED!", *_grid);
//      return true;
//    }
//    TNCT_LOG_ERR("not solved");
//    return false;
//  }
//};

// struct test_026 {
//   static std::string desc() { return "Solve grid with two words and 2
//   thread"; }

//  bool operator()(const lib::sof::program_options &) {

//    crosswords::dat::entries _entries{{"viravira", "expl viravira"},
//                                      {"exumar", "expl exumar"}};

//    crosswords::dat::assembler _solver(crosswords::evt::bus::dispatcher::create());

//    auto _start{std::chrono::high_resolution_clock::now()};
//    std::shared_ptr<crosswords::dat::grid> _grid{_solver.start(
//        _entries, crosswords::dat::index{11}, crosswords::dat::index{11}, 2)};
//    auto _end{std::chrono::high_resolution_clock::now()};
//    std::chrono::duration<double> diff = _end - _start;
//    TNCT_LOG_TST("time: ", diff.count());
//    if (_grid) {
//      TNCT_LOG_TST("SOLVED!", *_grid);
//      return true;
//    }
//    TNCT_LOG_ERR("not solved");
//    return false;
//  }
//};

// struct test_027 {
//   static std::string desc() {
//     return "Trying to solve a grid with 18 words and 5 threads ";
//     // time: 0.31991967900000001
//   }

//  bool operator()(const lib::sof::program_options &) {

//    //    crosswords::dat::entries _entries{{"chat", "expl chat"},
//    //                          {"crepom", "expl crepom"},
//    //                          {"debute", "expl debute"},
//    //                          {"regis", "expl regis"},
//    //                          {"gases", "expl gases"},
//    //                          {"exumar", "expl exumar"},
//    //                          {"dias", "expl dias"},
//    //                          {"pai", "expl pai"},
//    //                          {"lesante", "expl lesante"},
//    //                          {"ma", "expl ma"},
//    //                          {"afunilar", "expl afunilar"},
//    //                          {"atoba", "expl atoba"},
//    //                          {"ot", "expl ot"},
//    //                          {"viravira", "expl viravira"},
//    //                          {"sideral", "expl sideral"},
//    //                          {"gim", "expl gim"},
//    //                          {"oval", "expl oval"},
//    //                          {"rapina", "expl rapina"},
//    //                          {"lados", "expl lados"},
//    //                          {"rotor", "expl rotor"},
//    //                          {"aresta", "expl aresta"},
//    //                          {"poxa", "expl poxa"},
//    //                          {"hexa", "expl hexa"},
//    //                          {"aguipa", "expl aguipa"},
//    //                          {"tim", "expl tim"},
//    //                          {"salutar", "expl salutar"},
//    //                          {"renovar", "expl renovar"},
//    //                          {"eg", "expl eg"},
//    //                          {"badalar", "expl badalar"},
//    //                          {"usina", "expl usina"},
//    //                          {"teatro", "expl teatro"},
//    //                          {"esse", "expl esse"},
//    //                          {"sola", "expl sola"},
//    //                          {"avivar", "expl avivar"},
//    //                          {"idade", "expl idade"},
//    //                          {"farelos", "expl farelos"},
//    //                          {"st", "expl st"},
//    //                          {"sibliar", "expl sibliar"},
//    //                          {"pop", "expl pop"},
//    //                          {"agito", "expl agito"},
//    //                          {"inox", "expl inox"},
//    //                          {"tamara", "expl tamara"}};

//    crosswords::dat::entries _entries{
//        {"viravira", "expl viravira"}, {"exumar", "expl exumar"},
//        {"rapina", "expl rapina"},     {"tamara", "expl tamara"},
//        {"teatro", "expl teatro"},     {"badalar", "expl badalar"},
//        {"farelos", "expl farelos"},   {"afunilar", "expl afunilar"},
//        {"sibliar", "expl sibliar"},   {"renovar", "expl renovar"},
//        {"lesante", "expl lesante"},   {"sideral", "expl sideral"},
//        {"salutar", "expl salutar"},   {"aguipa", "expl aguipa"},
//        {"aresta", "expl aresta"},     {"avivar", "expl avivar"},
//        {"crepom", "expl crepom"},     {"debute", "expl debute"}/*,
//        {"usina", "expl usina"}*/};

//    /*    aguipa aresta avivar crepom debute exumar rapina tamara teatro
//    badalar
//     * farelos viravira afunilar sibliar renovar lesante sideral salutar*/

//    crosswords::dat::assembler _solver(crosswords::evt::bus::dispatcher::create());

//    auto _start{std::chrono::high_resolution_clock::now()};
//    std::shared_ptr<crosswords::dat::grid> _grid{_solver.start(
//        _entries, crosswords::dat::index{11}, crosswords::dat::index{11}, 2)};
//    auto _end{std::chrono::high_resolution_clock::now()};
//    std::chrono::duration<double> diff = _end - _start;
//    TNCT_LOG_TST("time: ", diff.count());
//    if (_grid) {
//      TNCT_LOG_TST("SOLVED!!! permutation ", _grid->get_permutation_number(),
//                   *_grid);
//      return true;
//    }
//    TNCT_LOG_ERR("Could not solve... 8(");
//    return false;
//  }
//};

// struct test_028 {
//   static std::string desc() {
//     return "Trying to solve a grid with 18 words and 20 threads ";
//     // time: 0.251665574
//   }

//  bool operator()(const lib::sof::program_options &) {

//    //    crosswords::dat::entries _entries{{"chat", "expl chat"},
//    //                          {"crepom", "expl crepom"},
//    //                          {"debute", "expl debute"},
//    //                          {"regis", "expl regis"},
//    //                          {"gases", "expl gases"},
//    //                          {"exumar", "expl exumar"},
//    //                          {"dias", "expl dias"},
//    //                          {"pai", "expl pai"},
//    //                          {"lesante", "expl lesante"},
//    //                          {"ma", "expl ma"},
//    //                          {"afunilar", "expl afunilar"},
//    //                          {"atoba", "expl atoba"},
//    //                          {"ot", "expl ot"},
//    //                          {"viravira", "expl viravira"},
//    //                          {"sideral", "expl sideral"},
//    //                          {"gim", "expl gim"},
//    //                          {"oval", "expl oval"},
//    //                          {"rapina", "expl rapina"},
//    //                          {"lados", "expl lados"},
//    //                          {"rotor", "expl rotor"},
//    //                          {"aresta", "expl aresta"},
//    //                          {"poxa", "expl poxa"},
//    //                          {"hexa", "expl hexa"},
//    //                          {"aguipa", "expl aguipa"},
//    //                          {"tim", "expl tim"},
//    //                          {"salutar", "expl salutar"},
//    //                          {"renovar", "expl renovar"},
//    //                          {"eg", "expl eg"},
//    //                          {"badalar", "expl badalar"},
//    //                          {"usina", "expl usina"},
//    //                          {"teatro", "expl teatro"},
//    //                          {"esse", "expl esse"},
//    //                          {"sola", "expl sola"},
//    //                          {"avivar", "expl avivar"},
//    //                          {"idade", "expl idade"},
//    //                          {"farelos", "expl farelos"},
//    //                          {"st", "expl st"},
//    //                          {"sibliar", "expl sibliar"},
//    //                          {"pop", "expl pop"},
//    //                          {"agito", "expl agito"},
//    //                          {"inox", "expl inox"},
//    //                          {"tamara", "expl tamara"}};

//    crosswords::dat::entries _entries{
//            {"viravira", "expl viravira"}, {"exumar", "expl exumar"},
//            {"rapina", "expl rapina"},     {"tamara", "expl tamara"},
//            {"teatro", "expl teatro"},     {"badalar", "expl badalar"},
//            {"farelos", "expl farelos"},   {"afunilar", "expl afunilar"},
//            {"sibliar", "expl sibliar"},   {"renovar", "expl renovar"},
//            {"lesante", "expl lesante"},   {"sideral", "expl sideral"},
//            {"salutar", "expl salutar"},   {"aguipa", "expl aguipa"},
//            {"aresta", "expl aresta"},     {"avivar", "expl avivar"},
//            {"crepom", "expl crepom"},     {"debute", "expl debute"}/*,
//        {"usina", "expl usina"}*/};

//    crosswords::dat::assembler _solver(crosswords::evt::bus::dispatcher::create());

//    auto _start{std::chrono::high_resolution_clock::now()};
//    std::shared_ptr<crosswords::dat::grid> _grid{_solver.start(
//        _entries, crosswords::dat::index{11}, crosswords::dat::index{11},
//        20)};
//    auto _end{std::chrono::high_resolution_clock::now()};
//    std::chrono::duration<double> diff = _end - _start;
//    TNCT_LOG_TST("time: ", diff.count());
//    if (_grid) {
//      TNCT_LOG_TST("SOLVED!!! permutation ", _grid->get_permutation_number(),
//                   *_grid);
//      return true;
//    }
//    TNCT_LOG_ERR("Could not solve... 8(");
//    return false;
//  }
//};

// struct test_029 {
//   static std::string desc() {
//     return "Trying to solve a grid with 19 words and 20 threads ";
//   }

//  bool operator()(const lib::sof::program_options &) {

//    //    crosswords::dat::entries _entries{{"chat", "expl chat"},
//    //                          {"crepom", "expl crepom"},
//    //                          {"debute", "expl debute"},
//    //                          {"regis", "expl regis"},
//    //                          {"gases", "expl gases"},
//    //                          {"exumar", "expl exumar"},
//    //                          {"dias", "expl dias"},
//    //                          {"pai", "expl pai"},
//    //                          {"lesante", "expl lesante"},
//    //                          {"ma", "expl ma"},
//    //                          {"afunilar", "expl afunilar"},
//    //                          {"atoba", "expl atoba"},
//    //                          {"ot", "expl ot"},
//    //                          {"viravira", "expl viravira"},
//    //                          {"sideral", "expl sideral"},
//    //                          {"gim", "expl gim"},
//    //                          {"oval", "expl oval"},
//    //                          {"rapina", "expl rapina"},
//    //                          {"lados", "expl lados"},
//    //                          {"rotor", "expl rotor"},
//    //                          {"aresta", "expl aresta"},
//    //                          {"pox a", "expl poxa"},
//    //                          {"hexa", "expl hexa"},
//    //                          {"aguipa", "expl aguipa"},
//    //                          {"tim", "expl tim"},
//    //                          {"salutar", "expl salutar"},
//    //                          {"renovar", "expl renovar"},
//    //                          {"eg", "expl eg"},
//    //                          {"badalar", "expl badalar"},
//    //                          {"usina", "expl usina"},
//    //                          {"teatro", "expl teatro"},
//    //                          {"esse", "expl esse"},
//    //                          {"sola", "expl sola"},
//    //                          {"avivar", "expl avivar"},
//    //                          {"idade", "expl idade"},
//    //                          {"farelos", "expl farelos"},
//    //                          {"st", "expl st"},
//    //                          {"sibliar", "expl sibliar"},
//    //                          {"pop", "expl pop"},
//    //                          {"agito", "expl agito"},
//    //                          {"inox", "expl inox"},
//    //                          {"tamara", "expl tamara"}};

//    crosswords::dat::entries _entries{
//        {"viravira", "expl viravira"}, {"exumar", "expl exumar"},
//        {"rapina", "expl rapina"},     {"tamara", "expl tamara"},
//        {"teatro", "expl teatro"},     {"badalar", "expl badalar"},
//        {"farelos", "expl farelos"},   {"afunilar", "expl afunilar"},
//        {"sibliar", "expl sibliar"},   {"renovar", "expl renovar"},
//        {"lesante", "expl lesante"},   {"sideral", "expl sideral"},
//        {"salutar", "expl salutar"},   {"aguipa", "expl aguipa"},
//        {"aresta", "expl aresta"},     {"avivar", "expl avivar"},
//        {"crepom", "expl crepom"},     {"debute", "expl debute"},
//        {"usina", "expl usina"}};

//    crosswords::dat::assembler _solver(crosswords::evt::bus::dispatcher::create());

//    auto _start{std::chrono::high_resolution_clock::now()};
//    std::shared_ptr<crosswords::dat::grid> _grid{_solver.start(
//        _entries, crosswords::dat::index{11}, crosswords::dat::index{11},
//        20)};
//    auto _end{std::chrono::high_resolution_clock::now()};
//    std::chrono::duration<double> diff = _end - _start;
//    TNCT_LOG_TST("time: ", diff.count());
//    if (_grid) {
//      TNCT_LOG_TST("SOLVED!!! permutation ", _grid->get_permutation_number(),
//                   *_grid);
//      return true;
//    }
//    TNCT_LOG_ERR("Could not solve... 8(");
//    return false;
//  }
//};

// struct test_030 {
//   static std::string desc() {
//     return "Trying to solve a grid with 25 words and 50 threads, with 10000 "
//            "attempts ";
//   }

//  test_030()
//      : m_dispatcher(crosswords::evt::bus::dispatcher::create()),
//        m_solver(m_dispatcher) {

//    m_dispatcher->subscribe<crosswords::evt::grid_create_new_attempt>(
//        [](auto p_event) -> void {
//          TNCT_LOG_TST("attempt # ", p_event.number);
//        });
//  }

//  bool operator()(const lib::sof::program_options &) {

//    crosswords::dat::entries _entries{
//        {"afunilar", "expl afunilar"}, {"viravira", "expl viravira"},
//        {"badalar", "expl badalar"},   {"farelos", "expl farelos"},
//        {"lesante", "expl lesante"},   {"renovar", "expl renovar"},
//        {"salutar", "expl salutar"},   {"sibliar", "expl sibliar"},
//        {"sideral", "expl sideral"},   {"aguipa", "expl aguipa"},
//        {"aresta", "expl aresta"},     {"avivar", "expl avivar"},
//        {"crepom", "expl crepom"},     {"debute", "expl debute"},
//        {"exumar", "expl exumar"},     {"rapina", "expl rapina"},
//        {"teatro", "expl teatro"},     {"tamara", "expl tamara"},
//        {"usina", "expl usina"},       {"agito", "expl agito"},
//        {"atoba", "expl atoba"},       {"gases", "expl gases"},
//        {"idade", "expl idade"},       {"lados", "expl lados"},
//        {"regis", "expl regis"}};

//    auto _start{std::chrono::high_resolution_clock::now()};
//    std::shared_ptr<crosswords::dat::grid> _grid{
//        m_solver.start(_entries, crosswords::dat::index{11},
//                       crosswords::dat::index{11}, 50, 10000)};
//    auto _end{std::chrono::high_resolution_clock::now()};
//    std::chrono::duration<double> diff = _end - _start;
//    TNCT_LOG_TST("time: ", diff.count());
//    if (_grid) {
//      TNCT_LOG_ERR("solved, but it should not have been");
//      return false;
//    }
//    TNCT_LOG_TST("Not solved, as expected, and number of attempts = ",
//                 m_solver.get_num_attempts());
//    return true;
//  }

// private:
//   crosswords::evt::bus::dispatcher::ptr m_dispatcher{
//       crosswords::evt::bus::dispatcher::create()};
//   crosswords::dat::assembler m_solver{m_dispatcher};
// };

// struct test_031 {
//   static std::string desc() {
//     return "Defines 100000 attempts to solve a grid with 25 words and 50 "
//            "threads, but stops whem it reaches 5000 attempts";
//   }

//  test_031()
//      : m_dispatcher(crosswords::evt::bus::dispatcher::create()),
//        m_solver(m_dispatcher) {

//    m_dispatcher->subscribe<crosswords::evt::grid_create_new_attempt>(
//        [this](auto p_event) -> void {
//          if (p_event.number == 5000) {
//            TNCT_LOG_TST("number of attempts = ", p_event.number);
//            m_solver.stop();
//          }
//        });
//  }

//  bool operator()(const lib::sof::program_options &) {

//    crosswords::dat::entries _entries{
//        {"afunilar", "expl afunilar"}, {"viravira", "expl viravira"},
//        {"badalar", "expl badalar"},   {"farelos", "expl farelos"},
//        {"lesante", "expl lesante"},   {"renovar", "expl renovar"},
//        {"salutar", "expl salutar"},   {"sibliar", "expl sibliar"},
//        {"sideral", "expl sideral"},   {"aguipa", "expl aguipa"},
//        {"aresta", "expl aresta"},     {"avivar", "expl avivar"},
//        {"crepom", "expl crepom"},     {"debute", "expl debute"},
//        {"exumar", "expl exumar"},     {"rapina", "expl rapina"},
//        {"teatro", "expl teatro"},     {"tamara", "expl tamara"},
//        {"usina", "expl usina"},       {"agito", "expl agito"},
//        {"atoba", "expl atoba"},       {"gases", "expl gases"},
//        {"idade", "expl idade"},       {"lados", "expl lados"},
//        {"regis", "expl regis"}};

//    auto _start{std::chrono::high_resolution_clock::now()};
//    std::shared_ptr<crosswords::dat::grid> _grid{
//        m_solver.start(_entries, crosswords::dat::index{11},
//                       crosswords::dat::index{11}, 50, 100000)};
//    auto _end{std::chrono::high_resolution_clock::now()};
//    std::chrono::duration<double> diff = _end - _start;
//    TNCT_LOG_TST("time: ", diff.count());
//    if (_grid) {
//      TNCT_LOG_ERR("solved, but it should not have been");
//      return false;
//    }
//    TNCT_LOG_TST("Not solved, as expected, and number of attempts = ",
//                 m_solver.get_num_attempts());
//    return true;
//  }

// private:
//   crosswords::evt::bus::dispatcher::ptr m_dispatcher{
//       crosswords::evt::bus::dispatcher::create()};
//   crosswords::dat::assembler m_solver{m_dispatcher};
// };

// struct test_032 {
//   static std::string desc() {
//     return "Defines 100000 attempts to solve a grid with 25 words and 50 "
//            "threads, but stops whem it reaches 5000 attempts";
//   }

//  test_032()
//      : m_dispatcher(crosswords::evt::bus::dispatcher::create()),
//        m_solver(m_dispatcher) {

//    m_dispatcher->subscribe<crosswords::evt::grid_create_new_attempt>(
//        [this](auto p_event) -> void {
//          if (p_event.number == 5000) {
//            TNCT_LOG_TST("number of attempts = ", p_event.number);
//            m_solver.stop();
//          }
//        });
//  }

//  bool operator()(const lib::sof::program_options &) {

//    crosswords::dat::entries _entries{
//        {"afunilar", "expl afunilar"}, {"viravira", "expl viravira"},
//        {"badalar", "expl badalar"},   {"farelos", "expl farelos"},
//        {"lesante", "expl lesante"},   {"renovar", "expl renovar"},
//        {"salutar", "expl salutar"},   {"sibliar", "expl sibliar"},
//        {"sideral", "expl sideral"},   {"aguipa", "expl aguipa"},
//        {"aresta", "expl aresta"},     {"avivar", "expl avivar"},
//        {"crepom", "expl crepom"},     {"debute", "expl debute"},
//        {"exumar", "expl exumar"},     {"rapina", "expl rapina"},
//        {"teatro", "expl teatro"},     {"tamara", "expl tamara"},
//        {"usina", "expl usina"},       {"agito", "expl agito"},
//        {"atoba", "expl atoba"},       {"gases", "expl gases"},
//        {"idade", "expl idade"},       {"lados", "expl lados"},
//        {"regis", "expl regis"}};

//    auto _start{std::chrono::high_resolution_clock::now()};
//    std::shared_ptr<crosswords::dat::grid> _grid{
//        m_solver.start(_entries, crosswords::dat::index{13},
//                       crosswords::dat::index{13}, 50, 100000)};
//    auto _end{std::chrono::high_resolution_clock::now()};
//    std::chrono::duration<double> diff = _end - _start;
//    TNCT_LOG_TST("time: ", diff.count());
//    if (_grid) {
//      TNCT_LOG_TST("SOLVED!!! permutation ", _grid->get_permutation_number(),
//                   *_grid);
//      return true;
//    }
//    TNCT_LOG_TST("Not solved after #", m_solver.get_num_attempts(),
//                 " attempts");
//    return false;
//  }

// private:
//   crosswords::evt::bus::dispatcher::ptr m_dispatcher{
//       crosswords::evt::bus::dispatcher::create()};
//   crosswords::dat::assembler m_solver{m_dispatcher};
// };

// struct test_033 {
//   static std::string desc() {
//     return "Tries to solve a grid with 25 words starting at 11x11 until
//     16x16, "
//            "waiting for 2 minutes in each attempt";
//   }

//  test_033()
//      : m_dispatcher(crosswords::evt::bus::dispatcher::create()),
//        m_solver(m_dispatcher) {
//    //    m_dispatcher->subscribe<crosswords::evt::grid_create_new_attempt>(
//    //        [this](auto p_event) -> void {
//    //          if (p_event.number == 5000) {
//    //            TNCT_LOG_TST("number of attempts = ", p_event.number);
//    //            m_solver.stop();
//    //          }
//    //        });
//  }

//  bool operator()(const lib::sof::program_options &) {
//    crosswords::dat::entries _entries{
//        {"afunilar", "expl afunilar"}, {"viravira", "expl viravira"},
//        {"badalar", "expl badalar"},   {"farelos", "expl farelos"},
//        {"lesante", "expl lesante"},   {"renovar", "expl renovar"},
//        {"salutar", "expl salutar"},   {"sibliar", "expl sibliar"},
//        {"sideral", "expl sideral"},   {"aguipa", "expl aguipa"},
//        {"aresta", "expl aresta"},     {"avivar", "expl avivar"},
//        {"crepom", "expl crepom"},     {"debute", "expl debute"},
//        {"exumar", "expl exumar"},     {"rapina", "expl rapina"},
//        {"teatro", "expl teatro"},     {"tamara", "expl tamara"},
//        {"usina", "expl usina"},       {"agito", "expl agito"},
//        {"atoba", "expl atoba"},       {"gases", "expl gases"},
//        {"idade", "expl idade"},       {"lados", "expl lados"},
//        {"regis", "expl regis"}};

//    //    crosswords::dat::entries _entries1{
//    //        {"afunilar", "expl afunilar"}, {"viravira", "expl
//    viravira"},
//    //        {"badalar", "expl badalar"},   {"farelos", "expl farelos"},
//    //        {"lesante", "expl lesante"},   {"renovar", "expl renovar"},
//    //        {"salutar", "expl salutar"},   {"sibliar", "expl sibliar"},
//    //        {"sideral", "expl sideral"},   {"aguipa", "expl aguipa"},
//    //        {"aresta", "expl aresta"},     {"avivar", "expl avivar"},
//    //        {"crepom", "expl crepom"},     {"debute", "expl debute"},
//    //        {"exumar", "expl exumar"},     {"rapina", "expl rapina"},
//    //        {"teatro", "expl teatro"},     {"tamara", "expl tamara"},
//    //        {"usina", "expl usina"},       {"agito", "expl agito"},
//    //        {"atoba", "expl atoba"},       {"gases", "expl gases"},
//    //        {"idade", "expl idade"},       {"lados", "expl lados"},
//    //        {"regis", "expl regis"}};

//    auto _start{std::chrono::high_resolution_clock::now()};
//    std::shared_ptr<crosswords::dat::grid> _grid{
//        m_solver.start(_entries, crosswords::dat::index{11},
//                       crosswords::dat::index{11}, 30s, 16)};
//    auto _end{std::chrono::high_resolution_clock::now()};
//    std::chrono::duration<double> diff = _end - _start;
//    TNCT_LOG_TST("time: ", diff.count());
//    if (_grid) {
//      TNCT_LOG_TST("SOLVED!!! permutation ", _grid->get_permutation_number(),
//                   *_grid);
//      return true;
//    }
//    TNCT_LOG_TST("Not solved after #", m_solver.get_num_attempts(),
//                 " attempts");
//    return false;
//  }

// private:
//   crosswords::evt::bus::dispatcher::ptr m_dispatcher{
//       crosswords::evt::bus::dispatcher::create()};
//   crosswords::dat::assembler m_solver{m_dispatcher};
// };

// struct test_034 {
//   static std::string desc() {
//     return "Tries to solve a grid with 25 words starting at 11x11 until
//     16x16, "
//            "waiting for 2 minutes in each attempt";
//   }

//  test_034()
//      : m_dispatcher(crosswords::evt::bus::dispatcher::create()),
//        m_solver(m_dispatcher) {
//    m_entries = crosswords::dat::entries{
//        {"afunilar", "expl afunilar"}, {"viravira", "expl viravira"},
//        {"badalar", "expl badalar"},   {"farelos", "expl farelos"},
//        {"lesante", "expl lesante"},   {"renovar", "expl renovar"},
//        {"salutar", "expl salutar"},   {"sibliar", "expl sibliar"},
//        {"sideral", "expl sideral"},   {"aguipa", "expl aguipa"},
//        {"aresta", "expl aresta"},     {"avivar", "expl avivar"},
//        {"crepom", "expl crepom"},     {"debute", "expl debute"},
//        {"exumar", "expl exumar"},     {"rapina", "expl rapina"},
//        {"teatro", "expl teatro"},     {"tamara", "expl tamara"},
//        {"usina", "expl usina"},       {"agito", "expl agito"},
//        {"atoba", "expl atoba"},       {"gases", "expl gases"},
//        {"idade", "expl idade"},       {"lados", "expl lados"},
//        {"regis", "expl regis"}};

//    //    m_dispatcher->subscribe<crosswords::evt::grid_create_new_attempt>(
//    //        [this](auto p_event) -> void {
//    //          if (p_event.number == 5000) {
//    //            TNCT_LOG_TST("number of attempts = ", p_event.number);
//    //            m_solver.stop();
//    //          }
//    //        });
//  }

//  bool operator()(const lib::sof::program_options &) {
//    crosswords::dat::index _current_num_rows{11};
//    crosswords::dat::index _current_num_cols{11};
//    crosswords::dat::index _max_num_rows{16};

//    while (_current_num_rows <= _max_num_rows) {
// #ifdef TENACITAS_LOG
//      TNCT_LOG_DEB("Trying grid ", _current_num_rows, 'x', _current_num_cols);
// #endif

//      std::future<std::shared_ptr<crosswords::dat::grid>> _future{
//          std::async(std::launch::async, [&]() {
//            return solve(_current_num_rows, _current_num_cols);
//          })};

// #ifdef TENACITAS_LOG
//       TNCT_LOG_DEB("Waiting for grid ", _current_num_rows, 'x',
//                    _current_num_cols);
// #endif

//      uint16_t _counter{0};
//      std::chrono::milliseconds _wait{
//          static_cast<std::chrono::milliseconds>(15s).count() / 10};

//      std::future_status _status;
//      do {
//        _status = _future.wait_for(_wait);
//        switch (_status) {
//        case std::future_status::deferred:
// #ifdef TENACITAS_LOG
//          TNCT_LOG_DEB("Future returned for ", _current_num_rows, 'x',
//                       _current_num_cols);
// #endif
//          break;
//        case std::future_status::timeout:
// #ifdef TENACITAS_LOG
//          TNCT_LOG_DEB("Future timeout for ", _current_num_rows, 'x',
//                       _current_num_cols);
// #endif
//          break;
//        case std::future_status::ready:
// #ifdef TENACITAS_LOG
//          TNCT_LOG_DEB("Future ready for ", _current_num_rows, 'x',
//                       _current_num_cols);
// #endif
//          break;
//        }
//        ++_counter;
// #ifdef TENACITAS_LOG
//        TNCT_LOG_DEB("Counter = ", _counter, " for ", _current_num_rows, 'x',
//                     _current_num_cols);
// #endif

//      } while ((_status != std::future_status::ready) && (_counter < 10));

//      if (_status == std::future_status::timeout) {
//        TNCT_LOG_TST("Timeout for ", _current_num_rows, 'x',
//        _current_num_cols);
//      } else if (_status == std::future_status::deferred) {
//        TNCT_LOG_TST("Deferred for ", _current_num_rows, 'x',
//                     _current_num_cols);
//      } else {
//        TNCT_LOG_TST("Ready for ", _current_num_rows, 'x', _current_num_cols);

//        std::shared_ptr<crosswords::dat::grid> _grid{_future.get()};
//        if (_grid) {
//          TNCT_LOG_TST("Solved for ", _current_num_rows, 'x',
//          _current_num_cols,
//                       _grid);
//          return true;
//        }
//      }

//      TNCT_LOG_TST("Not solved for ", _current_num_rows, 'x',
//                   _current_num_cols);
//      ++_current_num_rows;
//      ++_current_num_cols;
//    }
//    TNCT_LOG_ERR("Not solved for all the grid sizes");
//    return false;
//  }

// private:
//   std::shared_ptr<crosswords::dat::grid>
//   solve(crosswords::dat::index p_num_rows, crosswords::dat::index p_num_cols)
//   {
//     crosswords::dat::internal::assembler _solver{m_dispatcher};
//     auto _start{std::chrono::high_resolution_clock::now()};
//     std::shared_ptr<crosswords::dat::grid> _grid{
//         m_solver.start(m_entries, p_num_rows, p_num_cols, 2min)};
//     auto _end{std::chrono::high_resolution_clock::now()};
//     std::chrono::duration<double> diff = _end - _start;
//     TNCT_LOG_TST("time: ", diff.count());
//     if (_grid) {
//       TNCT_LOG_TST("SOLVED!!! permutation ",
//       _grid->get_permutation_number()); return _grid;
//     }
//     //    TNCT_LOG_TST("Not solved after #", m_solver.get_num_attempts(),
//     //                 " attempts");
//     return {};
//   }

// private:
//   crosswords::evt::bus::dispatcher::ptr m_dispatcher{
//       crosswords::evt::bus::dispatcher::create()};
//   crosswords::dat::entries m_entries;
//   crosswords::dat::internal::assembler m_solver{m_dispatcher};
// };

// struct test_035 {
//   using events_published = std::tuple<crosswords::evt::internal::grid_create_start>;

//   using events_subscribed =
//       std::tuple<crosswords::evt::internal::grid_create_unsolved,
//                  crosswords::evt::internal::grid_create_solved,
//                  crosswords::evt::internal::grid_permutations_tried>;

//   static std::string desc() {
//     return "Tries to solve a grid with 25 words in a 15x15, waiting for 1 "
//            "minute";
//   }

//   bool operator()(const lib::sof::program_options &) {
//     bool _solved{false};
//     std::condition_variable _cond;
//     std::mutex _mutex;

//     crosswords::dat::entries _entries{
//         {"afunilar", "expl afunilar"}, {"viravira", "expl viravira"},
//         {"badalar", "expl badalar"},   {"farelos", "expl farelos"},
//         {"lesante", "expl lesante"},   {"renovar", "expl renovar"},
//         {"salutar", "expl salutar"},   {"sibliar", "expl sibliar"},
//         {"sideral", "expl sideral"},   {"aguipa", "expl aguipa"},
//         {"aresta", "expl aresta"},     {"avivar", "expl avivar"},
//         {"crepom", "expl crepom"},     {"debute", "expl debute"},
//         {"exumar", "expl exumar"},     {"rapina", "expl rapina"},
//         {"teatro", "expl teatro"},     {"tamara", "expl tamara"},
//         {"usina", "expl usina"},       {"agito", "expl agito"},
//         {"atoba", "expl atoba"},       {"gases", "expl gases"},
//         {"idade", "expl idade"},       {"lados", "expl lados"},
//         {"regis", "expl regis"}};

//     crosswords::evt::bus::dispatcher::ptr _dispatcher{
//         crosswords::evt::bus::dispatcher::create()};
//     crosswords::dat::grid_creator _grid_creator{_dispatcher};

//     _dispatcher->subscribe<test_035, crosswords::evt::internal::grid_create_solved>(
//         [&](auto p_event) {
//           _solved = true;
//           TNCT_LOG_TST("SOLVED: ", p_event.grid);
//           _cond.notify_one();
//         });

//     _dispatcher->subscribe<test_035,
//     crosswords::evt::internal::grid_permutations_tried>(
//         [&](auto p_event) {
//           TNCT_LOG_TST("Attempt # ", p_event.permutations);
//         });

//     _dispatcher->subscribe<test_035, crosswords::evt::internal::grid_create_unsolved>(
//         [&](auto) {
//           _solved = false;
//           TNCT_LOG_TST("UNSOLVED");
//           _cond.notify_one();
//         });

//     TNCT_LOG_TST("##################### ABOUT TO START");

//     _dispatcher->publish<test_035, crosswords::evt::internal::grid_create_start>(
//         _entries, crosswords::dat::index{15}, crosswords::dat::index{15},
//         1min, crosswords::dat::index{15});
//     // {
//     //   TNCT_LOG_TST("sleeping");
//     //   std::this_thread::sleep_for(15s);
//     //   TNCT_LOG_TST("waking up - notifying");
//     //   //_cond.notify_one();
//     //   TNCT_LOG_TST("waking up - notified");
//     // }

//     {
//       std::unique_lock<std::mutex> _lock{_mutex};
//       _cond.wait(_lock);
//     }

//     return _solved;
//   }

// private:
// };

// struct test_036 {
//   using events_published = std::tuple<crosswords::evt::internal::grid_create_start>;

//   using events_subscribed =
//       std::tuple<crosswords::evt::internal::grid_create_unsolved,
//                  crosswords::evt::internal::grid_create_solved,
//                  crosswords::evt::internal::grid_permutations_tried>;

//   static std::string desc() {
//     return "Tries to unsolvable grid with 25 words in a 10x10, waiting for 1
//     "
//            "minute";
//   }

//   bool operator()(const lib::sof::program_options &) {
//     bool _solved{false};
//     std::condition_variable _cond;
//     std::mutex _mutex;

//     m_dispatcher->subscribe<test_036, crosswords::evt::internal::grid_create_solved>(
//         [&](auto p_event) {
//           _solved = true;
//           TNCT_LOG_TST("SOLVED: ", p_event.grid);
//           _cond.notify_one();
//         });

//     m_dispatcher->subscribe<test_036,
//     crosswords::evt::internal::grid_permutations_tried>(
//         [&](auto p_event) {
//           TNCT_LOG_TST("Attempt # ", p_event.permutations);
//         });

//     m_dispatcher->subscribe<test_036, crosswords::evt::internal::grid_create_unsolved>(
//         [&](auto) {
//           _solved = false;
//           TNCT_LOG_TST("UNSOLVED");
//           _cond.notify_one();
//         });

//     m_dispatcher->publish<test_036, crosswords::evt::internal::grid_create_start>(
//         m_entries, crosswords::dat::index{10}, crosswords::dat::index{10},
//         60s, crosswords::dat::index{10});

//     {
//       std::unique_lock<std::mutex> _lock{_mutex};
//       _cond.wait(_lock);
//     }

//     return (_solved == false);
//   }

// private:
//   crosswords::evt::bus::dispatcher::ptr m_dispatcher{
//       crosswords::evt::bus::dispatcher::create()};
//   crosswords::dat::grid_creator m_grid_creator{m_dispatcher};
//   crosswords::dat::entries m_entries{
//       {"afunilar", "expl afunilar"}, {"viravira", "expl viravira"},
//       {"badalar", "expl badalar"},   {"farelos", "expl farelos"},
//       {"lesante", "expl lesante"},   {"renovar", "expl renovar"},
//       {"salutar", "expl salutar"},   {"sibliar", "expl sibliar"},
//       {"sideral", "expl sideral"},   {"aguipa", "expl aguipa"},
//       {"aresta", "expl aresta"},     {"avivar", "expl avivar"},
//       {"crepom", "expl crepom"},     {"debute", "expl debute"},
//       {"exumar", "expl exumar"},     {"rapina", "expl rapina"},
//       {"teatro", "expl teatro"},     {"tamara", "expl tamara"},
//       {"usina", "expl usina"},       {"agito", "expl agito"},
//       {"atoba", "expl atoba"},       {"gases", "expl gases"},
//       {"idade", "expl idade"},       {"lados", "expl lados"},
//       {"regis", "expl regis"}};
// };

// struct test_037 {
//   using events_published = std::tuple<crosswords::evt::internal::grid_create_start>;

//   using events_subscribed =
//       std::tuple<crosswords::evt::internal::grid_create_unsolved,
//                  crosswords::evt::internal::grid_create_solved,
//                  crosswords::evt::internal::grid_permutations_tried>;

//   static std::string desc() {
//     return "Tries to unsolvable a grid with 2 words in a 3x8, waiting for 5s
//     ";
//   }

//   bool operator()(const lib::sof::program_options &) {
//     bool _solved{false};
//     std::condition_variable _cond;
//     std::mutex _mutex;

//     m_dispatcher->subscribe<test_037, crosswords::evt::internal::grid_create_solved>(
//         [&](auto p_event) {
//           _solved = true;
//           TNCT_LOG_TST("SOLVED: ", p_event.grid);
//           _cond.notify_one();
//         });

//     m_dispatcher->subscribe<test_037,
//     crosswords::evt::internal::grid_permutations_tried>(
//         [&](auto p_event) {
//           TNCT_LOG_TST("Attempt # ", p_event.permutations);
//         });

//     m_dispatcher->subscribe<test_037, crosswords::evt::internal::grid_create_unsolved>(
//         [&](auto) {
//           _solved = false;
//           TNCT_LOG_TST("UNSOLVED");
//           _cond.notify_one();
//         });

//     m_dispatcher->publish<test_037, crosswords::evt::internal::grid_create_start>(
//         m_entries, crosswords::dat::index{3}, crosswords::dat::index{8}, 5s,
//         crosswords::dat::index{3});

//     {
//       std::unique_lock<std::mutex> _lock{_mutex};
//       _cond.wait(_lock);
//     }

//     return (_solved == false);
//   }

// private:
//   crosswords::evt::bus::dispatcher::ptr m_dispatcher{
//       crosswords::evt::bus::dispatcher::create()};
//   crosswords::dat::grid_creator m_grid_creator{m_dispatcher};
//   crosswords::dat::entries m_entries{{"afunilar", "expl afunilar"},
//                                      {"ovo", "expl ovo"}};
// };

// struct test_038 {
//   using events_published = std::tuple<crosswords::evt::internal::grid_create_start>;

//   using events_subscribed =
//       std::tuple<crosswords::evt::internal::grid_create_unsolved,
//                  crosswords::evt::internal::grid_create_solved,
//                  crosswords::evt::internal::grid_permutations_tried>;

//   static std::string desc() {
//     return "Tries to solve a grid with 25 words starting with a 12x12 until "
//            "a 15x15 grid, waiting for 3 minutes between each attempt";
//   }

//   bool operator()(const lib::sof::program_options &) {
//     bool _solved{false};
//     std::condition_variable _cond;
//     std::mutex _mutex;

//     m_dispatcher->subscribe<test_038, crosswords::evt::internal::grid_create_solved>(
//         [&](auto p_event) {
//           _solved = true;
//           TNCT_LOG_TST("SOLVED: ", *p_event.grid);
//           _cond.notify_one();
//         });

//     m_dispatcher->subscribe<test_038,
//     crosswords::evt::internal::grid_permutations_tried>(
//         [&](auto p_event) {
//           TNCT_LOG_TST("Attempt # ", p_event.permutations);
//         });

//     m_dispatcher->subscribe<test_038, crosswords::evt::internal::grid_create_unsolved>(
//         [&](auto) {
//           _solved = false;
//           TNCT_LOG_TST("UNSOLVED");
//           _cond.notify_one();
//         });

//     m_dispatcher->publish<test_038, crosswords::evt::internal::grid_create_start>(
//         m_entries, crosswords::dat::index{12}, crosswords::dat::index{12},
//         3min, crosswords::dat::index{15});

//     {
//       std::unique_lock<std::mutex> _lock{_mutex};
//       _cond.wait(_lock);
//     }

//     return (_solved == true);
//   }

// private:
//   crosswords::evt::bus::dispatcher::ptr m_dispatcher{
//       crosswords::evt::bus::dispatcher::create()};
//   crosswords::dat::grid_creator m_grid_creator{m_dispatcher};
//   crosswords::dat::entries m_entries{
//       {"afunilar", "expl afunilar"}, {"viravira", "expl viravira"},
//       {"badalar", "expl badalar"},   {"farelos", "expl farelos"},
//       {"lesante", "expl lesante"},   {"renovar", "expl renovar"},
//       {"salutar", "expl salutar"},   {"sibliar", "expl sibliar"},
//       {"sideral", "expl sideral"},   {"aguipa", "expl aguipa"},
//       {"aresta", "expl aresta"},     {"avivar", "expl avivar"},
//       {"crepom", "expl crepom"},     {"debute", "expl debute"},
//       {"exumar", "expl exumar"},     {"rapina", "expl rapina"},
//       {"teatro", "expl teatro"},     {"tamara", "expl tamara"},
//       {"usina", "expl usina"},       {"agito", "expl agito"},
//       {"atoba", "expl atoba"},       {"gases", "expl gases"},
//       {"idade", "expl idade"},       {"lados", "expl lados"},
//       {"regis", "expl regis"}};
// };

// struct test_039 {
//   using events_published = std::tuple<crosswords::evt::internal::grid_create_start>;

//   using events_subscribed =
//       std::tuple<crosswords::evt::internal::grid_create_unsolved,
//                  crosswords::evt::internal::grid_create_solved,
//                  crosswords::evt::internal::grid_permutations_tried>;

//   static std::string desc() {
//     return "Tries to organize a grid with 60 words strting at 15x15 up to "
//            "18x18, each attempt taking 1 minute, and it must succeed";
//   }

//   bool operator()(const lib::sof::program_options &) {
//     bool _solved{false};
//     std::condition_variable _cond;
//     std::mutex _mutex;

//     m_dispatcher->subscribe<test_039, crosswords::evt::internal::grid_create_solved>(
//         [&](auto p_event) {
//           _solved = true;
//           TNCT_LOG_TST("SOLVED: ", *p_event.grid);
//           _cond.notify_one();
//         });

//     m_dispatcher->subscribe<test_039,
//     crosswords::evt::internal::grid_permutations_tried>(
//         [&](auto p_event) {
//           TNCT_LOG_TST("Attempt # ", p_event.permutations);
//         });

//     m_dispatcher->subscribe<test_039, crosswords::evt::internal::grid_create_unsolved>(
//         [&](auto) {
//           _solved = false;
//           TNCT_LOG_TST("UNSOLVED");
//           _cond.notify_one();
//         });

//     m_dispatcher->publish<test_039, crosswords::evt::internal::grid_create_start>(
//         m_entries, crosswords::dat::index{15}, crosswords::dat::index{15},
//         60s, crosswords::dat::index{18});

//     {
//       std::unique_lock<std::mutex> _lock{_mutex};
//       _cond.wait(_lock);
//     }

//     return (_solved == true);
//   }

// private:
//   crosswords::evt::bus::dispatcher::ptr m_dispatcher{
//       crosswords::evt::bus::dispatcher::create()};
//   crosswords::dat::grid_creator m_grid_creator{m_dispatcher};
//   crosswords::dat::entries m_entries{{"precoce", "expl precoce"},
//                                      {"piar", "expl piar"},
//                                      {"reter", "expl reter"},
//                                      {"cliente", "expl cliente"},
//                                      {"item", "expl item"},
//                                      {"goiana", "expl goiana"},
//                                      {"mar", "expl mar"},
//                                      {"grande", "expl grande"},
//                                      {"cp", "expl cp"},
//                                      {"ad", "expl ad"},
//                                      {"tronco", "expl tronco"},
//                                      {"não", "expl não"},
//                                      {"tourette", "expl tourette"},
//                                      {"mais", "expl mais"},
//                                      {"mamãe", "expl mamãe"},
//                                      {"samba", "expl samba"},
//                                      {"obvio", "expl obvio"},
//                                      {"autor", "expl autor"},
//                                      {"avião", "expl avião"},
//                                      {"suturar", "expl suturar"},
//                                      {"lago", "expl lago"},
//                                      {"perito", "expl perito"},
//                                      {"aro", "expl aro"},
//                                      {"janela", "expl janela"},
//                                      {"em", "expl em"},
//                                      {"si", "expl si"},
//                                      {"paina", "expl paina"},
//                                      {"dedo", "expl dedo"},
//                                      {"ofensa", "expl ofensa"},
//                                      {"bonus", "expl bonus"},
//                                      {"prima", "expl prima"},
//                                      {"retado", "expl retado"},
//                                      {"eter", "expl eter"},
//                                      {"cem", "expl cem"},
//                                      {"or", "expl or"},
//                                      {"alas", "expl alas"},
//                                      {"ovário", "expl ovário"},
//                                      {"umbigo", "expl umbigo"},
//                                      {"travão", "expl travão"},
//                                      {"grêmio", "expl grêmio"},
//                                      {"grotão", "expl o"},
//                                      {"ecoante", "expl coante"},
//                                      {"lince", "expl lince"},
//                                      {"piado", "expl piado"},
//                                      {"iene", "expl iene"},
//                                      {"ana", "expl ana"},
//                                      {"rt", "expl rt"},
//                                      {"pe", "expl pe"},
//                                      {"jan", "expl jan"},
//                                      {"pais", "expl pais"},
//                                      {"senna", "expl senna"},
//                                      {"aurea", "expl aurea"},
//                                      {"sutil", "expl sutil"},
//                                      {"matutado", "expl matutado"},
//                                      {"namoro", "expl namoro"},
//                                      {"do", "expl do"},
//                                      {"en", "expl en"},
//                                      {"edu", "expl edu"},
//                                      {"ramos", "expl ramos"},
//                                      {"caibra", "expl ibra"},
//                                      {"esposa", "expl esposa"}};
// };

// struct test_040 {
//   using events_published = std::tuple<crosswords::evt::internal::grid_create_start>;

//   using events_subscribed =
//       std::tuple<crosswords::evt::internal::grid_create_unsolved,
//                  crosswords::evt::internal::grid_create_solved,
//                  crosswords::evt::internal::grid_permutations_tried>;

//   static std::string desc() {
//     return "Tries a grid with 'esdrúxulo' and 'baú' in a grid 11x11";
//   }

//   bool operator()(const lib::sof::program_options &) {
//     bool _solved{false};
//     std::condition_variable _cond;
//     std::mutex _mutex;

//     m_dispatcher->subscribe<test_040, crosswords::evt::internal::grid_create_solved>(
//         [&](auto p_event) {
//           _solved = true;
//           TNCT_LOG_TST("SOLVED: ", *p_event.grid);
//           _cond.notify_one();
//         });

//     m_dispatcher->subscribe<test_040,
//     crosswords::evt::internal::grid_permutations_tried>(
//         [&](auto p_event) {
//           TNCT_LOG_TST("Attempt # ", p_event.permutations);
//         });

//     m_dispatcher->subscribe<test_040, crosswords::evt::internal::grid_create_unsolved>(
//         [&](auto) {
//           _solved = false;
//           TNCT_LOG_TST("UNSOLVED");
//           _cond.notify_one();
//         });

//     m_dispatcher->publish<test_040, crosswords::evt::internal::grid_create_start>(
//         m_entries, crosswords::dat::index{11}, crosswords::dat::index{11},
//         60min, crosswords::dat::index{11});

//     {
//       std::unique_lock<std::mutex> _lock{_mutex};
//       _cond.wait(_lock);
//     }

//     return (_solved == true);
//   }

// private:
//   crosswords::evt::bus::dispatcher::ptr m_dispatcher{
//       crosswords::evt::bus::dispatcher::create()};
//   crosswords::dat::grid_creator m_grid_creator{m_dispatcher};
//   crosswords::dat::entries m_entries{{"esdrúxulo", "expl esdrúxulo"},
//                                      {"baú", "expl baú"}};
// };

// struct test_041 {
//   using events_published = std::tuple<crosswords::evt::internal::grid_create_start>;

//   using events_subscribed =
//       std::tuple<crosswords::evt::internal::grid_create_unsolved,
//                  crosswords::evt::internal::grid_create_solved,
//                  crosswords::evt::internal::grid_permutations_tried>;

//   static std::string desc() {
//     return "Tries to create a grid for 'door' and 'day'in a 3x8, waiting";
//   }

//   bool operator()(const lib::sof::program_options &) {
//     bool _solved{false};
//     std::condition_variable _cond;
//     std::mutex _mutex;

//     m_dispatcher->subscribe<test_041, crosswords::evt::internal::grid_create_solved>(
//         [&](auto p_event) {
//           _solved = true;
//           TNCT_LOG_TST("SOLVED: ", *p_event.grid);
//           _cond.notify_one();
//         });

//     m_dispatcher->subscribe<test_041,
//     crosswords::evt::internal::grid_permutations_tried>(
//         [&](auto p_event) {
//           TNCT_LOG_TST("Attempt # ", p_event.permutations);
//         });

//     m_dispatcher->subscribe<test_041, crosswords::evt::internal::grid_create_unsolved>(
//         [&](auto) {
//           _solved = false;
//           TNCT_LOG_TST("UNSOLVED");
//           _cond.notify_one();
//         });

//     m_dispatcher->publish<test_041, crosswords::evt::internal::grid_create_start>(
//         m_entries, crosswords::dat::index{3}, crosswords::dat::index{8}, 50s,
//         crosswords::dat::index{3});

//     {
//       std::unique_lock<std::mutex> _lock{_mutex};
//       _cond.wait(_lock);
//     }

//     return (_solved == true);
//   }

// private:
//   crosswords::evt::bus::dispatcher::ptr m_dispatcher{
//       crosswords::evt::bus::dispatcher::create()};
//   crosswords::dat::grid_creator m_grid_creator{m_dispatcher};
//   crosswords::dat::entries m_entries{{"door", "expl door"},
//                                      {"day", "expl day"}};
// };

// struct test_042 {
//   static std::string desc() { return "Creates grid 8x8 with 'door' and
//   'day'"; } bool operator()(const lib::sof::program_options &) {
//     using namespace crosswords;
//     dat::entries _original{{"door", "expl door"}, {"day", "expl day"}};

//     dat::internal::organizer _organizer(evt::bus::dispatcher::create());

//     auto _compare_entries = [](const dat::entry &p_e1, const dat::entry
//     &p_e2) {
//       if (p_e1.get_word().size() == p_e2.get_word().size()) {
//         return p_e1.get_word() < p_e2.get_word();
//       }
//       return p_e1.get_word().size() < p_e2.get_word().size();
//     };

//     auto _entries{std::make_shared<dat::entries>(_original)};
//     std::sort(_entries->begin(), _entries->end(),
//               [&](const dat::entry &p_e1, const dat::entry &p_e2) {
//                 return _compare_entries(p_e1, p_e2);
//               });

//     dat::permutation _permutation;
//     for (dat::entries::const_entry_ite _entry = _entries->begin();
//          _entry != _entries->end(); ++_entry) {
//       _permutation.push_back(_entry);
//     }

//     size_t _permutation_counter{0};

//     auto _grid{std::make_shared<dat::grid>(_entries, _permutation, 8, 8,
//                                            ++_permutation_counter)};

//     bool _organized{_organizer(_grid)};

//     if (!_organized) {
//       TNCT_LOG_ERR("not organized, but it should have");
//     } else {
//       TNCT_LOG_TST(*_grid);
//     }
//     return _organized == true;
//   }
// };

// struct test_043 {
//   static std::string desc() {
//     return "Creates grid 11x11 with 'exdrúxulo' and 'baú'";
//   }
//   bool operator()(const lib::sof::program_options &) {
//     using namespace crosswords;

//     dat::entries _original{{"esdrúxulo", "expl esdrúxulo"},
//                            {"baú", "expl baú"}};

//     dat::internal::organizer _organizer(evt::bus::dispatcher::create());

//     auto _compare_entries = [](const dat::entry &p_e1, const dat::entry
//     &p_e2) {
//       if (p_e1.get_word().size() == p_e2.get_word().size()) {
//         return p_e1.get_word() < p_e2.get_word();
//       }
//       return p_e1.get_word().size() < p_e2.get_word().size();
//     };

//     auto _entries{std::make_shared<dat::entries>(_original)};
//     std::sort(_entries->begin(), _entries->end(),
//               [&](const dat::entry &p_e1, const dat::entry &p_e2) {
//                 return _compare_entries(p_e1, p_e2);
//               });

//     dat::permutation _permutation;
//     for (dat::entries::const_entry_ite _entry = _entries->begin();
//          _entry != _entries->end(); ++_entry) {
//       _permutation.push_back(_entry);
//     }

//     size_t _permutation_counter{0};

//     auto _grid{std::make_shared<dat::grid>(_entries, _permutation, 11, 11,
//                                            ++_permutation_counter)};

//     bool _organized{_organizer(_grid)};

//     if (!_organized) {
//       TNCT_LOG_ERR("not organized, but it should have");
//     } else {
//       TNCT_LOG_TST(*_grid);
//     }
//     return _organized == true;
//   }
// };

// struct test_044 {
//   static std::string desc() { return "'coelho' and 'alho' in a 22x8 grid"; }

//   bool operator()(const lib::sof::program_options &) {
//     using namespace crosswords;

//     dat::entries _original{{"alho", "expl alho"}, {"coelho", "expl coelho"}};

//     dat::internal::organizer _organizer(evt::bus::dispatcher::create());

//     auto _compare_entries = [](const dat::entry &p_e1, const dat::entry
//     &p_e2) {
//       if (p_e1.get_word().size() == p_e2.get_word().size()) {
//         return p_e1.get_word() < p_e2.get_word();
//       }
//       return p_e1.get_word().size() < p_e2.get_word().size();
//     };

//     auto _entries{std::make_shared<dat::entries>(_original)};
//     std::sort(_entries->begin(), _entries->end(),
//               [&](const dat::entry &p_e1, const dat::entry &p_e2) {
//                 return _compare_entries(p_e1, p_e2);
//               });

//     dat::permutation _permutation;
//     for (dat::entries::const_entry_ite _entry = _entries->begin();
//          _entry != _entries->end(); ++_entry) {
//       _permutation.push_back(_entry);
//     }

//     size_t _permutation_counter{0};

//     auto _grid{std::make_shared<dat::grid>(_entries, _permutation, 12, 5,
//                                            ++_permutation_counter)};

//     bool _organized{_organizer(_grid)};

//     if (!_organized) {
//       TNCT_LOG_ERR("not organized, but it should have");
//     } else {
//       TNCT_LOG_TST(*_grid);
//     }
//     return _organized == true;
//   }
// };

// struct test_045 {
//   using events_published = std::tuple<crosswords::evt::internal::grid_create_start>;

//   using events_subscribed =
//       std::tuple<crosswords::evt::internal::grid_create_unsolved,
//                  crosswords::evt::internal::grid_create_solved,
//                  crosswords::evt::internal::grid_permutations_tried>;

//   static std::string desc() {
//     return "Tries to solve a grid with 2 words in a 5x5, waiting for 1 "
//            "minute";
//   }

//   bool operator()(const lib::sof::program_options &) {
//     bool _solved{false};
//     std::condition_variable _cond;
//     std::mutex _mutex;

//     crosswords::dat::entries _entries{{"abc", "expl abc"}, {"cde", "expl
//     cde"}};

//     crosswords::evt::bus::dispatcher::ptr _dispatcher{
//         crosswords::evt::bus::dispatcher::create()};
//     crosswords::dat::grid_creator _grid_creator{_dispatcher};

//     _dispatcher->subscribe<test_045, crosswords::evt::internal::grid_create_solved>(
//         [&](auto p_event) {
//           _solved = true;
//           TNCT_LOG_TST("SOLVED: ", *p_event.grid);
//           _cond.notify_one();
//         });

//     _dispatcher->subscribe<test_045,
//     crosswords::evt::internal::grid_permutations_tried>(
//         [&](auto p_event) {
//           TNCT_LOG_TST("Attempt # ", p_event.permutations);
//         });

//     _dispatcher->subscribe<test_045, crosswords::evt::internal::grid_create_unsolved>(
//         [&](auto) {
//           _solved = false;
//           TNCT_LOG_TST("UNSOLVED");
//           _cond.notify_one();
//         });

//     TNCT_LOG_TST("##################### ABOUT TO START");

//     _dispatcher->publish<test_045, crosswords::evt::internal::grid_create_start>(
//         _entries, crosswords::dat::index{5}, crosswords::dat::index{5}, 1min,
//         crosswords::dat::index{5});
//     // {
//     //   TNCT_LOG_TST("sleeping");
//     //   std::this_thread::sleep_for(15s);
//     //   TNCT_LOG_TST("waking up - notifying");
//     //   //_cond.notify_one();
//     //   TNCT_LOG_TST("waking up - notified");
//     // }

//     {
//       std::unique_lock<std::mutex> _lock{_mutex};
//       _cond.wait(_lock);
//     }

//     return _solved;
//   }

// private:
// };

int main(int argc, char **argv)
{

  //  log::dat::set_file_writer("crosswords");

  tester::test _tester(argc, argv);

  run_test(_tester, crosswords::tst::dat_000);
  run_test(_tester, crosswords::tst::dat_001);

  run_test(_tester, crosswords::tst::organizer_test_000);
  run_test(_tester, crosswords::tst::organizer_test_001);
  run_test(_tester, crosswords::tst::organizer_test_002);

  run_test(_tester, crosswords::tst::assembler_test_000);
  run_test(_tester, crosswords::tst::assembler_test_001);
  run_test(_tester, crosswords::tst::assembler_test_002);

  //  run_test(_tester, test_001);
  //  run_test(_tester, test_003);
  //  run_test(_tester, test_004);
  //  run_test(_tester, test_005);
  //  run_test(_tester, test_006);
  //  run_test(_tester, test_007);
  //  run_test(_tester, test_007a);
  //  run_test(_tester, test_008);
  //  run_test(_tester, test_009);
  //  run_test(_tester, test_010);
  //  run_test(_tester, test_010a);
  //  run_test(_tester, test_011);
  //  run_test(_tester, test_012);
  //  run_test(_tester, test_013);
  //  run_test(_tester, test_014);
  //  run_test(_tester, test_015);
  //  run_test(_tester, test_016);
  //  run_test(_tester, test_017);
  //  run_test(_tester, test_018);
  //  run_test(_tester, test_019);
  //  run_test(_tester, test_020);
  //  run_test(_tester, test_021);
  //  run_test(_tester, test_023);
  //  run_test(_tester, test_024);
  //  run_test(_tester, test_025);
  //  run_test(_tester, test_026);
  //  run_test(_tester, test_027);
  //  run_test(_tester, test_028);
  //  run_test(_tester, test_029);
  //  run_test(_tester, test_030);
  //  run_test(_tester, test_031);
  //  run_test(_tester, test_032);
  //  run_test(_tester, test_033);
  //  run_test(_tester, test_034);
  // run_test(_tester, test_035);
  // run_test(_tester, test_036);
  // run_test(_tester, test_037);
  // run_test(_tester, test_038);
  // run_test(_tester, test_039);
  // run_test(_tester, test_040);
  // run_test(_tester, test_041);
  // run_test(_tester, test_042);
  // run_test(_tester, test_043);
  // run_test(_tester, test_044);
  // run_test(_tester, test_045);
}
