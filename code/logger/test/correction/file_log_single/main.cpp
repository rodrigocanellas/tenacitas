/// \example
#include <iostream>

#include <concurrent/business/sleeping_loop.h>
#include <logger/business/file.h>
#include <tester/business/run.h>

using namespace tenacitas::logger::business;
using namespace tenacitas::concurrent::business;

typedef sleeping_loop_t<void, tenacitas::logger::business::log> sleeping_loop;

class file_log_single
{

  public:
    bool operator()()
    {
        try {
            using namespace tenacitas;

            tenacitas::logger::business::configure_file_log(
              ".", "file_log_single", 10 * 1024, std::chrono::minutes(1));
            log::set_debug();

            sleeping_loop _loop1(
              std::chrono::milliseconds(500),
              []() {
                  log::test("file_log_single", __LINE__, "work!");
                  log::test("file_log_single", __LINE__, std::string(10, 't'));
                  log::debug(
                    "file_log_single", __LINE__, std::string(511, 'd'));
                  log::info("file_log_single", __LINE__, std::string(511, 'i'));
                  log::warn("file_log_single", __LINE__, std::string(511, 'w'));
                  log::error(
                    "file_log_single", __LINE__, std::string(511, 'e'));
                  log::test("file_log_single", __LINE__, std::string(511, 'T'));
                  log::debug(
                    "file_log_single", __LINE__, std::string(511, 'D'));
                  log::info("file_log_single", __LINE__, std::string(511, 'I'));
                  log::warn("file_log_single", __LINE__, std::string(511, 'W'));
                  log::error(
                    "file_log_single", __LINE__, std::string(511, 'E'));
                  return true;
              },
              std::chrono::seconds(5));

            _loop1.run();

            log::test("file_log_single", __LINE__, "---- sleeping 3 minutes");
            std::this_thread::sleep_for(std::chrono::minutes(3));
            log::test("file_log_single", __LINE__, "---- waking up");

            return true;
        } catch (std::exception& _ex) {
            log::fatal("file_log_single",
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
