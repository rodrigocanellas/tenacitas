/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Caellas - rodrigo.caellas at gmail.com

#ifndef TENACITAS_LIB_TRAITS_TEST_TUPLE_LIKE_TEST_H
#define TENACITAS_LIB_TRAITS_TEST_TUPLE_LIKE_TEST_H

#include <iostream>
#include <tuple>
// #include <vector> -- this must be uncommented in order to verify that test
// tuple_like_002 actually causes a compile error

#include <tenacitas.lib/program/options.h>
#include <tenacitas.lib/traits/tuple_like.h>

using namespace tenacitas::lib;

namespace tenacitas::lib::traits::test {

template <traits::tuple_like t_tuple> void func(t_tuple &&p_tuple) {
  std::cerr << "tuple<0>    = " << std::get<0>(p_tuple) << std::endl;
}

struct tuple_like_000 {
  static std::string desc() {
    return "Passing a 'std::tuple' object to a template function which type "
           "parameter is restricted by the concept 'tuple:::tuple_like'";
  }

  bool operator()(const program::options &) {

    // this should compile
    func(std::make_tuple(int{-9}));

    return true;
  }
};

struct tuple_like_001 {
  static std::string desc() {
    return "Passing a 'int' object to a template function which type parameter "
           "is restricted by the concept 'tuple:::tuple_like'. The code will "
           "not compile, so the code is commented.";
  }

  bool operator()(const program::options &) {

    // this will NOT compile
    //    func(int{-9});

    return true;
  }
};

struct tuple_like_002 {
  static std::string desc() {
    return "Passing a 'std::vector<double>' object to a template function "
           "which type parameter is restricted by the concept "
           "'tuple:::tuple_like'. The code will not compile, so the code is "
           "commented.";
  }

  bool operator()(const program::options &) {

    // this will NOT compile
    // func(std::vector<double>{0.21, 3.14, -8312121.84});

    return true;
  }
};

} // namespace tenacitas::lib::traits::test

#endif
