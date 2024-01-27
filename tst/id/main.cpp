/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <string>

#include <tenacitas.lib/sof/id.h>
#include <tenacitas.lib/log/log.h>
#include <tenacitas.lib/sof/program_options.h>
#include <tenacitas.lib/sof/tester.h>
#include <tenacitas.lib/sta/id.h>

using namespace tenacitas::lib;

struct test001 {
  static std::string desc() { return "id _i {4}, which does not compile"; }

  bool operator()(const sof::program_options &) {

    // THIS FAILS TO COMPILE BECAUSE '4' IS AUTOMATICALLY DEDUCED TO 'int',
    // WHICH DOES NOT SATISFY 'id'

    //    tnctl::id auto _i{4};
    return true;
  }
};

struct test002 {
  static std::string desc() { return "id _i {-4}, which does not compile"; }

  bool operator()(const sof::program_options &) {

    // THIS FAILS TO COMPILE BECAUSE '-4' IS AUTOMATICALLY DEDUCED TO 'int',
    // WHICH DOES NOT SATISFY 'id'

    // id auto _i{-4};

    return true;
  }
};

struct test003 {
  static std::string desc() {
    return "id _i {std::numeric_limits<size_t>::max()}";
  }

  bool operator()(const sof::program_options &) {
    sof::id _i{std::numeric_limits<size_t>::max()};

    return (_i == sof::id(std::numeric_limits<size_t>::max()));
  }
};

struct test004 {
  static std::string desc() { return "id _i {uint16_t{4}}"; }

  bool operator()(const sof::program_options &) {
    sof::id _i{uint16_t{4}};

    return (_i == sof::id(4));
  }
};

int main(int argc, char **argv) {

  sof::tester _tester(argc, argv);

  run_test(_tester, test003);
  run_test(_tester, test004);
}
