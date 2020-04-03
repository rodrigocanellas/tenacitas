#include <logger/cout/log.h>
#include <tester/_bus/test.h>

using namespace tenacitas;
struct cout_how_to {
  bool operator()() {
    try {

      logger::cout::log::set_debug();

      logger::cout::log::debug("cout_how_to", __LINE__, "hello! ", 309);
      logger::cout::log::debug("cout_how_to", __LINE__, "how are you doing? ", 3.14);
      logger::cout::log::info("cout_how_to", __LINE__, "fine!! ", 'W');
      logger::cout::log::info("cout_how_to", __LINE__, "and you?");
      logger::cout::log::warn("cout_how_to", __LINE__, "great! got a new job!! ",
                         6987.58f);
      logger::cout::log::warn("cout_how_to", __LINE__, "nice!! ", 10);
      return true;
    } catch (std::exception &_ex) {
      std::cout << "ERRO cout_log_creation: '" << _ex.what() << "'"
                << std::endl;
    }
    return false;
  }

  static std::string desc() { return "Simple 'cout' log usage"; }
  static std::string name() { return "cout_how_to"; }
};

int main(int argc, char **argv) { tester::_bus::test::run<cout_how_to>(argc, argv); }
