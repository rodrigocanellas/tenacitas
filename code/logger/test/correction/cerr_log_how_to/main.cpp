#include <logger/business/cerr.h>
#include <tester/business/run.h>

struct cerr_log_how_to
{
    bool operator()()
    {
        try {
            tenacitas::logger::business::configure_cerr_log();

            cerr_set_debug();

            cerr_debug("hello! ", 309);
            cerr_debug("how are you doing? ", 3.14);
            cerr_info("fine!! ", 'W');
            cerr_info("and you?");
            cerr_warn("great! got a new job!! ", 6987.58f);
            cerr_warn("nice!! ", 10);
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
