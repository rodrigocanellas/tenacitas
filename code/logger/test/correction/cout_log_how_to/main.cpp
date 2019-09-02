#include <logger/business/cout.h>
#include <tester/business/run.h>

struct cout_log_how_to
{
    bool operator()()
    {
        try {
            using namespace tenacitas::logger::business;
            configure_cout_log();

            log::set_debug();

            log::debug("cout_log_how_to", __LINE__, "hello!", 309);
            log::debug(
              "cout_log_how_to", __LINE__, "how are you doing? ", 3.14);
            log::info("cout_log_how_to", __LINE__, "fine!! ", 'W');
            log::info("cout_log_how_to", __LINE__, "and you?");
            log::warn(
              "cout_log_how_to", __LINE__, "great! got a new job!! ", 6987.58f);
            log::warn("cout_log_how_to", __LINE__, "nice!! ", 10);
            return true;
        } catch (std::exception& _ex) {
            std::cout << "ERRO cout_log_creation: '" << _ex.what() << "'"
                      << std::endl;
        }
        return false;
    }
};

int
main(int argc, char** argv)
{
    run_test(cout_log_how_to, argc, argv, "Simple 'cout_log' usage");
}
