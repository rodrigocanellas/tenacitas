/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Caellas - rodrigo.caellas at gmail.com

#ifndef TENACITAS_TST_OSTREAM_PAIR_OUTPUT_H
#define TENACITAS_TST_OSTREAM_PAIR_OUTPUT_H

#include <iostream>
#include <utility>

#include "tenacitas/src/ostream/pair.h"
#include "tenacitas/src/program/options.h"

using namespace tenacitas;

namespace tenacitas::src::ostream::tst
{

struct pair_output_000
{
  static std ::string desc()
  {
    return "Simple pair output";
  }

  bool operator()(const src::program::options &)
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

  bool operator()(const src::program::options &)
  {
    auto _pair{std::make_pair(-8, std::make_pair(0.3331, -99))};

    std::cerr << "pair = " << _pair << std::endl;

    return true;
  }
};

} // namespace tenacitas::src::ostream::tst

#endif
