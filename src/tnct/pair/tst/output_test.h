/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Caellas - rodrigo.caellas at gmail.com

#ifndef TNCT_PAIR_TST_OUTPUT_TEST_H
#define TNCT_PAIR_TST_OUTPUT_TEST_H

#include <iostream>
#include <utility>

#include "tnct/pair/output.h"
#include "tnct/program/bus/options.h"

using namespace tnct;

namespace tnct::pair::tst
{

struct pair_output_000
{
  static std ::string desc()
  {
    return "Simple pair output";
  }

  bool operator()(const program::bus::options &)
  {
    std::pair<int, char> _pair{-8, 'H'};

    std::cerr << "pair = " << _pair << std::endl;

    return true;
  }
};

struct pair_output_001
{
  static std ::string desc()
  {
    return "Prints a pair with a pair inside";
  }

  bool operator()(const program::bus::options &)
  {
    auto _pair{std::make_pair(-8, std::make_pair(0.3331, -99))};

    std::cerr << "pair = " << _pair << std::endl;

    return true;
  }
};

} // namespace tnct::pair::tst

#endif
