/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Caellas - rodrigo.caellas at gmail.com

#ifndef TNCT_TUPLE_TST_CONTAINS_TYPE_TEST_H
#define TNCT_TUPLE_TST_CONTAINS_TYPE_TEST_H

#include <tuple>

#include "tnct/program/options.h"
#include "tnct/tuple/contains_type.h"

using namespace tnct;

namespace tnct::tuple::tst
{

struct contains_type_000
{
  static std::string desc()
  {
    return "Tries to find existing simple types in a std::tuple";
  }

  bool operator()(const program::options &)
  {
    using my_tuple = std::tuple<int, char, float>;

    auto _found{tuple::contains_type<my_tuple, int>()};
    if (!_found)
    {
      std::cerr << "ERROR! 'int' was not found, but it should have been "
                << std::endl;
      return false;
    }
    std::cerr << "'int' found, as expected" << std::endl;

    _found = tuple::contains_type<my_tuple, char>();
    if (!_found)
    {
      std::cerr << "ERROR! 'char' was not found, but it should have been "
                << std::endl;
      return false;
    }
    std::cerr << "'char' found, as expected" << std::endl;

    _found = tuple::contains_type<my_tuple, float>();
    if (!_found)
    {
      std::cerr << "ERROR! 'flaot' was not found, but it should have been "
                << std::endl;
      return false;
    }
    std::cerr << "'float' found, as expected" << std::endl;
    return true;
  }
};

struct contains_type_001
{
  static std::string desc()
  {
    return "Tries to find a non existing type in a std::tuple";
  }

  bool operator()(const program::options &)
  {
    using my_tuple = std::tuple<int, char, float>;

    auto _found{tuple::contains_type<my_tuple, std::string>()};
    if (_found)
    {
      std::cerr << "'std::string' should not be found, but is was" << std::endl;
      return false;
    }
    std::cerr << "'std::string' was not found, as it should" << std::endl;
    return true;
  }
};

struct contains_type_002
{
  static std::string desc()
  {
    return "Tries to find a type in a std::tuple<>";
  }

  bool operator()(const program::options &)
  {
    using my_tuple = std::tuple<>;

    auto _found{tuple::contains_type<my_tuple, std::string>()};
    if (_found)
    {
      std::cerr << "'std::string' should not be found, but is was" << std::endl;
      return false;
    }
    std::cerr << "'std::string' was not found, as it should" << std::endl;
    return true;
  }
};
} // namespace tnct::tuple::tst

#endif
