/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Caellas - rodrigo.caellas at gmail.com

#ifndef TENACITAS_LIB_TRAITS_TEST_TUPLE_CONTAINS_TUPLE_TEST_H
#define TENACITAS_LIB_TRAITS_TEST_TUPLE_CONTAINS_TUPLE_TEST_H

#include <tuple>

#include <tenacitas.lib/program/options.h>
#include <tenacitas.lib/traits/tuple_contains_tuple.h>
#include <tenacitas.lib/traits/tuple_like.h>

using namespace tenacitas::lib;

template <traits::tuple_like t_container, traits::tuple_like t_contained>
requires traits::tuple_contains_tuple<t_container, t_contained>
void func() {}

namespace tenacitas::lib::traits::test {

struct tuple_contains_tuple_000 {
  static std::string desc() {
    return "Checks if 'std::tuple<char>' is contained in 'std::tuple<int, "
           "char, float>'";
  }

  bool operator()(const program::options &) {
    using tuple_1 = std::tuple<int, char, float>;
    using tuple_2 = std::tuple<char>;

    // this should compile
    func<tuple_1, tuple_2>();

    return true;
  }
};

struct tuple_contains_tuple_001 {
  static std::string desc() {
    return "Assures that to check if 'std::tuple<char>' is contained in "
           "'std::tuple<int, char, float>' does not compile because of the "
           "size of the tuples";
  }

  bool operator()(const program::options &) {
    // using tuple_1 = std::tuple<int, char, float>;
    // using tuple_2 = std::tuple<char>;

    // this should not compile because
    /*
tuple_contains_tuple_test.h:49:5: No matching function for call to 'func'
tuple_contains_tuple_test.h:20:6: candidate template ignored: constraints not
satisfied [with t_container = tuple_2, t_contained = tuple_1]
tuple_contains_tuple_test.h:19:10: because
'tuple::tuple_contains_tuple<std::tuple<char>, std::tuple<int, char, float> >'
evaluated to false tuple_contains_tuple.h:28:8: because
'std::tuple_size_v<std::tuple<char> > > std::tuple_size_v<std::tuple<int, char,
float> >' (1 > 3) evaluated to false
*/
    //    func<tuple_2, tuple_1>();

    return true;
  }
};

struct tuple_contains_tuple_002 {
  static std::string desc() {
    return "Assures that to check if 'double' is contained in "
           "'std::tuple<int, char, float>' does not compile";
  }

  bool operator()(const program::options &) {
    // using tuple_1 = std::tuple<int, char, float>;
    // using tuple_2 = std::tuple<double>;

    // this should compile because
    /*
tuple_contains_tuple_test.h:80:5: No matching function for call to 'func'
tuple_contains_tuple_test.h:20:6: candidate template ignored: constraints
not satisfied [with t_container = tuple_1, t_contained = tuple_2]
tuple_contains_tuple_test.h:19:10: because
'tuple::tuple_contains_tuple<std::tuple<int, char, float>,
std::tuple<double> >' evaluated to false tuple_contains_tuple.h:33:11:
because '[]<std::size_t ...t_idx>(std::index_sequence<t_idx...>) { return
((is_type_in_tuple<std::tuple<int, char, float>, std::tuple_element_t<t_idx,
tuple<double> > >()) && ...);
}(std::make_index_sequence<std::tuple_size_v<tuple<double> > >())' evaluated
to false
 */
    // func<tuple_1, tuple_2>();

    return true;
  }
};

struct tuple_contains_tuple_003 {
  static std::string desc() {
    return "Checks if 'std::tuple<int>' is contained in 'std::tuple<int, "
           "char, float>'";
  }

  bool operator()(const program::options &) {
    using tuple_1 = std::tuple<int, char, float>;
    using tuple_2 = std::tuple<int>;

    // this should compile
    func<tuple_1, tuple_2>();

    return true;
  }
};

struct tuple_contains_tuple_004 {
  static std::string desc() {
    return "Checks if 'std::tuple<float>' is contained in 'std::tuple<int, "
           "char, float>'";
  }

  bool operator()(const program::options &) {
    using tuple_1 = std::tuple<int, char, float>;
    using tuple_2 = std::tuple<float>;

    // this should compile
    func<tuple_1, tuple_2>();

    return true;
  }
};

struct tuple_contains_tuple_005 {
  static std::string desc() {
    return "Checks if 'std::tuple<int, char, float>' is contained in "
           "'std::tuple<int, "
           "char, float>'";
  }

  bool operator()(const program::options &) {
    using tuple_1 = std::tuple<int, char, float>;
    using tuple_2 = std::tuple<int, char, float>;

    // this should compile
    func<tuple_1, tuple_2>();

    return true;
  }
};

struct tuple_contains_tuple_006 {
  static std::string desc() {
    return "Checks if 'std::tuple<float, int, char>' is contained in "
           "'std::tuple<int, "
           "char, float>'";
  }

  bool operator()(const program::options &) {
    using tuple_1 = std::tuple<int, char, float>;
    using tuple_2 = std::tuple<float, int, char>;

    // this should compile
    func<tuple_1, tuple_2>();

    return true;
  }
};

struct tuple_contains_tuple_007 {
  static std::string desc() {
    return "Checks if 'std::tuple<int, char>' is contained in "
           "'std::tuple<int, "
           "char, float>'";
  }

  bool operator()(const program::options &) {
    using tuple_1 = std::tuple<int, char, float>;
    using tuple_2 = std::tuple<int, char>;

    // this should compile
    func<tuple_1, tuple_2>();

    return true;
  }
};

} // namespace tenacitas::lib::traits::test

#endif
