
#include <string>

#include <tenacitas/number/cpt/id.h>
#include <tenacitas/number/typ/id.h>
#include <tenacitas/program/alg/options.h>
#include <tenacitas/test/alg/tester.h>

using namespace tenacitas::lib;

struct test001 {
  static std::string desc() { return "id _i {4}, which does not compile"; }

  bool operator()(const program::alg::options &) {

    // THIS FAILS TO COMPILE BECAUSE '4' IS AUTOMATICALLY DEDUCED TO 'int',
    // WHICH DOES NOT SATISFY 'id'

    //    concepts::cpt::id auto _i{4};
    return true;
  }
};

struct test002 {
  static std::string desc() { return "id _i {-4}, which does not compile"; }

  bool operator()(const program::alg::options &) {

    // THIS FAILS TO COMPILE BECAUSE '-4' IS AUTOMATICALLY DEDUCED TO 'int',
    // WHICH DOES NOT SATISFY 'id'

    //    concepts::cpt::id auto _i{-4};

    return true;
  }
};

struct test003 {
  static std::string desc() {
    return "id _i {std::numeric_limits<size_t>::max()}";
  }

  bool operator()(const program::alg::options &) {
    number::cpt::id auto _i{std::numeric_limits<size_t>::max()};

    return (_i == std::numeric_limits<size_t>::max());
  }
};

struct test004 {
  static std::string desc() { return "id _i {uint16_t{4}}"; }

  bool operator()(const program::alg::options &) {
    number::cpt::id auto _i{uint16_t{4}};

    return (_i == 4);
  }
};

int main(int argc, char **argv) {
  using namespace tenacitas;
  lib::test::alg::tester _tester(argc, argv);

  run_test(_tester, test003);
  run_test(_tester, test004);
}
