/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

/// \example program_options_001/main.cpp

#include <iostream>

#include <tnct/lib/alg/program_options.h>

using namespace std;

int main() {
  try {

    using namespace std;

    const char *argv[] = {"pgm-name", "--set_1",    "{",
                          "v0",       "v1",         "}",
                          "--bool_1", "--single_1", "single_value_1"};
    const int argc = 9;

    tla::program_options _pgm_options;

    _pgm_options.parse(argc, (char **)argv, {"bool_1"});

    cerr << "options: " << _pgm_options << endl;

    optional<bool> _bool = _pgm_options.get_bool_param("bool_1");
    if (_bool) {
      cerr << "bool param = " << _bool.value() << endl;
    } else {
      cerr << "ERROR! no value for paramenter 'bool_1' was found" << endl;
    }

    optional<tla::program_options::value> _single =
        _pgm_options.get_single_param("single_1");
    if (_single) {
      cerr << "single param = " << _single.value() << endl;
    } else {
      cerr << "ERROR! no value for paramenter 'single_1' was found" << endl;
    }

    optional<list<tla::program_options::value>> _set =
        _pgm_options.get_set_param("set_1");
    if (_set) {
      if (_set.value().size() != 2) {
        cerr << "ERROR! number of values in set parameter should be 2, but it "
                "is "
             << _set.value().size() << endl;
      } else {
        cerr << "set param 'set_1' = ";
        for (tla::program_options::value &_value : _set.value()) {
          cerr << _value << " ";
        }
        cerr << endl;
      }
    } else {
      cerr << "ERROR! no set of values found for parameter 'set_1'" << endl;
    }
  } catch (std::exception &_ex) {
    cerr << "ERROR! '" << _ex.what() << "'" << endl;
  }

  return 0;
}
