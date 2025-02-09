/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TNCT_TESTER_TEST_H
#define TNCT_TESTER_TEST_H

#include <algorithm>
#include <iostream>
#include <set>
#include <string>

#include "tnct/program/options.h"

/// \brief Runs a test
///
/// \param test is an instance of tnct::test defined
/// below
///
/// \param test is the name of a class that implements
///
/// \code
/// bool operator()(const tnct::program::options &)
///
/// static std::string desc()
/// \endcode
///
#define run_test(test, test_name) test.run<test_name>(#test_name)

namespace tnct::tester {
/// \brief The test struct executes tests implemented in classes
///
/// \tparam use makes tnct::tester to be compiled only if
/// actually used
///
/// \code
/// #include <iostream>
/// #include <string>
///
/// #include <tester/test.h>
///
/// using namespace tnct::src;
///
/// struct test_ok {
///   bool operator()(const program::options &) { return true; }
///
///   static std::string desc() { return "an ok test"; }
/// };
///
/// struct test_fail {
///   bool operator()(const program::options &) { return true; }
///   static std::string desc() { return "a fail test"; }
/// };
///
/// struct test_error {
///  bool operator()(const program::options &) {
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
///  int main(int argc, char **argv) {
///  try {
///    test::tester _tester(argc, argv);
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
template <bool use = true> struct test {
  /// \brief Constructor
  /// If '--desc' is passed, \p operator() will print a description of the
  /// tests.
  /// If '--exec' is passed, \p operator() will execute the tests
  /// If '--exec { <test-name-1> <test-name-2> ... }' is passed, \p operator()
  /// will execute the tests between '{' and '}'
  ///
  /// \param argc number of strings in \p argv
  ///
  /// \param argv parameters passed to the program
  test(
      int argc, char **argv,
      std::initializer_list<program::options::name> &&p_mandatory = {}) noexcept
      : m_argc(argc), m_argv(argv) {
    m_pgm_name = m_argv[0];

    try {
      m_options.parse(m_argc, m_argv, std::move(p_mandatory));

      if (m_options.get_bool_param("exec")) {
        m_execute_tests = true;
      } else if (m_options.get_bool_param("desc")) {
        m_print_desc = true;
      } else {
        std::optional<std::list<program::options::value>> _maybe =
            m_options.get_set_param("exec");
        if (_maybe) {
          m_execute_tests = true;
          std::list<program::options::value> _tests_to_exec =
              std::move(*_maybe);
          m_tests_to_exec.insert(_tests_to_exec.begin(), _tests_to_exec.end());
        }
      }

      if ((!m_execute_tests) && (!m_print_desc)) {
        print_mini_howto();
      }
    } catch (std::exception &_ex) {
      std::cout << "EXCEPTION '" << _ex.what() << "'" << std::endl;
      return;
    }
  }

  /// \brief Default constructor not allowed
  test() = delete;

  /// \brief Copy constructor not allowed
  test(const test &) = delete;

  /// \brief Copy constructor not allowed
  test(test &&) = delete;

  /// \brief Copy assignment not allowed
  test &operator=(const test &) = delete;

  /// \brief Move assignment not allowed
  test &operator=(test &&) = delete;

  /// \brief Executes the test
  ///  If the test passes, the message "SUCCESS for <name>" will be
  /// printed; if the test does not pass, the message "FAIL for <name>" will
  /// be printed; if an error occurr while executing the test , the messae
  /// "ERROR for <name> <desc>" will be printed
  ///
  /// \tparam t_test_class must implement:
  /// \code
  /// bool operator()(const program::options &)
  ///
  /// static std::string desc()
  /// \endcode
  ///
  /// \details You can use the macro 'run_test' defined above, instead of
  /// calling this method
  template <typename t_test_class>
  void run(const std::string &p_test_name) noexcept {
    using namespace std;
    try {
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
    } catch (std::exception &_ex) {
      std::cout << "EXCEPTION '" << _ex.what() << "'" << std::endl;
      return;
    }
  }

private:
  /// \brief Executes the test
  /// \tparam t_test_class must implement:
  /// \code
  /// bool operator()(const program::options &)
  ///
  /// static std::string desc()
  /// \endcode
  template <typename t_test_class> void exec(const std::string p_test_name) {
    using namespace std;
    bool result = false;
    try {
      t_test_class _test_obj;
      cerr << "\n############ -> " << p_test_name << " - "
           << t_test_class::desc() << endl;
      result = _test_obj(m_options);
      //            cout << (result ? "SUCCESS" : "FAIL") << " for " <<
      //            p_test_name
      //                 << endl;
      cout << p_test_name << (result ? " SUCCESS" : " FAIL") << endl;
    } catch (exception &_ex) {
      cout << "ERROR for " << p_test_name << " '" << _ex.what() << "'" << endl;
    }
    cerr << "############ <- " << p_test_name << endl;
  }

  /// \brief print_mini_howto prints a mini how-to for using the \p test class
  void print_mini_howto() {
    using namespace std;
    cout << "Syntax:\n"
         << "\t'" << m_pgm_name
         << " --desc' will display a description of the test\n"
         << "\t'" << m_pgm_name << " --exec' will execute the all the tests\n"
         << "\t'" << m_pgm_name
         << " --exec { <test-name-1> <test-name-2> ...}' will execute tests "
            "defined between '{' and '}'\n"
         << "\t'" << m_pgm_name << "' displays this message\n\n"
         << "For the programmers: \n"
         << "\t1 - Programmers should use 'std::cerr' to print messages\n"
         << "\t2 - If do not want your 'std::cerr' messages to be "
            "displayed, "
            "use\n"
         << "\t'" << m_pgm_name
         << " --exec 2> /dev/null' to execute the tests\n\n"
         << "Output:\n"
         << "\tIf the test passes, the message \"SUCCESS for <name>\" will "
            "be "
            "printed\n"
         << "\tIf the test does not pass, the message \"FAIL for <name>\" "
            "will "
            "be "
            "printed\n"
         << "\tIf an error occurr while executing the test , the message "
            "\"ERROR "
            "for <name> <desc>\" will be printed\n"
         << "\tIf an exception occurrs, the message \"EXCEPTION "
            "<description>\" "
            "will be printed"
         << endl;
  }

private:
  /// \brief Name of the test program
  std::string m_pgm_name;

  /// \brief Indicates if the tests should actually be executed, or if theirs
  /// description should be printed
  bool m_execute_tests = {false};

  /// \brief Prints test decription to \p cout
  bool m_print_desc = {false};

  /// \brief Number of parameters passed to the \p test object
  int m_argc = {-1};

  /// \brief Parameters passed to the \p test object
  char **m_argv = {nullptr};

  /// \brief Set of tests to execute
  std::set<std::string> m_tests_to_exec;

  program::options m_options;
};

} // namespace tnct::tester

#endif
