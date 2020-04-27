#include <algorithm>
#include <array>
#include <cstdint>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <program/options.h>

using namespace std;

int main() {
  using namespace tenacitas::program;

  const int argc = 12;
  const char *argv[] = {"pgm-name",
                        "--desc",
                        "desc.csv",
                        "--data",
                        "in_quote-50.csv",
                        "--field",
                        ",",
                        "--dec",
                        ",",
                        "--value-encoded",
                        "--dbf",
                        "out_quote-50.dbf"};

  options _pgm_options;

  _pgm_options.parse(argc, (char **)argv,
                     {"desc", "data", "field", "dec", "value-encoded", "dbf"});

  std::stringstream _stream;
  _stream << _pgm_options;

  std::cerr << "options: " << _stream.str() << std::endl;

  bool _enclosed_value = false;
  if (_pgm_options["value-encoded"]) {
    _enclosed_value = true;
  }

  std::cerr << std::boolalpha << "enclosed value? " << _enclosed_value
            << std::endl;

  std::optional<std::string> _field_sep = _pgm_options["field"];
  if (!_field_sep) {
    std::cerr
        << "parameter 'field', that defines the field separator is missing"
        << std::endl;
    return 20;
  }

  std::optional<std::string> _decimal_sep = _pgm_options["dec"];
  if (!_decimal_sep) {
    std::cerr << "parameter 'dec' that define the decimal separator is missing"
              << std::endl;
    return 30;
  }

  std::optional<std::string> _desc = _pgm_options["desc"];
  if (!_desc) {
    std::cerr << "parameter 'desc' that defines the name of the file is missing"
              << std::endl;
    return 40;
  }

  std::optional<std::string> _data = _pgm_options["data"];
  if (!_data) {
    std::cerr
        << "parameter 'data' that defines the name of the data file is missing"
        << std::endl;
    return 50;
  }

  std::optional<std::string> _dbf_file = _pgm_options["dbf"];
  if (!_dbf_file) {
    std::cerr << "parameter 'dbf' that defines the DBF file is missing"
              << std::endl;
    return 60;
  }

  return 0;
}
