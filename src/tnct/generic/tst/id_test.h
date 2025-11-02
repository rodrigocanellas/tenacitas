/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_GENERIC_TST_ID_TEST_H
#define TNCT_GENERIC_TST_ID_TEST_H

#include <string>

#include "tnct/generic/dat/id.h"
#include "tnct/program/options.h"

namespace tnct::generic::tst
{

struct id_001
{
  static std::string desc()
  {
    return "id _i {4}, which does not compile";
  }

  bool operator()(const program::options &)
  {

    // THIS FAILS TO COMPILE BECAUSE '4' IS AUTOMATICALLY DEDUCED TO 'int',
    // WHICH DOES NOT SATISFY 'id'

    // id<0> auto _i{4};
    return true;
  }
};

struct id_002
{
  static std::string desc()
  {
    return "id _i {-4}, which does not compile";
  }

  bool operator()(const program::options &)
  {

    // THIS FAILS TO COMPILE BECAUSE '-4' IS AUTOMATICALLY DEDUCED TO 'int',
    // WHICH DOES NOT SATISFY 'id'

    // id<0> auto _i{-4};

    return true;
  }
};

struct id_003
{
  static std::string desc()
  {
    return "Simply prints a id";
  }

  bool operator()(const program::options &)
  {
    using id_0 = generic::dat::id<int>;
    id_0 _i;

    std::cout << _i << std::endl;

    return true;
  }
};

struct id_004
{
  static std::string desc()
  {
    return "Test that id objects are not the same";
  }

  bool operator()(const program::options &)
  {
    using id_0 = generic::dat::id<int>;
    id_0 _i;
    id_0 _j;

    std::cout << "i = " << _i << ", and j = " << _j << std::endl;

    return _i != _j;
  }
};

struct id_005
{
  static std::string desc()
  {
    return "Fails to compile when trying to compare id<0> and id<1>";
  }

  bool operator()(const program::options &)
  {
    using id_0 = generic::dat::id<int>;
    using id_1 = generic::dat::id<std::string>;
    id_0 _i;
    id_1 _j;

    std::cout << "i = " << _i << ", and j = " << _j << std::endl;

    //        return _i != _j; THIS FAILS TO COMPILE, AS IT SHOULD
    return true;
  }
};
} // namespace tnct::generic::tst
#endif
