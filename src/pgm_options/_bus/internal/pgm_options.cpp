#include <pgm_options/bus/pgm_options.h>

namespace tenacitas {
namespace pgm_options {
namespace bus {

void
pgm_options::parse(int p_argc,
                   const char** p_argv,
                   std::set<option>&& p_mandatory)
{
  int _i = 1;
  while (_i < p_argc) {
    if (!is_option(p_argv[_i])) {
      throw std::runtime_error("parameter " + std::string(p_argv[_i]) +
                               " should be an option");
    }
    if (_i == (p_argc - 1)) {
      m_map.insert(
        { std::string(&(p_argv[_i][2]), &(p_argv[_i][strlen(p_argv[_i])])),
          "" });
      break;
    } else if ((_i == p_argc) || (is_option(p_argv[_i + 1]))) {
      m_map.insert(
        { std::string(&(p_argv[_i][2]), &(p_argv[_i][strlen(p_argv[_i])])),
          "1" });
      ++_i;
    } else {
      m_map.insert(
        { std::string(&(p_argv[_i][2]), &(p_argv[_i][strlen(p_argv[_i])])),
          p_argv[_i + 1] });
      _i += 2;
    }
  }

  for (const option& _option : p_mandatory) {
    if (m_map.find(_option) == m_map.end()) {
      throw std::runtime_error("option " + _option + " was not supplied");
    }
  }
}

const pgm_options::value&
pgm_options::get_value(const pgm_options::option& p_option) const
{
  static const value _not_found = "";
  map::const_iterator _ite = m_map.find(p_option);
  if (_ite == m_map.end()) {
    return _not_found;
  }
  return _ite->second;
}

bool
pgm_options::is_defined(const pgm_options::option& p_option) const
{
  map::const_iterator _ite = m_map.find(p_option);
  if (_ite == m_map.end()) {
    return false;
  }
  return true;
}

std::ostream&
operator<<(std::ostream& p_out, const pgm_options& p_options)
{
  for (const pgm_options::map::value_type& _value : p_options.m_map) {
    p_out << "[" << _value.first << "," << _value.second << "]";
  }
  return p_out;
}

} // namespace bus
} // namespace pgm_options
} // namespace tenacitas
