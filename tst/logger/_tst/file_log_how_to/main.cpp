#include <logger/file/log.h>
#include <tester/test.h>

using namespace tenacitas;

struct file_log_how_to {
  bool operator()() {

    try {
      logger::file::log::configure(".", "how_to", 10 * 1024,
                                   std::chrono::minutes(15));

      logger::file::log::set_debug();

      logger::file::log::debug("file_log_how_to", __LINE__, "hello! ", 309);
      logger::file::log::debug("file_log_how_to", __LINE__,
                               "how are you doing? ", 3.14);
      logger::file::log::info("file_log_how_to", __LINE__, "fine!! ", 'W');
      logger::file::log::info("file_log_how_to", __LINE__, "and you?");
      logger::file::log::warn("file_log_how_to", __LINE__,
                              "great! got a new job!! ", 6987.58f);
      logger::file::log::warn("file_log_how_to", __LINE__, "nice!! ", 10);
      return true;
    } catch (std::exception &_ex) {
      std::cerr << "ERRO cerr_log_creation: '" << _ex.what() << "'"
                << std::endl;
    }
    return false;
  }
  static std::string desc() { return "Simple 'file_log' usage"; }
  static std::string name() { return "file_log_how_to"; }
};

int main(int argc, char **argv) {
  logger::file::log::set_debug();
  tester::_bus::test::run<file_log_how_to>(argc, argv);
}
