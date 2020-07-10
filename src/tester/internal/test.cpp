
#include <iostream>

#include <tester/test.h>

namespace tenacitas {
namespace tester {

test::test(int argc, char **argv) : m_argc(argc), m_argv(argv) {
  m_pgm_name = m_argv[0];

  try {

    program::options _options;

    _options.parse(m_argc, m_argv);
    if (_options.get_bool_param("exec").first) {
      m_execute_tests = true;
    } else if (_options.get_bool_param("desc").first) {
      m_print_desc = true;
    } else {

      std::pair<bool, std::list<program::options::value>> _tests_to_exec =
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

void test::print_mini_howto() {
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
      << "\t2 - If do not want your 'std::cerr' messages to be displayed, use\n"
      << "\t'" << m_pgm_name << " --exec 2> /dev/null' to execute the tests\n\n"
      << "Output:\n"
      << "\tIf the test passes, the message \"SUCCESS for <name>\" will be "
         "printed\n"
      << "\tIf the test does not pass, the message \"FAIL for <name>\" will be "
         "printed\n"
      << "\tIf an error occurr while executing the test , the message \"ERROR "
         "for <name> <desc>\" will be printed\n"
      << "\tIf an exception occurrs, the message \"EXCEPTION <description>\" "
         "will be printed"
      << endl;
}

} // namespace tester
} // namespace tenacitas
