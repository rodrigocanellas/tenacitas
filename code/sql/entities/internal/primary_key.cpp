#include <sql/entities/primary_key.h>
#include <sql/entities/table.h>

namespace capemisa {
namespace sql {
namespace entities {

std::ostream&
operator<<(std::ostream& p_out, const primary_key& p_pk)
{
  p_out << "{"
        << "\"pk_columns\" : " << p_pk.m_pks_columns << ", "
        << "\"fk_columns\" : " << p_pk.m_fks_columns << "}";

  return p_out;
}

const table&
primary_key::get_table() const
{
  return *m_table;
}

} // namespace entities
} // namespace sql
} // namespace capemisa
