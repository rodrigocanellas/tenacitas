#include <iostream>
#include <utility>

#include <crosswords/business/positioner000.h>
#include <crosswords/entities/board.h>
#include <crosswords/entities/word.h>
#include <crosswords/entities/words.h>
#include <logger/business/cerr.h>
#include <tester/business/run.h>

using namespace tenacitas::crosswords::entities;
using namespace tenacitas::crosswords::business;
using namespace tenacitas::logger::business;

struct positioning_001
{
    bool operator()()
    {
        typedef positioner000_t<tenacitas::logger::business::log> positioner;
        typedef board_t<positioner> board;

        positioner _positioner(9, 12);
        _positioner.add("abacaxi", "fruta com coroa");
        _positioner.add("lego", "brinquedo de montar");

        board _board(std::move(_positioner));

        for (words::const_iterator _pos = _board.begin();
             _pos != _board.end();
             ++_pos) {
            std::cout << *_pos << std::endl;
        }
        return true;
    }
};

int
main(int argc, char** argv)
{
    tenacitas::logger::business::configure_cerr_log();
    run_test(positioning_001, argc, argv, "could not position 'abacaxi' and 'lego'");
    return 0;
}
