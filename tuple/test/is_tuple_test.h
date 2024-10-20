/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Caellas - rodrigo.caellas at gmail.com

#ifndef TENACITAS_LIB_TUPLE_TEST_IS_TUPLE_TEST_H
#define TENACITAS_LIB_TUPLE_TEST_IS_TUPLE_TEST_H

#include <tuple>
#include <vector>

#include <tenacitas.lib/program/options.h>
#include <tenacitas.lib/tuple/is_tuple.h>

using namespace tenacitas::lib;

namespace tenacitas::lib::tuple::test {

struct is_tuple_000 {
  static std::string desc() { return "Checking if a 'int' is not a tuple"; }

  bool operator()(const program::options &) { return !tuple::is_tuple_v<int>; }
};

struct is_tuple_001 {
  static std::string desc() {
    return "Checking if a 'std::tuple<>' is a tuple";
  }

  bool operator()(const program::options &) {
    return tuple::is_tuple_v<std::tuple<>>;
  }
};

struct is_tuple_002 {
  static std::string desc() {
    return "Checking if a 'std::tuple<char>' is a tuple";
  }

  bool operator()(const program::options &) {
    return tuple::is_tuple_v<std::tuple<char>>;
  }
};

struct is_tuple_003 {
  static std::string desc() {
    return "Checking if a 'std::tuple<char, std::string>' is a tuple";
  }

  bool operator()(const program::options &) {
    return tuple::is_tuple_v<std::tuple<char, std::string>>;
  }
};

struct is_tuple_004 {
  static std::string desc() {
    return "Checking if a 'std::vector<int>' is not a tuple";
  }

  bool operator()(const program::options &) {
    return !tuple::is_tuple_v<std::vector<int>>;
  }
};

struct is_tuple_005 {
  static std::string desc() {
    return "Checking if a 'std::vector<std::tuple<double>>' is not a tuple";
  }

  bool operator()(const program::options &) {
    return !tuple::is_tuple_v<std::vector<std::tuple<double>>>;
  }
};

struct is_tuple_006 {
  static std::string desc() {
    return "Checking if a 'std::tuple<double, std::vector<char>>' is a tuple";
  }

  bool operator()(const program::options &) {
    return tuple::is_tuple_v<std::tuple<double, std::vector<char>>>;
  }
};

} // namespace tenacitas::lib::tuple::test

#endif
