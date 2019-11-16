#include <sql/entities/table.h>

namespace capemisa {
namespace sql {
namespace entities {

std::ostream&
operator<<(std::ostream& p_out, const table& p_table)
{
  p_out << "{\"name\": \"" << p_table.get_name()
        << "\", \"columns\" : " << p_table.m_columns
        << ", \"primary_key\" : " << p_table.get_primary_key()
        << ", \"foreigners_keys\" : " << p_table.m_foreigners_keys;

  p_out << "}";
  return p_out;
}

} // namespace entities
} // namespace sql
} // namespace capemisa
