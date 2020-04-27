#ifndef TENACITAS_PGM_OPTIONS_BUSINESS_OPTIONS_H
#define TENACITAS_PGM_OPTIONS_BUSINESS_OPTIONS_H

#include <cstdint>
#include <cstring>
#include <initializer_list>
#include <iostream>
#include <map>
#include <optional>
#include <sstream>
#include <string>

namespace tenacitas {
namespace program {

///
/// \brief Class to parse program options
///
struct options {

  typedef std::string name;
  typedef std::string value;

  ///
  /// \brief parse parses de options passed to a program
  /// \param p_argc number of options
  /// \param p_argv vector of strings with the options
  /// \param p_mandatory set of options that are mandatory
  ///
  /// \details an option must be preceded with '--', like '--file-name', and the
  /// value for the option right after the option, like '--file-name
  /// file000.txt'. An option may also have no value associated, like
  /// '--value-encoded', in this case, the value '1' is associated to the
  /// option, in order to set apart to the value "", which means 'option not
  /// found'.
  ///
  /// \code
  /// some_pmg --desc desc.csv --data in_quote-50.csv --field , --dec ,
  /// --value-encoded --dbf out_quote-50.dbf
  /// \endcode
  ///
  /// \throws std::runtime_error
  void parse(int p_argc, char **p_argv,
             std::initializer_list<name> &&p_mandatory = {});

  ///
  /// \brief get_value gives access to the value of an option
  /// \param p_option name of the option
  /// \return value associated to the option, or std::nullopt if the option  was
  /// not found
  ///
  std::optional<value> operator[](const name &p_name) const;

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
  /// \brief map type for the options and theirs values
  ///
  typedef std::map<name, value> map;

private:
  ///
  /// \brief m_map options and theirs values
  ///
  map m_map;
};

} // namespace program
} // namespace tenacitas

#endif // OPTIONS_H
