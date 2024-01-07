/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

/// \example program_options_000/main.cpp

#include <iostream>

#include <tenacitas.lib/src/alg/program_options.h>

using namespace std;

int main(int argc, char **argv) {

  try {
    tla::program_options _pgm_options;

    _pgm_options.parse(argc, (char **)argv);

    std::cerr << "options: " << _pgm_options << std::endl;

  } catch (std::exception &_ex) {
    std::cerr << "ERROR! '" << _ex.what() << "'" << std::endl;
  }
  return 0;
}
