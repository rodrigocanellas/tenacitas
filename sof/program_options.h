/// \copyright This file is under GPL 3 license. Please read the \p LICENSE file
/// at the root of \p tenacitas directory

/// \author Rodrigo Canellas - rodrigo.canellas at gmail.com

#ifndef TENACITAS_LIB_SOF_PROGRAM_OPTIONS_H
#define TENACITAS_LIB_SOF_PROGRAM_OPTIONS_H

#include <cstring>
#include <iostream>
#include <list>
#include <map>
#include <optional>
#include <set>
#include <string>

namespace tenacitas::lib::sof {

/// \brief Program options parser
struct program_options {

  /// \brief name of an option
  using name = std::string;

  /// \brief value of an option
  using value = std::string;

  program_options() = default;
  ~program_options() = default;
  program_options(const sof::program_options &) = delete;
  program_options(program_options &&) = delete;
  program_options &operator=(const sof::program_options &) = delete;
  program_options &operator=(program_options &&) = delete;
  void *operator new(size_t) = delete;

  /// \brief parses the options passed to a program
  ///
  /// \param p_argc number of options
  /// \param p_argv vector of strings with the options
  /// \param p_mandatory set of options that are mandatory
  ///
  /// \details an option must be preceded with '--', like '--log_file-name
  ///  abc.txt'.
  ///
  /// There are 3 types of parameter:
  /// \p boolean, where the parameter has no value, like '--print-log'
  ///
  /// \p single, where the parameter has a single value, like '--log_file-name
  /// abc.txt'
  ///
  /// \p set, where the parameter has a set of values, like '--tests { test1
  /// test2 test3 }
  ///
  /// \throws std::runtime_error
  void parse(int p_argc, char **p_argv,
             std::initializer_list<name> &&p_mandatory = {}) {
    int _last = p_argc - 1;
    int _i = 1;
    while (_i <= _last) {
      if (!is_option(p_argv[_i])) {
        throw std::runtime_error("parameter '" + std::string(p_argv[_i]) +
                                 "' should be an option");
      }

      name _name(
          std::string(&(p_argv[_i][2]), &(p_argv[_i][strlen(p_argv[_i])])));

      if (_i == _last) {
        m_booleans.insert(std::move(_name));
        break;
      }

      ++_i;
      if (is_option(p_argv[_i])) {
        m_booleans.insert(std::move(_name));
      } else {
        if (p_argv[_i][0] == '{') {
          _i = parse_set(std::move(_name), _last, p_argv, _i);
        } else {
          std::string _str(&p_argv[_i][0], &p_argv[_i][strlen(p_argv[_i])]);
          m_singles.insert({std::move(_name), std::move(_str)});
          ++_i;
        }
      }
    }

    for (const name &_name : p_mandatory) {
      if ((!get_bool_param(_name)) && (!get_single_param(_name)) &&
          (!get_set_param(_name))) {
        throw std::runtime_error("parameter '" + _name +
                                 "' should have been defined, but it was not");
      }
    }
  }

  /// \brief Retrieves the bool parameter, if possible
  ///
  /// \param p_name is the name of the parameter
  ///
  /// \return {true} if it was possible to retrieve, and \p p_name exists;
  /// {} otherwise
  std::optional<bool> get_bool_param(const name &p_name) const {
    booleans::const_iterator _ite =
        std::find(m_booleans.begin(), m_booleans.end(), p_name);
    if (_ite == m_booleans.end()) {
      return {};
    }
    return {true};
  }

  /// \brief Retrieves a single parameter, if possible
  ///
  /// \param p_name is the name of the parameter
  ///
  /// \return {<some-value>} if \p p_name was found; {} if
  /// not
  std::optional<value> get_single_param(const name &p_name) const {
    singles::const_iterator _ite =
        std::find_if(m_singles.begin(), m_singles.end(),
                     [p_name](const std::pair<name, value> &p_single) -> bool {
                       return p_single.first == p_name;
                     });
    if (_ite == m_singles.end()) {
      return {};
    }
    return {_ite->second};
  }

