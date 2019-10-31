#include <business/logger/cerr.h>
#include <business/logger/log.h>
#include <business/tester/run.h>

using namespace tenacitas::business;

struct cerr_log_how_to
{
  bool operator()()
  {

    try {
      logger::configure_cerr_log();

      logger::log::set_debug();

      logger::log::debug("cerr_log_how_to", __LINE__, "hello! ", 309);
      logger::log::debug(
            "cerr_log_how_to", __LINE__, "how are you doing? ", 3.14);
      logger::log::info("cerr_log_how_to", __LINE__, "fine!! ", 'W');
      logger::log::info("cerr_log_how_to", __LINE__, "and you?");
      logger::log::warn(
            "cerr_log_how_to", __LINE__, "great! got a new job!! ", 6987.58f);
      logger::log::warn("cerr_log_how_to", __LINE__, "nice!! ", 10);
      return true;
    } catch (std::exception& _ex) {
      std::cerr << "ERRO cerr_log_creation: '" << _ex.what() << "'"
                << std::endl;
    }
    return false;
  }
};

int
main(int argc, char** argv)
{
  run_test(cerr_log_how_to, argc, argv, "Simple 'cerr_log' usage");
}
