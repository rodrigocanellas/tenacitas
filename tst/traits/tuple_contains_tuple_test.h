/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Caellas - rodrigo.caellas at gmail.com

#ifndef TENACITAS_TST_TRAITS_TUPLE_CONTAINS_TUPLE_H
#define TENACITAS_TST_TRAITS_TUPLE_CONTAINS_TUPLE_H

#include <tuple>

#include "tenacitas/src/program/options.h"
#include "tenacitas/src/traits/tuple/contains_tuple.h"
#include "tenacitas/src/traits/tuple/like.h"

using namespace tenacitas;

template <src::traits::tuple::like t_container,
          src::traits::tuple::like t_contained>
// requires src::traits::tuple::contains_tuple<t_container, t_contained>
void func() {}

namespace tenacitas::tst::traits {

struct tuple_contains_tuple_000 {
  static std::string desc() {
    return "Checks if 'std::tuple<char>' is contained in 'std::tuple<int, "
           "char, float>'";
  }

  bool operator()(const src::program::options &) {
    using tuple_1 = std::tuple<int, char, float>;
    using tuple_2 = std::tuple<char>;

    static_assert(
        src::traits::tuple::contains_tuple_helper<tuple_1, tuple_2>());

    // this should compile
    func<tuple_1, tuple_2>();

    return true;
  }
};

struct tuple_contains_tuple_001 {
  static std::string desc() {
    return "Assures that to check if 'std::tuple<int, char, float>' is "
           "contained in 'std::tuple<char>' does not the first tuple is longer "
           " than the first ";
  }

  bool operator()(const src::program::options &) {
    // using tuple_1 = std::tuple<int, char, float>;
    // using tuple_2 = std::tuple<char>;

    // this should not compile because
    /*
tuple_contains_tuple_test.h:49:5: No matching function for call to 'func'
tuple_contains_tuple_test.h:20:6: candidate template ignored: constraints not
satisfied [with t_container = tuple_2, t_contained = tuple_1]
tuple_contains_tuple_test.h:19:10: because
'tuple::contains_tuple<std::tuple<char>, std::tuple<int, char, float> >'
evaluated to false contains_tuple.h:28:8: because
'std::tuple_size_v<std::tuple<char> > > std::tuple_size_v<std::tuple<int,
char, float> >' (1 > 3) evaluated to false
*/
    // func<tuple_2, tuple_1>();

    return true;
  }
};

struct tuple_contains_tuple_002 {
  static std::string desc() {
    return "Assures that to check if 'double' is contained in "
           "'std::tuple<int, char, float>' does not compile";
  }

  bool operator()(const src::program::options &) {
    // using tuple_1 = std::tuple<int, char, float>;
    // using tuple_2 = std::tuple<double>;

    // this should not compile because
    /*
tuple_contains_tuple_test.h:80:5: No matching function for call to 'func'
tuple_contains_tuple_test.h:20:6: candidate template ignored: constraints
not satisfied [with t_container = tuple_1, t_contained = tuple_2]
tuple_contains_tuple_test.h:19:10: because
'tuple::contains_tuple<std::tuple<int, char, float>,
std::tuple<double> >' evaluated to false contains_tuple.h:33:11:
because '[]<std::size_t ...t_idx>(std::index_sequence<t_idx...>) { return
((contains_type<std::tuple<int, char, float>, std::tuple_element_t<t_idx,
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

  bool operator()(const src::program::options &) {
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

  bool operator()(const src::program::options &) {
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

  bool operator()(const src::program::options &) {
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

  bool operator()(const src::program::options &) {
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

  bool operator()(const src::program::options &) {
    using tuple_1 = std::tuple<int, char, float>;
    using tuple_2 = std::tuple<int, char>;

    // this should compile
    func<tuple_1, tuple_2>();

    return true;
  }
};

struct tuple_contains_tuple_008 {
  static std::string desc() {
    return "Checks if 'std::tuple<int, char>' is contained in "
           "'std::tuple<int, float>'";
  }

  bool operator()(const src::program::options &) {
    // using tuple_1 = std::tuple<int, float>;
    // using tuple_2 = std::tuple<int, char>;

    // this should not compile
    /*
tuple_contains_tuple_test.h:197:5: No matching function for call to 'func'
tuple_contains_tuple_test.h:19:6: candidate template ignored: constraints not
satisfied [with t_container = tuple_1, t_contained = tuple_2]
tuple_contains_tuple_test.h:18:10: because
'src::traits::contains_tuple<std::tuple<int, float>, std::tuple<int, char> >'
evaluated to false contains_tuple.h:32:9: because '[]<std::size_t
...t_idx>(std::index_sequence<t_idx...>) { return
((contains_type<std::tuple<int, float>, std::tuple_element_t<t_idx,
tuple<int, char> > >) && ...);
}(std::make_index_sequence<std::tuple_size_v<tuple<int, char> > >())'
evaluated to false
     */
    // func<tuple_1, tuple_2>();

    return true;
  }
};

} // namespace tenacitas::tst::traits

#endif
