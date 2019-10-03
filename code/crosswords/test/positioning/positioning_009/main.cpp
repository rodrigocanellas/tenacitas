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

struct positioning_009
{
    bool operator()()
    {
        typedef positioner000_t<tenacitas::logger::business::log> positioner;

        positioner _positioner(coordinate::x(9), coordinate::y(11));
        _positioner.add("reluzente", "teste");
        _positioner.add("negar", "teste");
        _positioner.add("franqueza", "teste");
        _positioner.add("silvestre", "teste");
        _positioner.add("confundir", "teste");
        _positioner.add("ovo", "teste");
        _positioner.add("aroma", "teste");
        _positioner.add("rmn", "teste");
        _positioner.add("socio", "teste");
        _positioner.add("rei", "teste");
        _positioner.add("lagoa", "teste");
        _positioner.add("lindo", "teste");
        _positioner.add("zurique", "teste");
        _positioner.add("uma", "teste");
        _positioner.add("noz", "teste");
        _positioner.add("estadio", "teste");
        _positioner.add("planear", "teste");

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
        return _res;
    }
};

int
main(int argc, char** argv)
{
    tenacitas::logger::business::configure_cerr_log();
    run_test(positioning_009, argc, argv, "positioning a bunch of words");
    return 0;
}
