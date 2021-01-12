#ifndef TENACITAS_TESTER_H
#define TENACITAS_TESTER_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#include <iostream>

#include <tenacitas/program.h>

/// \brief master namespace
namespace tenacitas {

/// \brief helps creates tests
namespace tester {

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
template <bool use = true> struct test {

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
  test(int argc, char **argv) : m_argc(argc), m_argv(argv) {
    m_pgm_name = m_argv[0];

    try {

      program::options _options;

      _options.parse(m_argc, m_argv);
      if (_options.get_bool_param("exec").first) {
        m_execute_tests = true;
      } else if (_options.get_bool_param("desc").first) {
        m_print_desc = true;
      } else {

        std::pair<bool, std::list<program::options<>::value>> _tests_to_exec =
            _options.get_set_param("exec");
        if (_tests_to_exec.first) {
          m_execute_tests = true;
          m_tests_to_exec.insert(_tests_to_exec.second.begin(),
                                 _tests_to_exec.second.end());
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

  test() = delete;
  test(const test &) = delete;
  test(test &&) = delete;

  test &operator=(const test &) = delete;
  test &operator=(test &&) = delete;

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

  /// \brief print_mini_howto prints a mini how-to for using the \p test class
  void print_mini_howto() {
    using namespace std;
    cout
        << "Syntax:\n"
        << "\t'" << m_pgm_name
        << " --desc' will display a description of the test\n"
        << "\t'" << m_pgm_name << " --exec' will execute the all the tests\n"
        << "\t'" << m_pgm_name
        << " --exec { <test-name-1> <test-name-2> ...}' will execute tests "
           "defined between '{' and '}'\n"
        << "\t'" << m_pgm_name << "' displays this message\n\n"
        << "For the programmers: \n"
        << "\t1 - Programmers should use 'std::cerr' to print messages\n"
        << "\t2 - If do not want your 'std::cerr' messages to be displayed, "
           "use\n"
        << "\t'" << m_pgm_name
        << " --exec 2> /dev/null' to execute the tests\n\n"
        << "Output:\n"
        << "\tIf the test passes, the message \"SUCCESS for <name>\" will be "
           "printed\n"
        << "\tIf the test does not pass, the message \"FAIL for <name>\" will "
           "be "
           "printed\n"
        << "\tIf an error occurr while executing the test , the message "
           "\"ERROR "
           "for <name> <desc>\" will be printed\n"
        << "\tIf an exception occurrs, the message \"EXCEPTION <description>\" "
           "will be printed"
        << endl;
  }

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

#endif
