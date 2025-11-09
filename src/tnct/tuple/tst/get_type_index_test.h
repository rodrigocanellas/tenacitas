/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Caellas - rodrigo.caellas at gmail.com

#ifndef TNCT_TUPLE_TST_GET_TYPE_INDEX_TEST_H
#define TNCT_TUPLE_TST_GET_TYPE_INDEX_TEST_H

#include <cstdint>
#include <tuple>

#include "tnct/program/bus/options.h"
#include "tnct/tuple/bus/get_type_index.h"

using namespace tnct;

namespace tnct::tuple::tst
{

struct get_type_index_000
{
  static std::string desc()
  {
    return "Tries to find existing simple types in a std::tuple";
  }

  bool operator()(const program::bus::options &)
  {
    using my_tuple = std::tuple<int, char, float>;

    auto _idx{tuple::bus::get_type_index<my_tuple, int>()};
    if (!_idx)
    {
      std::cerr << "ERROR! idx should be 0, but the 'int' was not found "
                << std::endl;
      return false;
    }
    auto _index{_idx.value()};
    if (_index != 0)
    {
      std::cerr << "idx should be 0, but it is " << _index << std::endl;
      return false;
    }
    std::cerr << "idx is " << _index << ", as expected" << std::endl;

    _idx = tuple::bus::get_type_index<my_tuple, char>();
    if (!_idx)
    {
      std::cerr << "ERROR! idx should be 1, but the 'char' was not found "
                << std::endl;
      return false;
    }
    _index = _idx.value();
    if (_index != 1)
    {
      std::cerr << "idx should be 1, but it is " << _index << std::endl;
      return false;
    }
    std::cerr << "idx is " << _index << ", as expected" << std::endl;

    _idx = tuple::bus::get_type_index<my_tuple, float>();
    if (!_idx)
    {
      std::cerr << "ERROR! idx should be 2, but it 'char' was not found "
                << std::endl;
      return false;
    }
    _index = _idx.value();
    if (_index != 2)
    {
      std::cerr << "idx should be 2, but it is " << _index << std::endl;
      return false;
    }
    std::cerr << "idx is " << _index << ", as expected" << std::endl;

    return true;
  }
};

struct get_type_index_001
{
  static std::string desc()
  {
    return "Tries to find a non existing type in a std::tuple";
  }

  bool operator()(const program::bus::options &)
  {
    using my_tuple = std::tuple<int, char, float>;

    auto _idx{tuple::bus::get_type_index<my_tuple, std::string>()};
    if (_idx)
    {
      std::cerr << "'std::string' should not be found, but it is "
                << _idx.value() << std::endl;
      return false;
    }
    std::cerr << "'std::string' was not found, as it should" << std::endl;
    return true;
  }
};

struct get_type_index_002
{
  static std::string desc()
  {
    return "Tries to find a type in a std::tuple<>";
  }

  bool operator()(const program::bus::options &)
  {
    using my_tuple = std::tuple<>;

    auto _idx{tuple::bus::get_type_index<my_tuple, std::string>()};
    if (_idx)
    {
      std::cerr << "'std::string' should not be found, but is is "
                << _idx.value() << std::endl;
      return false;
    }
    std::cerr << "'std::string' was not found, as it should" << std::endl;
    return true;
  }
};

struct get_type_index_003
{
  static std ::string desc()
  {
    return "Finds a non POD in a tuple";
  }

  bool operator()(const program::bus::options &)
  {

    using my_tuple = std::tuple<abc, xyz>;

    auto _check{[]<tuple::cpt::is_tuple t_tuple, std::size_t t_idx>()
                {
                  using inner_type =
                      typename std::tuple_element_t<t_idx, t_tuple>::type;
                  return std::is_same_v<inner_type, char>;
                }

    };

    const auto optional{tuple::bus::get_type_index<my_tuple>(_check)};

    return (optional.has_value() && (optional == 0));
  }

private:
  struct abc
  {
    using type = char;
  };

  struct xyz
  {
    using type = float;
  };
};

struct get_type_index_004
{
  static std ::string desc()
  {
    return "Fails to find a non POD in tuple";
  }

  bool operator()(const program::bus::options &)
  {

    using my_tuple = std::tuple<abc, xyz>;

    auto _check{[]<tuple::cpt::is_tuple t_tuple, std::size_t t_idx>()
                {
                  using inner_type =
                      typename std::tuple_element_t<t_idx, t_tuple>::type;
                  return std::is_same_v<inner_type, std::uint16_t>;
                }

    };

    const auto optional{tuple::bus::get_type_index<my_tuple>(_check)};

    return (!optional.has_value());
  }

private:
  struct abc
  {
    using type = char;
  };

  struct xyz
  {
    using type = float;
  };
};

} // namespace tnct::tuple::tst

#endif
