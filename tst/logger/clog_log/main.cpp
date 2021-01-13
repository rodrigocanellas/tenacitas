#include <tenacitas/logger.h>
#include <tenacitas/tester.h>

using namespace tenacitas;

struct clog_how_to {

  bool operator()() {
    try {
      using namespace tenacitas;
      m_log.debug(this, __LINE__, "teste de clog para debug ", -345, ' ', 19023,
                  ' ', 'W', ' ', 3.1415);
      m_log.info(this, __LINE__, "teste de clog para debug ", -345, ' ', 19023,
                 ' ', 'W', ' ', 3.1415);
      m_log.warn(this, __LINE__, "teste de clog para debug ", -345, ' ', 19023,
                 ' ', 'W', ' ', 3.1415);
      m_log.error(this, __LINE__, "teste de clog para debug ", -345, ' ', 19023,
                  ' ', 'W', ' ', 3.1415);
      m_log.fatal(this, __LINE__, "teste de clog para debug ", -345, ' ', 19023,
                  ' ', 'W', ' ', 3.1415);

      return true;

    } catch (std::exception &_ex) {
      std::cout << "ERRO cout_log_creation: '" << _ex.what() << "'"
                << std::endl;
    }
    return false;
  }

  static std::string desc() { return "Simple 'clog' log usage"; }

private:
  logger::clog<> m_log{"clog_how_to"};
};

int main(int argc, char **argv) {
  logger::set_debug_level();
  tester::test _tester(argc, argv);
  run_test(_tester, clog_how_to);
}
