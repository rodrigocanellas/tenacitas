#include <iostream>
#include <string>
#include <utility>
#include <fstream>

#include <crosswords/business/positioner003.h>
#include <crosswords/entities/board.h>
#include <crosswords/entities/coordinate.h>
#include <crosswords/entities/word.h>
#include <crosswords/entities/words.h>
#include <logger/business/cerr.h>

using namespace tenacitas::crosswords::entities;
using namespace tenacitas::crosswords::business;
using namespace tenacitas::logger::business;


int
main(int argc, char** argv)
{
    using namespace std;
    if (argc != 2)  {
        cout << "Syntax: " << argv[0] << " <file-name>" << endl
             << "Where 'file-name' is the name of the file with the following syntax: " << endl
             << "<num-cols>" << endl
             << "<num-rows>" << endl
             << "<word-0>" << endl
             << "<word-1>" << endl
             << "." << endl
             << "." << endl
             << "." << endl
             << "<word-n>" << endl;
        return 1;
    }

    ifstream _file(argv[1]);
    if (!_file.is_open()) {
        cout << "Could not open file '" << argv[1] << "'" << endl;
        return 2;
    }

    tenacitas::logger::business::configure_cerr_log();
    tenacitas::logger::business::log::set_debug();

    string _line;

    getline(_file, _line);
    if (!_file.good()) {
        cout << "Error reading number of columns" << endl;
        return 3;
    }
    coordinate::x _x_limit (atoi(_line.c_str()));

    getline(_file, _line);
    if (!_file.good()) {
        cout << "Error reading number of rows" << endl;
        return 4;
    }
    coordinate::y _y_limit (atoi(_line.c_str()));

    typedef positioner003_t<tenacitas::logger::business::log> positioner;

    positioner _positioner(_x_limit, _y_limit);

    uint16_t _counter = 1;
    while (true) {
        getline(_file, _line);
        if (_file.eof()) {
            break;
        }
        if (!_file.good()) {
            cout << "Error reading word #" << _counter++ << endl;
            return 5;
        }

        _positioner.add(std::move(_line), "word #" + to_string(_counter));

    }

    words _words(_positioner());

    bool _res = true;
    for (words::const_iterator _pos = _words.begin(); _pos != _words.end();
         ++_pos) {
        if (!_pos->positioned()) {
            cout << *_pos << " NOT positioned" << endl;
            _res = false;
        } else {
            cout << *_pos <<  " positioned" << endl;
        }
    }
    if (!_res) {
        cout << "ERRO!!!" << endl;
        return 6;
    }
    _words.print_positioned(_x_limit, _y_limit);
    return 0;
}

