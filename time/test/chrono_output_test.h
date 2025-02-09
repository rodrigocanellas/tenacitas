/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Caellas - rodrigo.caellas at gmail.com

#ifndef TENACITAS_LIB_TIME_TEST_CHRONO_OUTPUT_TEST_H
#define TENACITAS_LIB_TIME_TEST_CHRONO_OUTPUT_TEST_H

#include <chrono>
#include <iostream>
#include <sstream>

#include "tenacitas.lib/program/options.h"
#include "tenacitas.lib/time/chrono_output.h"

using namespace tenacitas::lib;

namespace tenacitas::lib::time::test {

struct time_output_000 {
  static std ::string desc() { return "Output nanoseconds"; }

  bool operator()(const program::options &) {
    std::ostringstream _stream;

    _stream << std::chrono::nanoseconds(39);

    const std::string _str{_stream.str()};
    std::cerr << "time = " << _str << std::endl;

    return _str == "39 ns";
  }
};

struct time_output_001 {
  static std ::string desc() { return "Output microseconds"; }

  bool operator()(const program::options &) {

    std::ostringstream _stream;

    _stream << std::chrono::microseconds(39);

    const std::string _str{_stream.str()};
    std::cerr << "time = " << _str << std::endl;

    return _str == "39 us";
  }
};

struct time_output_002 {
  static std ::string desc() { return "Output milliseconds"; }

  bool operator()(const program::options &) {

    std::ostringstream _stream;

    _stream << std::chrono::milliseconds(39);

    const std::string _str{_stream.str()};
    std::cerr << "time = " << _str << std::endl;

    return _str == "39 ms";
  }
};

struct time_output_003 {
  static std ::string desc() { return "Output seconds"; }

  bool operator()(const program::options &) {

    std::ostringstream _stream;

    _stream << std::chrono::seconds(39);

    const std::string _str{_stream.str()};
    std::cerr << "time = " << _str << std::endl;

    return _str == "39 s";
  }
};

struct time_output_004 {
  static std ::string desc() { return "Output minutes"; }

  bool operator()(const program::options &) {

    std::ostringstream _stream;

    _stream << std::chrono::minutes(39);

    const std::string _str{_stream.str()};
    std::cerr << "time = " << _str << std::endl;

    return _str == "39 m";
  }
};

struct time_output_005 {
  static std ::string desc() { return "Output hours"; }

  bool operator()(const program::options &) {

    std::ostringstream _stream;

    _stream << std::chrono::hours(39);

    const std::string _str{_stream.str()};
    std::cerr << "time = " << _str << std::endl;

    return _str == "39 h";
  }
};

struct time_output_006 {
  static std ::string desc() { return "Output days"; }

  bool operator()(const program::options &) {

    std::ostringstream _stream;

    _stream << std::chrono::days(39);

    const std::string _str{_stream.str()};
    std::cerr << "time = " << _str << std::endl;

    return _str == "39 d";
  }
};

struct time_output_007 {
  static std ::string desc() { return "Output months"; }

  bool operator()(const program::options &) {

    std::ostringstream _stream;

    _stream << std::chrono::months(39);

    const std::string _str{_stream.str()};
    std::cerr << "time = " << _str << std::endl;

    return _str == "39 M";
  }
};

struct time_output_008 {
  static std ::string desc() { return "Output years"; }

  bool operator()(const program::options &) {

    std::ostringstream _stream;

    _stream << std::chrono::years(39);

    const std::string _str{_stream.str()};
    std::cerr << "time = " << _str << std::endl;

    return _str == "39 y";
  }
};

} // namespace tenacitas::lib::time::test

#endif
