
#include <string>

#include <tnct/lib/alg/log.h>
#include <tnct/lib/alg/program_options.h>
#include <tnct/lib/alg/tester.h>
#include <tnct/lib/cpt/id.h>
#include <tnct/lib/dat/id.h>

using namespace tnct::lib;

struct test001 {
  static std::string desc() { return "id _i {4}, which does not compile"; }

  bool operator()(const tla::program_options &) {

    // THIS FAILS TO COMPILE BECAUSE '4' IS AUTOMATICALLY DEDUCED TO 'int',
    // WHICH DOES NOT SATISFY 'id'

    //    tnctl::id auto _i{4};
    return true;
  }
};

struct test002 {
  static std::string desc() { return "id _i {-4}, which does not compile"; }

  bool operator()(const tla::program_options &) {

    // THIS FAILS TO COMPILE BECAUSE '-4' IS AUTOMATICALLY DEDUCED TO 'int',
    // WHICH DOES NOT SATISFY 'id'

    // tlc::id auto _i{-4};

    return true;
  }
};

struct test003 {
  static std::string desc() {
    return "id _i {std::numeric_limits<size_t>::max()}";
  }

  bool operator()(const tla::program_options &) {
    tlc::id auto _i{std::numeric_limits<size_t>::max()};

    return (_i == std::numeric_limits<size_t>::max());
  }
};

struct test004 {
  static std::string desc() { return "id _i {uint16_t{4}}"; }

  bool operator()(const tla::program_options &) {
    tlc::id auto _i{uint16_t{4}};

    return (_i == 4);
  }
};

int main(int argc, char **argv) {

  tla::tester _tester(argc, argv);

  run_test(_tester, test003);
  run_test(_tester, test004);
}
