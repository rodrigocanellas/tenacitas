#include <iostream>
#include <utility>

#include <crosswords/business/positioner003.h>
#include <crosswords/entities/board.h>
#include <crosswords/entities/coordinate.h>
#include <crosswords/entities/word.h>
#include <crosswords/entities/words.h>
#include <logger/business/cerr.h>
#include <tester/business/run.h>

using namespace tenacitas::crosswords::entities;
using namespace tenacitas::crosswords::business;
using namespace tenacitas::logger::business;

struct positioning_010
{
    bool operator()()
    {
        typedef positioner003_t<tenacitas::logger::business::log> positioner;

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

        // 408966 tentativas
        // interesse estibordo discoteca borboleta tubarao horario habitat tutor
        // lebre molusco set pos psd ima iab

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
    run_test(positioning_010,
             argc,
             argv,
             "positioning a bunch of words using 'positioner_002'");
    return 0;
}

//#include <stdio.h>
//#include <iostream>
//#include <vector>
//#include <string>

// typedef std::vector<std::string> words;
// typedef words::size_type size;
// typedef words::value_type value;

// void troca(words & p_words, size i, size j)
//{
//    value aux = p_words[i];
//    p_words[i] = p_words[j];
//    p_words[j] = aux;
//}

// void permuta(words & p_words, size inf, size sup)
//{
//    if(inf == sup)
//    {
//        for(size i = 0; i <= sup; i++)
//            std::cout << p_words[i] << " ";
//        std::cout << std::endl;
//    }
//    else
//    {
//        for(size i = inf; i <= sup; i++)
//        {
//            troca(p_words, inf, i);
//            permuta(p_words, inf + 1, sup);
//            troca(p_words, inf, i); // backtracking
//        }
//    }
//}

// int main()
//{
//    words _words = {"interesse", "estibordo", "borboleta", "discoteca"};

//    permuta(_words, 0, _words.size() - 1);

//    return 0;
//}
