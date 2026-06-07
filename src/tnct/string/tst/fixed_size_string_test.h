/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Caellas - rodrigo.caellas at gmail.com

#ifndef TNCT_STRING_TST_FIXED_SIZE_STRING_TEST_H
#define TNCT_STRING_TST_FIXED_SIZE_STRING_TEST_H

#include <iostream>

#include "tnct/program/bus/options.h"
#include "tnct/string/dat/fixed_size_string.h"

using namespace tnct;

namespace tnct::string::tst {

struct fixed_size_string_000 {
  static std ::string desc() { return "Comparing 2 fixed_size_string"; }

  bool operator()(const program::bus::options &) {
    string::dat::fixed_size_string<10> _str1{"hello!!"};
    string::dat::fixed_size_string<10> _str2{"hello!!"};

    std::cerr << "str 1 = " << _str1 << ", str2 = " << _str2 << std::endl;

    return _str1 == _str2;
  }
};

struct fixed_size_string_001 {
  static std ::string desc() {
    return "Testing fixed_size_string::empty() on a empty "
           "fixed_size_string";
  }

  bool operator()(const program::bus::options &) {
    string::dat::fixed_size_string<10> _str1;

    std::cerr << "str 1 = " << _str1 << std::endl;

    return _str1.empty();
  }
};

struct fixed_size_string_002 {
  static std ::string desc() {
    return "Testing fixed_size_string::empty() on a non empty "
           "fixed_size_string";
  }

  bool operator()(const program::bus::options &) {
    string::dat::fixed_size_string<10> _str1{"a"};

    std::cerr << "str 1 = " << _str1 << std::endl;

    return !_str1.empty();
  }
};

struct fixed_size_string_003 {
  static std ::string desc() {
    return "Compile time error trying to create a fixe_size_string with a "
           "'const char *' size greater than defined ";
  }

  bool operator()(const program::bus::options &) {
    // this code generates a compile time error
    // string::dat::fixed_size_string<10> _str1{"0123456789A"};

    return true;
  }
};

struct fixed_size_string_004 {
  static std ::string desc() {
    return "Creating from a std::string with size less than maximum";
  }

  bool operator()(const program::bus::options &) {
    const std::string _s{"012345"};
    string::dat::fixed_size_string<10> _str1{_s};

    std::cerr << "str 1 = " << _str1 << std::endl;

    string::dat::fixed_size_string<10> _str2{"012345"};
    return _str1 == _str2;
  }
};

struct fixed_size_string_005 {
  static std ::string desc() {
    return "Creating from a std::string with size greather than maximum";
  }

  bool operator()(const program::bus::options &) {
    try {
      const std::string _s{"0123456789A"};
      string::dat::fixed_size_string<10> _str1{_s};

      std::cerr << "str 1 = " << _str1 << std::endl;

      return false;
    } catch (std::exception &_ex) {
      std::cerr << "error: " << _ex.what() << std::endl;
    }
    return true;
  }
};

struct fixed_size_string_006 {
  static std ::string desc() {
    return "Converts a fixed_size_string to a std::string";
  }

  bool operator()(const program::bus::options &) {
    try {
      string::dat::fixed_size_string<15> _fixed_size_string{"hello!!!"};
      std::string _str{_fixed_size_string};
      return (_str == "hello!!!");
    } catch (std::exception &_ex) {
    }
    return false;
  }
};

struct fixed_size_string_007 {
  static std ::string desc() {
    return "Checks non compatibility between fixed_size_string::const_iterator "
           "and std::string::const_iterator::pointer";
  }

  bool operator()(const program::bus::options &) {
    static_assert(
        std::is_same_v<
            typename string::dat::fixed_size_string<2>::const_iterator,
            std::string::const_iterator::pointer>,
        "fixed_size_string::const_iterator and std::string::const_iterator are "
        "not compatible");
    return true;
  }
};
} // namespace tnct::string::tst

#endif
