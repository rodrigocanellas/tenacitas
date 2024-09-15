/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_TEST_GENERIC_ID_H
#define TENACITAS_LIB_TEST_GENERIC_ID_H
#include <string>

#include <tenacitas.lib/generic/id.h>
#include <tenacitas.lib/program/options.h>

using namespace tenacitas::lib;

struct id_001 {
  static std::string desc() { return "id _i {4}, which does not compile"; }

  bool operator()(const program::options &) {

    // THIS FAILS TO COMPILE BECAUSE '4' IS AUTOMATICALLY DEDUCED TO 'int',
    // WHICH DOES NOT SATISFY 'id'

    // traits::id<0> auto _i{4};
    return true;
  }
};

struct id_002 {
  static std::string desc() { return "id _i {-4}, which does not compile"; }

  bool operator()(const program::options &) {

    // THIS FAILS TO COMPILE BECAUSE '-4' IS AUTOMATICALLY DEDUCED TO 'int',
    // WHICH DOES NOT SATISFY 'id'

    // traits::id<0> auto _i{-4};

    return true;
  }
};

struct id_003 {
  static std::string desc() {
    return "id _i {std::numeric_limits<size_t>::max()}";
  }

  bool operator()(const program::options &) {
    using id_0 = generic::id<0>;
    id_0 _i{std::numeric_limits<size_t>::max()};

    return (_i == id_0(std::numeric_limits<size_t>::max()));
  }
};

struct id_004 {
  static std::string desc() { return "id _i {uint16_t{4}}"; }

  bool operator()(const program::options &) {
    using id_abc = generic::id<372>;
    id_abc _i{uint16_t{4}};

    return (_i == id_abc(static_cast<uint8_t>(4)));
  }
};

struct id_005 {
  static std::string desc() {
    return "compiler error because id<2> can not be compared to id<921>";
  }

  bool operator()(const program::options &) {
    using id_2 = generic::id<2>;
    using id_921 = generic::id<921>;
    id_2 _i_2{uint16_t{4}};
    id_921 _id_921(static_cast<uint8_t>(87));

    // this causes a compiler error
    // if (_i_2 == _id_921) {
    //   return false;
    // }

    return true;
  }
};

#endif
