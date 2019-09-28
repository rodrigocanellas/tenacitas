#include <iostream>
#include <utility>

#include <crosswords/business/positioner000.h>
#include <crosswords/entities/board.h>
#include <crosswords/entities/coordinate.h>
#include <crosswords/entities/word.h>
#include <crosswords/entities/words.h>
#include <logger/business/cerr.h>
#include <tester/business/run.h>

using namespace tenacitas::crosswords::entities;
using namespace tenacitas::crosswords::business;
using namespace tenacitas::logger::business;

struct positioning_002
{
    bool operator()()
    {
        typedef positioner000_t<tenacitas::logger::business::log> positioner;

        positioner _positioner(coordinate::x(4), coordinate::y(4));
        if (_positioner.add("police", "organization to server and protect") !=
            positioner::result::ok) {
            return true;
        }
        return false;
    }
};

int
main(int argc, char** argv)
{
    tenacitas::logger::business::configure_cerr_log();
    run_test(
      positioning_002, argc, argv, "positioning 'police' in a 4x4 board");
    return 0;
}
