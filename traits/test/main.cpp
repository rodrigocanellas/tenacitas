/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <iostream>

#include <tenacitas.lib/tester/test.h>

using namespace tenacitas::lib;

int main(int argc, char **argv) {

  try {
    generic::test _test(argc, argv);
  } catch (std::exception &_ex) {
    std::cout << "EXCEPTION: '" << _ex.what() << "'" << std::endl;
  }
}
