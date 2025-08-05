/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Caellas - rodrigo.caellas at gmail.com

#ifndef TENACITAS_TST_TRAITS_TUPLE_LIKE_H
#define TENACITAS_TST_TRAITS_TUPLE_LIKE_H

#include <iostream>
#include <tuple>
// #include <vector> -- this must be uncommented in order to verify that test
// tuple_like_002 actually causes a compile error

#include "tenacitas/src/program/options.h"
#include "tenacitas/src/traits/tuple/is_tuple.h"

using namespace tenacitas;

namespace tenacitas::tst::traits {

template <src::traits::tuple::is_tuple t_tuple> void func(t_tuple &&p_tuple) {
  std::cerr << "tuple<0>    = " << std::get<0>(p_tuple) << std::endl;
}

struct tuple_like_000 {
  static std::string desc() {
    return "Passing a 'std::tuple' object to a template function which type "
           "parameter is restricted by the concept 'tuple:::like'";
  }

  bool operator()(const src::program::options &) {

    // this should compile
    func(std::make_tuple(int{-9}));

    return true;
  }
};

struct tuple_like_001 {
  static std::string desc() {
    return "Passing a 'int' object to a template function which type parameter "
           "is restricted by the concept 'tuple:::like'. The code will "
           "not compile, so the code is commented.";
  }

  bool operator()(const src::program::options &) {

    // this will NOT compile
    //    func(int{-9});

    return true;
  }
};

struct tuple_like_002 {
  static std::string desc() {
    return "Passing a 'std::vector<double>' object to a template function "
           "which type parameter is restricted by the concept "
           "'tuple:::like'. The code will not compile, so the code is "
           "commented.";
  }

  bool operator()(const src::program::options &) {

    // this will NOT compile
    // func(std::vector<double>{0.21, 3.14, -8312121.84});

    return true;
  }
};

} // namespace tenacitas::tst::traits

#endif
