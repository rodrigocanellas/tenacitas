#include <tenacitas/logger.h>
#include <tenacitas/tester.h>

using namespace tenacitas;

struct clog_how_to {

  bool operator()() {
    try {
      using namespace tenacitas;
      logger::log::debug(__FILE__, __LINE__, "teste de clog para debug ", -345,
                         ' ', 19023, ' ', 'W', ' ', 3.1415);
      logger::log::info(__FILE__, __LINE__, "teste de clog para debug ", -345,
                        ' ', 19023, ' ', 'W', ' ', 3.1415);
      logger::log::warn(__FILE__, __LINE__, "teste de clog para debug ", -345,
                        ' ', 19023, ' ', 'W', ' ', 3.1415);
      logger::log::error(__FILE__, __LINE__, "teste de clog para debug ", -345,
                         ' ', 19023, ' ', 'W', ' ', 3.1415);
      logger::log::fatal(__FILE__, __LINE__, "teste de clog para debug ", -345,
                         ' ', 19023, ' ', 'W', ' ', 3.1415);

      return true;

    } catch (std::exception &_ex) {
      std::cout << "ERRO clog_how_to: '" << _ex.what() << "'" << std::endl;
    }
    return false;
  }

  static std::string desc() { return "Simple 'clog' log usage"; }
};

int main(int argc, char **argv) {
  logger::log::set_debug_level();
  logger::log::use_clog();
  tester::test _tester(argc, argv);
  run_test(_tester, clog_how_to);
}
