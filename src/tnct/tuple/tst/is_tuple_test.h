/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Caellas - rodrigo.caellas at gmail.com

#ifndef TNCT_TUPLE_TST_IS_TUPLE_TEST_H
#define TNCT_TUPLE_TST_IS_TUPLE_TEST_H

#include <vector>

#include "tnct/program/bus/options.h"
#include "tnct/tuple/cpt/is_tuple.h"

namespace tnct::tuple::tst
{
struct is_tuple_000
{
  static std::string desc()
  {
    return "Checking if 'std::tuple<int>' is compatible with "
           "'tuple::cpt::is_tuple'";
  }

  bool operator()(const program::bus::options &)
  {

    static_assert(
        tuple::cpt::is_tuple<std::tuple<int>>,
        "'std::tuple<int>' shuld be compatible with 'tuple::cpt::is_tuple");

    return true;
  }
};

struct is_tuple_001
{
  static std::string desc()
  {
    return "Checking if 'int' is not compatible with 'tuple::cpt::is_tuple'";
  }

  bool operator()(const program::bus::options &)
  {
    static_assert(!tuple::cpt::is_tuple<int>,
                  "'int' shuld be not compatible with 'tuple::cpt::is_tuple");

    return true;
  }
};

struct is_tuple_002
{
  static std::string desc()
  {
    return "Checking if 'std::vector<double>' is not compatible with "
           "'tuple::cpt::is_tuple'";
  }

  bool operator()(const program::bus::options &)
  {
    static_assert(!tuple::cpt::is_tuple<std::vector<double>>,
                  "'std::vector<double>' shuld be not compatible with "
                  "'tuple::cpt::is_tuple");
    return true;
  }
};

} // namespace tnct::tuple::tst

#endif
