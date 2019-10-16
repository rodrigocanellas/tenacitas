#include <iostream>
#include <utility>

#include <crosswords/business/positioner004.h>
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
        typedef positioner004_t<tenacitas::logger::business::log> positioner;

        positioner _positioner(coordinate::x(9), coordinate::y(12));
        _positioner.add("aa", "teste");
        _positioner.add("bb", "teste");


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
        return !_res;
    }
};

int
main(int argc, char** argv)
{
    tenacitas::logger::business::configure_cerr_log();
    run_test(positioning_002, argc, argv, "checking if it stops when it was not possible to position 'aa' and 'bb'");
    return 0;
}
