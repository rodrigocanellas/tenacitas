/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Caellas - rodrigo.caellas at gmail.com

#ifndef TNCT_TUPLE_TST_CONTAINS_TUPLE_TEST_H
#define TNCT_TUPLE_TST_CONTAINS_TUPLE_TEST_H

#include "tnct/program/options.h"
#include "tnct/tuple/cpt/contains_tuple.h"

namespace tnct::tuple::tst
{

struct contains_tuple_000
{
  static std::string desc()
  {
    return "Checks if 'std::tuple<char>' is contained in 'std::tuple<int, "
           "char, float>'";
  }

  bool operator()(const program::options &)
  {
    using tuple_1 = std::tuple<int, char, float>;
    using tuple_2 = std::tuple<char>;

    static_assert(
        tuple::cpt::contains_tuple<tuple_1, tuple_2>,
        "'std::tuple<int, char, float>' should contain 'std::tuple<char>'");

    return true;
  }
};

struct contains_tuple_001
{
  static std::string desc()
  {
    return "Assures that 'std::tuple<int, char, float>' is not contained in "
           "'std::tuple<char>'";
  }

  bool operator()(const program::options &)
  {
    using tuple_1 = std::tuple<int, char, float>;
    using tuple_2 = std::tuple<char>;

    static_assert(
        !tuple::cpt::contains_tuple<tuple_2, tuple_1>,
        "'std::tuple<char>' should not contain 'std::tuple<int, char, float>'");

    return true;
  }
};

struct contains_tuple_002
{
  static std::string desc()
  {
    return "Assures that to check if 'std::tuple<double>' is contained in "
           "'std::tuple<int, char, float>' does not compile";
  }

  bool operator()(const program::options &)
  {
    using tuple_1 = std::tuple<int, char, float>;
    using tuple_2 = std::tuple<double>;

    static_assert(!tuple::cpt::contains_tuple<tuple_2, tuple_1>,
                  "'std::tuple<int, char, float>' should not contain "
                  "'std::tuple<double>'");

    return true;
  }
};

struct contains_tuple_003
{
  static std::string desc()
  {
    return "Checks if 'std::tuple<int>' is contained in 'std::tuple<int, "
           "char, float>'";
  }

  bool operator()(const program::options &)
  {
    using tuple_1 = std::tuple<int, char, float>;
    using tuple_2 = std::tuple<int>;

    static_assert(tuple::cpt::contains_tuple<tuple_1, tuple_2>,
                  "'std::tuple<int, char, float>' should contain 'tuple_2'");
    return true;
  }
};

struct contains_tuple_004
{
  static std::string desc()
  {
    return "Checks if 'std::tuple<float>' is contained in 'std::tuple<int, "
           "char, float>'";
  }

  bool operator()(const program::options &)
  {
    using tuple_1 = std::tuple<int, char, float>;
    using tuple_2 = std::tuple<float>;

    static_assert(
        tuple::cpt::contains_tuple<tuple_1, tuple_2>,
        "std::tuple<int, char, float> should contain std::tuple<float>");
    return true;
  }
};

struct contains_tuple_005
{
  static std::string desc()
  {
    return "Checks if 'std::tuple<int, char, float>' is contained in "
           "'std::tuple<int, "
           "char, float>'";
  }

  bool operator()(const program::options &)
  {
    using tuple_1 = std::tuple<int, char, float>;
    using tuple_2 = std::tuple<int, char, float>;

    static_assert(tuple::cpt::contains_tuple<tuple_1, tuple_2>,
                  "std::tuple<int, char, float> should contain std::tuple<int, "
                  "char, float>");

    return true;
  }
};

struct contains_tuple_006
{
  static std::string desc()
  {
    return "Checks if 'std::tuple<float, int, char>' is contained in "
           "'std::tuple<int, char, float>'";
  }

  bool operator()(const program::options &)
  {
    using tuple_1 = std::tuple<int, char, float>;
    using tuple_2 = std::tuple<float, int, char>;

    static_assert(tuple::cpt::contains_tuple<tuple_1, tuple_2>,
                  "std::tuple<int, char, float> should contain "
                  "std::tuple<float, int, char>");

    return true;
  }
};

struct contains_tuple_007
{
  static std::string desc()
  {
    return "Checks if 'std::tuple<int, char>' is contained in "
           "'std::tuple<int, "
           "char, float>'";
  }

  bool operator()(const program::options &)
  {
    using tuple_1 = std::tuple<int, char, float>;
    using tuple_2 = std::tuple<int, char>;

    // this should compile
    static_assert(tuple::cpt::contains_tuple<tuple_1, tuple_2>,
                  "std::tuple<int, char, float> should contain "
                  "std::tuple<int, char>");

    return true;
  }
};

} // namespace tnct::tuple::tst

#endif
