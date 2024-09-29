/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#ifndef TENACITAS_LIB_TEST_TRAITS_TUPLE_H
#define TENACITAS_LIB_TEST_TRAITS_TUPLE_H

#include <iostream>
#include <string>
#include <tuple>

#include <tenacitas.lib/program/options.h>
#include <tenacitas.lib/generic/tuple_output.h>

using namespace tenacitas::lib;

struct a {
  friend std::ostream &operator<<(std::ostream &p_out, const a &) {
    p_out << "a obj";
    return p_out;
  }
};

struct b {
  friend std::ostream &operator<<(std::ostream &p_out, const b &) {
    p_out << "b obj";
    return p_out;
  }
};

using my_tuple = std::tuple<a, char, b, float>;

struct tuple_000 {
  static std::string desc() {
    return "Traverses a tuple through all its values";
  }

  bool operator()(const program::options &) {
    my_tuple _my_tuple;

    traits::tuple_value_traverse(
        [&]<size_t t_idx>(my_tuple &) {
          std::cerr << "at index " << t_idx << " there is a "
                    << typeid(std::tuple_element_t<t_idx, my_tuple>).name()
                    << std::endl;
          return true;
        },
        _my_tuple);

    return true;
  }
};

struct tuple_001 {
  static std::string desc() {
    return "Traverses a tuple through all its values until type 'b' is found";
  }

  bool operator()(const program::options &) {
    my_tuple _my_tuple;
    traits::tuple_value_traverse(
        [&]<size_t t_idx>(my_tuple &) {
          if (std::is_same_v<std::tuple_element_t<t_idx, my_tuple>, b>) {
            std::cerr << "'b' is at index " << t_idx << std::endl;
            return false;
          }
          std::cerr << "at index " << t_idx << " there is a "
                    << typeid(std::tuple_element_t<t_idx, my_tuple>).name()
                    << std::endl;
          return true;
        },
        _my_tuple);

    return true;
  }
};

struct tuple_002 {
  static std::string desc() {
    return "Traverses a tuple through all its types until type 'b' is found";
  }

  bool operator()(const program::options &) {

    constexpr auto _f = [&]<typename t_tuple, size_t t_idx>() {
      if (std::is_same_v<std::tuple_element_t<t_idx, t_tuple>, b>) {
        return false;
      }
      return true;
    };

    constexpr auto _idx(traits::tuple_type_traverse<my_tuple>(_f));

    std::cerr << "at index " << _idx << " there is a '"
              << typeid(std::tuple_element_t<_idx, my_tuple>).name() << '\''
              << std::endl;

    return true;
  }
};

struct tuple_003 {
  static std::string desc() { return "Output operator with tuple object"; }

  bool operator()(const program::options &) {
    my_tuple _my_tuple{a(), 'c', b(), 3.14};
    std::cerr << _my_tuple << std::endl;
    return true;
  }
};

struct tuple_004 {
  static std::string desc() { return "Output operator with 'make_tuple'"; }

  bool operator()(const program::options &) {
    std::cerr << std::make_tuple(a(), 'c', b(), 3.14) << std::endl;
    return true;
  }
};

struct tuple_005 {
  static std::string desc() {
    return "Output operator with 'make_tuple' with a tuple as one of the "
           "elements";
  }

  bool operator()(const program::options &) {
    std::cerr << std::make_tuple(a(), 'c', b(),
                                 std::make_tuple("hi!!", -320, 70257), 3.14)
              << std::endl;
    return true;
  }
};

struct tuple_006 {
  static std::string desc() {
    return "Prints all the types in a tuple which is the concatenation of two "
           "tuples";
  }

  bool operator()(const program::options &) {
    auto _visitor([&]<traits::tuple_like t_tuple, size_t t_idx>() {
      std::cerr << typeid(std::tuple_element_t<t_idx, t_tuple>).name() << ' ';
      return true;
    });

    traits::tuple_type_traverse<tuple_3>(_visitor);

    return true;
  }

private:
  using tuple_1 = std::tuple<char, float>;
  using tuple_2 = std::tuple<int, double>;
  using tuple_3 = decltype(std::tuple_cat(tuple_1(), tuple_2()));
};

struct pair_000 {
  static std::string desc() { return "'pair' output"; }

  bool operator()(const program::options &) {
    std::cerr << std::make_pair(2.12, 'c') << std::endl;
    return true;
  }
};

struct tuple_pair_000 {
  static std::string desc() { return "Mixes 'pair' and 'tuple' output"; }

  bool operator()(const program::options &) {
    using namespace std;

    std::cerr << make_tuple(
                     2.12,

                     make_pair("hi", 3.14),

                     'c',

                     make_pair(
                         -45,

                         make_tuple("inside", -0.21,

                                    make_tuple('W', a(), b(),
                                               make_pair("inner pair", 291)))))
              << std::endl;
    // std::cerr << make_tuple(make_pair('a', 1), 3.14, -21);
    // std::cerr << make_pair(make_tuple('a', 1, 3.14), -21);

    return true;
  }
};

#endif
