/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Caellas - rodrigo.caellas at gmail.com

#ifndef TNCT_STRING_TST_TO_NUMBER_TEST_H
#define TNCT_STRING_TST_TO_NUMBER_TEST_H

#include "tnct/program/bus/options.h"
#include "tnct/string/bus/to_number.h"

using namespace tnct;

namespace tnct::string::tst {

struct to_number_000 {
  static std ::string desc() {
    return "Conversion from string '3215' to a std::uint16_t";
  }

  bool operator()(const program::bus::options &) {

    const auto _number{string::bus::to_number<std::uint16_t>("3215")};

    if (!_number) {
      return false;
    }

    if (*_number != 3215) {
      return false;
    }

    std::cout << "number = " << *_number << std::endl;

    return true;
  }
};

struct to_number_001 {
  static std ::string desc() {
    return "Fails to conversion '321a5' to a std::uint16_t";
  }

  bool operator()(const program::bus::options &) {

    const auto _number{string::bus::to_number<std::uint16_t>("321a5")};

    if (_number) {
      return false;
    }

    return true;
  }
};

struct to_number_002 {
  static std ::string desc() {
    return "Fails to conversion '3215' to a std::uint8_t";
  }

  bool operator()(const program::bus::options &) {

    const auto _number{string::bus::to_number<std::uint8_t>("3215")};

    if (_number) {
      return false;
    }

    return true;
  }
};

} // namespace tnct::string::tst

#endif
