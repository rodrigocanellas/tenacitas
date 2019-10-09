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

struct positioning_007
{
    bool operator()()
    {
        typedef positioner000_t<tenacitas::logger::business::log> positioner;

        positioner _positioner(coordinate::x(13), coordinate::y(13));

        _positioner.add("interesse", "teste");
        _positioner.add("estibordo", "teste");
        _positioner.add("borboleta", "teste");
        _positioner.add("discoteca", "teste");

        _positioner.add("habitat", "teste");
        _positioner.add("horario", "teste");
        _positioner.add("molusco", "teste");
        _positioner.add("tubarao", "teste");

        _positioner.add("lebre", "teste");

        _positioner.add("tutor", "teste");

        _positioner.add("pos", "teste");
        _positioner.add("ima", "teste");
        _positioner.add("psd", "teste");
        _positioner.add("iab", "teste");
        _positioner.add("set", "teste");

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
    run_test(positioning_007, argc, argv, "positioning a bunch of words");
    return 0;
}
