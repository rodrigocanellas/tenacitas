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

#include <logger/business/cout.h>
#include <logger/business/log.h>
#include <pgm_options/bus/pgm_options.h>

using namespace std;

int
main()
{
  using namespace tenacitas::pgm_options::bus;

  typedef tenacitas::logger::business::log logger;

  tenacitas::logger::business::configure_cout_log();

  const int argc = 12;
  const char* argv[] = { "pgm-name",
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
                         "out_quote-50.dbf" };

  pgm_options _pgm_options;

  _pgm_options.parse(
    argc, argv, { "desc", "data", "field", "dec", "value-encoded", "dbf" });

  std::stringstream _stream;
  _stream << _pgm_options;

  logger::info("main", __LINE__, _stream.str());

  bool _enclosed_value = false;
  if (_pgm_options.is_defined("value_encoded")) {
    _enclosed_value = true;
  }

  logger::debug("main",
                __LINE__,
                "enclosed value? " +
                  (_enclosed_value ? std::string("sim") : std::string("nao")));

  std::string _field_sep = _pgm_options.get_value("field");
  if (_field_sep.empty()) {
    logger::error(
      "main",
      __LINE__,
      "parametro 'field', que define o separador de campos nao foi definido");
    return 20;
  }

  std::string _decimal_sep = _pgm_options.get_value("dec");
  if (_decimal_sep.empty()) {
    logger::error(
      "main",
      __LINE__,
      "parametro 'dec' que define o separador decimal nao foi definido");
    return 30;
  }

  std::string _desc = _pgm_options.get_value("desc");
  if (_desc.empty()) {
    logger::error("main",
                  __LINE__,
                  "parametro 'desc' que define o nome do arquivo descritor nao "
                  "foi definido");
    return 40;
  }

  std::string _data = _pgm_options.get_value("data");
  if (_data.empty()) {
    logger::error("main",
                  __LINE__,
                  "parametro 'data' que define o nome do arquivo de dados nao "
                  "foi definido");
    return 50;
  }

  std::string _dbf_file = _pgm_options.get_value("dbf");
  if (_dbf_file.empty()) {
    logger::error("main",
                  __LINE__,
                  "parametro 'dbf' que define o nome do arquivo a ser gerado "
                  "nao foi definido");
    return 60;
  }

  return 0;
}
