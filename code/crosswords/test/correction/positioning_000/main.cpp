#include <iostream>
#include <utility>
#include <fstream>
#include <sstream>
#include <stdexcept>

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

struct positioning_000
{

  positioning_000(std::string && p_file_name)
  {
    using namespace std;
    ifstream _file(p_file_name);
    if (!_file.is_open()) {
      throw ("Could not open file '" + p_file_name +"'");
    }

    string _line;

    const string _result ("RESULT");

    while (true) {
      getline(_file, _line);
      if (!_file.good()) {
        throw ("Error reading");
      }
      if (_line[0] != '#') {
        break;
      }
      m_desc.append(&(_line[1]), &_line[_line.size()]);
    }

    if (_line.substr(0,_result.size()) != "RESULT") {
      throw ("Expected RESULT key word, but parsed '" + _line+ " instead");
    }

    string::size_type _pos = _line.find_first_not_of(' ',_result.size() );

    if (_pos == string::npos) {
      throw ("Could not find value of RESULT key word");
    }

    m_result = _line.substr(_pos, _line.size());


    getline(_file, _line);
    if (!_file.good()) {
      throw ("Error reading number of columns");
    }
    coordinate::x _x_limit (atoi(_line.c_str()));

    getline(_file, _line);
    if (!_file.good()) {
      throw ("Error reading number of rows");
    }
    coordinate::y _y_limit (atoi(_line.c_str()));

    m_positioner = positioner(_x_limit, _y_limit);

    uint16_t _counter = 1;
    while (true) {
      getline(_file, _line);
      if (_file.eof()) {
        break;
      }
      if (!_file.good()) {
        throw ("Error reading word #" + to_string(_counter++));
      }

      m_positioner.add(std::move(_line), "word #" + to_string(_counter++));
    }
  }

  inline const std::string & get_desc() const {return m_desc;}

  void operator()()
  {
    using namespace std;
    words _words(m_positioner());

    uint16_t _ok_counter = 0;

    for (words::const_iterator _pos = _words.begin(); _pos != _words.end();
         ++_pos) {
      if (_pos->positioned()) {
        crosswords_log_error(log, *_pos, " positioned");
        ++_ok_counter;
      } else {
        crosswords_log_test(log, *_pos, " NOT positioned");
      }
    }

    words::size _num_words = _words.get_size();

    cout << endl;
    if (m_result == "all") {
      if (_ok_counter == _num_words) {
        cout << "SUCCESS";
      }
      else {
        cout << "FAIL";
      }
    }
    else if (m_result == "none") {
      if (_ok_counter == 0) {
        cout << "SUCCESS";
      }
      else {
        cout << "FAIL";
      }
    }
    else if (m_result == "some") {
      if ( (_ok_counter == 0) || (_ok_counter == _num_words)) {
        cout << "FAIL";
      }
      else {
        cout << "SUCCESS";
      }
    }
    else {
      cout << "UNDEFINED RESULT";
    }
    cout << endl;


  }

private:
  typedef positioner004_t<tenacitas::logger::business::log> positioner;
private:
  positioner m_positioner;
  std::string m_desc;
  std::string m_result;
};

int
main(int argc, char** argv)
{
  using namespace std;
  tenacitas::logger::business::configure_cerr_log();

  try {
    if (argc != 4) {
      cout << "Syntax: " << argv[0] << " <file-name> <log-level> <action>" << endl << endl
           << "Where <log-level> can be 'debug', 'info', 'warn' or 'error'" << endl
           << "Where <action> can be 'desc' or 'exec'" << endl
           << "and where <file-name> has the following syntax: " << endl
           << "# comment line 0 " << endl
           << "# comment line i" << endl
           << "# ." << endl
           << "# ." << endl
           << "# ." << endl
           << "# comment line n " << endl
           << "RESULT: <all>/<some>/<none>"
           << "x-dimension" << endl
           << "y-dimension" << endl
           << "word-1" << endl
           << "word-2" << endl
           << "." << endl
           << "." << endl
           << "." << endl
           << "word-n" << endl << endl << endl
           << "Example:" << endl
           << "# Test of a 13x13 that should all be positioned" << endl
           << "RESULT all" << endl
           << "13" << endl
           << "13" << endl
           << "pos" << endl
           << "ima" << endl
           << "tutor" << endl
           << "psd" << endl
           << "iab" << endl
           << "interesse" << endl
           << "habitat" << endl
           << "horario" << endl
           << "molusco" << endl
           << "tubarao" << endl
           << "estibordo" << endl
           << "borboleta" << endl
           << "discoteca" << endl
           << "lebre" << endl
           << "set"  << endl << endl << endl
           << "The RESULT line indicates if 'all', 'some', or 'none' words should be positioned"
           << endl;
      return 1;
    }

    string _log_level(argv[2]);

    if(_log_level == "debug") {
      log::set_debug();
    }
    else if (_log_level == "info") {
      log::set_info();
    }
    else if (_log_level == "warn") {
      log::set_warn();
    }
    else {
      log::set_error();
    }

    positioning_000 _positioning_000(argv[1]);




    string _action(argv[3]);
    if (_action == "desc") {
      cout << _positioning_000.get_desc() << endl;
    }
    else {
      _positioning_000();
    }
  } catch (std::exception & _ex) {
    cout << "ERROR: " << _ex.what();
  }
  return 0;
}
