
#include <string>

#include <tenacitas.lib/src/number/cpt/id.h>
#include <tenacitas.lib/src/number/typ/id.h>
#include <tenacitas.lib/src/program/alg/options.h>
#include <tenacitas.lib/src/test/alg/tester.h>

namespace tenacitas::lib::tst::number::alg {
struct test000 {
  static std::string desc() { return "'id' from a number"; }

  bool operator()(const src::program::alg::options &) {
    src::number::typ::id _id(54321);

    return _id.str() == "4321";
  }
};

struct test001 {
  static std::string desc() { return "id _i {4}, which does not compile"; }

  bool operator()(const src::program::alg::options &) {

    // THIS FAILS TO COMPILE BECAUSE '4' IS AUTOMATICALLY DEDUCED TO 'int',
    // WHICH DOES NOT SATISFY 'id'

    //    concepts::cpt::id auto _i{4};
    return true;
  }
};

struct test002 {
  static std::string desc() { return "id _i {-4}, which does not compile"; }

  bool operator()(const src::program::alg::options &) {

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

  bool operator()(const src::program::alg::options &) {
    src::number::cpt::id auto _i{std::numeric_limits<size_t>::max()};

    return (_i == std::numeric_limits<size_t>::max());
  }
};

struct test004 {
  static std::string desc() { return "id _i {uint16_t{4}}"; }

  bool operator()(const src::program::alg::options &) {
    src::number::cpt::id auto _i{uint16_t{4}};

    return (_i == 4);
  }
};
} // namespace tenacitas::lib::tst::number::alg
int main(int argc, char **argv) {
  using namespace tenacitas;
  lib::src::test::alg::tester _tester(argc, argv);

  run_test(_tester, lib::tst::number::alg::test000);
}
