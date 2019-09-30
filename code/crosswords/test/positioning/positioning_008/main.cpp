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

struct positioning_008
{
    bool operator()()
    {
        typedef positioner000_t<tenacitas::logger::business::log> positioner;

        positioner _positioner(coordinate::x(9), coordinate::y(11));
        _positioner.add("ab", "teste");
        _positioner.add("cd", "teste");
        _positioner.add("ef", "teste");
        _positioner.add("gh", "teste");

        words _words(_positioner());

        bool _res = true;
        for (words::const_iterator _pos = _words.begin(); _pos != _words.end();
             ++_pos) {
            if (!_pos->positioned()) {
                crosswords_log_error(log, *_pos, " NOT positioned");
                _res = false;
            } else {
                crosswords_log_test(log, *_pos, " positioned");
            }
        }
        return (_res ? false : true);
    }
};

int
main(int argc, char** argv)
{
    tenacitas::logger::business::configure_cerr_log();
    run_test(positioning_008,
             argc,
             argv,
             "testing if detects that there is no solution with 'ab' 'cd' 'ef' "
             "'gh'");
    return 0;
}
