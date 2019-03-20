#include <logger/bus/cerr.h>
#include <logger/bus/file.h>

int
main()
{
    tenacitas::logger::bus::configure_cerr_log();
    cerr_set_debug();
    tenacitas::logger::bus::configure_file_log(
      ".", "how_to", 10 * 1024, std::chrono::minutes(15));

    file_set_debug();

    file_debug("hello! ", 309);
    //    file_debug("how are you doing? ", 3.14);
    //    file_info("fine!! ", 'W');
    //    file_info("and you?");
    //    file_warn("great! got a new job!! ", 6987.58f);
    //    file_warn("nice!! ", 10);
}
