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

struct positioning_006
{
    bool operator()()
    {
        typedef positioner000_t<tenacitas::logger::business::log> positioner;

        positioner _positioner(coordinate::x(9), coordinate::y(12));
        _positioner.add("mamão", "fruta com pequenos caroços pretos");
        _positioner.add("abacaxi", "fruta com coroa");
        _positioner.add("xixi", "urina");
        _positioner.add("bola", "principal objeto do futebol");

        words _words(_positioner());

        for (words::const_iterator _pos = _words.begin(); _pos != _words.end();
             ++_pos) {
            if (!_pos->positioned()) {
                crosswords_log_error(log, *_pos, " not positioned");
                return false;
            }
            crosswords_log_test(log, *_pos);
        }
        return true;
    }
};

int
main(int argc, char** argv)
{
    tenacitas::logger::business::configure_cerr_log();
    run_test(positioning_006,
             argc,
             argv,
             "positioning 'abacaxi' 'mamão', 'xixi' and 'bola' ");
    return 0;
}