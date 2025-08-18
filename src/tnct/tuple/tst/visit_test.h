/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Caellas - rodrigo.caellas at gmail.com

#ifndef TNCT_TUPLE_TST_VISIT_TEST_H
#define TNCT_TUPLE_TST_VISIT_TEST_H

#include <string>
#include <tuple>

#include "tnct/program/options.h"
#include "tnct/tuple/cpt/is_tuple.h"
#include "tnct/tuple/cpt/visit.h"

namespace tnct::tuple::tst
{

struct visit_000
{
  static std::string desc()
  {
    return "Checks is a function is compatible with "
           "'visit_value_in_constant_tuple'";
  }

  bool operator()(const program::options &)
  {
    using my_tuple = std::tuple<char, float>;

    auto _visit = []<tuple::cpt::is_tuple t_tuple, std::size_t t_idx>(
                      const t_tuple &) { return true; };

    static_assert(
        tuple::cpt::visit_value_in_constant_tuple<decltype(_visit), my_tuple>,
        "'_visit' should be compatible with "
        "'tuple::cpt::visit_value_in_constant_tuple'");
    return true;
  }
};

struct visit_001
{
  static std::string desc()
  {
    return "Checks is a function is compatible with "
           "'visit_value_in_mutable_tuple'";
  }

  bool operator()(const program::options &)
  {
    using my_tuple = std::tuple<char, float>;

    auto _visit = []<tuple::cpt::is_tuple t_tuple, std::size_t t_idx>(t_tuple &)
    { return true; };

    static_assert(
        tuple::cpt::visit_value_in_mutable_tuple<decltype(_visit), my_tuple>,
        "'_visit' should be compatible with "
        "'tuple::cpt::visit_value_in_mutable_tuple'");

    return true;
  }
};

struct visit_002
{
  static std::string desc()
  {
    return "Checks is a function is compatible with "
           "'visit_type_in_tuple'";
  }

  bool operator()(const program::options &)
  {
    using my_tuple = std::tuple<char, float>;

    auto _visit = []<tuple::cpt::is_tuple t_tuple, std::size_t t_idx>()
    { return true; };

    static_assert(tuple::cpt::visit_type_in_tuple<decltype(_visit), my_tuple>,
                  "'_visit' should be compatible with "
                  "'tuple::cpt::visit_type_tuple'");
    return true;
  }
};

struct visit_003
{
  static std::string desc()
  {
    return "Checks is a function is compatible with "
           "'visit_value_in_constant_tuple'";
  }

  bool operator()(const program::options &)
  {
    using my_tuple = std::tuple<char, float>;

    auto _visit = []<tuple::cpt::is_tuple t_tuple, std::size_t t_idx>(t_tuple &)
    { return true; };

    static_assert(
        !tuple::cpt::visit_value_in_constant_tuple<decltype(_visit), my_tuple>,
        "'_visit' should not be compatible with "
        "'tuple::cpt::visit_value_in_constant_tuple'");
    return true;
  }
};

struct visit_004
{
  static std::string desc()
  {
    return "Checks is a function is compatible with "
           "'visit_value_in_mutable_tuple'";
  }

  bool operator()(const program::options &)
  {
    using my_tuple = std::tuple<char, float>;

    auto _visit = []<tuple::cpt::is_tuple t_tuple, std::size_t t_idx>()
    { return true; };

    static_assert(
        !tuple::cpt::visit_value_in_mutable_tuple<decltype(_visit), my_tuple>,
        "'_visit' should be not compatible with "
        "'tuple::cpt::visit_value_in_mutable_tuple'");

    return true;
  }
};

struct visit_005
{
  static std::string desc()
  {
    return "Checks is a function is compatible with "
           "'visit_type_in_tuple'";
  }

  bool operator()(const program::options &)
  {
    using my_tuple = std::tuple<char, float>;

    auto _visit = []<tuple::cpt::is_tuple t_tuple, std::size_t t_idx>(t_tuple)
    { return true; };

    static_assert(!tuple::cpt::visit_type_in_tuple<decltype(_visit), my_tuple>,
                  "'_visit' should not be compatible with "
                  "'tuple::cpt::visit_type_tuple'");
    return true;
  }
};

} // namespace tnct::tuple::tst

#endif
