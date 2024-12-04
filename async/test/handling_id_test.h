/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_ASYNC_TEST_HANDLING_ID_H
#define TENACITAS_LIB_ASYNC_TEST_HANDLING_ID_H

#include <limits>

#include "tenacitas.lib/async/handling_id.h"

#include "tenacitas.lib/format/fmt.h"
#include "tenacitas.lib/log/cerr.h"
#include "tenacitas.lib/program/options.h"

using namespace tenacitas::lib;

struct handling_id_000 {
  static std::string desc() { return "Simple async::handling_id creation"; }

  bool operator()(const program::options &) {
    log::cerr _log;
    async::handling_id _handling_id(1);

    _log.tst(format::fmt("id = ", _handling_id));

    return _handling_id == 1;
  }
};

struct handling_id_001 {
  static std::string desc() {
    return "Creating a async::handling_id with max possible value";
  }

  bool operator()(const program::options &) {
    log::cerr _log;
    async::handling_id _handling_id(
        std::numeric_limits<async::handling_id>::max());

    _log.tst(format::fmt("id = ", _handling_id));

    return _handling_id == std::numeric_limits<async::handling_id>::max();
  }
};

struct handling_id_002 {
  static std::string desc() {
    return "Comparing two async::handling_id with the same size and "
           "equals";
  }

  bool operator()(const program::options &) {
    log::cerr _log;
    async::handling_id _handling_id_1(5);
    async::handling_id _handling_id_2(5);

    _log.tst(
        format::fmt("id 1 = ", _handling_id_1, ", id 2 = ", _handling_id_2));

    return (_handling_id_1 == 5) && (_handling_id_2 == 5) &&
           (_handling_id_1 == _handling_id_2);
  }
};

struct handling_id_004 {
  static std::string desc() { return "Copying async::handling_id"; }

  bool operator()(const program::options &) {
    log::cerr _log;
    async::handling_id _handling_id_1(4);
    async::handling_id _handling_id_2(5);

    _log.tst(format::fmt("BEFORE id 1 = ", _handling_id_1,
                         ", id 2 = ", _handling_id_2));

    _handling_id_1 = _handling_id_2;

    _log.tst(format::fmt("AFTER id 1 = ", _handling_id_1,
                         ", id 2 = ", _handling_id_2));

    return (_handling_id_1 == 5) && (_handling_id_2 == 5) &&
           (_handling_id_1 == _handling_id_2);
  }
};

struct handling_id_005 {
  static std::string desc() { return "Moving async::handling_id"; }

  bool operator()(const program::options &) {
    log::cerr _log;
    async::handling_id _handling_id_1(4);
    async::handling_id _handling_id_2(5);

    _log.tst(format::fmt("BEFORE id 1 = ", _handling_id_1,
                         ", id 2 = ", _handling_id_2));

    _handling_id_1 = std::move(_handling_id_2);

    _log.tst(format::fmt("AFTER id 1 = ", _handling_id_1,
                         ", id 2 = ", _handling_id_2));

    return (_handling_id_1 == 5);
  }
};

struct handling_id_006 {
  static std::string desc() {
    return "Less than in equal size async::handling_id";
  }

  bool operator()(const program::options &) {
    log::cerr _log;
    async::handling_id _handling_id_1(2);
    async::handling_id _handling_id_2(3);

    _log.tst(
        format::fmt("id 1 = ", _handling_id_1, ", id 2 = ", _handling_id_2));

    return (_handling_id_1 < _handling_id_2);
  }
};

#endif
