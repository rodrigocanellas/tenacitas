#include <business/logger/cout.h>
#include <business/tester/run.h>

using namespace tenacitas::business;

struct cout_log_how_to
{
  bool operator()()
  {
    try {

      logger::configure_cout_log();

      logger::log::set_debug();

      logger::log::debug("cout_log_how_to", __LINE__, "hello!", 309);
      logger::log::debug(
            "cout_log_how_to", __LINE__, "how are you doing? ", 3.14);
      logger::log::info("cout_log_how_to", __LINE__, "fine!! ", 'W');
      logger::log::info("cout_log_how_to", __LINE__, "and you?");
      logger::log::warn(
            "cout_log_how_to", __LINE__, "great! got a new job!! ", 6987.58f);
      logger::log::warn("cout_log_how_to", __LINE__, "nice!! ", 10);
      return true;
    } catch (std::exception& _ex) {
      std::cout << "ERRO cout_log_creation: '" << _ex.what() << "'"
                << std::endl;
    }
    return false;
  }
};

int
main(int argc, char** argv)
{
  run_test(cout_log_how_to, argc, argv, "Simple 'cout_log' usage");
}
