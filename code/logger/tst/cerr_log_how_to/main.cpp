#include <logger/cerr/log.h>
#include <tester/test.h>

using namespace tenacitas;
struct cerr_log_how_to {
  bool operator()() {

    try {

      logger::cerr::log::set_debug();

      logger::cerr::log::debug("cerr_log_how_to", __LINE__, "hello! ", 309);
      logger::cerr::log::debug("cerr_log_how_to", __LINE__,
                               "how are you doing? ", 3.14);
      logger::cerr::log::info("cerr_log_how_to", __LINE__, "fine!! ", 'W');
      logger::cerr::log::info("cerr_log_how_to", __LINE__, "and you?");
      logger::cerr::log::warn("cerr_log_how_to", __LINE__,
                              "great! got a new job!! ", 6987.58f);
      logger::cerr::log::warn("cerr_log_how_to", __LINE__, "nice!! ", 10);
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
