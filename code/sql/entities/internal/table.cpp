#include <sql/entities/table.h>

namespace capemisa {
namespace sql {
namespace entities {

std::ostream&
operator<<(std::ostream& p_out, const table& p_table)
{
  p_out << "{ \"name\": \"" << p_table.get_name() << "\", "
        << "\"columns\": [";
  columns::const_iterator _end = p_table.end_column();
  for (columns::const_iterator _ite = p_table.begin_column(); _ite != _end;
       ++_ite) {
    p_out << *_ite;
    if (std::next(_ite) != _end) {
      p_out << ",";
    }
  }
  p_out << "] ";

  p_out << "," << (*p_table.get_primary_key());

  p_out << ", " << p_table.m_foreigners_keys;

  p_out << "}";
  return p_out;
}

} // namespace entities
} // namespace sql
} // namespace capemisa
