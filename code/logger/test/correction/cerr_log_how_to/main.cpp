#include <logger/bus/cerr.h>

int
main()
{
    tenacitas::logger::bus::configure_cerr_log();

    cerr_set_debug();

    cerr_debug("hello! ", 309);
    cerr_debug("how are you doing? ", 3.14);
    cerr_info("fine!! ", 'W');
    cerr_info("and you?");
    cerr_warn("great! got a new job!! ", 6987.58f);
    cerr_warn("nice!! ", 10);
}
