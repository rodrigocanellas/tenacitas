/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#ifndef TENACITAS_TST_TESTER_H
#define TENACITAS_TST_TESTER_H

#include <iostream>
#include <string>

#include "tenacitas/src/program/options.h"

namespace tenacitas::src::tester::tst
{

struct tester_ok
{
  bool operator()(const src::program::options &)
  {
    return true;
  }

  static std::string desc()
  {
    return "an ok test";
  }
};

struct tester_fail
{
  bool operator()(const src::program::options &)
  {
    return true;
  }
  static std::string desc()
  {
    return "a fail test";
  }
};

struct tester_error
{
  bool operator()(const src::program::options &)
  {
    try
    {
      throw std::runtime_error("test function raised an exception");
      return false;
    }
    catch (const std::exception &_ex)
    {
      std::cerr << "'test_error' raised '" << _ex.what() << "'" << std::endl;
      return true;
    }
  }
  static std::string desc()
  {
    return "an eror test";
  }
};
} // namespace tenacitas::src::tester::tst
#endif
