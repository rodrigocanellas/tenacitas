#include <program/options.h>

namespace tenacitas {
namespace program {

void options::parse(int p_argc, char **p_argv,
                    std::initializer_list<name> &&p_mandatory) {
  int _i = 1;
  while (_i < p_argc) {
    if (!is_option(p_argv[_i])) {
      throw std::runtime_error("parameter '" + std::string(p_argv[_i]) +
                               "' should be an option");
    }
    if (_i == (p_argc - 1)) {
      m_map.insert(
          {std::string(&(p_argv[_i][2]), &(p_argv[_i][strlen(p_argv[_i])])),
           ""});
      break;
    } else if ((_i == p_argc) || (is_option(p_argv[_i + 1]))) {
      m_map.insert(
          {std::string(&(p_argv[_i][2]), &(p_argv[_i][strlen(p_argv[_i])])),
           "1"});
      ++_i;
    } else {
      m_map.insert(
          {std::string(&(p_argv[_i][2]), &(p_argv[_i][strlen(p_argv[_i])])),
           p_argv[_i + 1]});
      _i += 2;
    }
  }

  for (const name &_option : p_mandatory) {
    if (m_map.find(_option) == m_map.end()) {
      throw std::runtime_error("option " + _option + " was not supplied");
    }
  }
}

std::optional<options::value> options::
operator[](const options::name &p_name) const {
  map::const_iterator _ite = m_map.find(p_name);
  if (_ite == m_map.end()) {
    return {};
  }
  return _ite->second;
}

std::ostream &operator<<(std::ostream &p_out, const options &p_options) {
  for (const options::map::value_type &_value : p_options.m_map) {
    p_out << "[" << _value.first << "," << _value.second << "]";
  }
  return p_out;
}

} // namespace program
} // namespace tenacitas
