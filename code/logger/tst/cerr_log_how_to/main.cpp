#include <logger/cerr/cerr.h>
#include <tester/test.h>

using namespace tenacitas;
struct cerr_log_how_to {
  bool operator()() {

    try {

      logger::cerr::set_debug();

      logger::cerr::debug("cerr_log_how_to", __LINE__, "hello! ", 309);
      logger::cerr::debug("cerr_log_how_to", __LINE__, "how are you doing? ",
                          3.14);
      logger::cerr::info("cerr_log_how_to", __LINE__, "fine!! ", 'W');
      logger::cerr::info("cerr_log_how_to", __LINE__, "and you?");
      logger::cerr::warn("cerr_log_how_to", __LINE__, "great! got a new job!! ",
                         6987.58f);
      logger::cerr::warn("cerr_log_how_to", __LINE__, "nice!! ", 10);
      return true;
    } catch (std::exception &_ex) {
      std::cerr << "ERRO cerr_log_creation: '" << _ex.what() << "'"
                << std::endl;
    }
    return false;
  }

  static std::string desc() { return "Simple 'cerr' log usage"; }
  static std::string name() { return "cerr_log_how_to"; }
};

int main(int argc, char **argv) {
  tester::test::run<cerr_log_how_to>(argc, argv);
}
