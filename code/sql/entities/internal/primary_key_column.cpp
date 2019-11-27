#include <sql/entities/primary_key.h>
#include <sql/entities/primary_key_column.h>

namespace capemisa {
namespace sql {
namespace entities {

std::ostream&
operator<<(std::ostream& p_out, const primary_key_column& p_pk_col)
{
  p_out << "{" << (const column)p_pk_col << ", \"autoincrement\" : \""
        << (p_pk_col.is_autoincrement() ? "true" : "false") << "\", "
        << "\"unique\" : \"" << (p_pk_col.is_unique() ? "true" : "false")
        << "\""
        << "}";

  return p_out;
}

const primary_key&
primary_key_column::get_primary_key() const
{
  return *m_primary_key;
}

} // namespace entities
} // namespace sql
} // namespace capemisa
