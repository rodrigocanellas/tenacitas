#ifndef TENACITAS_TESTER_TEST_H
#define TENACITAS_TESTER_TEST_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>

#include <program/options.h>

/// \brief Runs a test
///
/// \param tester is an instance of tenacitas::tester::test defined below
///
/// \param test is the name of a class that implements
///
/// \code
/// bool operator()()
///
/// static std::string desc()
/// \endcode
///
#define run_test(tester, test) tester.run<test>(#test)

namespace tenacitas {
namespace tester {

///
/// \brief The test struct executes tests implemented in classes
///
/// \code
/// #include <iostream>
/// #include <string>
///
/// #include <tester/test.h>
///
/// using namespace tenacitas;
///
/// struct test_ok {
///   bool operator()() { return true; }
///
///   static std::string desc() { return "an ok test"; }
/// };
///
/// struct test_fail {
///   bool operator()() { return true; }
///   static std::string desc() { return "a fail test"; }
/// };
///
/// struct test_error {
///  bool operator()() {
///    try {
///      throw std::runtime_error("test function raised an exception");
///      return false;
///    } catch (const std::exception &_ex) {
///      std::cerr << "'test_error' raised '" << _ex.what() << "'" << std::endl;
///      return true;
///    }
///  }
///  static std::string desc() { return "an eror test"; }
///};
///
/// int main(int argc, char **argv) {
///  try {
///    tester::test _tester(argc, argv);
///
///    run_test(_tester, test_ok);
///    run_test(_tester, test_fail);
///    run_test(_tester, test_error);
///
///  } catch (std::exception &_ex) {
///    std::cout << "EXCEPTION: '" << _ex.what() << "'" << std::endl;
///  }
///}
///
/// \endcode
///
struct test {

  ///
  /// \brief test constructor
  ///
  /// \param argc number of strings in \p argv
  /// \param argv parameters passed to the program
  ///
  /// \details
  /// If '--desc' is passed, \p operator() will print a description of the
  /// tests.
  /// If '--exec' is passed, \p operator() will execute the tests
  /// If '--exec { <test-name-1> <test-name-2> ... }' is passed, \p operator()
  /// will execute the tests will execute tests  defined between '{' and '}'
  ///
  /// \details the user prints should use \p cerr
  ///
  /// \throw std::runtime_error
  test(int argc, char **argv);

  test() = delete;
  test(const test &) = delete;
  test(test &&) = delete;

  test &operator=(const test &) = delete;
  test &operator=(test &&) = delete;

  ///
  /// \brief run executes the test
  ///
  /// \tparam t_test_class must implement:
  /// \code
  /// bool operator()()
  ///
  /// static std::string desc()
  /// \endcode
  ///
  /// \details You can use the macro 'run_test' defined above, instead of
  /// calling this method
  ///
  /// \details user should use <tt>std::cerr</tt> to print specific test
  /// messages
  ///
  /// \details if the test passes, the message "SUCCESS for <name>" will be
  /// printed; if the test does not pass, the message "FAIL for <name>" will be
  /// printed; if an error occurr while executing the test , the messae "ERROR
  /// for <name> <desc>" will be printed; if an exception occurrs, the message
  /// 'EXCEPTION <description>' will be displayed
  ///
  template <typename t_test_class> void run(const std::string p_test_name) {
    using namespace std;

    if (m_print_desc) {
      cout << p_test_name << ": " << t_test_class::desc() << "\n" << endl;
      return;
    }

    if (m_execute_tests) {
      if (!m_tests_to_exec.empty()) {
        if ((std::find(m_tests_to_exec.begin(), m_tests_to_exec.end(),
                       p_test_name)) != m_tests_to_exec.end()) {
          exec<t_test_class>(p_test_name);
        }
      } else {
        exec<t_test_class>(p_test_name);
      }
    }
  }

private:
  template <typename t_test_class> void exec(const std::string p_test_name) {
    using namespace std;
    bool result = false;
    try {
      cerr << "\n############ -> " << p_test_name << " - "
           << t_test_class::desc() << endl;
      result = t_test_class()();
      cout << (result ? "SUCCESS" : "FAIL") << " for " << p_test_name << endl;
    } catch (exception &_ex) {
      cout << "ERROR for " << p_test_name << " '" << _ex.what() << "'" << endl;
    }
    cerr << "############ <- " << p_test_name << endl;
  }

  ///
  /// \brief print_mini_howto prints a mini how-to for using the \p test class
  ///
  void print_mini_howto();

private:
  /// \brief name of the test program
  std::string m_pgm_name;

  /// \brief indicates if the tests should actually be executed, or if theirs
  /// description should be printed
  bool m_execute_tests = {false};

  /// prints test decription to \p cout
  bool m_print_desc = {false};

  /// \brief number of parameters passed to the \p test object
  int m_argc = {-1};

  /// \brief parameters passed to the \p test object
  char **m_argv = {nullptr};

  std::set<std::string> m_tests_to_exec;
};

} // namespace tester
} // namespace tenacitas

#endif // TEST_H
