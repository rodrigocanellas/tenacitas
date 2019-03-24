#include <logger/business/cerr.h>
#include <logger/business/file.h>
#include <tester/business/run.h>

struct file_log_how_to
{
    bool operator()()
    {
        try {
            tenacitas::logger::business::configure_file_log(
              ".", "how_to", 10 * 1024, std::chrono::minutes(15));

            file_set_debug();

            file_debug("hello! ", 309);
            file_debug("how are you doing? ", 3.14);
            file_info("fine!! ", 'W');
            file_info("and you?");
            file_warn("great! got a new job!! ", 6987.58f);
            file_warn("nice!! ", 10);
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
