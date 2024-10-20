
#include <optional>
#include <sstream>
#include <string>

#include <tenacitas.lib/program/options.h>
#include <tenacitas.lib/tester/test.h>
#include <tenacitas.lib/log/cerr.h>
#include <tenacitas.lib/parser/ini_file.h>

using namespace tenacitas::lib;

struct test_000 {
  static std::string desc() { return ""; }

  bool operator()(const program::options &p_options) {

    try {
      std::string _ini_file_name{
          p_options.get_single_param("ini").value_or("")};
      if (_ini_file_name.empty()) {
        m_logger.err("parameter --ini expected");
        return false;
      }

      using ini_file = parser::ini_file<logger>;

      ini_file _ini_file(m_logger);

      std::optional<ini_file::sections> _sections(
          _ini_file.read(_ini_file_name).value_or(ini_file::sections{}));

      if (_sections.value().empty()) {
        m_logger.err(
            format::fmt("no sections found in '", _ini_file_name, '\''));
        return false;
      }

      std::stringstream _stream;

      _ini_file.traverse(
          _sections.value(),
          [&](const ini_file::section &p_section,
              const ini_file::properties &p_properties) -> bool {
            _stream << "\nsection = '" << p_section << "'\n";

            _ini_file.traverse(
                p_properties,
                [&_stream](const ini_file::key &p_key,
                           const ini_file::value &p_value) -> bool {
                  _stream << "\tkey = '" << p_key << "', value = '" << p_value
                          << "'\n";
                  return true;
                });

            return true;
          });

      m_logger.tst(_stream.str());
      return true;

    } catch (std::exception &_ex) {
      m_logger.err(_ex.what());
    }
    return false;
  }

private:
  using logger = log::cerr;
  logger m_logger;
};

int main(int argc, char **argv) {

  generic::test _test(argc, argv, {"ini"});
  run_test(_test, test_000);
}
