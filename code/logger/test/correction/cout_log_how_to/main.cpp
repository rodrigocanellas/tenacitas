#include <logger/business/cout.h>
#include <tester/business/run.h>

struct cout_log_how_to
{
    bool operator()()
    {
        try {
            tenacitas::logger::business::configure_cout_log();

            cout_set_debug();

            cout_debug("hello! ", 309);
            cout_debug("how are you doing? ", 3.14);
            cout_info("fine!! ", 'W');
            cout_info("and you?");
            cout_warn("great! got a new job!! ", 6987.58f);
            cout_warn("nice!! ", 10);
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
