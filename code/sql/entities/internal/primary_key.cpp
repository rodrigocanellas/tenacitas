#include <sql/entities/primary_key.h>
#include <sql/entities/table.h>

namespace capemisa {
namespace sql {
namespace entities {

std::ostream&
operator<<(std::ostream& p_out, const primary_key& p_pk)
{
  p_out << "{"
        << "\"table\" : \"" << p_pk.get_table_name()
        << "\", \"columns\" : " << p_pk.m_columns << "}";
  return p_out;
}

const name&
primary_key::get_table_name() const
{
  return m_table->get_name();
}

} // namespace entities
} // namespace sql
} // namespace capemisa
