/// \example
#include <iostream>

#include <business/concurrent/traits.h>
#include <business/concurrent/sleeping_loop.h>
#include <business/logger/file.h>
#include <business/tester/run.h>

using namespace tenacitas::business;

typedef concurrent::sleeping_loop_t<void, logger::log> sleeping_loop;

class file_log_single
{

public:
  bool operator()()
  {
    try {
      using namespace tenacitas;

      logger::configure_file_log(
            ".", "file_log_single", 10 * 1024, std::chrono::minutes(1));
      logger::log::set_debug();

      sleeping_loop _loop1(
            std::chrono::milliseconds(500),
            []() -> concurrent::result {
              logger::log::test("file_log_single", __LINE__, "work!");
              logger::log::test("file_log_single", __LINE__, std::string(10, 't'));
              logger::log::debug(
              "file_log_single", __LINE__, std::string(511, 'd'));
              logger::log::info("file_log_single", __LINE__, std::string(511, 'i'));
              logger::log::warn("file_log_single", __LINE__, std::string(511, 'w'));
              logger::log::error(
              "file_log_single", __LINE__, std::string(511, 'e'));
              logger::log::test("file_log_single", __LINE__, std::string(511, 'T'));
              logger::log::debug(
              "file_log_single", __LINE__, std::string(511, 'D'));
              logger::log::info("file_log_single", __LINE__, std::string(511, 'I'));
              logger::log::warn("file_log_single", __LINE__, std::string(511, 'W'));
              logger::log::error(
              "file_log_single", __LINE__, std::string(511, 'E'));
              return concurrent::result::dont_stop;
            },
            std::chrono::seconds(5));

      _loop1.run();

      logger::log::test("file_log_single", __LINE__, "---- sleeping 3 minutes");
      std::this_thread::sleep_for(std::chrono::minutes(3));
      logger::log::test("file_log_single", __LINE__, "---- waking up");

      return true;
    } catch (std::exception& _ex) {
      logger::log::fatal("file_log_single",
                         __LINE__,
                         "ERRO log::log_single: '",
                         _ex.what(),
                         "'");
    }
    return false;
  }
};

int
main(int argc, char** argv)
{
  run_test(file_log_single, argc, argv, "Single thread logging to 'file'");
}
