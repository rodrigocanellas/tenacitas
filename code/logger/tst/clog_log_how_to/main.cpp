#include <logger/clog/clog.h>
#include <tester/bus/test.h>

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
  static std::string name() { return "clog_how_to"; }
};

int main(int argc, char **argv) { tester::bus::test::run<clog_how_to>(argc, argv); }
