#ifndef TENACITAS_TESTER_TEST_H
#define TENACITAS_TESTER_TEST_H

#include <functional>
#include <iostream>
#include <sstream>
#include <string>

#include <program/options.h>

namespace tenacitas {
namespace tester {

///
/// \brief The test struct executes tests implemented in classes
///
/// \code
/// #include <cstdint>
/// #include <iostream>
/// #include <string>
///
/// #include <tester/test.h>
///
/// using namespace tenacitas;
///
/// struct test_ok {
///   bool operator()() { return true; }
/// };
///
/// struct test_fail {
///   bool operator()() { return true; }
/// };
///
/// struct test_error {
///   bool operator()() {
///     throw std::runtime_error("test function raised an exception");
///   }
/// };
///
/// int main(int argc, char **argv) {
///   try {
///     tester::test _test(argc, argv);
///
///     _test(test_ok(), "test_ok", "runs a correct test");
///     _test(test_fail(), "test_fail", "runs a fail test");
///     _test(test_error(), "test_error", "runs a error test");
///
///   } catch (std::exception &_ex) {
///     std::cout << "EXCEPTION: '" << _ex.what() << "'" << std::endl;
///   }
/// }
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
  /// \param p_function test function to be executed
  /// \param p_name name of the test
  /// \param p_desc description of the test
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

  template <typename t_test_class> void run() {
    using namespace std;

    if (!m_options_parsed) {

      m_options_parsed = true;

      m_pgm_name = m_argv[0];

      try {

        program::options _options;

        _options.parse(m_argc, m_argv);
        if (_options["exec"]) {
          m_execute_tests = true;
        }
        if (_options["desc"]) {
          m_print_desc = true;
        }
      } catch (std::exception &_ex) {
        cout << "EXCEPTION '" << _ex.what() << "'" << endl;
        return;
      }
    }

    if (m_print_desc) {
      cout << t_test_class::name() << ": " << t_test_class::desc() << "\n"
           << endl;
      return;
    }

    if (m_execute_tests) {
      bool result = false;
      try {
        cerr << "\n############ -> " << t_test_class::name() << endl;
        result = t_test_class()();
        cout << (result ? "SUCCESS" : "FAIL") << " for " << t_test_class::name()
             << endl;
      } catch (exception &_ex) {
        cout << "ERROR for " << t_test_class::name() << " '" << _ex.what()
             << "'" << endl;
      }
      cerr << "############ <- " << t_test_class::name() << endl;
      return;
    }

    print_mini_howto();
  }

private:
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

  bool m_options_parsed = {false};
};

} // namespace tester
} // namespace tenacitas

#endif // TEST_H
