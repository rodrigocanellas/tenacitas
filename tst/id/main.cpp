
#include <string>

#include <tenacitas.h>

using namespace tenacitas::lib;

struct test001 {
  static std::string desc() { return "id _i {4}, which does not compile"; }

  bool operator()(const tncta::program_options &) {

    // THIS FAILS TO COMPILE BECAUSE '4' IS AUTOMATICALLY DEDUCED TO 'int',
    // WHICH DOES NOT SATISFY 'id'

    //    tnctc::id auto _i{4};
    return true;
  }
};

struct test002 {
  static std::string desc() { return "id _i {-4}, which does not compile"; }

  bool operator()(const tncta::program_options &) {

    // THIS FAILS TO COMPILE BECAUSE '-4' IS AUTOMATICALLY DEDUCED TO 'int',
    // WHICH DOES NOT SATISFY 'id'

    // tnctc::id auto _i{-4};

    return true;
  }
};

struct test003 {
  static std::string desc() {
    return "id _i {std::numeric_limits<size_t>::max()}";
  }

  bool operator()(const tncta::program_options &) {
    tnctc::id auto _i{std::numeric_limits<size_t>::max()};

    return (_i == std::numeric_limits<size_t>::max());
  }
};

struct test004 {
  static std::string desc() { return "id _i {uint16_t{4}}"; }

  bool operator()(const tncta::program_options &) {
    tnctc::id auto _i{uint16_t{4}};

    return (_i == 4);
  }
};

int main(int argc, char **argv) {
  using namespace tenacitas;
  tncta::tester _tester(argc, argv);

  run_test(_tester, test003);
  run_test(_tester, test004);
}
