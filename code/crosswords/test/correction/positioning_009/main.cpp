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

// from http://www.emogic.com/crosswords/common/1570122125/index.html#self
struct positioning_009
{
    bool operator()()
    {
        typedef positioner000_t<tenacitas::logger::business::log> positioner;

        positioner _positioner(coordinate::x(13), coordinate::y(13));
        _positioner.add("THREECAMPBEDS", "teste");
        _positioner.add("IDOUBTIT", "teste");
        _positioner.add("ENNA", "teste");
        _positioner.add("POMPOMS", "teste");
        _positioner.add("GTPSY", "teste");
        _positioner.add("OVOID", "teste");
        _positioner.add("MOBBISH", "teste");
        _positioner.add("HALB", "teste");
        _positioner.add("PNEUMATO", "teste");
        _positioner.add("HERDSOFCATTLE", "teste");
        _positioner.add("SHED", "teste");
        _positioner.add("REPUMP", "teste");
        _positioner.add("ICETIME", "teste");
        _positioner.add("SMOTE", "teste");
        _positioner.add("OBVERT", "teste");
        _positioner.add("ADONISES", "teste");
        _positioner.add("OBVIATES", "teste");
        _positioner.add("COGENCY", "teste");
        _positioner.add("IMBEDS", "teste");
        _positioner.add("BSMITH", "teste");
        _positioner.add("APTOS", "teste");
        _positioner.add("TMLR ", "teste");


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
