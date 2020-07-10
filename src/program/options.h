#ifndef TENACITAS_PGM_OPTIONS_BUSINESS_OPTIONS_H
#define TENACITAS_PGM_OPTIONS_BUSINESS_OPTIONS_H

/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas rodrigo.canellas@gmail.com

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <initializer_list>
#include <iostream>
#include <list>
#include <map>
#include <optional>
#include <set>
#include <sstream>
#include <string>
#include <utility>

namespace tenacitas {
namespace program {

///
/// \brief Class to parse program options
///
struct options {

  typedef std::string name;
  typedef std::string value;

  ///
  /// \brief parse parses the options passed to a program
  /// \param p_argc number of options
  /// \param p_argv vector of strings with the options
  /// \param p_mandatory set of options that are mandatory
  ///
  /// \details an option must be preceded with '--', like '--file-name abc.txt'.
  /// There are 3 types of parameter:
  /// \p boolean, where the parameter has no value, like '--print-log'
  ///
  /// \p single, where the parameter has a single value, like '--file-name
  /// abc.txt'
  ///
  /// \p set, where the paramter has a set of values, like '--tests { test1
  /// test2 test3 }
  ///
  ///
  /// \code
  ///#include <iomanip>
  ///#include <iostream>
  ///#include <iterator>
  ///#include <map>
  ///#include <sstream>
  ///#include <string>
  ///#include <utility>
  ///#include <vector>
  ///
  ///#include <program/options.h>
  ///
  ///  using namespace std;
  ///
  ///  int main() {
  ///    try {
  ///
  ///      using namespace tenacitas::program;
  ///      using namespace std;
  ///
  ///      const char *argv[] = {"pgm-name", "--set_1",    "{",
  ///                             "v0",       "v1",         "}",
  ///                             "--bool_1", "--single_1", "single_value_1"};
  ///      const int argc = 9;
  ///
  ///      options _pgm_options;
  ///
  ///      _pgm_options.parse(argc, (char **)argv, {"bool_1"});
  ///
  ///      cerr << "options: " << _pgm_options << endl;
  ///
  ///      optional<bool> _bool = _pgm_options.get_bool_param("bool_1");
  ///      if (_bool) {
  ///        cerr << "bool param = " << _bool.value() << endl;
  ///      } else {
  ///        cerr << "ERROR! no value for paramenter 'bool_1' was found" <<
  ///        endl;
  ///      }
  ///
  ///      optional<options::value> _single =
  ///        _pgm_options.get_single_param("single_1");
  ///      if (_single) {
  ///        cerr << "single param = " << _single.value() << endl;
  ///      } else {
  ///        cerr << "ERROR! no value for paramenter 'single_1' was found" <<
  ///        endl;
  ///      }
  ///
  ///      optional<list<options::value>> _set =
  ///      _pgm_options.get_set_param("set_1"); if (_set) {
  ///        if (_set.value().size() != 2) {
  ///          cerr << "ERROR! number of values in set parameter should be 2,
  ///          but it "
  ///                  "is "
  ///               << _set.value().size() << endl;
  ///        } else {
  ///          cerr << "set param 'set_1' = ";
  ///          for (options::value _value : _set.value()) {
  ///            cerr << _value << " ";
  ///          }
  ///          cerr << endl;
  ///        }
  ///      } else {
  ///        cerr << "ERROR! no set of values found for parameter 'set_1'" <<
  ///        endl;
  ///      }
  ///    } catch (std::exception &_ex) {
  ///      cerr << "ERROR! '" << _ex.what() << "'" << endl;
  ///    }
  ///
  ///    return 0;
  ///  }
  /// \endcode
  ///
  /// \throws std::runtime_error
  void parse(int p_argc, char **p_argv,
             std::initializer_list<name> &&p_mandatory = {});

  std::pair<bool, bool> get_bool_param(const name &p_name) const;

  std::pair<bool, value> get_single_param(const name &p_name) const;

  std::pair<bool, std::list<value>> get_set_param(const name &p_name) const;

  ///
  /// \brief operator <<
  /// \param p_out
  /// \param p_options
  /// \return
  ///
  friend std::ostream &operator<<(std::ostream &p_out,
                                  const options &p_options);

private:
  inline bool is_option(const char *p_str) {
    return ((p_str[0] == '-') && (p_str[1] == '-'));
  }

private:
  ///
  /// \brief The type enum defines the type of the option
  ///
  enum class type : char { single = 's', boolean = 'b', set = 't' };

  ///
  /// \brief booleans type for the options that are boolean, i.e., they do not
  /// have value
  ///
  typedef std::set<name> booleans;

  ///
  /// \brief singles type for the options that have a single value associated
  ///
  typedef std::map<name, value> singles;

  ///
  /// \brief values type for the list of values used in the paramters that
  /// define a set of values
  ///
  typedef std::list<value> values;

  ///
  /// \brief map type for the options that have a set of values associated
  ///
  typedef std::map<name, values> sets;

private:
  int parse_set(name &&p_name, int p_last, char **p_argv, int p_index);

private:
  booleans m_booleans;
  singles m_singles;
  sets m_sets;
};

} // namespace program
} // namespace tenacitas

#endif // OPTIONS_H
