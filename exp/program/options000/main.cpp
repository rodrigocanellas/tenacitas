/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <tenacitas/program/alg/options.h>

using namespace std;

int main(int argc, char **argv) {
  using namespace tenacitas::lib::program;

  try {
    alg::options _pgm_options;

    _pgm_options.parse(argc, (char **)argv);

    std::cerr << "options: " << _pgm_options << std::endl;

  } catch (std::exception &_ex) {
    std::cerr << "ERROR! '" << _ex.what() << "'" << std::endl;
  }
  return 0;
}
