
/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <concepts>
#include <iostream>

#include <tenacitas.lib/async/cpt/event.h>
#include <tenacitas.lib/log/alg/logger.h>
#include <tenacitas.lib/program/alg/options.h>
#include <tenacitas.lib/test/alg/tester.h>

using namespace tenacitas::lib;
using namespace std::chrono_literals;

struct a {
  int i{-3};
  friend std::ostream &operator<<(std::ostream &p_out, const a &p_a) {
    p_out << p_a.i;
    return p_out;
  }
};

struct b {};

struct c {
  int i{-3};
  friend std::ostream &operator<<(std::ostream &p_out, const c &p_c) {
    p_out << p_c.i;
    return p_out;
  }
};

struct d {
  int i{-3};
  friend std::ostream &operator<<(std::ostream &p_out, const d &p_d) {
    p_out << p_d.i;
    return p_out;
  }
};

struct test002 {
  static std::string desc() {
    return "causes a compiler error due to unsatisfied 'printable' concept";
  }

  bool operator()(const program::alg::options &) {

    // UNCOMENT THE LINES BELOW TO SEE A COMPILER ERROR BECAUSE 'b' does not
    // implement <<
    //    b _b;
    return true;
  }
};

int main(int argc, char **argv) {

  log::alg::set_debug_level();
  test::alg::tester _tester(argc, argv);

  run_test(_tester, test002);
}
