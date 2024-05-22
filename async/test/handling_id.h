/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_ASYNC_TEST_HANDLING_ID_H
#define TENACITAS_LIB_ASYNC_TEST_HANDLING_ID_H

#include <tenacitas.lib/async/handling_id.h>
#include <tenacitas.lib/generic/fmt.h>
#include <tenacitas.lib/program/options.h>
#include <tenacitas.lib/log/cerr.h>

using namespace tenacitas::lib;

struct handling_id_000 {
  static std::string desc() { return "Simple async::handling_id creation"; }

  bool operator()(const program::options &) {
    log::cerr _log;
    async::handling_id _handling_id("id 1");

    _log.tst(generic::fmt("id = ", _handling_id));

    return _handling_id == "id 1";
  }
};

struct handling_id_001 {
  static std::string desc() {
    return "Creating a async::handling_id with more than 10 chars";
  }

  bool operator()(const program::options &) {
    log::cerr _log;
    async::handling_id _handling_id("0123456789ABC");

    _log.tst(generic::fmt("id = ", _handling_id));

    return _handling_id == "0123456789";
  }
};

struct handling_id_002 {
  static std::string desc() {
    return "Comparing two async::handling_id with the same size and "
           "equals";
  }

  bool operator()(const program::options &) {
    log::cerr _log;
    async::handling_id _handling_id_1("012345");
    async::handling_id _handling_id_2("012345");

    _log.tst(
        generic::fmt("id 1 = ", _handling_id_1, ", id 2 = ", _handling_id_2));

    return (_handling_id_1 == "012345") && (_handling_id_2 == "012345") &&
           (_handling_id_1 == _handling_id_2);
  }
};

struct handling_id_003 {
  static std::string desc() {
    return "Comparing two async::handling_id with the diffent sizes";
  }

  bool operator()(const program::options &) {
    log::cerr _log;
    async::handling_id _handling_id_1("01234");
    async::handling_id _handling_id_2("012345");

    _log.tst(
        generic::fmt("id 1 = ", _handling_id_1, ", id 2 = ", _handling_id_2));

    return (_handling_id_1 == "01234") && (_handling_id_2 == "012345") &&
           (_handling_id_1 != _handling_id_2);
  }
};

struct handling_id_004 {
  static std::string desc() { return "Copying async::handling_id"; }

  bool operator()(const program::options &) {
    log::cerr _log;
    async::handling_id _handling_id_1("01234");
    async::handling_id _handling_id_2("012345");

    _log.tst(generic::fmt("BEFORE id 1 = ", _handling_id_1,
                          ", id 2 = ", _handling_id_2));

    _handling_id_1 = _handling_id_2;

    _log.tst(generic::fmt("AFTER id 1 = ", _handling_id_1,
                          ", id 2 = ", _handling_id_2));

    return (_handling_id_1 == "012345") && (_handling_id_2 == "012345") &&
           (_handling_id_1 == _handling_id_2);
  }
};

struct handling_id_005 {
  static std::string desc() { return "Moving async::handling_id"; }

  bool operator()(const program::options &) {
    log::cerr _log;
    async::handling_id _handling_id_1("01234");
    async::handling_id _handling_id_2("012345");

    _log.tst(generic::fmt("BEFORE id 1 = ", _handling_id_1,
                          ", id 2 = ", _handling_id_2));

    _handling_id_1 = std::move(_handling_id_2);

    _log.tst(generic::fmt("AFTER id 1 = ", _handling_id_1,
                          ", id 2 = ", _handling_id_2));

    return (_handling_id_1 == "012345") && (_handling_id_2.size() == 0);
  }
};

struct handling_id_006 {
  static std::string desc() {
    return "Less than in equal size async::handling_id";
  }

  bool operator()(const program::options &) {
    log::cerr _log;
    async::handling_id _handling_id_1("ABC");
    async::handling_id _handling_id_2("ABD");

    _log.tst(
        generic::fmt("id 1 = ", _handling_id_1, ", id 2 = ", _handling_id_2));

    return (_handling_id_1 < _handling_id_2);
  }
};

struct handling_id_007 {
  static std::string desc() {
    return "Less than in different size async::handling_id";
  }

  bool operator()(const program::options &) {
    log::cerr _log;
    async::handling_id _handling_id_1("ABCEF");
    async::handling_id _handling_id_2("ABD");

    _log.tst(
        generic::fmt("id 1 = ", _handling_id_1, ", id 2 = ", _handling_id_2));

    return (_handling_id_1 < _handling_id_2);
  }
};

#endif
