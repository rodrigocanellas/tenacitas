#ifndef TENACITAS_TESTER_TEST_H
#define TENACITAS_TESTER_TEST_H

#include <iostream>
#include <sstream>
#include <string>

namespace tenacitas {
namespace tester {

//
/// \brief The test struct executes tests implemented in classes
//
struct test {

  //
  // \tparam must implement:
  // \code
  // static std::string name()
  // static std::string desc()
  // bool operator()()
  // \endcode
  //
  template <typename t_class> static void run(int argc, char **argv) {
    bool _execute = false;
    std::string _name(t_class::name());

    if ((argc) == 1) {
      std::cout << "Syntax: \n"
                << argv[0] << " exec  --> executes the test\n"
                << argv[0] << " desc  --> displays a description of the test\n";
    } else if ((argc) == 2) {
      std::string _param((argv)[1]);
      if (_param == "exec") {
        _execute = true;
      } else if (_param == "desc") {
        std::cout << _name << ": " << t_class::desc() << std::endl;
      }
    } else {
      std::cout << argc << " is an invalid number of parameters" << std::endl;
    }
    if (_execute) {
      t_class _obj;
      bool result = false;
      try {
        std::cout << "############ " << __LINE__ << " - " << _name << std::endl;
        result = _obj();
      } catch (std::exception &_ex) {
        std::cout << "ERROR " << _name << ": '" << _ex.what() << "'"
                  << std::endl;
        result = false;
      }
      std::clog << _name << (result ? " SUCCESS" : " FAIL") << std::endl;
      std::cout << _name << " - " << __LINE__ << " ############" << std::endl;
    }
  }
};

} // namespace tester
} // namespace tenacitas

#endif // TEST_H
