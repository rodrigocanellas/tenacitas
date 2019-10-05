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

        positioner _positioner(coordinate::x(13), coordinate::y(13));
        _positioner.add("STYRENE", "teste");
        _positioner.add("IRONS", "teste");
        _positioner.add("URANIAN", "teste");
        _positioner.add("DENOM", "teste");
        _positioner.add("ZONKOUT", "teste");
        _positioner.add("OLANA", "teste");
        _positioner.add("AUA", "teste");
        _positioner.add("TENN", "teste");
        _positioner.add("LLN", "teste");
        _positioner.add("NTNSVCRNT", "teste");
        _positioner.add("LID", "teste");
        _positioner.add("FIH", "teste");
        _positioner.add("WHIFFS", "teste");
        _positioner.add("ONECL", "teste");
        _positioner.add("ARSES", "teste");
        _positioner.add("MEASLY", "teste");
        _positioner.add("AVI", "teste");
        _positioner.add("ARR", "teste");
        _positioner.add("AISLEDUEL", "teste");
        _positioner.add("RIM", "teste");
        _positioner.add("GNLG", "teste");
        _positioner.add("NEN", "teste");
        _positioner.add("ONADE", "teste");
        _positioner.add("AOKTREE", "teste");
        _positioner.add("SERER", "teste");
        _positioner.add("YUPPIES", "teste");
        _positioner.add("ASKUS", "teste");
        _positioner.add("SSHAPES", "teste");
        _positioner.add("SUZAN", "teste");
        _positioner.add("TROUT", "teste");
        _positioner.add("YANAN", "teste");
        _positioner.add("RNK", "teste");
        _positioner.add("EIO", "teste");
        _positioner.add("NAUTCH", "teste");
        _positioner.add("ENTER", "teste");
        _positioner.add("IDONTHAVE", "teste");
        _positioner.add("REL", "teste");
        _positioner.add("ONALLFS", "teste");
        _positioner.add("NONLIFE", "teste");
        _positioner.add("SMANDSS", "teste");
        _positioner.add("NNW", "teste");
        _positioner.add("SFCS", "teste");
        _positioner.add("VILLAGERS", "teste");
        _positioner.add("IRID", "teste");
        _positioner.add("OMAROSA", "teste");
        _positioner.add("NERINES", "teste");
        _positioner.add("EARMARK", "teste");
        _positioner.add("YIN", "teste");
        _positioner.add("ALGOUS", "teste");
        _positioner.add("SLAYS", "teste");
        _positioner.add("UNRIP", "teste");
        _positioner.add("EEEEE", "teste");
        _positioner.add("LNESS", "teste");
        _positioner.add("DEU", "teste");
        _positioner.add("KPH", "teste");
        _positioner.add("TPA", "teste");

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
