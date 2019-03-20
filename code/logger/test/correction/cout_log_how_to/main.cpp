#include <logger/bus/cout.h>

int
main()
{
    tenacitas::logger::bus::configure_cout_log();

    cout_set_debug();

    cout_debug("hello! ", 309);
    cout_debug("how are you doing? ", 3.14);
    cout_info("fine!! ", 'W');
    cout_info("and you?");
    cout_warn("great! got a new job!! ", 6987.58f);
    cout_warn("nice!! ", 10);
}
