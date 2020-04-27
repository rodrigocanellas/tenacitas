
#include <tester/test.h>

namespace tenacitas {
namespace tester {

test::test(int argc, char **argv) : m_argc(argc), m_argv(argv) {}

void test::print_mini_howto() {
  using namespace std;
  cout
      << "Syntax:\n"
      << "\t'" << m_pgm_name
      << " --desc' will display a description of the test\n"
      << "\t'" << m_pgm_name << " --exec' will execute the tests\n"
      << "\t'" << m_pgm_name << "' displays this message\n\n"
      << "Attention: \n"
      << "\tProgrammers should use 'std::cerr' to print messages\n"
      << "\tIf do not want your 'std::cerr' messages to be displayed, use\n"
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
