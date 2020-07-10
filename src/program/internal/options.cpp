#include <program/options.h>

namespace tenacitas {
namespace program {

// void options::parse(int p_argc, char **p_argv,
//                    std::initializer_list<name> &&p_mandatory) {
//  int _i = 1;
//  while (_i < p_argc) {
//    if (!is_option(p_argv[_i])) {
//      throw std::runtime_error("parameter '" + std::string(p_argv[_i]) +
//                               "' should be an option");
//    }
//    if (_i == (p_argc - 1)) {
//      m_list.push_back(
//          {std::string(&(p_argv[_i][2]), &(p_argv[_i][strlen(p_argv[_i])])),
//           ""});
//      break;
//    } else if ((_i == p_argc) || (is_option(p_argv[_i + 1]))) {
//      m_list.push_back(
//          {std::string(&(p_argv[_i][2]), &(p_argv[_i][strlen(p_argv[_i])])),
//           "1"});
//      ++_i;
//    } else {
//      m_list.push_back(
//          {std::string(&(p_argv[_i][2]), &(p_argv[_i][strlen(p_argv[_i])])),
//           p_argv[_i + 1]});
//      _i += 2;
//    }
//  }

//  for (const name &_option : p_mandatory) {
//    if (!(*this)[_option]) {
//      throw std::runtime_error("option " + _option + " was not supplied");
//    }
//  }
//}

void options::parse(int p_argc, char **p_argv,
                    std::initializer_list<name> &&p_mandatory) {
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
    if ((!get_bool_param(_name).first) && (!get_single_param(_name).first) &&
        (!get_set_param(_name).first)) {
      throw std::runtime_error("parameter '" + _name +
                               "' should have been defined, but it was not");
    }
  }
}

int options::parse_set(name &&p_name, int p_last, char **p_argv, int p_index) {
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
  // && (p_argv[p_index][0] != '}')) {
  //    _values.push_back(p_argv[p_index]);
  //  }
  if (p_index > p_last) {
    throw std::runtime_error("option '" + p_name +
                             "' is a set, but '}' was not found");
  }
  m_sets.insert({std::move(p_name), std::move(_values)});
  ++p_index;

  return p_index;
} // namespace program

std::pair<bool, bool> options::get_bool_param(const name &p_name) const {
  booleans::const_iterator _ite =
      std::find(m_booleans.begin(), m_booleans.end(), p_name);
  if (_ite == m_booleans.end()) {
    return {false, false};
  }
  return {true, true};
}

std::pair<bool, options::value>
options::get_single_param(const name &p_name) const {
  singles::const_iterator _ite =
      std::find_if(m_singles.begin(), m_singles.end(),
                   [p_name](const std::pair<name, value> &p_single) -> bool {
                     return p_single.first == p_name;
                   });
  if (_ite == m_singles.end()) {
    return {false, value()};
  }
  return {true, _ite->second};
}

std::pair<bool, std::list<options::value>>
options::get_set_param(const name &p_name) const {
  sets::const_iterator _ite =
      std::find_if(m_sets.begin(), m_sets.end(),
                   [p_name](const std::pair<name, values> &p_set) -> bool {
                     return p_set.first == p_name;
                   });
  if (_ite == m_sets.end()) {
    return {false, std::list<options::value>()};
  }
  return {true, _ite->second};
}

std::ostream &operator<<(std::ostream &p_out, const options &p_options) {
  for (const options::name &_boolean : p_options.m_booleans) {
    p_out << "[" << _boolean << "] ";
  }

  for (const std::pair<options::name, options::value> &_single :
       p_options.m_singles) {
    p_out << "[" << _single.first << "," << _single.second << "] ";
  }

  for (const std::pair<options::name, options::values> &_set :
       p_options.m_sets) {
    p_out << "[" << _set.first << " { ";
    for (const options::value &_value : _set.second) {
      p_out << _value << " ";
    }
    p_out << "} ]";
  }

  return p_out;
}

} // namespace program
} // namespace tenacitas