  /// \brief Retrieves the values of a parameter
  ///
  /// \param p_name is the name of the parameter
  ///
  /// \return {list with the values} if \p p_name was found; {} if not
  std::optional<std::list<value>> get_set_param(const name &p_name) const {
    sets::const_iterator _ite =
        std::find_if(m_sets.begin(), m_sets.end(),
                     [p_name](const std::pair<name, values> &p_set) -> bool {
                       return p_set.first == p_name;
                     });
    if (_ite == m_sets.end()) {
      return {};
    }
    return {_ite->second};
  }

  /// \brief Output operator
  friend std::ostream &operator<<(std::ostream &p_out,
                                  const sof::program_options &p_options) {
    for (const sof::program_options::name &_boolean : p_options.m_booleans) {
      p_out << "[" << _boolean << "] ";
    }

    for (program_options::singles ::const_iterator _ite =
             p_options.m_singles.begin();
         _ite != p_options.m_singles.end(); ++_ite) {
      p_out << "[" << _ite->first << "," << _ite->second << "] ";
    }

    for (program_options::sets::const_iterator _ite = p_options.m_sets.begin();
         _ite != p_options.m_sets.end(); ++_ite) {
      p_out << "[" << _ite->first << " { ";
      for (const sof::program_options::value &_value : _ite->second) {
        p_out << _value << " ";
      }
      p_out << "} ]";
    }

    return p_out;
  }

private:
  /// \brief Checks if a string is the start of an option
  /// An option must be preceded with '--'
  ///
  /// \param p_str is string to be checked
  ///
  /// \return true if it is, false otherwise
  inline bool is_option(const char *p_str) {
    return ((p_str[0] == '-') && (p_str[1] == '-'));
  }

private:
  /// \brief The type enum defines the type of the option
  enum class type : char { single = 's', boolean = 'b', set = 't' };

  /// \brief booleans type for the options that are boolean, i.e., they do not
  /// have value
  using booleans = std::set<name>;

  /// \brief singles type for the options that have a single value associated
  using singles = std::map<name, value>;

  /// \brief values type for the list of values used in the paramters that
  /// define a set of values
  using values = std::list<value>;

  /// \brief map type for the options that have a set of values associated
  using sets = std::map<name, values>;

private:
  /// \brief Parses a set of options
  ///
  /// \brief p_name is the name of the option
  ///
  /// \brief p_argv string vector with the set of options
  ///
  /// \brief p_index position in \p p_argv where the set of options starts
  int parse_set(name &&p_name, int p_last, char **p_argv, int p_index) {
    std::string _str(&p_argv[p_index][0],
                     &p_argv[p_index][strlen(p_argv[p_index])]);

    values _values;
    if (_str.length() != 1) {
      if (_str[1] == '}') {
        m_sets.insert({std::move(p_name), std::move(_values)});
        ++p_index;
        return p_index;
      }
      _str = std::string(&p_argv[p_index][1],
                         &p_argv[p_index][strlen(p_argv[p_index])]);
      _values.push_back(std::move(_str));
    }

    ++p_index;

    while (p_index <= p_last) {
      int _len = strlen(p_argv[p_index]);
      if (p_argv[p_index][_len - 1] == '}') {
        if (_len > 1) {
          _values.push_back(
              value(&p_argv[p_index][0], &p_argv[p_index][_len - 1]));
        }
        break;
      } else {
        _values.push_back(value(&p_argv[p_index][0], &p_argv[p_index][_len]));
      }
      ++p_index;
    }

    if (p_index > p_last) {
      throw std::runtime_error("option '" + p_name +
                               "' is a set, but '}' was not found");
    }
    m_sets.insert({std::move(p_name), std::move(_values)});
    ++p_index;

    return p_index;
  }

private:
  /// \brief Booleans options
  booleans m_booleans;

  /// \brief Single value options
  singles m_singles;

  /// \brief Sets options
  sets m_sets;
};

} // namespace tenacitas::lib::sof

#endif
