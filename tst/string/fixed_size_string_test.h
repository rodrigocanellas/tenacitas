/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Caellas - rodrigo.caellas at gmail.com

#ifndef TENACITAS_TST_STRING_FIXED_SIZE_STRING_H
#define TENACITAS_TST_STRING_FIXED_SIZE_STRING_H

#include <iostream>

#include "tenacitas/src/program/options.h"
#include "tenacitas/src/string/fixed_size_string.h"

using namespace tenacitas;

namespace tenacitas::tst::string {

struct fixed_size_string_000 {
  static std ::string desc() { return "Comparing 2 fixed_size_string"; }

  bool operator()(const src::program::options &) {
    src::string::fixed_size_string<10> _str1{"hello!!"};
    src::string::fixed_size_string<10> _str2{"hello!!"};

    std::cerr << "str 1 = " << _str1 << ", str2 = " << _str2 << std::endl;

    return _str1 == _str2;
  }
};

struct fixed_size_string_001 {
  static std ::string desc() {
    return "Testing fixed_size_src::string::empty() on a empty "
           "fixed_size_string";
  }

  bool operator()(const src::program::options &) {
    src::string::fixed_size_string<10> _str1;

    std::cerr << "str 1 = " << _str1 << std::endl;

    return _str1.empty();
  }
};

struct fixed_size_string_002 {
  static std ::string desc() {
    return "Testing fixed_size_src::string::empty() on a non empty "
           "fixed_size_string";
  }

  bool operator()(const src::program::options &) {
    src::string::fixed_size_string<10> _str1{"a"};

    std::cerr << "str 1 = " << _str1 << std::endl;

    return !_str1.empty();
  }
};

struct fixed_size_string_003 {
  static std ::string desc() {
    return "Compile time error trying to create a fixe_size_string with a "
           "'const char *' size greater than defined ";
  }

  bool operator()(const src::program::options &) {
    // this code generates a compile time error
    // src::string::fixed_size_string<10> _str1{"0123456789A"};

    return true;
  }
};

struct fixed_size_string_004 {
  static std ::string desc() {
    return "Creating from a std::string with size less than maximum";
  }

  bool operator()(const src::program::options &) {
    const std::string _s{"012345"};
    src::string::fixed_size_string<10> _str1{_s};

    std::cerr << "str 1 = " << _str1 << std::endl;

    src::string::fixed_size_string<10> _str2{"012345"};
    return _str1 == _str2;
  }
};

struct fixed_size_string_005 {
  static std ::string desc() {
    return "Creating from a std::string with size greather than maximum";
  }

  bool operator()(const src::program::options &) {
    try {
      const std::string _s{"0123456789A"};
      src::string::fixed_size_string<10> _str1{_s};

      std::cerr << "str 1 = " << _str1 << std::endl;

      return false;

    } catch (std::exception &_ex) {
      std::cerr << "error: " << _ex.what() << std::endl;
    }
    return true;
  }
};

} // namespace tenacitas::tst::string

#endif
