/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Caellas - rodrigo.caellas at gmail.com

#ifndef TENACITAS_TST_TUPLE_TUPLE_TRANSFORM_H
#define TENACITAS_TST_TUPLE_TUPLE_TRANSFORM_H

#include <iostream>
#include <memory>
#include <tuple>
#include <type_traits>
#include <typeinfo>

#include "tenacitas/src/program/options.h"
#include "tenacitas/src/tuple/tuple_transform.h"

using namespace tenacitas;

namespace tenacitas::tst::tuple {

struct tuple_transform_000 {
  static std::string desc() {
    return "Checks if src::tuple::tuple_transform_t<std::shared_ptr, "
           "std::tuple<int, char>> works";
  }

  bool operator()(const src::program::options &) {
    using my_tuple = std::tuple<int, char>;

    using my_transformed_tuple =
        src::tuple::tuple_transform_t<std::shared_ptr, my_tuple>;

    if constexpr (!std::is_same_v<std::tuple_element_t<0, my_transformed_tuple>,
                                  std::shared_ptr<int>>) {
      std::cerr << "First type of my_transformed_tuple should be "
                   "std::shared_ptr<int>, but it is "
                << typeid(std::tuple_element_t<0, my_transformed_tuple>).name()
                << std::endl;
      return false;
    }

    if constexpr (!std::is_same_v<std::tuple_element_t<1, my_transformed_tuple>,
                                  std::shared_ptr<char>>) {
      std::cerr << "First type of my_transformed_tuple should be "
                   "std::shared_ptr<char>, but it is "
                << typeid(std::tuple_element_t<1, my_transformed_tuple>).name()
                << std::endl;
      return false;
    }

    return true;
  }
};

struct tuple_transform_001 {
  static std::string desc() {
    return "Checks if std::tuple_element_t<0, "
           "src::tuple::tuple_transform_t<std::shared_ptr, std::tuple<int, "
           "char>>> "
           "is not 'std::shared_ptr<char>' ";
  }

  bool operator()(const src::program::options &) {
    using my_tuple = std::tuple<int, char>;

    using my_transformed_tuple =
        src::tuple::tuple_transform_t<std::shared_ptr, my_tuple>;

    if constexpr (std::is_same_v<std::tuple_element_t<0, my_transformed_tuple>,
                                 std::shared_ptr<char>>) {
      std::cerr << "First type of my_transformed_tuple should be not be "
                   "std::shared_ptr<char>, but it is "
                << typeid(std::tuple_element_t<0, my_transformed_tuple>).name()
                << std::endl;
      return false;
    }

    return true;
  }
};

} // namespace tenacitas::tst::tuple

#endif
