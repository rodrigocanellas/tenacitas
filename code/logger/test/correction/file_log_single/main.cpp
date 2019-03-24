
#include <iostream>

#include <concurrent/bus/sleeping_loop.h>
#include <logger/bus/file.h>
#include <tester/bus/run.h>

class file_log_single
{

  public:
    bool operator()()
    {
        try {
            using namespace tenacitas;

            tenacitas::logger::bus::configure_file_log(
              ".", "file_log_single", 10 * 1024, std::chrono::minutes(1));
            file_set_debug();

            concurrent::bus::sleeping_loop<void> _loop1(
              std::chrono::milliseconds(500),
              []() {
                  cerr_test("work!");
                  file_test(std::string(10, 't'));
                  file_debug(std::string(511, 'd'));
                  file_info(std::string(511, 'i'));
                  file_warn(std::string(511, 'w'));
                  file_error(std::string(511, 'e'));
                  file_test(std::string(511, 'T'));
                  file_debug(std::string(511, 'D'));
                  file_info(std::string(511, 'I'));
                  file_warn(std::string(511, 'W'));
                  file_error(std::string(511, 'E'));
                  return true;
              },
              std::chrono::seconds(5));

            _loop1.run();

            cerr_test("---- sleeping 3 minutes");
            std::this_thread::sleep_for(std::chrono::minutes(3));
            cerr_test("---- waking up");

            return true;
        } catch (std::exception& _ex) {
            cerr_fatal("ERRO file_log_single: '", _ex.what(), "'");
        }
        return false;
    }
};

int
main(int argc, char** argv)
{
    run_test(file_log_single, argc, argv, "Single thread logging to 'file'");
}
