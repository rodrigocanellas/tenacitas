#include <logger/business/cerr.h>
#include <logger/business/log.h>
#include <tester/business/run.h>

struct cerr_log_how_to
{
    bool operator()()
    {
        using namespace tenacitas::logger::business;
        try {
            configure_cerr_log();

            log::set_debug();

            log::debug("cerr_log_how_to", __LINE__, "hello! ", 309);
            log::debug(
              "cerr_log_how_to", __LINE__, "how are you doing? ", 3.14);
            log::info("cerr_log_how_to", __LINE__, "fine!! ", 'W');
            log::info("cerr_log_how_to", __LINE__, "and you?");
            log::warn(
              "cerr_log_how_to", __LINE__, "great! got a new job!! ", 6987.58f);
            log::warn("cerr_log_how_to", __LINE__, "nice!! ", 10);
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
