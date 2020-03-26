#include <logger/business/cerr.h>
#include <logger/business/file.h>
#include <tester/business/run.h>

struct file_log_how_to
{
    bool operator()()
    {
        using namespace tenacitas::logger::business;
        try {
            configure_file_log(
              ".", "how_to", 10 * 1024, std::chrono::minutes(15));

            log::set_debug();

            log::debug("file_log_how_to", __LINE__, "hello! ", 309);
            log::debug(
              "file_log_how_to", __LINE__, "how are you doing? ", 3.14);
            log::info("file_log_how_to", __LINE__, "fine!! ", 'W');
            log::info("file_log_how_to", __LINE__, "and you?");
            log::warn(
              "file_log_how_to", __LINE__, "great! got a new job!! ", 6987.58f);
            log::warn("file_log_how_to", __LINE__, "nice!! ", 10);
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
