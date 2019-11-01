#include <business/logger/cerr.h>
#include <business/logger/file.h>
#include <business/tester/run.h>

using namespace tenacitas::business;

struct file_log_how_to
{
  bool operator()()
  {

    try {
      logger::configure_file_log(
            ".", "how_to", 10 * 1024, std::chrono::minutes(15));

      logger::log::set_debug();

      logger::log::debug("file_log_how_to", __LINE__, "hello! ", 309);
      logger::log::debug(
            "file_log_how_to", __LINE__, "how are you doing? ", 3.14);
      logger::log::info("file_log_how_to", __LINE__, "fine!! ", 'W');
      logger::log::info("file_log_how_to", __LINE__, "and you?");
      logger::log::warn(
            "file_log_how_to", __LINE__, "great! got a new job!! ", 6987.58f);
      logger::log::warn("file_log_how_to", __LINE__, "nice!! ", 10);
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
  run_test(file_log_how_to, argc, argv, "Simple 'file_log' usage");
}
