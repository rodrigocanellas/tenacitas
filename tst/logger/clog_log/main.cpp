#include <logger/clog/log.h>
#include <tester/test.h>

using namespace tenacitas;

struct clog_how_to {

  bool operator()() {
    try {
      using namespace tenacitas;
      logger::clog::log::set_debug();
      logger::clog::log::debug("clog_how_to", __LINE__,
                               "teste de clog para debug ", -345, ' ', 19023,
                               ' ', 'W', 3.1415);
      logger::clog::log::info("clog_how_to", __LINE__,
                              "teste de clog para debug ", -345, ' ', 19023,
                              ' ', 'W', 3.1415);
      logger::clog::log::warn("clog_how_to", __LINE__,
                              "teste de clog para debug ", -345, ' ', 19023,
                              ' ', 'W', 3.1415);
      logger::clog::log::error("clog_how_to", __LINE__,
                               "teste de clog para debug ", -345, ' ', 19023,
                               ' ', 'W', 3.1415);
      logger::clog::log::fatal("clog_how_to", __LINE__,
                               "teste de clog para debug ", -345, ' ', 19023,
                               ' ', 'W', 3.1415);

      return true;

    } catch (std::exception &_ex) {
      std::cout << "ERRO cout_log_creation: '" << _ex.what() << "'"
                << std::endl;
    }
    return false;
  }

  static std::string desc() { return "Simple 'clog' log usage"; }
};

int main(int argc, char **argv) {
  tester::test _tester(argc, argv);
  run_test(_tester, clog_how_to);
}
